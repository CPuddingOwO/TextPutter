#include <TextPutter/Application.hpp>
#include <TextPutter/SharedLocator.hpp>
#include <TextPutter/io/SDL.hpp>
#include <TextPutter/io/State.hpp>

#include <imgui.h>
#include <SDL3/SDL.h>
//#include <SDL3_image/SDL_image.h>
#include <imgui/imgui_impl_sdlrenderer3.h>
#include <imgui/imgui_impl_sdl3.h>

#include <stdexcept>
#include <spdlog/spdlog.h>

namespace tp {

    Application::Application() {
        init();
    }

    Application::~Application() {
        quit();
    }

    void Application::init() {
        { // Init Contexts
            auto ctx = std::make_shared<tp::SDL_Context>();
            ctx->window_title = "TextPutter";
            ctx->window_size = glm::ivec2(640, 480);
            SharedLocator::Provide<tp::SDL_Context>( ctx );

            auto state = std::make_shared<tp::State>();
            SharedLocator::Provide<tp::State>( state );
        }

        auto sdl_ctx = SharedLocator::Get<tp::SDL_Context>();
        auto app_state = SharedLocator::Get<tp::State>();

        { // Init SDL
            if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
                throw std::runtime_error("Error: SDL_Init(): " + std::string(SDL_GetError()));
            }
        }

        { // Create Window
            spdlog::info("Creating window...");
            // Set Window Properties
            SDL_PropertiesID props = SDL_CreateProperties();
            SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING,
                                  sdl_ctx->window_title.c_str());
            SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, sdl_ctx->window_size.x);
            SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, sdl_ctx->window_size.y);
            SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_BORDERLESS_BOOLEAN,true); // Borderless
            SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HIGH_PIXEL_DENSITY_BOOLEAN, true); // HDPI

            // Init Window
            sdl_ctx->window = SDL_CreateWindowWithProperties(props);
            if (sdl_ctx->window == nullptr) {
                throw std::runtime_error("Error: SDL_CreateWindow(): " + std::string(SDL_GetError()));
            }
            SDL_DestroyProperties(props);
        }

        { // Create Renderer
            spdlog::info("Creating renderer...");
            // Init Renderer
            SDL_PropertiesID props = SDL_CreateProperties();
            SDL_SetStringProperty(props, SDL_PROP_RENDERER_CREATE_NAME_STRING, "Software");
            SDL_SetPointerProperty(props, SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, sdl_ctx->window);
            SDL_SetNumberProperty(props, SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER, false);

            sdl_ctx->renderer = SDL_CreateRendererWithProperties(props);
            if (sdl_ctx->renderer == nullptr) {
                throw std::runtime_error("Error: SDL_CreateRenderer(): " + std::string(SDL_GetError()));
            }
            SDL_DestroyProperties(props);
        }
        app_state->isRunning = true;

        SDL_SetWindowPosition(sdl_ctx->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        SDL_ShowWindow(sdl_ctx->window);

        { // Init Imgui
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO(); (void)io;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

            //io.Fonts->AddFontDefault();
            //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
            //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
            //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
            //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
            //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
            //IM_ASSERT(font != nullptr);

            ImGui::StyleColorsLight();
            ImGui_ImplSDL3_InitForSDLRenderer(sdl_ctx->window, sdl_ctx->renderer);
            ImGui_ImplSDLRenderer3_Init(sdl_ctx->renderer);
        }
    }

    void Application::run() {
        auto app_state = SharedLocator::Get<tp::State>();
        auto sdl_ctx = SharedLocator::Get<tp::SDL_Context>();

        while (app_state->isRunning) {
            { // Event Polling
                SDL_Event event;
                while (SDL_PollEvent(&event)) {
                    ImGui_ImplSDL3_ProcessEvent(&event);
                    if (event.type == SDL_EVENT_QUIT)
                        app_state->isRunning = false;
                    if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(sdl_ctx->window))
                        app_state->isRunning = false;
                }
                if (SDL_GetWindowFlags(sdl_ctx->window) & SDL_WINDOW_MINIMIZED)
                {
                    SDL_Delay(10);
                    continue;
                }

            }

            ImGui_ImplSDLRenderer3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();
            { // Rendering
                ImGui::Begin("#App#Main");
                ImGui::Text("qwq");
                ImGui::End();
            }
            ImGui::Render();
//            SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
            SDL_SetRenderDrawColor(sdl_ctx->renderer, 30, 30, 30, 255);
            SDL_RenderClear(sdl_ctx->renderer);
            ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), sdl_ctx->renderer);
            SDL_RenderPresent(sdl_ctx->renderer);
        }
    }

    void Application::quit() {
        auto app_state = SharedLocator::Get<tp::State>();
        auto sdl_ctx = SharedLocator::Get<tp::SDL_Context>();

        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();

        SDL_DestroyRenderer(sdl_ctx->renderer);
        SDL_DestroyWindow(sdl_ctx->window);
        SDL_Quit();
        SharedLocator::Destroy();
    }
}