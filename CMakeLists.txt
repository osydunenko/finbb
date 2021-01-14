cmake_minimum_required(VERSION 3.16)

project(finbb)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

add_compile_definitions("$<$<CONFIG:Debug>:DEBUG=1>")

find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS 
    system
    program_options
)

add_executable(${PROJECT_NAME})
target_sources(
    ${PROJECT_NAME}
    PRIVATE
        src/main.cpp
        src/router.cpp
)

add_subdirectory(3rdParty/beast-router)
add_subdirectory(3rdParty/spdlog)

target_compile_options(
    ${PROJECT_NAME}
    PUBLIC
        $<$<CXX_COMPILER_ID:GNU>:-Wall>
        $<$<CXX_COMPILER_ID:GNU>:-Wextra>
        $<$<CXX_COMPILER_ID:GNU>:-Wpedantic>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-unused-parameter>
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        Boost::headers
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
        Threads::Threads
        Boost::system
        Boost::program_options
        router::router
        spdlog::spdlog
)
