/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : Swapchain.cpp
    
    Content : Implementation of Swapchain. A part of Vulkan backend

=================================================*/

#include "Swapchain.hpp"

#include "Misc.hpp"

#include <iostream>
#include <algorithm>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

rhi::vulkan::Swapchain::Swapchain(rhi::vulkan::Device& device) : m_Device(device) {
    auto& swapchain_support = m_Device.m_SwapchainSupportDetails;

    VkSurfaceFormatKHR surface_format = chooseSwapSurfaceFormat(swapchain_support.formats);
    VkPresentModeKHR   present_mode   = chooseSwapPresentMode(swapchain_support.present_modes);
    VkExtent2D         extent         = chooseSwapExtent(swapchain_support.capabilities);

    uint32_t image_count = swapchain_support.capabilities.minImageCount + 1;
    if (swapchain_support.capabilities.maxImageCount > 0 && image_count > swapchain_support.capabilities.maxImageCount) {
        image_count = swapchain_support.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR create_info{};
    create_info.sType   = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_info.surface = m_Device.m_Surface;

    create_info.minImageCount    = image_count;
    create_info.imageFormat      = surface_format.format;
    create_info.imageColorSpace  = surface_format.colorSpace;
    create_info.imageExtent      = extent;
    create_info.imageArrayLayers = 1;
    create_info.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    auto&    indices                = m_Device.m_QueueFamilyIndices;
    uint32_t queue_family_indices[] = { indices.graphics_family.value(), indices.present_family.value() };

    if (indices.graphics_family != indices.present_family) {
        create_info.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
        create_info.queueFamilyIndexCount = 2;
        create_info.pQueueFamilyIndices   = queue_family_indices;
    }
    else {
        create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    create_info.preTransform   = swapchain_support.capabilities.currentTransform;
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    create_info.presentMode    = present_mode;
    create_info.clipped        = VK_TRUE;


    if (vkCreateSwapchainKHR(m_Device.m_Context.device, &create_info, nullptr, &m_Swapchain) != VK_SUCCESS) {
        std::cerr << "ERROR : Failed to create swapchain" << std::endl;
    }

    std::vector<VkImage> swapchain_images{}; // TODO : Handle this

    vkGetSwapchainImagesKHR(m_Device.m_Context.device, m_Swapchain, &image_count, nullptr);
    swapchain_images.resize(image_count);
    vkGetSwapchainImagesKHR(m_Device.m_Context.device, m_Swapchain, &image_count, swapchain_images.data());

    m_SwapchainImageFormat = surface_format.format;
    m_SwapchainExtent      = extent;
}

rhi::vulkan::Swapchain::~Swapchain() {
}

RHI_NODISCARD rhi::TextureHandle rhi::vulkan::Swapchain::Acquire() {
    auto& device = m_Device.m_Context.device;
    auto& frame  = m_Device.m_Frames[m_Device.m_FrameIndex];

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
    auto& device = m_Device.m_Context.device;

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

VkSurfaceFormatKHR rhi::vulkan::Swapchain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats) {
    for (const auto& available_format : available_formats) {
        if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return available_format;
        }
    }

    return available_formats[0];
}

VkPresentModeKHR rhi::vulkan::Swapchain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& available_present_modes) {
    for (const auto& available_present_mode : available_present_modes) {
        if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return available_present_mode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D rhi::vulkan::Swapchain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    }

    int width  = 0;
    int height = 0;

    //m_Device.w

    VkExtent2D actual_extent = {
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height)
    };

    actual_extent.width  = std::clamp(actual_extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actual_extent.height = std::clamp(actual_extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    return actual_extent;
}
