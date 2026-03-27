cmake_minimum_required(VERSION 3.16)
project(AnimalMod VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Fetch RED4ext.SDK

include(FetchContent)
FetchContent_Declare(
    red4ext_sdk
    GIT_REPOSITORY https://github.com/WopsS/RED4ext.SDK.git
    GIT_TAG master
)

FetchContent_MakeAvailable(red4ext_sdk)

# Define our plugin DLL
add_library(AnimalMod SHARED src/main.cpp)

# Output directory config
set_target_properties(AnimalMod PROPERTIES PREFIX "")
set_target_properties(AnimalMod PROPERTIES SUFFIX ".dll")

# Link RED4ext.SDK
target_link_libraries(AnimalMod PRIVATE RED4ext::SDK)
