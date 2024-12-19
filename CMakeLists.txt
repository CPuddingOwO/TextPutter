cmake_minimum_required(VERSION 3.28)
project(TextPutter)

set(CMAKE_CXX_STANDARD 20)

if (NOT DEFINED CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT OR NOT CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/install")
endif ()

include(GNUInstallDirs)
list(APPEND CMAKE_MODULE_PATH "${CatBirdRemake_SOURCE_DIR}/cmake")

add_subdirectory(external/sdl)
add_subdirectory(external/sdl_image)

find_package(nlohmann_json)
find_package(spdlog)

set(SRC_CODE
    src/main.cpp
)

set(DEBUG_CODE

)

list(APPEND SRC_CODE ${DEBUG_CODE})

add_executable(TextPutter ${SRC_CODE})
install(TARGETS TextPutter
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

target_include_directories(sandbox PRIVATE
        include
)

target_link_libraries(sandbox PRIVATE
        spdlog::spdlog
        nlohmann_json::nlohmann_json
)
