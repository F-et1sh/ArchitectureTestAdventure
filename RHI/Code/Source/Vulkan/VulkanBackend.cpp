/*=================================================

    Copyright (C) 2025 Farrakh.  All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : VulkanBackend.cpp
    
    Content : Implementation of Vulkan classes

=================================================*/

#include "VulkanBackend.hpp"

#include <nvrhi/vulkan.h>

struct rhi::vulkan::Device::Impl {
    nvrhi::vulkan::DeviceHandle m_Device;
    VkQueue                     m_GraphicsQueue;

    // swapchain, fences, etc..
};

rhi::vulkan::Device::Device() {
    m_Impl = new Impl();
}

rhi::vulkan::Device::~Device() {
    delete m_Impl;
}

void rhi::vulkan::Device::BeginFrame() {
    // m_Impl->m_Device...
}

void rhi::vulkan::Device::EndFrame() {
    // m_Impl->m_Device...
}