/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : CommandList.cpp
    
    Content : Implementation of Command List. A part of Vulkan backend

=================================================*/

#include "CommandList.hpp"

void rhi::vulkan::CommandList::BeginFrame() {
    m_NVRHICommandList->open();
}

void rhi::vulkan::CommandList::EndFrame() {
    m_NVRHICommandList->close();
}

void rhi::vulkan::CommandList::setPipeline(const Pipeline* pipeline) {
}

void rhi::vulkan::CommandList::setVertexBuffer(const Buffer* buffer) {
}

void rhi::vulkan::CommandList::setIndexBuffer(const Buffer* buffer) {
}

void rhi::vulkan::CommandList::setRenderTarget(TextureHandle handle) {
}

void rhi::vulkan::CommandList::DrawIndexed(uint32_t instance_count, uint32_t first_index, uint32_t first_instance, uint32_t first_vertex, uint32_t vertex_count) {
    nvrhi::DrawArguments args{};
    args.setInstanceCount(instance_count);
    args.setStartIndexLocation(first_index);
    args.setStartInstanceLocation(first_instance);
    args.setStartVertexLocation(first_vertex);
    args.setVertexCount(vertex_count);

    m_NVRHICommandList->drawIndexed(args);
}