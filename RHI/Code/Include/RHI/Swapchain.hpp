/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : Swapchain.hpp
    
    Content : Interface of Swapchain

=================================================*/

#pragma once

#include "Common/Resource.hpp"
#include "Common/Attributes.hpp"

namespace rhi {
    class Swapchain {
    public:
        Swapchain()          = default;
        virtual ~Swapchain() = default;

        virtual RHI_NODISCARD rhi::TextureHandle Acquire() = 0;
        virtual void                             Present() = 0;

        virtual void Resize(uint32_t width, uint32_t height) = 0;

        virtual RHI_NODISCARD uint32_t getWidth() const  = 0;
        virtual RHI_NODISCARD uint32_t getHeight() const = 0;
    };
} // namespace rhi
