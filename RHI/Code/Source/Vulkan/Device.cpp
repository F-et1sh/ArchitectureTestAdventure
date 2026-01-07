/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : Device.cpp
    
    Content : Implementation of Device. A part of Vulkan backend

=================================================*/

#include "Device.hpp"

#include "Source/Common/Resource.hpp"

#include "CommandList.hpp"
#include "Swapchain.hpp"

rhi::vulkan::Device::~Device() {
    
}

std::unique_ptr<rhi::CommandList> rhi::vulkan::Device::CreateCommandList() {
    auto cmd = m_NVRHIDevice->createCommandList();
    return std::make_unique<rhi::vulkan::CommandList>(cmd);
}

RHI_NODISCARD std::unique_ptr<rhi::Swapchain> rhi::vulkan::Device::CreateSwapchain(void* window_handle) {
    return std::make_unique<rhi::vulkan::Swapchain>(*this, window_handle);
}

void rhi::vulkan::Device::Submit(rhi::CommandList* cmd) {
    auto& frame  = m_Frames[m_FrameIndex];
    auto* vk_cmd = static_cast<rhi::vulkan::CommandList*>(cmd);

    nvrhi::ICommandList* lists[] = {
        vk_cmd->getNVRHICommandListHandle()
    };

    m_NVRHIDevice->executeCommandLists(
        lists,
        1,
        nvrhi::CommandQueue::Graphics);

    m_FrameIndex = (m_FrameIndex + 1) % m_Frames.size();
}

RHI_NODISCARD void* rhi::vulkan::Device::CreateBackendTexture(const rhi::TextureDesc& desc) {
    nvrhi::TextureHandle handle = m_NVRHIDevice->createTexture(rhi::to_nvrhi(desc));
    return static_cast<void*>(handle.Get());
}

void rhi::vulkan::Device::DestroyBackendTexture(void* backend_handle) {
    if (!backend_handle) return;

    static_cast<nvrhi::ITexture*>(backend_handle)->Release();
    backend_handle = nullptr;
}