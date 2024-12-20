#pragma once
#include <string>
#include <vector>
#include <imgui.h>

namespace tp {

    struct Data final{
        std::string weekDay;

        ImColor bbColor = ImColor(255, 0, 0, 255);
        ImColor lcColor = ImColor(255, 0, 0, 255);
        ImColor ncColor = ImColor(255, 0, 0, 255);
        ImColor background = ImColor(255, 0, 0, 255);

        std::string Blackboard;
        std::string LoonCleaner;
        std::string NightCleaner;
    };
}