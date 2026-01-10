/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : Logging.hpp
    
    Content : overrided part of logging especially for Vulkan

=================================================*/

#pragma once

#include "Source/Common/Logging.hpp"
#include <vulkan/vulkan_core.h>
#include <vulkan/vk_enum_string_helper.h>

#define RHI_VK_CHECK_DEBUG(func, message)                                               \
    {                                                                                   \
        VkResult result = func;                                                         \
        if (result != VK_SUCCESS) {                                                     \
            rhi::logging::debug("%s\nVkResult : %s", message, string_VkResult(result)); \
        }                                                                               \
    }

#define RHI_VK_CHECK_INFO(func, message)                                               \
    {                                                                                  \
        VkResult result = func;                                                        \
        if (result != VK_SUCCESS) {                                                    \
            rhi::logging::info("%s\nVkResult : %s", message, string_VkResult(result)); \
        }                                                                              \
    }

#define RHI_VK_CHECK_WARNING(func, message)                                               \
    {                                                                                     \
        VkResult result = func;                                                           \
        if (result != VK_SUCCESS) {                                                       \
            rhi::logging::warning("%s\nVkResult : %s", message, string_VkResult(result)); \
        }                                                                                 \
    }

#define RHI_VK_CHECK_ERROR(func, message)                                               \
    {                                                                                   \
        VkResult result = func;                                                         \
        if (result != VK_SUCCESS) {                                                     \
            rhi::logging::error("%s\nVkResult : %s", message, string_VkResult(result)); \
        }                                                                               \
    }

#define RHI_VK_CHECK_FATAL(func, message)                                               \
    {                                                                                   \
        VkResult result = func;                                                         \
        if (result != VK_SUCCESS) {                                                     \
            rhi::logging::fatal("%s\nVkResult : %s", message, string_VkResult(result)); \
        }                                                                               \
    }
