/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : Misc.hpp
    
    Content : some useful structs. A part of Vulkan backend

=================================================*/

#pragma once

#include <nvrhi/vulkan.h>

namespace rhi::vulkan {
    struct VulkanContext {
    public:
        VkInstance               instance;
        VkPhysicalDevice         physical_device;
        VkDevice                 device;
        VkQueue                  graphics_queue;
        VkQueue                  present_queue;
        VkQueue                  compute_queue;
        VkQueue                  transfer_queue;

        VulkanContext()  = default;
        ~VulkanContext() = default;
    };

    struct FrameSync {
    public:
        VkSemaphore             image_available;
        VkSemaphore             render_finished;
        VkFence                 in_flight;
        nvrhi::EventQueryHandle frame_complete;

        FrameSync()  = default;
        ~FrameSync() = default;
    };
} // namespace rhi::vulkan
