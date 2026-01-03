/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : DeviceManager.hpp
    
    Content : device manager

=================================================*/

#pragma once

#include <memory>
#include "Device.hpp"

namespace rhi {
    enum class GraphicsAPI : uint8_t {
        //D3D11,
        //D3D12,
        VK
    };

    class DeviceManager {
    public:
        DeviceManager()  = default;
        ~DeviceManager() = default;

        static std::unique_ptr<Device> Create(GraphicsAPI backend);
    };
} // namespace rhi
