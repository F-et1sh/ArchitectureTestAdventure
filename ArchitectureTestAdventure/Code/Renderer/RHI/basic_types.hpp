/*=================================================

    Copyright (C) 2025 Farrakh.  All Rights Reserved.
    
    This source is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : basic_types.hpp
    
    Content : Basic Types - Part of RHI. Mostly a copy of NVRHI ( https://github.com/NVIDIA-RTX/NVRHI )

=================================================*/

#pragma once
#include "Renderer/Common/resource.hpp"

namespace ata {
    namespace rhi {

#define ATA_ENUM_CLASS_FLAG_OPERATORS(T)      \
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

        struct Color {
            float r, g, b, a;

            Color() : r(0.f), g(0.f), b(0.f), a(0.f) {}
            Color(float c) : r(c), g(c), b(c), a(c) {}
            Color(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {}

            bool operator==(const Color& _b) const { return r == _b.r && g == _b.g && b == _b.b && a == _b.a; }
            bool operator!=(const Color& _b) const { return !(*this == _b); }
        };

        struct Viewport {
            float minX, maxX;
            float minY, maxY;
            float minZ, maxZ;

            Viewport() : minX(0.f), maxX(0.f), minY(0.f), maxY(0.f), minZ(0.f), maxZ(1.f) {}

            Viewport(float width, float height) : minX(0.f), maxX(width), minY(0.f), maxY(height), minZ(0.f), maxZ(1.f) {}

            Viewport(float _minX, float _maxX, float _minY, float _maxY, float _minZ, float _maxZ)
                : minX(_minX), maxX(_maxX), minY(_minY), maxY(_maxY), minZ(_minZ), maxZ(_maxZ) {}

            bool operator==(const Viewport& b) const {
                return minX == b.minX && minY == b.minY && minZ == b.minZ && maxX == b.maxX && maxY == b.maxY && maxZ == b.maxZ;
            }
            bool operator!=(const Viewport& b) const { return !(*this == b); }

            ATA_NODISCARD float width() const { return maxX - minX; }
            ATA_NODISCARD float height() const { return maxY - minY; }
        };

        struct Rect {
            int minX, maxX;
            int minY, maxY;

            Rect() : minX(0), maxX(0), minY(0), maxY(0) {}
            Rect(int width, int height) : minX(0), maxX(width), minY(0), maxY(height) {}
            Rect(int _minX, int _maxX, int _minY, int _maxY) : minX(_minX), maxX(_maxX), minY(_minY), maxY(_maxY) {}
            explicit Rect(const Viewport& viewport)
                : minX(int(floorf(viewport.minX))), maxX(int(ceilf(viewport.maxX))), minY(int(floorf(viewport.minY))), maxY(int(ceilf(viewport.maxY))) {
            }

            bool operator==(const Rect& b) const {
                return minX == b.minX && minY == b.minY && maxX == b.maxX && maxY == b.maxY;
            }
            bool operator!=(const Rect& b) const { return !(*this == b); }

            ATA_NODISCARD int width() const { return maxX - minX; }
            ATA_NODISCARD int height() const { return maxY - minY; }
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

        enum class FormatKind : uint8_t {
            Integer,
            Normalized,
            Float,
            DepthStencil
        };

        struct FormatInfo {
            Format      format;
            const char* name;
            uint8_t     bytesPerBlock;
            uint8_t     blockSize;
            FormatKind  kind;
            bool        hasRed : 1;
            bool        hasGreen : 1;
            bool        hasBlue : 1;
            bool        hasAlpha : 1;
            bool        hasDepth : 1;
            bool        hasStencil : 1;
            bool        isSigned : 1;
            bool        isSRGB : 1;
        };

        const FormatInfo& getFormatInfo(Format format);

        enum class FormatSupport : uint32_t {
            None = 0,

            Buffer       = 1u << 0,
            IndexBuffer  = 1u << 1,
            VertexBuffer = 1u << 2,

            Texture      = 1u << 3,
            DepthStencil = 1u << 4,
            RenderTarget = 1u << 5,
            Blendable    = 1u << 6,

            ShaderLoad     = 1u << 7,
            ShaderSample   = 1u << 8,
            ShaderUavLoad  = 1u << 9,
            ShaderUavStore = 1u << 10,
            ShaderAtomic   = 1u << 11,
        };

        ATA_ENUM_CLASS_FLAG_OPERATORS(FormatSupport)

        enum class CPU_AccessMode : uint8_t {
            None,
            Read,
            Write
        };

        enum class ResourceStates : uint32_t {
            Unknown                    = 0,
            Common                     = 1u << 0,
            ConstantBuffer             = 1u << 1,
            VertexBuffer               = 1u << 2,
            IndexBuffer                = 1u << 3,
            IndirectArgument           = 1u << 4,
            ShaderResource             = 1u << 5,
            UnorderedAccess            = 1u << 6,
            RenderTarget               = 1u << 7,
            DepthWrite                 = 1u << 8,
            DepthRead                  = 1u << 9,
            StreamOut                  = 1u << 10,
            CopyDest                   = 1u << 11,
            CopySource                 = 1u << 12,
            ResolveDest                = 1u << 13,
            ResolveSource              = 1u << 14,
            Present                    = 1u << 15,
            AccelStructRead            = 1u << 16,
            AccelStructWrite           = 1u << 17,
            AccelStructBuildInput      = 1u << 18,
            AccelStructBuildBlas       = 1u << 19,
            ShadingRateSurface         = 1u << 20,
            OpacityMicromapWrite       = 1u << 21,
            OpacityMicromapBuildInput  = 1u << 22,
            ConvertCoopVecMatrixInput  = 1u << 23,
            ConvertCoopVecMatrixOutput = 1u << 24,
        };

        ATA_ENUM_CLASS_FLAG_OPERATORS(ResourceStates)

        // flags for resources that need to be shared with other graphics APIs or other GPU devices.
        enum class SharedResourceFlags : uint32_t {
            None = 0,

            // Vulkan : adds vk::ExternalMemoryImageCreateInfo and vk::ExportMemoryAllocateInfo/vk::ExternalMemoryBufferCreateInfo
            // OpenGL : ignored
            Shared = 0x01
        };

        ATA_ENUM_CLASS_FLAG_OPERATORS(SharedResourceFlags)
    } // namespace rhi
} // namespace ata
