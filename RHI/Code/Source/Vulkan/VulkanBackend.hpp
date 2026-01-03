/*=================================================

    Copyright (C) 2025 Farrakh.  All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : VulkanBackend.hpp
    
    Content : Implementation of Vulkan classes

=================================================*/

#pragma once

#include "RHI/RHI.hpp"

namespace rhi::vulkan {
    class DeviceManager final : public rhi::DeviceManager {
    public:
        DeviceManager()  = default;
        ~DeviceManager() = default;

    private:

    };
} // namespace rhi::vulkan
