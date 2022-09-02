cmake_minimum_required(VERSION 3.13)
project(gpio)

file(GLOB SRC "src/*.cpp")

add_library(gpio SHARED ${SRC})
target_link_libraries(gpio PRIVATE pigpio)
target_link_libraries(gpio PRIVATE rt)
target_link_libraries(gpio PRIVATE sdrpp_core)
set_target_properties(gpio PROPERTIES PREFIX "")

target_include_directories(gpio PRIVATE "src/")

if (MSVC)
    target_compile_options(gpio PRIVATE /O2 /Ob2 /std:c++17 /EHsc)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(gpio PRIVATE -O3 -std=c++17 -Wno-unused-command-line-argument -undefined dynamic_lookup)
else ()
    target_compile_options(gpio PRIVATE -O3 -std=c++17)
endif ()

# Install directives
install(TARGETS gpio DESTINATION lib/sdrpp/plugins)