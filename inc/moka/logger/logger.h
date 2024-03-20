#pragma once
#include <fstream>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>

#define MOKA_LOG(logger, level, str) moka::log::Logger::GetLogger(logger)->Log(level, str, __LINE__, __FILE__)
#define MOKA_LOG_DEBUG(logger, str) MOKA_LOG(logger, moka::log::LogLevel::DEBUG, str)
#define MOKA_LOG_INFO(logger, str) MOKA_LOG(logger, moka::log::LogLevel::INFO, str)
#define MOKA_LOG_WARNING(logger, str) MOKA_LOG(logger, moka::log::LogLevel::WARNING, str)
#define MOKA_LOG_ERROR(logger, str) MOKA_LOG(logger, moka::log::LogLevel::ERROR, str)

#define MOKA_LOGF(logger, level, format, ...) moka::log::Logger::GetLogger(logger)->LogFormat(level, format, __LINE__, __FILE__, __VA_ARGS__)
#define MOKA_LOGF_DEBUG(logger, format, ...) MOKA_LOGF(logger, moka::log::LogLevel::DEBUG, format, __VA_ARGS__)
#define MOKA_LOGF_INFO(logger, format, ...) MOKA_LOGF(logger, moka::log::LogLevel::INFO, format, __VA_ARGS__)
#define MOKA_LOGF_WARNING(logger, format, ...) MOKA_LOGF(logger, moka::log::LogLevel::WARNING, format, __VA_ARGS__)
#define MOKA_LOGF_ERROR(logger, format, ...) MOKA_LOGF(logger, moka::log::LogLevel::ERROR, format, __VA_ARGS__)

namespace moka::log
{
  enum class LogLevel {
    DEBUG = 0,
    INFO = 1,
    WARNING = 2,
    ERROR = 3
  };

  const std::map<LogLevel, std::string> LOG_LEVEL_STRINGS = {
    {LogLevel::DEBUG, "DEBUG"},
    {LogLevel::INFO, "INFO"},
    {LogLevel::WARNING, "WARNING"},
    {LogLevel::ERROR, "ERROR"}
  };

  const std::unordered_map<LogLevel, const std::string> COLOR_CODES = {
    {LogLevel::DEBUG, "\033[2137m"},
    {LogLevel::INFO, "\033[1;37m"},
    {LogLevel::WARNING, "\033[1;33m"}, 
    {LogLevel::ERROR, "\033[1;31m"}
  };

  struct LoggerConfig
  {
    LogLevel level = LogLevel::DEBUG;
    bool alwaysLogToFile = true;
    bool appendToFile = false;
    std::string filePath = "";

    LoggerConfig(const std::string& logFilePath);
    LoggerConfig();
  };

  class Logger
  {
  public:
    Logger& operator=(Logger&& other);
    Logger& operator=(const Logger& other);
    Logger(Logger&& other);
    Logger();

    static Logger* GetLogger(const std::string& id);
    static Logger* RegisterLogger(const std::string& id);

    ~Logger();

    void Log(LogLevel level, const std::string& str, size_t line, const char* file) const;

    template<typename ... Args>
    void LogFormat(LogLevel level, const std::string& format, size_t line, const char* file, Args ... args) const
    {
      // Credit for the formatting goes to this awesome StackOverflow post:
      // https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf

      int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
      if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
      auto size = static_cast<size_t>( size_s );
      std::unique_ptr<char[]> buf( new char[ size ] );
      std::snprintf( buf.get(), size, format.c_str(), args ... );

      Log(level, std::string( buf.get(), buf.get() + size - 1 ), line, file);
    }

    // Warning - config will be moved
    void SetConfig(LoggerConfig& config, bool openFile=true);
    void SetConfig(LoggerConfig&& config, bool openFile=true);
    const LoggerConfig& GetConfig() const;

    void SetLevel(LogLevel level);

    void OpenLogFileFromConfig();

  private:
    static std::unordered_map<std::string, Logger> registry;

    void SetConfigBase(const LoggerConfig& config, bool openFile);

    mutable std::ofstream logFile;
    LoggerConfig config;
  };
}

namespace std
{

std::string to_string(const moka::log::LogLevel& logLevel);

}

