/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : CommandList.hpp
    
    Content : Implementation of Command List. A part of Vulkan backend

=================================================*/

#pragma once

#include "RHI/CommandList.hpp"
#include "Common/Attributes.hpp"

#include <nvrhi/vulkan.h>

namespace rhi::vulkan {
	class CommandList final : public rhi::CommandList {
    public:
        explicit CommandList(nvrhi::CommandListHandle handle) : m_NVRHICommandList(handle) {}
        ~CommandList() = default;

        void BeginFrame() override;
        void EndFrame() override;

        void setPipeline(const Pipeline* pipeline) override;

        void setVertexBuffer(const Buffer* buffer) override;
        void setIndexBuffer(const Buffer* buffer) override;

        void setRenderTarget(TextureHandle handle) override;

        void DrawIndexed(uint32_t instance_count, uint32_t first_index, uint32_t first_instance, uint32_t first_vertex, uint32_t vertex_count) override;

        RHI_NODISCARD nvrhi::CommandListHandle getNVRHICommandListHandle() const noexcept { return m_NVRHICommandList; }

    private:
        nvrhi::CommandListHandle m_NVRHICommandList;
    };
}