/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : Resource.hpp
    
    Content : converters from RHI/Common/Resource.hpp structures
        to NVRHI structures and vice versa

=================================================*/

#pragma once

#include "Common/Resource.hpp"

#include <nvrhi/nvrhi.h>

namespace rhi {
    // clang-format off
    nvrhi::TextureDimension to_nvrhi(rhi::TextureDimension dimension) {
        switch (dimension) {
            case rhi::TextureDimension::Texture1D: return nvrhi::TextureDimension::Texture1D;
            case rhi::TextureDimension::Texture1DArray: return nvrhi::TextureDimension::Texture1DArray;
            case rhi::TextureDimension::Texture2D: return nvrhi::TextureDimension::Texture2D;
            case rhi::TextureDimension::Texture2DArray: return nvrhi::TextureDimension::Texture2DArray;
            case rhi::TextureDimension::TextureCube: return nvrhi::TextureDimension::TextureCube;
            case rhi::TextureDimension::TextureCubeArray: return nvrhi::TextureDimension::TextureCubeArray;
            case rhi::TextureDimension::Texture2DMS: return nvrhi::TextureDimension::Texture2DMS;
            case rhi::TextureDimension::Texture2DMSArray: return nvrhi::TextureDimension::Texture2DMSArray;
            case rhi::TextureDimension::Texture3D: return nvrhi::TextureDimension::Texture3D;
            default: return nvrhi::TextureDimension::Unknown;
        }
    }
    // clang-format on

    // clang-format off
    nvrhi::Format to_nvrhi(rhi::Format format) {
        switch (format) {
            case rhi::Format::R8_UINT: return nvrhi::Format::R8_UINT;
            case rhi::Format::R8_SINT: return nvrhi::Format::R8_SINT;
            case rhi::Format::R8_UNORM: return nvrhi::Format::R8_UNORM;
            case rhi::Format::R8_SNORM: return nvrhi::Format::R8_SNORM;
            case rhi::Format::RG8_UINT: return nvrhi::Format::RG8_UINT;
            case rhi::Format::RG8_SINT: return nvrhi::Format::RG8_SINT;
            case rhi::Format::RG8_UNORM: return nvrhi::Format::RG8_UNORM;
            case rhi::Format::RG8_SNORM: return nvrhi::Format::RG8_SNORM;
            case rhi::Format::R16_UINT: return nvrhi::Format::R16_UINT;
            case rhi::Format::R16_SINT: return nvrhi::Format::R16_SINT;
            case rhi::Format::R16_UNORM: return nvrhi::Format::R16_UNORM;
            case rhi::Format::R16_SNORM: return nvrhi::Format::R16_SNORM;
            case rhi::Format::R16_FLOAT: return nvrhi::Format::R16_FLOAT;
            case rhi::Format::BGRA4_UNORM: return nvrhi::Format::BGRA4_UNORM;
            case rhi::Format::B5G6R5_UNORM: return nvrhi::Format::B5G6R5_UNORM;
            case rhi::Format::B5G5R5A1_UNORM: return nvrhi::Format::B5G5R5A1_UNORM;
            case rhi::Format::RGBA8_UINT: return nvrhi::Format::RGBA8_UINT;
            case rhi::Format::RGBA8_SINT: return nvrhi::Format::RGBA8_SINT;
            case rhi::Format::RGBA8_UNORM: return nvrhi::Format::RGBA8_UNORM;
            case rhi::Format::RGBA8_SNORM: return nvrhi::Format::RGBA8_SNORM;
            case rhi::Format::BGRA8_UNORM: return nvrhi::Format::BGRA8_UNORM;
            case rhi::Format::BGRX8_UNORM: return nvrhi::Format::BGRX8_UNORM;
            case rhi::Format::SRGBA8_UNORM: return nvrhi::Format::SRGBA8_UNORM;
            case rhi::Format::SBGRA8_UNORM: return nvrhi::Format::SBGRA8_UNORM;
            case rhi::Format::SBGRX8_UNORM: return nvrhi::Format::SBGRX8_UNORM;
            case rhi::Format::R10G10B10A2_UNORM: return nvrhi::Format::R10G10B10A2_UNORM;
            case rhi::Format::R11G11B10_FLOAT: return nvrhi::Format::R11G11B10_FLOAT;
            case rhi::Format::RG16_UINT: return nvrhi::Format::RG16_UINT;
            case rhi::Format::RG16_SINT: return nvrhi::Format::RG16_SINT;
            case rhi::Format::RG16_UNORM: return nvrhi::Format::RG16_UNORM;
            case rhi::Format::RG16_SNORM: return nvrhi::Format::RG16_SNORM;
            case rhi::Format::RG16_FLOAT: return nvrhi::Format::RG16_FLOAT;
            case rhi::Format::R32_UINT: return nvrhi::Format::R32_UINT;
            case rhi::Format::R32_SINT: return nvrhi::Format::R32_SINT;
            case rhi::Format::R32_FLOAT: return nvrhi::Format::R32_FLOAT;
            case rhi::Format::RGBA16_UINT: return nvrhi::Format::RGBA16_UINT;
            case rhi::Format::RGBA16_SINT: return nvrhi::Format::RGBA16_SINT;
            case rhi::Format::RGBA16_FLOAT: return nvrhi::Format::RGBA16_FLOAT;
            case rhi::Format::RGBA16_UNORM: return nvrhi::Format::RGBA16_UNORM;
            case rhi::Format::RGBA16_SNORM: return nvrhi::Format::RGBA16_SNORM;
            case rhi::Format::RG32_UINT: return nvrhi::Format::RG32_UINT;
            case rhi::Format::RG32_SINT: return nvrhi::Format::RG32_SINT;
            case rhi::Format::RG32_FLOAT: return nvrhi::Format::RG32_FLOAT;
            case rhi::Format::RGB32_UINT: return nvrhi::Format::RGB32_UINT;
            case rhi::Format::RGB32_SINT: return nvrhi::Format::RGB32_SINT;
            case rhi::Format::RGB32_FLOAT: return nvrhi::Format::RGB32_FLOAT;
            case rhi::Format::RGBA32_UINT: return nvrhi::Format::RGBA32_UINT;
            case rhi::Format::RGBA32_SINT: return nvrhi::Format::RGBA32_SINT;
            case rhi::Format::RGBA32_FLOAT: return nvrhi::Format::RGBA32_FLOAT;
            case rhi::Format::D16: return nvrhi::Format::D16;
            case rhi::Format::D24S8: return nvrhi::Format::D24S8;
            case rhi::Format::X24G8_UINT: return nvrhi::Format::X24G8_UINT;
            case rhi::Format::D32: return nvrhi::Format::D32;
            case rhi::Format::D32S8: return nvrhi::Format::D32S8;
            case rhi::Format::X32G8_UINT: return nvrhi::Format::X32G8_UINT;
            case rhi::Format::BC1_UNORM: return nvrhi::Format::BC1_UNORM;
            case rhi::Format::BC1_UNORM_SRGB: return nvrhi::Format::BC1_UNORM_SRGB;
            case rhi::Format::BC2_UNORM: return nvrhi::Format::BC2_UNORM;
            case rhi::Format::BC2_UNORM_SRGB: return nvrhi::Format::BC2_UNORM_SRGB;
            case rhi::Format::BC3_UNORM: return nvrhi::Format::BC3_UNORM;
            case rhi::Format::BC3_UNORM_SRGB: return nvrhi::Format::BC3_UNORM_SRGB;
            case rhi::Format::BC4_UNORM: return nvrhi::Format::BC4_UNORM;
            case rhi::Format::BC4_SNORM: return nvrhi::Format::BC4_SNORM;
            case rhi::Format::BC5_UNORM: return nvrhi::Format::BC5_UNORM;
            case rhi::Format::BC5_SNORM: return nvrhi::Format::BC5_SNORM;
            case rhi::Format::BC6H_UFLOAT: return nvrhi::Format::BC6H_UFLOAT;
            case rhi::Format::BC6H_SFLOAT: return nvrhi::Format::BC6H_SFLOAT;
            case rhi::Format::BC7_UNORM: return nvrhi::Format::BC7_UNORM;
            case rhi::Format::BC7_UNORM_SRGB: return nvrhi::Format::BC7_UNORM_SRGB;
            case rhi::Format::COUNT: return nvrhi::Format::COUNT;
            default: return nvrhi::Format::UNKNOWN;
        }
    }
    // clang-format on

    // clang-format off
    nvrhi::ResourceStates to_nvrhi(rhi::ResourceStates state) {
        switch (state) {
            case rhi::ResourceStates::Common: return nvrhi::ResourceStates::Common;
            case rhi::ResourceStates::ConstantBuffer: return nvrhi::ResourceStates::ConstantBuffer;
            case rhi::ResourceStates::VertexBuffer: return nvrhi::ResourceStates::VertexBuffer;
            case rhi::ResourceStates::IndexBuffer: return nvrhi::ResourceStates::IndexBuffer;
            case rhi::ResourceStates::IndirectArgument: return nvrhi::ResourceStates::IndirectArgument;
            case rhi::ResourceStates::ShaderResource: return nvrhi::ResourceStates::ShaderResource;
            case rhi::ResourceStates::UnorderedAccess: return nvrhi::ResourceStates::UnorderedAccess;
            case rhi::ResourceStates::RenderTarget: return nvrhi::ResourceStates::RenderTarget;
            case rhi::ResourceStates::DepthWrite: return nvrhi::ResourceStates::DepthWrite;
            case rhi::ResourceStates::DepthRead: return nvrhi::ResourceStates::DepthRead;
            case rhi::ResourceStates::StreamOut: return nvrhi::ResourceStates::StreamOut;
            case rhi::ResourceStates::CopyDest: return nvrhi::ResourceStates::CopyDest;
            case rhi::ResourceStates::CopySource: return nvrhi::ResourceStates::CopySource;
            case rhi::ResourceStates::ResolveDest: return nvrhi::ResourceStates::ResolveDest;
            case rhi::ResourceStates::ResolveSource: return nvrhi::ResourceStates::ResolveSource;
            case rhi::ResourceStates::Present: return nvrhi::ResourceStates::Present;
            case rhi::ResourceStates::AccelStructRead: return nvrhi::ResourceStates::AccelStructRead;
            case rhi::ResourceStates::AccelStructWrite: return nvrhi::ResourceStates::AccelStructWrite;
            case rhi::ResourceStates::AccelStructBuildInput: return nvrhi::ResourceStates::AccelStructBuildInput;
            case rhi::ResourceStates::AccelStructBuildBlas: return nvrhi::ResourceStates::AccelStructBuildBlas;
            case rhi::ResourceStates::ShadingRateSurface: return nvrhi::ResourceStates::ShadingRateSurface;
            case rhi::ResourceStates::OpacityMicromapWrite: return nvrhi::ResourceStates::OpacityMicromapWrite;
            case rhi::ResourceStates::OpacityMicromapBuildInput: return nvrhi::ResourceStates::OpacityMicromapBuildInput;
            case rhi::ResourceStates::ConvertCoopVecMatrixInput: return nvrhi::ResourceStates::ConvertCoopVecMatrixInput;
            case rhi::ResourceStates::ConvertCoopVecMatrixOutput: return nvrhi::ResourceStates::ConvertCoopVecMatrixOutput;
            default : return nvrhi::ResourceStates::Unknown;
        }
    }
    // clang-format on

    // clang-format off
    nvrhi::SharedResourceFlags to_nvrhi(rhi::SharedResourceFlags flags) {
        switch (flags) {
            case rhi::SharedResourceFlags::Shared: return nvrhi::SharedResourceFlags::Shared;
            case rhi::SharedResourceFlags::Shared_NTHandle: return nvrhi::SharedResourceFlags::Shared_NTHandle;
            case rhi::SharedResourceFlags::Shared_CrossAdapter: return nvrhi::SharedResourceFlags::Shared_CrossAdapter;
            default: return nvrhi::SharedResourceFlags::None;
        }
    }
    // clang-format on

    nvrhi::TextureDesc to_nvrhi(const rhi::TextureDesc& desc) {
        nvrhi::TextureDesc d{};
        d.setWidth(desc.width);
        d.setHeight(desc.height);
        d.setDepth(desc.depth);
        d.setArraySize(desc.array_size);
        d.setMipLevels(desc.mip_levels);
        d.setSampleCount(desc.sample_count);
        d.setSampleQuality(desc.sample_quality);
        d.setFormat(rhi::to_nvrhi(desc.format));
        d.setDimension(rhi::to_nvrhi(desc.dimension));
        d.setDebugName(desc.debug_name);
        d.setIsRenderTarget(desc.is_render_target);
        d.setIsUAV(desc.is_uav);
        d.setIsTypeless(desc.is_typeless);
        d.setIsVirtual(desc.is_virtual);
        d.setClearValue(nvrhi::Color{ desc.clear_value.r, desc.clear_value.g, desc.clear_value.b, desc.clear_value.a });
        d.setUseClearValue(desc.use_clear_value);
        d.setInitialState(rhi::to_nvrhi(desc.initial_state));
        d.setKeepInitialState(desc.keep_initial_state);
        d.setSharedResourceFlags(rhi::to_nvrhi(desc.shared_resource_flags));
        return d;
    }

    namespace vulkan {
        // clang-format off

        struct FormatMapping
        {
            nvrhi::Format nvrhi_format;
            VkFormat vk_format;
        };

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

        // clang-format on

    } // namespace vulkan
} // namespace rhi
