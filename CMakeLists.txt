cmake_minimum_required(VERSION 3.10)
project(Moka)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

include_directories("inc/")

file(
  GLOB SOURCE_FILES 
  "src/moka/logger/logger.cpp"
)

file (
  GLOB TESTS_SOURCE_FILES
  "tests/test_log.cpp"
  "tests/tests_main.cpp"
)

add_library(MokaLogger STATIC ${SOURCE_FILES})
target_link_libraries(MokaLogger ${MOKA_LOGGER_DEP_LIBS})

set_property(TARGET MokaLogger PROPERTY CXX_STANDARD 20)
set_property(TARGET MokaLogger PROPERTY CXX_STANDARD_REQUIRED 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function")

if (UNIT_TESTS)
  if (TESTS_SOURCE_FILES)
    include(FetchContent)
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )

    if (WIN32)
      # For Windows: Prevent overriding the parent project's compiler/linker settings
      set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
      FetchContent_MakeAvailable(googletest)
    endif()

    enable_testing()

    include(GoogleTest)
    add_executable(MokaLogger_UnitTests ${TESTS_SOURCE_FILES})
    target_link_libraries(MokaLogger_UnitTests gtest MokaLogger)
    set_property(TARGET MokaLogger_UnitTests PROPERTY CXX_STANDARD 20)
    set_property(TARGET MokaLogger_UnitTests PROPERTY CXX_STANDARD_REQUIRED 20)

    gtest_discover_tests(MokaLogger_UnitTests)
  endif()
endif()

set_property(GLOBAL PROPERTY MOKA_LOGGER 1)
set_property(GLOBAL PROPERTY MOKA_LOGGER_INCS "${CMAKE_CURRENT_SOURCE_DIR}/inc")
set_property(GLOBAL PROPERTY MOKA_LOGGER_LIBS MokaLogger)
