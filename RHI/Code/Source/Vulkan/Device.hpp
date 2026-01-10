/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : Device.hpp
    
    Content : Implementation of Device. A part of Vulkan backend.
        Based on Donut, MIT License ( https://github.com/NVIDIA-RTX/Donut )

=================================================*/

#pragma once

#include <vector>

#include "RHI/Device.hpp"
#include "Misc.hpp"

#include "Swapchain.hpp" // remove this

namespace rhi::vulkan {
    struct DefaultMessageCallback : public nvrhi::IMessageCallback {
        static DefaultMessageCallback& getInstance();
        void                           message(nvrhi::MessageSeverity severity, const char* message_text) override;
    };

    /* forward declarations */
    class Swapchain;
    class CommandList;

    class Device final : public rhi::Device {
    private:
        constexpr inline static int MAX_FRAMES_IN_FLIGHT = 2;

#if !defined(NDEBUG) && defined(RHI_ENABLE_VALIDATION)
        constexpr static bool ENABLE_VALIDATION_LAYERS = true;
#else
        constexpr inline static bool ENABLE_VALIDATION_LAYERS = false;
#endif

        constexpr static std::array VALIDATION_LAYERS{
            "VK_LAYER_KHRONOS_validation"
        };

    public:
        Device();
        ~Device();

        void Initialize(); // TODO : Add initialization by desc
        void InitializeForPresentation(void* window_handle);

        RHI_NODISCARD std::unique_ptr<rhi::CommandList> CreateCommandList() override;
        RHI_NODISCARD std::unique_ptr<rhi::Swapchain> CreateSwapchain() override;
        void                                          Submit(rhi::CommandList* cmd) override;

        RHI_NODISCARD void* CreateBackendTexture(const rhi::TextureDesc& desc) override;
        void                DestroyBackendTexture(void* backend_handle) override;

        inline RHI_NODISCARD Swapchain::SwapchainImage& getSwapchainImage(uint32_t) {  }

    private:
        void CreateInstance();
        void SetupDebugMessenger();
        void CreateSurface(void* window_handle);
        void PickPhysicalDevice();
        void CreateLogicalDevice();
        void CreateCommandPool();
        void CreateCommandBuffers();
        void CreateSyncObjects();
        void CreateNVRHIDevice();

    private:
        RHI_NODISCARD static bool                     checkValidationLayerSupport();
        RHI_NODISCARD static std::vector<const char*> getRequiredExtensions();
        static void                                   populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& create_info);
        RHI_NODISCARD bool                            checkDeviceExtensionSupport(VkPhysicalDevice device);
        RHI_NODISCARD bool                            isDeviceSuitable(VkPhysicalDevice device);
        RHI_NODISCARD bool                            findQueueFamilies(VkPhysicalDevice physical_device);
        void                                          findSwapchainSupportDetails(VkPhysicalDevice device);
        RHI_NODISCARD VkSampleCountFlagBits           getMaxUsableSampleCount() const;

        static VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT      message_severity,
                                                 VkDebugUtilsMessageTypeFlagsEXT             message_types,
                                                 const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
                                                 void*                                       user_data);

    private:
        struct VulkanExtensionSet {
            std::vector<const char*> instance;
            std::vector<const char*> layers;
            std::vector<const char*> device;
        };

        // minimal set of required extensions
        VulkanExtensionSet m_EnabledExtensions = {
            // instance
            {
                VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
            },
            // layers
            {},
            // device
            {
                VK_KHR_MAINTENANCE1_EXTENSION_NAME,
                VK_KHR_SWAPCHAIN_EXTENSION_NAME },
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

        std::vector<const char*> m_RayTracingExtensions = {
            VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
            VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
            VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME,
            VK_KHR_RAY_QUERY_EXTENSION_NAME,
            VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
            VK_NV_CLUSTER_ACCELERATION_STRUCTURE_EXTENSION_NAME
        };

        VulkanContext m_Context;

        VkSurfaceKHR m_Surface;
        uint32_t     m_SurfaceWidth  = 0;
        uint32_t     m_SurfaceHeight = 0;

        VkSampleCountFlagBits   m_MSAA_Samples = VK_SAMPLE_COUNT_1_BIT;
        QueueFamilyIndices      m_QueueFamilyIndices;
        SwapchainSupportDetails m_SwapchainSupportDetails;

        VkDebugUtilsMessengerEXT m_DebugMessenger;

        VkCommandPool                m_CommandPool;
        std::vector<VkCommandBuffer> m_CommandBuffers;

        nvrhi::vulkan::DeviceHandle m_NVRHIDevice;
        nvrhi::DeviceHandle         m_ValidationLayer;

        std::vector<FrameSync> m_Frames;
        uint32_t               m_FrameIndex;

        //#if VK_HEADER_VERSION >= 301
        //        typedef vk::detail::DynamicLoader VulkanDynamicLoader;
        //#else
        //        typedef VkDynamicLoader VulkanDynamicLoader;
        //#endif
        //
        //        std::unique_ptr<VulkanDynamicLoader> m_DynamicLoader;

        friend class rhi::vulkan::Swapchain;
    };
} // namespace rhi::vulkan
