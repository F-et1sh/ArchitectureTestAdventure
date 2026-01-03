/*=================================================

    Copyright (C) 2025 Farrakh.  All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : Device.hpp
    
    Content : Interface of Device

=================================================*/

#pragma once

namespace rhi {
    class Device {
    public:
        Device()          = default;
        virtual ~Device() = default;

        virtual void BeginFrame() = 0;
        virtual void EndFrame()   = 0;
    };
} // namespace rhi
