/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : DeviceManager.cpp
    
    Content : DeviceManager

=================================================*/

#include "RHI/DeviceManager.hpp"
#include "Source/Vulkan/VulkanBackend.hpp"

#include "Source/Common/Logging.hpp"

RHI_NODISCARD std::unique_ptr<rhi::Device> rhi::DeviceManager::Create(GraphicsAPI backend, void* window_handle) {
    switch (backend) {
        case GraphicsAPI::VK: {
            std::unique_ptr<rhi::vulkan::Device> device = std::make_unique<rhi::vulkan::Device>();
            device->InitializeForPresentation(window_handle);
            return device;
            break;
        }
        default:
            rhi::logging::error("Unknown Graphics API : %i \nFallbacks to Vulkan", backend);

            std::unique_ptr<rhi::vulkan::Device> device = std::make_unique<rhi::vulkan::Device>();
            device->InitializeForPresentation(window_handle);
            return device;
    }
}
