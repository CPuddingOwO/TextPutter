//#include <TextPutter/UniqueLocator.hpp>
#include <TextPutter/SharedLocator.hpp>

namespace tp {
//    std::unordered_map<std::type_index, std::any>                   tp::UniqueLocator::services;
//    std::mutex                                                      tp::UniqueLocator::mutex_;

    std::unordered_map<std::type_index, std::shared_ptr<void>>      tp::SharedLocator::services;
    std::mutex                                                      tp::SharedLocator::mutex_;
}