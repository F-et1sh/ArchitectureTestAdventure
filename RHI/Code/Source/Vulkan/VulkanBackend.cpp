#include "VulkanBackend.hpp"

// defenition of rhi::DeviceManager's member, not rhi::vulkan::DeviceManager
rhi::DeviceManager* rhi::DeviceManager::CreateVK() {
    return new rhi::vulkan::DeviceManager();
}

