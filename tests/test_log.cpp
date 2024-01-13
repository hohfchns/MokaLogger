#include <gtest/gtest.h>
#include "moka/logger/logger.h"

TEST(LoggerTest, BasicUsage) {
  std::string path = "/home/hohfchns/Dev/C++/MokaLogger/logs/log.log";
  auto config = moka::log::Logger::activeLogger->GetConfig();
  config.filePath = path;
  moka::log::Logger::activeLogger->SetConfig(config);
  MOKA_LOG_INFO("Hello Moka!");
  MOKA_LOGF_INFO("Info %s!", "Moka");
  MOKA_LOGF_WARNING("Warning %s!", "Moka");
  MOKA_LOGF_ERROR("Error %s!", "Moka");
  MOKA_LOGF_DEBUG("Debug %s!", "Moka");
  MOKA_LOGF_INFO("Info %s!", "Moka");
}
