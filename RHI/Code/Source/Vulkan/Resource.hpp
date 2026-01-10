/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : Resource.hpp
    
    Content : converters from NVRHI structures to Vulkan structures and vice versa.
        Based on Donut, MIT License ( https://github.com/NVIDIA-RTX/Donut )

=================================================*/

#pragma once

#include <nvrhi/vulkan.h>

namespace rhi::vulkan {
    struct FormatMapping {
        nvrhi::Format nvrhi_format;
        VkFormat      vk_format;
    };

    // clang-format off
    static const std::array<FormatMapping, size_t(Format::COUNT)> C_FORMAT_MAP = { {
        { nvrhi::Format::UNKNOWN,           VK_FORMAT_UNDEFINED                },
        { nvrhi::Format::R8_UINT,           VK_FORMAT_R8_UINT                  },
        { nvrhi::Format::R8_SINT,           VK_FORMAT_R8_SINT                  },
        { nvrhi::Format::R8_UNORM,          VK_FORMAT_R8_UNORM                 },
        { nvrhi::Format::R8_SNORM,          VK_FORMAT_R8_SNORM                 },
        { nvrhi::Format::RG8_UINT,          VK_FORMAT_R8G8_UINT                },
        { nvrhi::Format::RG8_SINT,          VK_FORMAT_R8G8_SINT                },
        { nvrhi::Format::RG8_UNORM,         VK_FORMAT_R8G8_UNORM               },
        { nvrhi::Format::RG8_SNORM,         VK_FORMAT_R8G8_SNORM               },
        { nvrhi::Format::R16_UINT,          VK_FORMAT_R16_UINT                 },
        { nvrhi::Format::R16_SINT,          VK_FORMAT_R16_SINT                 },
        { nvrhi::Format::R16_UNORM,         VK_FORMAT_R16_UNORM                },
        { nvrhi::Format::R16_SNORM,         VK_FORMAT_R16_SNORM                },
        { nvrhi::Format::R16_FLOAT,         VK_FORMAT_R16_SFLOAT               },
        { nvrhi::Format::BGRA4_UNORM,       VK_FORMAT_A4R4G4B4_UNORM_PACK16    }, // this format matches the bit layout of DXGI_FORMAT_B4G4R4A4_UNORM
        { nvrhi::Format::B5G6R5_UNORM,      VK_FORMAT_B5G6R5_UNORM_PACK16      },
        { nvrhi::Format::B5G5R5A1_UNORM,    VK_FORMAT_B5G5R5A1_UNORM_PACK16    },
        { nvrhi::Format::RGBA8_UINT,        VK_FORMAT_R8G8B8A8_UINT            },
        { nvrhi::Format::RGBA8_SINT,        VK_FORMAT_R8G8B8A8_SINT            },
        { nvrhi::Format::RGBA8_UNORM,       VK_FORMAT_R8G8B8A8_UNORM           },
        { nvrhi::Format::RGBA8_SNORM,       VK_FORMAT_R8G8B8A8_SNORM           },
        { nvrhi::Format::BGRA8_UNORM,       VK_FORMAT_B8G8R8A8_UNORM           },
        { nvrhi::Format::BGRX8_UNORM,       VK_FORMAT_UNDEFINED                }, // Not supported on Vulkan
        { nvrhi::Format::SRGBA8_UNORM,      VK_FORMAT_R8G8B8A8_SRGB            },
        { nvrhi::Format::SBGRA8_UNORM,      VK_FORMAT_B8G8R8A8_SRGB            },
        { nvrhi::Format::SBGRX8_UNORM,      VK_FORMAT_UNDEFINED                }, // Not supported on Vulkan
        { nvrhi::Format::R10G10B10A2_UNORM, VK_FORMAT_A2B10G10R10_UNORM_PACK32 },
        { nvrhi::Format::R11G11B10_FLOAT,   VK_FORMAT_B10G11R11_UFLOAT_PACK32  },
        { nvrhi::Format::RG16_UINT,         VK_FORMAT_R16G16_UINT              },
        { nvrhi::Format::RG16_SINT,         VK_FORMAT_R16G16_SINT              },
        { nvrhi::Format::RG16_UNORM,        VK_FORMAT_R16G16_UNORM             },
        { nvrhi::Format::RG16_SNORM,        VK_FORMAT_R16G16_SNORM             },
        { nvrhi::Format::RG16_FLOAT,        VK_FORMAT_R16G16_SFLOAT            },
        { nvrhi::Format::R32_UINT,          VK_FORMAT_R32_UINT                 },
        { nvrhi::Format::R32_SINT,          VK_FORMAT_R32_SINT                 },
        { nvrhi::Format::R32_FLOAT,         VK_FORMAT_R32_SFLOAT               },
        { nvrhi::Format::RGBA16_UINT,       VK_FORMAT_R16G16B16A16_UINT        },
        { nvrhi::Format::RGBA16_SINT,       VK_FORMAT_R16G16B16A16_SINT        },
        { nvrhi::Format::RGBA16_FLOAT,      VK_FORMAT_R16G16B16A16_SFLOAT      },
        { nvrhi::Format::RGBA16_UNORM,      VK_FORMAT_R16G16B16A16_UNORM       },
        { nvrhi::Format::RGBA16_SNORM,      VK_FORMAT_R16G16B16A16_SNORM       },
        { nvrhi::Format::RG32_UINT,         VK_FORMAT_R32G32_UINT              },
        { nvrhi::Format::RG32_SINT,         VK_FORMAT_R32G32_SINT              },
        { nvrhi::Format::RG32_FLOAT,        VK_FORMAT_R32G32_SFLOAT            },
        { nvrhi::Format::RGB32_UINT,        VK_FORMAT_R32G32B32_UINT           },
        { nvrhi::Format::RGB32_SINT,        VK_FORMAT_R32G32B32_SINT           },
        { nvrhi::Format::RGB32_FLOAT,       VK_FORMAT_R32G32B32_SFLOAT         },
        { nvrhi::Format::RGBA32_UINT,       VK_FORMAT_R32G32B32A32_UINT        },
        { nvrhi::Format::RGBA32_SINT,       VK_FORMAT_R32G32B32A32_SINT        },
        { nvrhi::Format::RGBA32_FLOAT,      VK_FORMAT_R32G32B32A32_SFLOAT      },
        { nvrhi::Format::D16,               VK_FORMAT_D16_UNORM                },
        { nvrhi::Format::D24S8,             VK_FORMAT_D24_UNORM_S8_UINT        },
        { nvrhi::Format::X24G8_UINT,        VK_FORMAT_D24_UNORM_S8_UINT        },
        { nvrhi::Format::D32,               VK_FORMAT_D32_SFLOAT               },
        { nvrhi::Format::D32S8,             VK_FORMAT_D32_SFLOAT_S8_UINT       },
        { nvrhi::Format::X32G8_UINT,        VK_FORMAT_D32_SFLOAT_S8_UINT       },
        { nvrhi::Format::BC1_UNORM,         VK_FORMAT_BC1_RGBA_UNORM_BLOCK     },
        { nvrhi::Format::BC1_UNORM_SRGB,    VK_FORMAT_BC1_RGBA_SRGB_BLOCK      },
        { nvrhi::Format::BC2_UNORM,         VK_FORMAT_BC2_UNORM_BLOCK          },
        { nvrhi::Format::BC2_UNORM_SRGB,    VK_FORMAT_BC2_SRGB_BLOCK           },
        { nvrhi::Format::BC3_UNORM,         VK_FORMAT_BC3_UNORM_BLOCK          },
        { nvrhi::Format::BC3_UNORM_SRGB,    VK_FORMAT_BC3_SRGB_BLOCK           },
        { nvrhi::Format::BC4_UNORM,         VK_FORMAT_BC4_UNORM_BLOCK          },
        { nvrhi::Format::BC4_SNORM,         VK_FORMAT_BC4_SNORM_BLOCK          },
        { nvrhi::Format::BC5_UNORM,         VK_FORMAT_BC5_UNORM_BLOCK          },
        { nvrhi::Format::BC5_SNORM,         VK_FORMAT_BC5_SNORM_BLOCK          },
        { nvrhi::Format::BC6H_UFLOAT,       VK_FORMAT_BC6H_UFLOAT_BLOCK        },
        { nvrhi::Format::BC6H_SFLOAT,       VK_FORMAT_BC6H_SFLOAT_BLOCK        },
        { nvrhi::Format::BC7_UNORM,         VK_FORMAT_BC7_UNORM_BLOCK          },
        { nvrhi::Format::BC7_UNORM_SRGB,    VK_FORMAT_BC7_SRGB_BLOCK           },
    
    } };
    // clang-format on

    VkFormat convert_format(nvrhi::Format format) {
        assert(sizeof(format) == sizeof(uint32_t));
        assert(format < nvrhi::Format::COUNT);
        assert(C_FORMAT_MAP[uint32_t(format)].nvrhi_format == format);

        return C_FORMAT_MAP[uint32_t(format)].vk_format;
    }

    nvrhi::Format convert_format(VkFormat format) {
        assert(sizeof(format) == sizeof(uint32_t));
        assert(format < VkFormat::VK_FORMAT_MAX_ENUM);
        assert(C_FORMAT_MAP[uint32_t(format)].vk_format == format);

        return C_FORMAT_MAP[uint32_t(format)].nvrhi_format;
    }

} // namespace rhi::vulkan
