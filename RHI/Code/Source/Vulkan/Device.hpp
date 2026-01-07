/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : Device.hpp
    
    Content : Implementation of Device. A part of Vulkan backend.
        Based on Donut, MIT License ( https://github.com/NVIDIA-RTX/Donut )

=================================================*/

#pragma once

#include "RHI/Device.hpp"

#include "Misc.hpp"

#include <unordered_set>
#include <optional>

#include <nvrhi/vulkan.h>
#include <vulkan/vulkan.h>

namespace rhi::vulkan {
    /* forward declarations */
    class Swapchain;
    class CommandList;

    class Device final : public rhi::Device {
    private:
#if !defined(NDEBUG) && defined(RHI_ENABLE_VALIDATION)
        constexpr inline static bool ENABLE_VALIDATION_LAYERS = true;
#else
        constexpr inline static bool ENABLE_VALIDATION_LAYERS = false;
#endif

        constexpr inline static std::array VALIDATION_LAYERS{
            "VK_LAYER_KHRONOS_validation"
        };

        struct QueueFamilyIndices {
        public:
            std::optional<uint32_t> graphics_family;
            std::optional<uint32_t> present_family;
            std::optional<uint32_t> compute_family;
            std::optional<uint32_t> transfer_family;

            inline RHI_NODISCARD bool is_complete() const noexcept {
                return graphics_family.has_value() && present_family.has_value() &&
                       compute_family.has_value() && transfer_family.has_value();
            }

            QueueFamilyIndices()  = default;
            ~QueueFamilyIndices() = default;
        };

        struct SwapChainSupportDetails {
        public:
            VkSurfaceCapabilitiesKHR        capabilities{};
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR>   present_modes;

            SwapChainSupportDetails()  = default;
            ~SwapChainSupportDetails() = default;
        };

    public:
        Device(void* window_handle);
        ~Device();

        RHI_NODISCARD std::unique_ptr<rhi::CommandList> CreateCommandList() override;
        RHI_NODISCARD std::unique_ptr<rhi::Swapchain> CreateSwapchain() override;
        void                                          Submit(rhi::CommandList* cmd) override;

        RHI_NODISCARD void* CreateBackendTexture(const rhi::TextureDesc& desc) override;
        void                DestroyBackendTexture(void* backend_handle) override;

    private:
        void CreateInstance();
        void SetupDebugMessenger();
        void CreateSurface(void* window_handle);
        void PickPhysicalDevice();
        void CreateLogicalDevice();
        void CreateCommandPool();
        void CreateCommandBuffers();
        void CreateSyncObjects();

    private:
        bool                     checkValidationLayerSupport();
        std::vector<const char*> getRequiredExtensions();
        void                     populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& create_info);
        bool                     checkDeviceExtensionSupport(VkPhysicalDevice device);
        bool                     isDeviceSuitable(VkPhysicalDevice device);
        bool                     findQueueFamilies(VkPhysicalDevice physical_device);
        SwapChainSupportDetails  querySwapChainSupport(VkPhysicalDevice device);
        VkSampleCountFlagBits    getMaxUsableSampleCount();

        static VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT      message_severity,
                                                 VkDebugUtilsMessageTypeFlagsEXT             message_types,
                                                 const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
                                                 void*                                       user_data);

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

        std::unordered_set<std::string> m_RayTracingExtensions = {
            VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
            VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
            VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME,
            VK_KHR_RAY_QUERY_EXTENSION_NAME,
            VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
            VK_NV_CLUSTER_ACCELERATION_STRUCTURE_EXTENSION_NAME
        };

        VulkanContext m_Context;

        VkSurfaceKHR m_Surface;

        VkSampleCountFlagBits m_MSAA_Samples = VK_SAMPLE_COUNT_1_BIT;

        VkDebugUtilsMessengerEXT m_DebugMessenger;

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
