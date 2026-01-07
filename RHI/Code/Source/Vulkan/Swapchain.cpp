/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : Swapchain.cpp
    
    Content : Implementation of Swapchain. A part of Vulkan backend

=================================================*/

#include "Swapchain.hpp"

#include <iostream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

rhi::vulkan::Swapchain::Swapchain(rhi::vulkan::Device& device) : m_Device(device) {}

rhi::vulkan::Swapchain::~Swapchain() {

}

RHI_NODISCARD rhi::TextureHandle rhi::vulkan::Swapchain::Acquire() {
    auto& device      = m_Device.m_Context.device;
    auto& frame = m_Device.m_Frames[m_Device.m_FrameIndex];

    vkWaitForFences(device, 1, &frame.in_flight, VK_TRUE, std::numeric_limits<uint64_t>::max());
    vkResetFences(device, 1, &frame.in_flight);

    VkResult result = vkAcquireNextImageKHR(
        device,
        m_Swapchain,
        std::numeric_limits<uint64_t>::max(),
        frame.image_available,
        VK_NULL_HANDLE,
        &m_Index);

    assert(result == VK_SUCCESS);

    m_Device.m_NVRHIDevice->queueWaitForSemaphore(
        nvrhi::CommandQueue::Graphics,
        frame.image_available,
        0);

    return m_Images[m_Index].rhi_handle;
}

void rhi::vulkan::Swapchain::Present() {
    auto& device      = m_Device.m_Context.device;

    auto& frame = m_Device.m_Frames[m_Device.m_FrameIndex];

    VkPresentInfoKHR info{};
    info.waitSemaphoreCount = 1;
    info.pWaitSemaphores    = &frame.render_finished;
    info.swapchainCount     = 1;
    info.pSwapchains        = &m_Swapchain;
    info.pImageIndices      = &m_Index;

    vkQueuePresentKHR(m_Device.m_Context.present_queue, &info);
}

void rhi::vulkan::Swapchain::Resize(uint32_t width, uint32_t height) {
}

RHI_NODISCARD uint32_t rhi::vulkan::Swapchain::getWidth() const {
    return 0;
}

RHI_NODISCARD uint32_t rhi::vulkan::Swapchain::getHeight() const {
    return 0;
}