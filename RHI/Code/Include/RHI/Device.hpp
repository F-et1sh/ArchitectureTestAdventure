/*=================================================

    Copyright (C) 2025 Farrakh.  All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : Device.hpp
    
    Content : Public header of Device. A part of RHI.

=================================================*/

#pragma once
#include <memory>

namespace rhi {
    enum class GraphicsAPI : uint8_t {
        // D3D11,
        // D3D12,
        VULKAN
    };

    class Device {
    public:
        Device()          = default;
        virtual ~Device() = default;

        virtual void BeginFrame() = 0;
        virtual void EndFrame()   = 0;

        static std::unique_ptr<Device> Create(GraphicsAPI backend);

    private:
    };
} // namespace rhi
