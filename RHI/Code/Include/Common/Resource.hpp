/*=================================================

    Copyright (C) 2025 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : Resource.hpp
    
    Content : some useful structures and handle declarations.
        Based on NVRHI, MIT License : ( https://github.com/NVIDIA-RTX/NVRHI )

=================================================*/

#pragma once

#include <cstdint>
#include <string>

namespace rhi {
#define RHI_ENUM_CLASS_FLAG_OPERATORS(T)      \
    inline T operator|(T a, T b) {            \
        return T(uint32_t(a) | uint32_t(b));  \
    }                                         \
    inline T operator&(T a, T b) {            \
        return T(uint32_t(a) & uint32_t(b));  \
    }                                         \
    inline T operator~(T a) {                 \
        return T(~uint32_t(a));               \
    }                                         \
    inline bool operator!(T a) {              \
        return uint32_t(a) == 0;              \
    }                                         \
    inline bool operator==(T a, uint32_t b) { \
        return uint32_t(a) == b;              \
    }                                         \
    inline bool operator!=(T a, uint32_t b) { \
        return uint32_t(a) != b;              \
    }

    enum class GraphicsAPI : uint8_t {
        //D3D11,
        //D3D12,
        VULKAN,
        SomeOtherGraphicsAPI
    };

    enum class Format : uint8_t {
        UNKNOWN,

        R8_UINT,
        R8_SINT,
        R8_UNORM,
        R8_SNORM,
        RG8_UINT,
        RG8_SINT,
        RG8_UNORM,
        RG8_SNORM,
        R16_UINT,
        R16_SINT,
        R16_UNORM,
        R16_SNORM,
        R16_FLOAT,
        BGRA4_UNORM,
        B5G6R5_UNORM,
        B5G5R5A1_UNORM,
        RGBA8_UINT,
        RGBA8_SINT,
        RGBA8_UNORM,
        RGBA8_SNORM,
        BGRA8_UNORM,
        BGRX8_UNORM,
        SRGBA8_UNORM,
        SBGRA8_UNORM,
        SBGRX8_UNORM,
        R10G10B10A2_UNORM,
        R11G11B10_FLOAT,
        RG16_UINT,
        RG16_SINT,
        RG16_UNORM,
        RG16_SNORM,
        RG16_FLOAT,
        R32_UINT,
        R32_SINT,
        R32_FLOAT,
        RGBA16_UINT,
        RGBA16_SINT,
        RGBA16_FLOAT,
        RGBA16_UNORM,
        RGBA16_SNORM,
        RG32_UINT,
        RG32_SINT,
        RG32_FLOAT,
        RGB32_UINT,
        RGB32_SINT,
        RGB32_FLOAT,
        RGBA32_UINT,
        RGBA32_SINT,
        RGBA32_FLOAT,

        D16,
        D24S8,
        X24G8_UINT,
        D32,
        D32S8,
        X32G8_UINT,

        BC1_UNORM,
        BC1_UNORM_SRGB,
        BC2_UNORM,
        BC2_UNORM_SRGB,
        BC3_UNORM,
        BC3_UNORM_SRGB,
        BC4_UNORM,
        BC4_SNORM,
        BC5_UNORM,
        BC5_SNORM,
        BC6H_UFLOAT,
        BC6H_SFLOAT,
        BC7_UNORM,
        BC7_UNORM_SRGB,

        COUNT,
    };

    struct Color {
    public:
        float r, g, b, a;

        bool operator==(const Color& _b) const { return r == _b.r && g == _b.g && b == _b.b && a == _b.a; }
        bool operator!=(const Color& _b) const { return !(*this == _b); }

        Color() : r(0.f), g(0.f), b(0.f), a(0.f) {}
        ~Color() = default;

        Color(float c) : r(c), g(c), b(c), a(c) {}
        Color(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {}
    };

    //////////////////////////////////////////////////////////////////////////
    //                              Texture
    //////////////////////////////////////////////////////////////////////////

    using TextureHandle = uint32_t;

    enum class TextureDimension : uint8_t {
        Unknown,
        Texture1D,
        Texture1DArray,
        Texture2D,
        Texture2DArray,
        TextureCube,
        TextureCubeArray,
        Texture2DMS,
        Texture2DMSArray,
        Texture3D
    };

    enum class ResourceStates : uint32_t {
        Unknown                    = 0,
        Common                     = 0x00000001,
        ConstantBuffer             = 0x00000002,
        VertexBuffer               = 0x00000004,
        IndexBuffer                = 0x00000008,
        IndirectArgument           = 0x00000010,
        ShaderResource             = 0x00000020,
        UnorderedAccess            = 0x00000040,
        RenderTarget               = 0x00000080,
        DepthWrite                 = 0x00000100,
        DepthRead                  = 0x00000200,
        StreamOut                  = 0x00000400,
        CopyDest                   = 0x00000800,
        CopySource                 = 0x00001000,
        ResolveDest                = 0x00002000,
        ResolveSource              = 0x00004000,
        Present                    = 0x00008000,
        AccelStructRead            = 0x00010000,
        AccelStructWrite           = 0x00020000,
        AccelStructBuildInput      = 0x00040000,
        AccelStructBuildBlas       = 0x00080000,
        ShadingRateSurface         = 0x00100000,
        OpacityMicromapWrite       = 0x00200000,
        OpacityMicromapBuildInput  = 0x00400000,
        ConvertCoopVecMatrixInput  = 0x00800000,
        ConvertCoopVecMatrixOutput = 0x01000000,
    };

    RHI_ENUM_CLASS_FLAG_OPERATORS(ResourceStates)

    // flags for resources that need to be shared with other graphics APIs or other GPU devices.
    enum class SharedResourceFlags : uint32_t {
        None = 0,

        // D3D11  : adds D3D11_RESOURCE_MISC_SHARED
        // D3D12  : adds D3D12_HEAP_FLAG_SHARED
        // Vulkan : adds vk::ExternalMemoryImageCreateInfo and vk::ExportMemoryAllocateInfo/vk::ExternalMemoryBufferCreateInfo
        Shared = 0x01,

        // D3D11         : adds (D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX | D3D11_RESOURCE_MISC_SHARED_NTHANDLE)
        // D3D12, Vulkan : ignored
        Shared_NTHandle = 0x02,

        // D3D12         : adds D3D12_RESOURCE_FLAG_ALLOW_CROSS_ADAPTER and D3D12_HEAP_FLAG_SHARED_CROSS_ADAPTER
        // D3D11, Vulkan : ignored
        Shared_CrossAdapter = 0x04,
    };

    RHI_ENUM_CLASS_FLAG_OPERATORS(SharedResourceFlags)

    struct TextureDesc {
    public:
        uint32_t         width          = 1;
        uint32_t         height         = 1;
        uint32_t         depth          = 1;
        uint32_t         array_size     = 1;
        uint32_t         mip_levels     = 1;
        uint32_t         sample_count   = 1;
        uint32_t         sample_quality = 0;
        Format           format         = Format::UNKNOWN;
        TextureDimension dimension      = TextureDimension::Texture2D;

        bool is_shader_resource      = true; // Note : is_shader_resource is initialized to 'true' for backward compatibility ( NVRHI )
        bool is_render_target        = false;
        bool is_uav                  = false;
        bool is_typeless             = false;
        bool is_shading_rate_surface = false;

        SharedResourceFlags shared_resource_flags = SharedResourceFlags::None;

        // indicates that the texture is created with no backing memory,
        // and memory is bound to the texture later using bindTextureMemory.
        // On DX12, the texture resource is created at the time of memory binding.
        bool is_virtual = false;
        bool is_tiled   = false;

        Color clear_value;
        bool  use_clear_value = false;

        ResourceStates initial_state = ResourceStates::Unknown;

        // if keep_initial_state is true, command lists that use the texture will automatically
        // begin tracking the texture from the initial state and transition it to the initial state
        // on command list close.
        bool keep_initial_state = false;

        std::string debug_name;

        // clang-format off
        constexpr TextureDesc& setWidth(uint32_t value) { width = value; return *this; }
        constexpr TextureDesc& setHeight(uint32_t value) { height = value; return *this; }
        constexpr TextureDesc& setDepth(uint32_t value) { depth = value; return *this; }
        constexpr TextureDesc& setArraySize(uint32_t value) { array_size = value; return *this; }
        constexpr TextureDesc& setMipLevels(uint32_t value) { mip_levels = value; return *this; }
        constexpr TextureDesc& setSampleCount(uint32_t value) { sample_count = value; return *this; }
        constexpr TextureDesc& setSampleQuality(uint32_t value) { sample_quality = value; return *this; }
        constexpr TextureDesc& setFormat(Format value) { format = value; return *this; }
        constexpr TextureDesc& setDimension(TextureDimension value) { dimension = value; return *this; }
        TextureDesc& setDebugName(const std::string& value) { debug_name = value; return *this; }
        constexpr TextureDesc& setIsRenderTarget(bool value) { is_render_target = value; return *this; }
        constexpr TextureDesc& setIsUAV(bool value) { is_uav = value; return *this; }
        constexpr TextureDesc& setIsTypeless(bool value) { is_typeless = value; return *this; }
        constexpr TextureDesc& setIsVirtual(bool value) { is_virtual = value; return *this; }
        constexpr TextureDesc& setClearValue(const Color& value) { clear_value = value; use_clear_value = true; return *this; }
        constexpr TextureDesc& setUseClearValue(bool value) { use_clear_value = value; return *this; }
        constexpr TextureDesc& setInitialState(ResourceStates value) { initial_state = value; return *this; }
        constexpr TextureDesc& setKeepInitialState(bool value) { keep_initial_state = value; return *this; }
        constexpr TextureDesc& setSharedResourceFlags(SharedResourceFlags value) { shared_resource_flags = value; return *this; }
        // clang-format on

        // Equivalent to .setInitialState(_initial_state).setKeepInitialState(true)
        constexpr TextureDesc& enableAutomaticStateTracking(ResourceStates initial_state) {
            this->initial_state      = initial_state;
            this->keep_initial_state = true;
            return *this;
        }

        TextureDesc()  = default;
        ~TextureDesc() = default;
    };

    // this is a GPU Texture - POD resource, which is managed by rhi::ResourceManager
    struct Texture {
    public:
        // backend-private storage
        void* backend_handle = nullptr;

        uint32_t width;
        uint32_t height;
        Format   format;

        Texture()  = default;
        ~Texture() = default;
    };

} // namespace rhi
