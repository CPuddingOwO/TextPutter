#include <TextPutter/Application.hpp>
#include <spdlog/spdlog.h>

#ifdef __WIN32
#include <Windows.h>
int WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
#else
int main() {
#endif

    // 重定向标准输出和标准错误
//    freopen("nul", "w", stdout);
//    freopen("nul", "w", stderr);

    spdlog::set_level(spdlog::level::info);
    spdlog::set_pattern("[%H:%M:%S.%e]%^[%L] %v%$");

    spdlog::info("████████╗███████╗██╗  ██╗████████╗██████╗ ██╗   ██╗████████╗████████╗███████╗██████╗");
    spdlog::info("╚══██╔══╝██╔════╝╚██╗██╔╝╚══██╔══╝██╔══██╗██║   ██║╚══██╔══╝╚══██╔══╝██╔════╝██╔══██╗");
    spdlog::info("   ██║   █████╗   ╚███╔╝    ██║   ██████╔╝██║   ██║   ██║      ██║   █████╗  ██████╔╝");
    spdlog::info("   ██║   ██╔══╝   ██╔██╗    ██║   ██╔═══╝ ██║   ██║   ██║      ██║   ██╔══╝  ██╔══██╗");
    spdlog::info("   ╚═╝   ╚══════╝╚═╝  ╚═╝   ╚═╝   ╚═╝      ╚═════╝    ╚═╝      ╚═╝   ╚══════╝╚═╝  ╚═╝");

    tp::Application app;
    app.run();
    return 0;
}
