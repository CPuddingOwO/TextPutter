#pragma once
#include <SDL3/SDL.h>
#include <string>
#include <glm/glm.hpp>

namespace tp {
    struct SDL_Context final {
        SDL_Window* window;
        SDL_Renderer* renderer;

        glm::ivec2 screen_size;

        std::string window_title;
        glm::ivec2 window_size;
    };
}