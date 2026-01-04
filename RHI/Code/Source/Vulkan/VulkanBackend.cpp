/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : VulkanBackend.cpp
    
    Content : Implementation of Vulkan classes.
        Based on Donut, MIT License : ( https://github.com/NVIDIA-RTX/Donut )

=================================================*/

#include "VulkanBackend.hpp"

#include "Source/Common/Resource.hpp"

#include <unordered_set>
#include <string>
#include <queue>

#include <nvrhi/vulkan.h>
#include <vulkan/vulkan.hpp>

struct rhi::vulkan::Device::Impl {
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

    vk::Instance               m_VulkanInstance;
    vk::DebugReportCallbackEXT m_DebugReportCallback;

    vk::PhysicalDevice m_VulkanPhysicalDevice;
    int                m_GraphicsQueueFamily = -1;
    int                m_ComputeQueueFamily  = -1;
    int                m_TransferQueueFamily = -1;
    int                m_PresentQueueFamily  = -1;

    vk::Device m_VulkanDevice;
    vk::Queue  m_GraphicsQueue;
    vk::Queue  m_ComputeQueue;
    vk::Queue  m_TransferQueue;
    vk::Queue  m_PresentQueue;

    nvrhi::vulkan::DeviceHandle m_NVRHIDevice;
    nvrhi::DeviceHandle         m_ValidationLayer;

    std::vector<vk::Semaphore> m_AcquireSemaphores;
    std::vector<vk::Semaphore> m_PresentSemaphores;
    uint32_t                   m_AcquireSemaphoreIndex = 0;

    std::queue<nvrhi::EventQueryHandle>  m_FramesInFlight;
    std::vector<nvrhi::EventQueryHandle> m_QueryPool;

    bool m_BufferDeviceAddressSupported = false;

#if VK_HEADER_VERSION >= 301
    typedef vk::detail::DynamicLoader VulkanDynamicLoader;
#else
    typedef vk::DynamicLoader VulkanDynamicLoader;
#endif

    std::unique_ptr<VulkanDynamicLoader> m_DynamicLoader;
};

rhi::vulkan::Device::Device() {
    this->m_Impl = new Impl();
}

rhi::vulkan::Device::~Device() {
    delete this->m_Impl;
}

std::unique_ptr<rhi::CommandList> rhi::vulkan::Device::CreateCommandList() {
    auto cmd = this->m_Impl->m_NVRHIDevice->createCommandList();
    return std::make_unique<rhi::vulkan::CommandList>(cmd);
}

void rhi::vulkan::Device::Submit(rhi::CommandList* cmd) {
}

RHI_NODISCARD void* rhi::vulkan::Device::CreateBackendTexture(const rhi::TextureDesc& desc) {
    nvrhi::TextureHandle handle = this->m_Impl->m_NVRHIDevice->createTexture(rhi::to_nvrhi(desc));
    return static_cast<void*>(handle.Get());
}

void rhi::vulkan::Device::DestroyBackendTexture(void* backend_handle) {
    
}

void rhi::vulkan::CommandList::BeginFrame() {
    m_NVRHICommandList->open();
}

void rhi::vulkan::CommandList::EndFrame() {
    m_NVRHICommandList->close();
}

void rhi::vulkan::CommandList::setPipeline(const Pipeline* pipeline) {
}

void rhi::vulkan::CommandList::setVertexBuffer(const Buffer* buffer) {
}

void rhi::vulkan::CommandList::setIndexBuffer(const Buffer* buffer) {
}

void rhi::vulkan::CommandList::DrawIndexed(uint32_t instance_count, uint32_t first_index, uint32_t first_instance, uint32_t first_vertex, uint32_t vertex_count) {
    nvrhi::DrawArguments args{};
    args.setInstanceCount(instance_count);
    args.setStartIndexLocation(first_index);
    args.setStartInstanceLocation(first_instance);
    args.setStartVertexLocation(first_vertex);
    args.setVertexCount(vertex_count);

    m_NVRHICommandList->drawIndexed(args);
}

struct rhi::vulkan::Swapchain::Impl {
    vk::SurfaceKHR m_WindowSurface;

    vk::SurfaceFormatKHR m_SwapchainFormat;
    vk::SwapchainKHR     m_Swapchain;
    bool                 m_SwapchainMutableFormatSupported = false;

    struct SwapChainImage {
        vk::Image            image;
        nvrhi::TextureHandle rhi_handle;
    };

    std::vector<SwapChainImage> m_SwapchainImages;
    uint32_t                    m_SwapchainIndex = uint32_t(-1);
};

rhi::vulkan::Swapchain::Swapchain() {
    this->m_Impl = new Impl();
}

rhi::vulkan::Swapchain::~Swapchain() {
    delete this->m_Impl;
}

RHI_NODISCARD rhi::TextureHandle rhi::vulkan::Swapchain::Acquire() {
    return {};
}

void rhi::vulkan::Swapchain::Present() {
}

void rhi::vulkan::Swapchain::Resize(uint32_t width, uint32_t height) {
}

RHI_NODISCARD uint32_t rhi::vulkan::Swapchain::getWidth() const {
    return 0;
}

RHI_NODISCARD uint32_t rhi::vulkan::Swapchain::getHeight() const {
    return 0;
}
