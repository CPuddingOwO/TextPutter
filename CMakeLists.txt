cmake_minimum_required(VERSION 3.28)
project(TextPutter)

set(CMAKE_CXX_STANDARD 20)
set(SDL3_STATIC ON)
set(SDL3_SHARED OFF)
set(SDLIMAGE_BUILD_SHARED_LIBS OFF)

if (NOT DEFINED CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT OR NOT CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/install")
endif ()

include(GNUInstallDirs)
list(APPEND CMAKE_MODULE_PATH "${CatBirdRemake_SOURCE_DIR}/cmake")

add_subdirectory(external/sdl)
add_subdirectory(external/sdl_image)

find_package(nlohmann_json)
find_package(spdlog)
find_package(imgui)
find_package(glm)

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

add_executable(TextPutter ${SRC_CODE})
install(TARGETS TextPutter
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(TARGETS TextPutter SDL3-shared SDL3_image-shared
        LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}
)

target_compile_definitions(TextPutter PRIVATE
)

target_include_directories(TextPutter PRIVATE
        include
        ${SDL3_INCLUDE_DIRS}
        ${SDL3_IMAGE_INCLUDE_DIRS}
)

target_link_libraries(TextPutter PRIVATE
        SDL3_image::SDL3_image
        SDL3::SDL3
        imgui::imgui
        spdlog::spdlog
        nlohmann_json::nlohmann_json
        glm::glm
)
