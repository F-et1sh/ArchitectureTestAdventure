/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : DeviceManager.hpp
    
    Content : device manager

=================================================*/

#pragma once

#include "Common/Resource.hpp"
#include "Device.hpp"

namespace rhi {
    class DeviceManager {
    public:
        DeviceManager()  = default;
        ~DeviceManager() = default;

        static RHI_NODISCARD std::unique_ptr<Device> Create(GraphicsAPI backend, void* window_handle);
    };
} // namespace rhi
