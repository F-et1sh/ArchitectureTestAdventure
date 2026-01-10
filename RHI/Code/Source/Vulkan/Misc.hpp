/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : Misc.hpp
    
    Content : some useful structs. A part of Vulkan backend

=================================================*/

#pragma once

#include <optional>
#include <vector>

#include <nvrhi/vulkan.h>

namespace rhi::vulkan {
    struct VulkanContext {
    public:
        VkInstance       instance;
        VkPhysicalDevice physical_device;
        VkDevice         device;
        VkQueue          graphics_queue;
        VkQueue          present_queue;
        VkQueue          compute_queue;
        VkQueue          transfer_queue;

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

    struct QueueFamilyIndices {
    public:
        std::optional<uint32_t> graphics_family;
        std::optional<uint32_t> present_family;
        std::optional<uint32_t> compute_family;
        std::optional<uint32_t> transfer_family;

        RHI_NODISCARD bool is_complete() const noexcept {
            return graphics_family.has_value() && present_family.has_value() &&
                   compute_family.has_value() && transfer_family.has_value();
        }

        QueueFamilyIndices()  = default;
        ~QueueFamilyIndices() = default;
    };

    struct SwapchainSupportDetails {
    public:
        VkSurfaceCapabilitiesKHR        capabilities{};
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR>   present_modes;

        SwapchainSupportDetails()  = default;
        ~SwapchainSupportDetails() = default;
    };

} // namespace rhi::vulkan
