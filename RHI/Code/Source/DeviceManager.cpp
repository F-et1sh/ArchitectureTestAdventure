#include "RHI/RHI.hpp"

rhi::DeviceManager* rhi::DeviceManager::Create(GraphicsAPI backend) {
    switch (backend) {
        case GraphicsAPI::VK:
            return rhi::DeviceManager::CreateVK();
        default:
            // TODO : Add logging //log::error("DeviceManager::Create : Unsupported Graphics API (%d)", backend);
            return nullptr;
    }
}
