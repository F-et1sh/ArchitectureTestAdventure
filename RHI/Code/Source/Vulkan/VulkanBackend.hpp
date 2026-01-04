/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : VulkanBackend.hpp
    
    Content : Implementation of Vulkan classes.
        Based on Donut, MIT License : ( https://github.com/NVIDIA-RTX/Donut )

=================================================*/

#pragma once

#include "RHI/Common/Resource.hpp"

#include "RHI/Device.hpp"
#include "RHI/CommandList.hpp"
#include "RHI/Swapchain.hpp"

#include <nvrhi/nvrhi.h>
#include <nvrhi/vulkan.h>
#include <vulkan/vulkan_raii.hpp>

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

    /* forward declarations */
    class Swapchain;

    class Device final : public rhi::Device {
    public:
        Device();
        ~Device();

        RHI_NODISCARD std::unique_ptr<rhi::CommandList> CreateCommandList() override;
        RHI_NODISCARD std::unique_ptr<rhi::Swapchain> CreateSwapchain() override;
        void                                          Submit(rhi::CommandList* cmd) override;

        RHI_NODISCARD void* CreateBackendTexture(const rhi::TextureDesc& desc) override;
        void                DestroyBackendTexture(void* backend_handle) override;

    private:
        struct Impl;
        Impl* m_Impl;

        friend class rhi::vulkan::Swapchain;
    };

    class CommandList final : public rhi::CommandList {
    public:
        explicit CommandList(nvrhi::CommandListHandle handle) : m_NVRHICommandList(handle) {}
        ~CommandList() = default;

        void BeginFrame() override;
        void EndFrame() override;

        void setPipeline(const Pipeline* pipeline) override;

        void setVertexBuffer(const Buffer* buffer) override;
        void setIndexBuffer(const Buffer* buffer) override;

        void setRenderTarget(TextureHandle handle) override;

        void DrawIndexed(uint32_t instance_count, uint32_t first_index, uint32_t first_instance, uint32_t first_vertex, uint32_t vertex_count) override;

        inline RHI_NODISCARD nvrhi::CommandListHandle getNVRHICommandListHandle() const noexcept { return m_NVRHICommandList; }

    private: // No PIMPL because it's too small
        nvrhi::CommandListHandle m_NVRHICommandList;
    };

    class Swapchain final : public rhi::Swapchain {
    public:
        explicit Swapchain(rhi::vulkan::Device* device);
        ~Swapchain();

        RHI_NODISCARD rhi::TextureHandle Acquire() override;
        void                             Present() override;

        void Resize(uint32_t width, uint32_t height) override;

        RHI_NODISCARD uint32_t getWidth() const override;
        RHI_NODISCARD uint32_t getHeight() const override;

    private:
        struct Impl;
        Impl* m_Impl;
    };
} // namespace rhi::vulkan
