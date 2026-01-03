/*=================================================

    Copyright (C) 2025 Farrakh.  All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : VulkanBackend.hpp
    
    Content : Implementation of Vulkan classes

=================================================*/

#pragma once

#include "RHI/Device.hpp"

namespace rhi::vulkan {
    class Device final : public rhi::Device {
    public:
        Device();
        ~Device();

        void BeginFrame() override;
        void EndFrame() override;

    private:
        struct Impl;
        Impl* m_Impl;
    };
} // namespace rhi::vulkan
