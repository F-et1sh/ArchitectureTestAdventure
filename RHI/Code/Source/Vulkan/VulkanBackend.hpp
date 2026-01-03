/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : VulkanBackend.hpp
    
    Content : Implementation of Vulkan classes

=================================================*/

#pragma once

#include "RHI/Common/Resource.hpp"

#include "RHI/Device.hpp"
#include "RHI/CommandList.hpp"
#include "RHI/Swapchain.hpp"

#include <nvrhi/nvrhi.h>

namespace rhi::vulkan {
    class Device final : public rhi::Device {
    public:
        Device();
        ~Device();

        std::unique_ptr<rhi::CommandList> CreateCommandList() override;
        void                              Submit(rhi::CommandList* cmd) override;

    private:
        struct Impl;
        Impl* m_Impl;
    };

    class CommandList final : public rhi::CommandList {
    public:
        explicit CommandList(nvrhi::CommandListHandle handle) : m_NVRHICommandList(handle) {}
        ~CommandList() = default;

        void BeginFrame() override;
        void EndFrame() override;

        void setPipeline(const Pipeline* pipeline) override;

        void setVertexBuffer(const Buffer* buffer) override;
        void setIndexBuffer(const Buffer* buffer) override;

        void DrawIndexed(uint32_t instance_count, uint32_t first_index, uint32_t first_instance, uint32_t first_vertex, uint32_t vertex_count) override;

    private:
        // No PIMPL because it's too small
        nvrhi::CommandListHandle m_NVRHICommandList;
    };

    class Swapchain final : public rhi::Swapchain {
    public:
        Swapchain();
        ~Swapchain();

        rhi::TextureHandle Acquire() override;
        void               Present() override;

        void Resize(uint32_t width, uint32_t height) override;

        uint32_t getWidth() const override;
        uint32_t getHeight() const override;

    private:
        struct Impl;
        Impl* m_Impl;
    };
} // namespace rhi::vulkan
