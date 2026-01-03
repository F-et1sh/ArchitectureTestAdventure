/*=================================================

    Copyright (C) 2025 Farrakh.  All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : VulkanBackend.cpp
    
    Content : Implementation of Vulkan classes

=================================================*/

#include "VulkanBackend.hpp"

// defenition of rhi::DeviceManager's member, not rhi::vulkan::DeviceManager
rhi::DeviceManager* rhi::DeviceManager::CreateVK() {
    return new rhi::vulkan::DeviceManager();
}

