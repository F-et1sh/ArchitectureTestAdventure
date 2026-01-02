/*=================================================

    Copyright (C) 2025 Farrakh.  All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : Device.cpp
    
    Content : Public header of Device. A part of RHI.

=================================================*/

#include "RHI/Device.hpp"
#include "Vulkan/Device.hpp"

using namespace rhi;

std::unique_ptr<Device> Device::Create(GraphicsAPI backend) {
    switch (backend) {
        case GraphicsAPI::VULKAN:
            return std::make_unique<rhi::vulkan::Device>();
            break;
        default:
            return nullptr;
            break;
    }


}
