/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : Renderer.cpp
    
    Content : Renderer aka RenderSystem of the engine

=================================================*/

#include "pch.hpp"
#include "Renderer.hpp"

void ata::Renderer::Release() {
    m_ResourceManager->Release();
}

void ata::Renderer::Initialize() {
    m_Device          = rhi::DeviceManager::Create(rhi::GraphicsAPI::VK, m_Window.getNativeHandle());
    m_Swapchain       = m_Device->CreateSwapchain();
    m_ResourceManager = std::make_unique<rhi::ResourceManager>(*m_Device);
    m_CommandList     = m_Device->CreateCommandList();

    this->TestLoop();
}

void ata::Renderer::TestLoop() {
    uint32_t instance_count = 1;
    uint32_t first_index    = 0;
    uint32_t first_instance = 0;
    uint32_t first_vertex   = 0;
    uint32_t vertex_count   = 3;
    
    rhi::Swapchain::BackbufferIndex backbuffer = m_Swapchain->Acquire();

    m_CommandList->BeginFrame();

    m_CommandList->setRenderTarget(backbuffer);
    //m_CommandList->setPipeline(...);
    //m_CommandList->setVertexBuffer(...);
    //m_CommandList->setIndexBuffer(...);
    m_CommandList->DrawIndexed(instance_count, first_index, first_instance, first_vertex, vertex_count);

    m_CommandList->EndFrame();

    m_Device->Submit(m_CommandList.get());

    m_Swapchain->Present();
}
