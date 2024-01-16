#include <stdexcept>
#include <iostream>
#include "moka/logger/logger.h"

using std::to_string;

namespace moka::log
{
  std::unordered_map<std::string, moka::log::Logger> Logger::registry{};

  Logger& Logger::operator=(Logger&& other)
  {
    SetConfig(other.config);
    return *this;
  }

  Logger& Logger::operator=(const Logger& other)
  {
    auto conf = other.config;
    SetConfig(conf);
    return *this;
  }

  Logger::Logger(Logger&& other)
  {
    SetConfig(other.config);
  }

  Logger::Logger()
  {
  }

  Logger* Logger::GetLogger(const std::string& id)
  {
    auto requested = registry.find(id);
    if (requested == registry.end())
    {
      if (id == "default")
      {
        registry["default"] = Logger();
        return &registry["default"];
      }

      return nullptr;
    }

    return &requested->second;
  }

  Logger* Logger::RegisterLogger(const std::string& id)
  {
    auto existing = registry.find(id);
    if (existing != registry.end())
    {
      throw std::runtime_error("Tried to register the same logger twice.");
    }

    registry[id] = Logger();
    return &(registry[id]);
  }

  void Logger::Log(LogLevel level, const std::string& str, size_t line, const char* file) const
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
      logFile << formattedStr << "\n";
    }
  }

  Logger::~Logger()
  {
    if (logFile.is_open())
    {
      logFile.close();
    }
  }

  void Logger::OpenLogFileFromConfig()
  {
    if (logFile.is_open())
    {
      logFile.close();
    }

    auto mode = std::ios::out;
    if (config.appendToFile)
    {
      mode |= std::ios::app;
    }

    logFile.open(config.filePath, mode);

    if (!logFile.is_open())
    {
      throw std::runtime_error("Failed to open / create log file at `" + config.filePath + "`");
    }
  }

  void Logger::SetConfigBase(const LoggerConfig& config, bool openFile)
  {
    if (this->config.filePath == config.filePath)
    {
      return;
    }

    this->config = std::move(config);

    if (this->config.filePath.empty())
    {
      if (logFile.is_open())
      {
        logFile.close();
      }

      return;
    }

    if (openFile)
    {
      OpenLogFileFromConfig();
    }
  }

  void Logger::SetConfig(LoggerConfig& config, bool openFile)
  {
    SetConfigBase(std::move(config), openFile);
  }

  void Logger::SetConfig(LoggerConfig&& config, bool openFile)
  {
    SetConfigBase(std::move(config), openFile);
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
      str = "DEBUG";
      break;
  }

  return str;
}

}
