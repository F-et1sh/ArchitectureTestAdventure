/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : CommandList.hpp
    
    Content : command list

=================================================*/

#pragma once

#include "Common/Resource.hpp"

namespace rhi {
    class Buffer;
    class Pipeline;

    class CommandList {
    public:
        CommandList()          = default;
        virtual ~CommandList() = default;

        virtual void BeginFrame() = 0;
        virtual void EndFrame()   = 0;

        virtual void setPipeline(const Pipeline* pipeline) = 0;

        virtual void setVertexBuffer(const Buffer* buffer) = 0;
        virtual void setIndexBuffer(const Buffer* buffer)  = 0;

        virtual void setRenderTarget(TextureHandle handle) = 0;

        virtual void DrawIndexed(uint32_t instance_count, uint32_t first_index, uint32_t first_instance, uint32_t first_vertex, uint32_t vertex_count) = 0;

    private:
    };
} // namespace rhi
