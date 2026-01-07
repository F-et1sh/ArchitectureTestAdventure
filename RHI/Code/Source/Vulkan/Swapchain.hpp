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
        void CreateSwapchain();
        void CreateImageViews();
        void CreateColorResources();
        void CreateDepthResources();

    private:
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats);
        VkPresentModeKHR   chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& available_present_modes);
        VkExtent2D         chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    private:
        rhi::vulkan::Device& m_Device;

        VkSwapchainKHR     m_Swapchain;
        VkSurfaceFormatKHR m_Format;
        bool               m_MutableFormatSupported = false;

        struct SwapchainImage {
            VkImage              image;
            nvrhi::TextureHandle nvrhi_handle;
            rhi::TextureHandle   rhi_handle;
        };

        uint32_t m_Index = -1;

        std::vector<SwapchainImage> m_Images;
        VkFormat                    m_ImageFormat;
        VkExtent2D                  m_Extent{};

        std::vector<VkImageView>   m_ImageViews;
        std::vector<VkFramebuffer> m_Framebuffers;

        VkImage        m_ColorImage{};
        VkDeviceMemory m_ColorImageMemory{};
        VkImageView    m_ColorImageView{};

        VkImage        m_DepthImage{};
        VkDeviceMemory m_DepthImageMemory{};
        VkImageView    m_DepthImageView{};
    };
} // namespace rhi::vulkan
