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
    m_Device          = rhi::DeviceManager::Create(rhi::GraphicsAPI::VULKAN);
    m_ResourceManager = std::make_unique<rhi::ResourceManager>(*m_Device);
    m_Swapchain       = m_Device->CreateSwapchain();
}
