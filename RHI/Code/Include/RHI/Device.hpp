/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : Device.hpp
    
    Content : Interface of Device

=================================================*/

#pragma once

#include "CommandList.hpp"

namespace rhi {
    class Device {
    public:
        Device()          = default;
        virtual ~Device() = default;

        virtual std::unique_ptr<rhi::CommandList> CreateCommandList() = 0;
        virtual void Submit(rhi::CommandList* cmd) = 0;
    };
} // namespace rhi
