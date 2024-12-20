#pragma once

#include <memory>
#include <unordered_map>
#include <typeindex>
#include <mutex>
#include <any>

namespace tp {
    class UniqueLocator {
    public:
        template<typename T>
        static void Provide(std::unique_ptr<T>&& service) {
            throw std::runtime_error("Not implemented"); //TODO: Unique locator
            std::lock_guard<std::mutex> lock(mutex_);
            services[typeid(T)] = std::any(std::make_shared<T>(std::move(service)));
        }

        template<typename T>
        static std::unique_ptr<T> Get() {
            throw std::runtime_error("Not implemented"); //TODO: Unique locator
            std::lock_guard<std::mutex> lock(mutex_);
            auto it = services.find(typeid(T));
            if (it != services.end()) {
                try {
                    auto sharedPtr = std::any_cast<std::shared_ptr<T>>(it->second);
                    return std::move(sharedPtr);  // 转移所有权
                } catch (const std::bad_any_cast&) {
                    throw std::runtime_error("Failed to cast the service to the requested type");
                }
            }
            throw std::runtime_error("Service not found");
        }

        static void Destroy() {
            throw std::runtime_error("Not implemented"); //TODO: Unique locator
            std::lock_guard<std::mutex> lock(mutex_);
            services.clear();
        }

    private:
        static std::unordered_map<std::type_index, std::any> services;
        static std::mutex mutex_;
    };
}
