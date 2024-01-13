#include <cstdarg>
#include <memory>
#include <stdexcept>
#include "moka/logger/logger.h"

using std::to_string;

namespace moka::log
{
  Logger* Logger::activeLogger = new Logger;

  void Logger::Log(LogLevel level, const std::string& str, size_t line, const char* file)
  {
    std::string formattedStr = "LOG::Level " + to_string(level) + "::FILE `" + file + "`::LINE " + to_string(line) + "::MESSAGE | `" + str + "`";
    if (level >= config.level)
    {
      std::cout << moka::log::colorCodes.at(level) << formattedStr << "\033[0m" << "\r\n";
    }

    if (config.filePath.empty())
    {
      return;
    }

    if (config.alwaysLogToFile || level >= config.level)
    {
      if (!logFile.is_open())
      {
        logFile.open(config.filePath, std::ios::out | std::ios::app);
      }

      if (!logFile.is_open())
      {
        throw std::runtime_error("Failed to open / create log file at `" + config.filePath + "`");
      }

      logFile << formattedStr << std::endl;
    }
  }

  void Logger::SetConfig(LoggerConfig& config)
  {
    this->config = config;

    if (logFile.is_open())
    {
      logFile.close();
    }
  }

  const LoggerConfig& Logger::GetConfig() const
  {
    return config;
  }
}

namespace std
{

std::string to_string(const moka::log::LogLevel& logLevel)
{
  std::string str = "";
  switch (logLevel)
  {
    case moka::log::LogLevel::INFO:
      str = "INFO";
      break;
    case moka::log::LogLevel::WARNING:
      str = "WARNING";
      break;
    case moka::log::LogLevel::ERROR:
      str = "ERROR";
      break;
    case moka::log::LogLevel::DEBUG:
      str = "ERROR";
      break;
  }

  return str;
}

}
