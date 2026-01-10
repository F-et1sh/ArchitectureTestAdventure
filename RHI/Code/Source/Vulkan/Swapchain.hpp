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

        RHI_NODISCARD BackbufferIndex Acquire() override;
        void                          Present() override;

        void Resize(uint32_t width, uint32_t height) override;

        RHI_NODISCARD uint32_t getWidth() const override;
        RHI_NODISCARD uint32_t getHeight() const override;

        inline RHI_NODISCARD nvrhi::TextureHandle getBackbuffer(BackbufferIndex index) const { return m_Images[index].nvrhi_texture; }

    private:
        void CreateSwapchain();
        void CreateImageViews();
        void CreateNVRHITextures();

    private:
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats);
        VkPresentModeKHR   chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& available_present_modes);
        VkExtent2D         chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    private:
        rhi::vulkan::Device& m_Device;

        VkSwapchainKHR m_Swapchain;

        VkFormat   m_ImageFormat;
        VkExtent2D m_Extent;

        uint32_t m_ImageIndex = 0;

        struct SwapchainImage {
            VkImage              image;
            VkImageView          image_view;
            nvrhi::TextureHandle nvrhi_texture;
        };
        std::vector<SwapchainImage> m_Images;

        VkSurfaceFormatKHR m_SurfaceFormat;
        bool               m_MutableFormatSupported;
    };
} // namespace rhi::vulkan
