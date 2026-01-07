/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : Device.hpp
    
    Content : Implementation of Device. A part of Vulkan backend

=================================================*/

#pragma once

#include "RHI/Device.hpp"

#include "Misc.hpp"

#include <unordered_set>

#include <nvrhi/vulkan.h>
#include <vulkan/vulkan.hpp>

namespace rhi::vulkan {
    /* forward declarations */
    class Swapchain;
    class CommandList;

    class Device final : public rhi::Device {
    public:
        Device() = default;
        ~Device();

        RHI_NODISCARD std::unique_ptr<rhi::CommandList> CreateCommandList() override;
        RHI_NODISCARD std::unique_ptr<rhi::Swapchain> CreateSwapchain(void* window_handle) override;
        void                                          Submit(rhi::CommandList* cmd) override;

        RHI_NODISCARD void* CreateBackendTexture(const rhi::TextureDesc& desc) override;
        void                DestroyBackendTexture(void* backend_handle) override;

    private:
        struct VulkanExtensionSet {
            std::unordered_set<std::string> instance;
            std::unordered_set<std::string> layers;
            std::unordered_set<std::string> device;
        };

        // minimal set of required extensions
        VulkanExtensionSet m_EnabledExtensions = {
            // instance
            {
                VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME },
            // layers
            {},
            // device
            {
                VK_KHR_MAINTENANCE1_EXTENSION_NAME },
        };

        // optional extensions
        VulkanExtensionSet m_OptionalExtensions = {
            // instance
            {
                VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
                VK_EXT_SAMPLER_FILTER_MINMAX_EXTENSION_NAME,
            },
            // layers
            {},
            // device
            {
                VK_EXT_DEBUG_MARKER_EXTENSION_NAME,
                VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,
                VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
                VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME,
                VK_KHR_MAINTENANCE_4_EXTENSION_NAME,
                VK_KHR_SWAPCHAIN_MUTABLE_FORMAT_EXTENSION_NAME,
                VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME,
                VK_NV_MESH_SHADER_EXTENSION_NAME,
                VK_EXT_MUTABLE_DESCRIPTOR_TYPE_EXTENSION_NAME,
#if RHI_WITH_AFTERMATH
                VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME,
                VK_NV_DEVICE_DIAGNOSTICS_CONFIG_EXTENSION_NAME,
#endif
            },
        };

        std::unordered_set<std::string> m_RayTracingExtensions = {
            VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
            VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
            VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME,
            VK_KHR_RAY_QUERY_EXTENSION_NAME,
            VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
            VK_NV_CLUSTER_ACCELERATION_STRUCTURE_EXTENSION_NAME
        };

        std::string m_RendererString;

        VkDebugReportCallbackEXT m_DebugReportCallback;

        VulkanContext m_Context;

        int m_GraphicsQueueFamily = -1;
        int m_ComputeQueueFamily  = -1;
        int m_TransferQueueFamily = -1;
        int m_PresentQueueFamily  = -1;

        nvrhi::vulkan::DeviceHandle m_NVRHIDevice;
        nvrhi::DeviceHandle         m_ValidationLayer;

        std::vector<FrameSync> m_Frames;
        uint32_t               m_FrameIndex;

        bool m_BufferDeviceAddressSupported = false;

#if VK_HEADER_VERSION >= 301
        typedef vk::detail::DynamicLoader VulkanDynamicLoader;
#else
        typedef VkDynamicLoader VulkanDynamicLoader;
#endif

        std::unique_ptr<VulkanDynamicLoader> m_DynamicLoader;

        friend class rhi::vulkan::Swapchain;
    };
} // namespace rhi::vulkan
