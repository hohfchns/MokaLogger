#include <filesystem>
#include <gtest/gtest.h>
#include "moka/logger/logger.h"

#define EXE_DIR_MAX_CHARACTERS 256
static char g_exePath[EXE_DIR_MAX_CHARACTERS];

#ifdef __linux__
#define RETRIEVE_EXE_PATH() std::string __temp = std::filesystem::canonical("/proc/self/exe"); strcpy(g_exePath, __temp.c_str())
#else
#define RETRIEVE_EXE_PATH() GetModuleFileName(NULL, g_exePath, sizeof(EXE_DIR_MAX_CHARACTERS))
#endif

TEST(LoggerTest, BasicUsage) {
  RETRIEVE_EXE_PATH();
  std::string path = std::filesystem::path(std::string(g_exePath)).parent_path().string() + "/log.log";
  auto config = moka::log::Logger::GetLogger("default")->GetConfig();
  config.filePath = path;
  moka::log::Logger::GetLogger("default")->SetConfig(config);
  MOKA_LOG_INFO("default", "Hello Moka!");
  MOKA_LOGF_INFO("default", "Info %s!", "Moka");
  MOKA_LOGF_WARNING("default", "Warning %s!", "Moka");
  MOKA_LOGF_ERROR("default", "Error %s!", "Moka");
  MOKA_LOGF_DEBUG("default", "Debug %s!", "Moka");
  MOKA_LOGF_INFO("default", "Info %s!", "Moka");
}
