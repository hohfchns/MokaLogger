#pragma once
#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>

#define MOKA_LOG(level, str) moka::log::Logger::GetActiveLogger()->Log(level, str, __LINE__, __FILE__)
#define MOKA_LOG_DEBUG(str) MOKA_LOG(moka::log::LogLevel::DEBUG, str)
#define MOKA_LOG_INFO(str) MOKA_LOG(moka::log::LogLevel::INFO, str)
#define MOKA_LOG_WARNING(str) MOKA_LOG(moka::log::LogLevel::WARNING, str)
#define MOKA_LOG_ERROR(str) MOKA_LOG(moka::log::LogLevel::ERROR, str)

#define MOKA_LOGF(level, format, ...) moka::log::Logger::GetActiveLogger()->LogFormat(level, format, __LINE__, __FILE__, __VA_ARGS__)
#define MOKA_LOGF_DEBUG(format, ...) MOKA_LOGF(moka::log::LogLevel::DEBUG, format, __VA_ARGS__)
#define MOKA_LOGF_INFO(format, ...) MOKA_LOGF(moka::log::LogLevel::INFO, format, __VA_ARGS__)
#define MOKA_LOGF_WARNING(format, ...) MOKA_LOGF(moka::log::LogLevel::WARNING, format, __VA_ARGS__)
#define MOKA_LOGF_ERROR(format, ...) MOKA_LOGF(moka::log::LogLevel::ERROR, format, __VA_ARGS__)

namespace moka::log
{
  enum class LogLevel {
    DEBUG = 0,
    INFO = 1,
    WARNING = 2,
    ERROR = 3
  };

  const std::unordered_map<LogLevel, const std::string> colorCodes = {
    {LogLevel::ERROR, "\033[1;31m"}, 
    {LogLevel::WARNING, "\033[1;33m"}, 
    {LogLevel::INFO, "\033[1;37m"},
    {LogLevel::DEBUG, "\033[2137m"},
  };

  struct LoggerConfig
  {
    LogLevel level = LogLevel::DEBUG;
    bool alwaysLogToFile = true;
    std::string filePath = "";

    LoggerConfig(const std::string& logFilePath)
      : filePath(logFilePath)
    {
    }
    LoggerConfig()
    {
    }
  };

  class Logger
  {
  public:
    static Logger* GetActiveLogger();
    static void SetActiveLogger(Logger* logger);

    void Log(LogLevel level, const std::string& str, size_t line, const char* file);

    template<typename ... Args>
    void LogFormat(LogLevel level, const std::string& format, size_t line, const char* file, Args ... args)
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
    void SetConfig(LoggerConfig& config);
    void SetConfig(LoggerConfig&& config);
    const LoggerConfig& GetConfig() const;

  private:
    static Logger* activeLogger;

    std::ofstream logFile;
    LoggerConfig config;

  };
}

namespace std
{

std::string to_string(const moka::log::LogLevel& logLevel);

}

