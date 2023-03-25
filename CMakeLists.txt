cmake_minimum_required(VERSION 3.21)
project(Final LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(Final src/main.cpp)
target_link_libraries(Final PRIVATE sfml-graphics sfml-audio)
target_compile_features(Final PRIVATE cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET Final POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:Final> $<TARGET_FILE_DIR:Final> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS Final)
