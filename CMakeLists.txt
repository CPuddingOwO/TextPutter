cmake_minimum_required(VERSION 3.28)
project(TextPutter)

set(CMAKE_CXX_STANDARD 20)
set(SDL_STATIC ON)
set(SDL_SHARED OFF)
set(__WIN32 ON) # 调整为 OFF 在Windows上显示命令行窗口

if (NOT DEFINED CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT OR NOT CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/install")
endif ()

if (__WIN32 AND WIN32)
    add_compile_definitions(__WIN32)
endif ()

include(GNUInstallDirs)
list(APPEND CMAKE_MODULE_PATH "${CatBirdRemake_SOURCE_DIR}/cmake")

add_subdirectory(external/sdl)

find_package(nlohmann_json REQUIRED)
find_package(spdlog REQUIRED)
find_package(imgui REQUIRED)
find_package(glm REQUIRED)
#find_package(yaml-cpp REQUIRED)
find_package(mini REQUIRED)

set(SRC_CODE
        src/imgui/imgui_impl_sdlrenderer3.cpp
        src/imgui/imgui_impl_sdl3.cpp

        src/main.cpp
        src/Application.cpp
        src/Locator.cpp
)

set(DEBUG_CODE

)

list(APPEND SRC_CODE ${DEBUG_CODE})

if (__WIN32 AND WIN32)
    add_executable(TextPutter WIN32 ${SRC_CODE})
else ()
    add_executable(TextPutter ${SRC_CODE})
endif ()
install(TARGETS TextPutter
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

target_compile_definitions(TextPutter PRIVATE
        SDL_MAIN_HANDLED
)

target_include_directories(TextPutter PRIVATE
        include
        ${SDL3_INCLUDE_DIR}
)

target_link_libraries(TextPutter PRIVATE
        SDL3-static
        imgui::imgui
        spdlog::spdlog
#        yaml-cpp::yaml-cpp
        mini::mini
        nlohmann_json::nlohmann_json
        glm::glm
)
