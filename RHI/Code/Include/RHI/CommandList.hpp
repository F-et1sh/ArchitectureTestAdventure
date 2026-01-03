/*=================================================

    Copyright (C) 2025 Farrakh.  All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : CommandList.hpp
    
    Content : command list

=================================================*/

#pragma once

#include <memory>

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

        virtual void DrawIndexed(uint32_t index_count,
                                 uint32_t first_index   = 0,
                                 uint32_t vertex_offset = 0) = 0;

    private:
    };
} // namespace rhi
