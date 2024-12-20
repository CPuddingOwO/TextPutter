#include <TextPutter/Application.hpp>
#include <spdlog/spdlog.h>

int main() {
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
