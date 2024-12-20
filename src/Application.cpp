#include <TextPutter/Application.hpp>
#include <TextPutter/SharedLocator.hpp>
#include <TextPutter/io/SDL.hpp>
#include <TextPutter/io/State.hpp>
#include <TextPutter/io/Data.hpp>

#include <imgui.h>
#include <imgui_internal.h>
#include <SDL3/SDL.h>
#include <imgui/imgui_impl_sdlrenderer3.h>
#include <imgui/imgui_impl_sdl3.h>

#include <stdexcept>
#include <spdlog/spdlog.h>
//#include <yaml-cpp/yaml.h>
#include <mini/ini.h>

namespace tp {

    Application::Application() {
        init();
    }

    Application::~Application() {
        quit();
    }

    ImColor HexToImColor(const std::string& hex) {
        int r = 0, g = 0, b = 0, a = 0;

        if (hex[0] == '#') {
            std::string ss = hex.substr(1);
            if (ss.size() == 8) {
                r = std::stoi(ss.substr(0, 2), nullptr, 16);  // 红色
                g = std::stoi(ss.substr(2, 2), nullptr, 16);  // 绿色
                b = std::stoi(ss.substr(4, 2), nullptr, 16);  // 蓝色
                a = std::stoi(ss.substr(6, 2), nullptr, 16);  // Alpha（透明度）
            } else if (ss.size() == 6) {
                r = std::stoi(ss.substr(0, 2), nullptr, 16);  // 红色
                g = std::stoi(ss.substr(2, 2), nullptr, 16);  // 绿色
                b = std::stoi(ss.substr(4, 2), nullptr, 16);  // 蓝色
            }
        }

        return {r, g, b, a};  // 返回ImColor对象
    }

    void Application::init() {
        { // Init Contexts
            auto ctx = std::make_shared<tp::SDL_Context>();
            ctx->window_title = "TextPutter";
            ctx->window_size = glm::ivec2(640, 170);
            SharedLocator::Provide<tp::SDL_Context>(ctx);

            auto state = std::make_shared<tp::State>();
            SharedLocator::Provide<tp::State>(state);

            auto students_task = std::make_shared<tp::Data>();
            SharedLocator::Provide<tp::Data>(students_task);
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
            SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_BORDERLESS_BOOLEAN, true); // Borderless
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
            ImGuiIO &io = ImGui::GetIO();
            (void) io;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

            //io.Fonts->AddFontDefault();
            // TODO: IMGUI的问题，字体不是矢量，而是UV图，字体越大，内存占用越大，放大会糊
            io.Fonts->AddFontFromFileTTF(R"(c:\Windows\Fonts\msyh.ttc)", 48.0f, nullptr,
                                         io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
//            io.Fonts->AddFontFromFileTTF(R"(c:\Windows\Fonts\msyh.ttc)", 32.0f);
//            io.Fonts->AddFontFromFileTTF(R"(c:\Windows\Fonts\segoeui.ttf)", 48.0f);
            //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
            //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
            //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
//            ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
            //IM_ASSERT(font != nullptr);

            ImGui::StyleColorsLight();
            ImGui_ImplSDL3_InitForSDLRenderer(sdl_ctx->window, sdl_ctx->renderer);
            ImGui_ImplSDLRenderer3_Init(sdl_ctx->renderer);
        }
        { // 读取数据
            auto task = SharedLocator::Get<tp::Data>();
            auto now = std::chrono::system_clock::now();
            auto weekday = std::chrono::floor<std::chrono::days>(now).time_since_epoch() / std::chrono::days(1) % 7;


            switch (weekday) {
                case 0: task->weekDay = "Sun";break;  // Sunday
                case 1: task->weekDay = "Mon";break;  // Monday
                case 2: task->weekDay = "Tue";break;  // Tuesday
                case 3: task->weekDay = "Wed";break;  // Wednesday
                case 4: task->weekDay = "Thu";break;  // Thursday
                case 5: task->weekDay = "Fri";break;  // Friday
                case 6: task->weekDay = "Sat";break;  // Saturday
                default: task->weekDay = "";
            }

            mINI::INIFile file("data.ini");
            mINI::INIStructure ini;
            file.read(ini);

            task->bbColor = HexToImColor(ini["Colors"]["BBColor"]);
            task->lcColor = HexToImColor(ini["Colors"]["LCColor"]);
            task->ncColor = HexToImColor(ini["Colors"]["NCColor"]);
            task->background = HexToImColor(ini["Colors"]["BackgroundColor"]);

            task->LoonCleaner = ini["LoonClearner"][task->weekDay];
            task->NightCleaner = ini["NightClearner"][task->weekDay];
            task->Blackboard = ini["BrashBlackBoard"][task->weekDay];


//            YAML::Node data = YAML::LoadFile("data.yml");
//            task->bbColor = HexToImColor(data["BBColor"].as<std::string>());
//            task->lcColor = HexToImColor(data["LCColor"].as<std::string>());
//            task->ncColor = HexToImColor(data["NCColor"].as<std::string>());
//            task->background = HexToImColor(data["BackgroundColor"].as<std::string>());
//
//            task->Blackboard = data["BrashBlackBoard"][0][task->weekDay.c_str()].as<std::string>();
//            task->LoonCleaner = data["LoonClearner"][0][task->weekDay.c_str()].as<std::string>();
//            task->NightCleaner = data["NightClearner"][0][task->weekDay.c_str()].as<std::string>();
        }
    }

    void Application::run() {
        auto app_state = SharedLocator::Get<tp::State>();
        auto sdl_ctx = SharedLocator::Get<tp::SDL_Context>();
        auto task = SharedLocator::Get<tp::Data>();

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
                {
                    ImGuiIO imgui_io = ImGui::GetIO();
                    ImGuiStyle& style = ImGui::GetStyle();
                    style.WindowBorderSize = 0;
                    ImGuiWindowFlags flags = 0;
                    flags |= ImGuiWindowFlags_NoTitleBar;
                    flags |= ImGuiWindowFlags_NoCollapse;
                    flags |= ImGuiWindowFlags_NoMove;
                    flags |= ImGuiWindowFlags_NoResize;
                    flags |= ImGuiWindowFlags_NoScrollbar;
                    flags |= ImGuiWindowFlags_NoBackground;
                    flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
                    flags |= ImGuiWindowFlags_NoFocusOnAppearing;

                    bool open = true;

                    ImGuiViewport* viewport = ImGui::GetMainViewport();
                    ImGui::SetNextWindowSize(viewport->Size);
                    ImGui::SetNextWindowPos(viewport->Pos);

                    ImGui::Begin("#App#Main", &open, flags);
                    {
                        ImGui::TextColored(task->bbColor, "擦黑板: ");ImGui::SameLine();
                        ImGui::TextColored(task->bbColor, "%s", task->Blackboard.c_str());

                        ImGui::SameLine(ImGui::GetWindowWidth() - ImGui::CalcTextSize("XX  ").x);
                        if (ImGui::Button("XX")) {
                            app_state->isRunning = false;
                        }
                        ImGui::SameLine(ImGui::GetWindowWidth() - ImGui::CalcTextSize("--  XX  ").x);
                        if (ImGui::Button("--")) {
                            SDL_MinimizeWindow(sdl_ctx->window);
                        }

                        ImGui::TextColored(task->lcColor, "中午卫生: ");ImGui::SameLine();
                        ImGui::TextColored(task->lcColor, "%s", task->LoonCleaner.c_str());
                        ImGui::TextColored(task->ncColor, "晚上卫生: ");ImGui::SameLine();
                        ImGui::TextColored(task->ncColor, "%s", task->NightCleaner.c_str());
                    }
                    ImGui::End();
                }
            }
            ImGui::Render();
//            auto io = ImGui::GetIO();
//            SDL_SetRenderScale(sdl_ctx->renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
            SDL_SetRenderDrawColorFloat(sdl_ctx->renderer, task->background.Value.x, task->background.Value.y, task->background.Value.z, task->background.Value.w);
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