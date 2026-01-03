/*=================================================

    Copyright (C) 2025 Farrakh.  All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : DeviceManager.cpp
    
    Content : Interface of DeviceManager

=================================================*/

#include "RHI/RHI.hpp"

rhi::DeviceManager* rhi::DeviceManager::Create(GraphicsAPI backend) {
    switch (backend) {
        case GraphicsAPI::VK:
            return rhi::DeviceManager::CreateVK();
        default:
            // TODO : Add logging //log::error("DeviceManager::Create : Unsupported Graphics API (%d)", backend);
            return nullptr;
    }
}
