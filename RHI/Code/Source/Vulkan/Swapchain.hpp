/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : Swapchain.hpp
    
    Content : Implementation of Swapchain. A part of Vulkan backend

=================================================*/

#pragma once

#include "RHI/Swapchain.hpp"

#include "Device.hpp"

namespace rhi::vulkan {
    class Swapchain final : public rhi::Swapchain {
    public:
        explicit Swapchain(rhi::vulkan::Device& device);
        ~Swapchain();

        RHI_NODISCARD rhi::TextureHandle Acquire() override;
        void                             Present() override;

        void Resize(uint32_t width, uint32_t height) override;

        RHI_NODISCARD uint32_t getWidth() const override;
        RHI_NODISCARD uint32_t getHeight() const override;

    private:
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats);
        VkPresentModeKHR   chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& available_present_modes);
        VkExtent2D         chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    private:
        rhi::vulkan::Device& m_Device;

        VkSwapchainKHR     m_Swapchain;
        VkSurfaceFormatKHR m_SwapchainFormat;
        bool               m_SwapchainMutableFormatSupported = false;

        struct SwapChainImage {
            VkImage              image;
            nvrhi::TextureHandle nvrhi_handle;
            rhi::TextureHandle   rhi_handle;
        };

        uint32_t m_Index = -1;

        std::vector<SwapChainImage> m_Images;
        VkFormat                    m_SwapchainImageFormat;
        VkExtent2D                  m_SwapchainExtent{};
    };
} // namespace rhi::vulkan
