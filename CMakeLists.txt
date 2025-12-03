cmake_minimum_required(VERSION 3.14)
project(oop_lab_05 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Флаги компиляции
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
endif()

# Директории с заголовками
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Основная программа
set(MAIN_SOURCES 
    src/main.cpp
    src/MemoryResource.cpp
)

add_executable(oop_lab05 ${MAIN_SOURCES})

# Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.0
)

# Настройка для Windows
if(WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

FetchContent_MakeAvailable(googletest)

# Тесты
enable_testing()

set(TEST_SOURCES 
    tests/test.cpp
    src/MemoryResource.cpp  # ВАЖНО: добавляем исходник!
)

add_executable(tests ${TEST_SOURCES})

# Подключаем заголовки для тестов
target_include_directories(tests 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${googletest_SOURCE_DIR}/googletest/include
        ${googletest_SOURCE_DIR}/googlemock/include
)

# Линковка библиотек
target_link_libraries(tests 
    PRIVATE 
        gtest_main
)

# Добавляем тест в CTest
add_test(NAME ListTests COMMAND tests)

# Опционально: запуск тестов после сборки
add_custom_command(TARGET tests
    POST_BUILD
    COMMAND $<TARGET_FILE:tests> --gtest_color=yes
    COMMENT "Running unit tests..."
    VERBATIM
)