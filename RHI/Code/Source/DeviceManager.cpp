/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : DeviceManager.cpp
    
    Content : DeviceManager

=================================================*/

#include "RHI/RHI.hpp"
#include "Vulkan/VulkanBackend.hpp"

RHI_NODISCARD std::unique_ptr<rhi::Device> rhi::DeviceManager::Create(GraphicsAPI backend, void* window_handle) {
    switch (backend) {
        case GraphicsAPI::VULKAN:
            return std::make_unique<rhi::vulkan::Device>(window_handle);
        default:
            // TODO : ERROR LOGGING
            return nullptr;
    }
}
