/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : Device.hpp
    
    Content : Interface of Device

=================================================*/

#pragma once

#include <memory>

#include "Common/Attributes.hpp"
#include "Common/Resource.hpp"
#include "CommandList.hpp"
#include "Swapchain.hpp"

namespace rhi {
    class Device {
    public:
        Device()          = default;
        virtual ~Device() = default;

        virtual RHI_NODISCARD std::unique_ptr<rhi::CommandList> CreateCommandList()         = 0;
        virtual RHI_NODISCARD std::unique_ptr<rhi::Swapchain> CreateSwapchain()             = 0;
        virtual void                                          Submit(rhi::CommandList* cmd) = 0;

        virtual RHI_NODISCARD void* CreateBackendTexture(const rhi::TextureDesc& desc) = 0;
        virtual void                DestroyBackendTexture(void* backend_handle)        = 0;
    };

    /*class TempDevice final : public Device {
    public:
        TempDevice()  = default;
        ~TempDevice() = default;

        RHI_NODISCARD std::unique_ptr<rhi::CommandList> CreateCommandList() { return std::make_unique<rhi::TempCommandList>(); }
        RHI_NODISCARD std::unique_ptr<rhi::Swapchain> CreateSwapchain() { return std::make_unique<rhi::TempSwapchain>(); }
        void                                          Submit(rhi::CommandList* cmd) {}

        RHI_NODISCARD void* CreateBackendTexture(const rhi::TextureDesc& desc) { return nullptr; }
        void                DestroyBackendTexture(void* backend_handle) {}
    };*/
} // namespace rhi
