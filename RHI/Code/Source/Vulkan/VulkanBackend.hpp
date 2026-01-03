/*=================================================

    Copyright (C) 2025 Farrakh.  All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : VulkanBackend.hpp
    
    Content : Implementation of Vulkan classes

=================================================*/

#pragma once

#include "RHI/Device.hpp"
#include "RHI/CommandList.hpp"

#include <unordered_set>
#include <string>
#include <queue>

#include <nvrhi/vulkan.h>
#include <vulkan/vulkan.hpp>

namespace rhi::vulkan {
    class Device final : public rhi::Device {
    public:
        Device();
        ~Device();

        void BeginFrame() override;
        void EndFrame() override;

        std::unique_ptr<rhi::CommandList> CreateCommandList() override;

    private:
        struct Impl;
        Impl* m_Impl;
    };

    class CommandList final : public rhi::CommandList {
    public:
        CommandList(nvrhi::CommandListHandle handle) : m_NVRHICommandList(handle) {}
        ~CommandList() = default;

        void BeginFrame() override;
        void EndFrame() override;

        void setPipeline(const Pipeline* pipeline) override;

        void setVertexBuffer(const Buffer* buffer) override;
        void setIndexBuffer(const Buffer* buffer) override;

        void DrawIndexed(uint32_t index_count, uint32_t first_index, uint32_t vertex_offset) override;

    private:
        // No PIMPL because it's too small
        nvrhi::CommandListHandle m_NVRHICommandList;
    };
} // namespace rhi::vulkan
