/*=================================================

    Copyright (C) 2025 Farrakh.  All Rights Reserved.
    
    This source is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : texture.hpp
    
    Content : Texture - Part of RHI. Mostly a copy of NVRHI ( https://github.com/NVIDIA-RTX/NVRHI )

=================================================*/

#pragma once
#include "Renderer/RHI/basic_types.hpp"
#include "Renderer/RHI/heap.hpp"

namespace ata {
    namespace rhi {
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

        using MipLevel   = uint32_t;
        using ArraySlice = uint32_t;

        struct TextureDesc {
            uint32_t         width         = 1;
            uint32_t         height        = 1;
            uint32_t         depth         = 1;
            uint32_t         arraySize     = 1;
            uint32_t         mipLevels     = 1;
            uint32_t         sampleCount   = 1;
            uint32_t         sampleQuality = 0;
            Format           format        = Format::UNKNOWN;
            TextureDimension dimension     = TextureDimension::Texture2D;
            std::string      debugName;

            bool isShaderResource     = false;
            bool isRenderTarget       = false;
            bool isUAV                = false;
            bool isTypeless           = false;
            bool isShadingRateSurface = false;

            SharedResourceFlags sharedResourceFlags = SharedResourceFlags::None;

            // indicates that the texture is created with no backing memory,
            // and memory is bound to the texture later using bindTextureMemory.
            bool isVirtual = false;
            bool isTiled   = false;

            Color clearValue;
            bool  useClearValue = false;

            ResourceStates initialState = ResourceStates::Unknown;

            // if keepInitialState is true, command lists that use the texture will automatically
            // begin tracking the texture from the initial state and transition it to the initial state
            // on command list close.
            bool keepInitialState = false;

            constexpr TextureDesc& setWidth(uint32_t value) {
                width = value;
                return *this;
            }
            constexpr TextureDesc& setHeight(uint32_t value) {
                height = value;
                return *this;
            }
            constexpr TextureDesc& setDepth(uint32_t value) {
                depth = value;
                return *this;
            }
            constexpr TextureDesc& setArraySize(uint32_t value) {
                arraySize = value;
                return *this;
            }
            constexpr TextureDesc& setMipLevels(uint32_t value) {
                mipLevels = value;
                return *this;
            }
            constexpr TextureDesc& setSampleCount(uint32_t value) {
                sampleCount = value;
                return *this;
            }
            constexpr TextureDesc& setSampleQuality(uint32_t value) {
                sampleQuality = value;
                return *this;
            }
            constexpr TextureDesc& setFormat(Format value) {
                format = value;
                return *this;
            }
            constexpr TextureDesc& setDimension(TextureDimension value) {
                dimension = value;
                return *this;
            }
            TextureDesc& setDebugName(const std::string& value) {
                debugName = value;
                return *this;
            }
            constexpr TextureDesc& setIsRenderTarget(bool value) {
                isRenderTarget = value;
                return *this;
            }
            constexpr TextureDesc& setIsUAV(bool value) {
                isUAV = value;
                return *this;
            }
            constexpr TextureDesc& setIsTypeless(bool value) {
                isTypeless = value;
                return *this;
            }
            constexpr TextureDesc& setIsVirtual(bool value) {
                isVirtual = value;
                return *this;
            }
            constexpr TextureDesc& setClearValue(const Color& value) {
                clearValue    = value;
                useClearValue = true;
                return *this;
            }
            constexpr TextureDesc& setUseClearValue(bool value) {
                useClearValue = value;
                return *this;
            }
            constexpr TextureDesc& setInitialState(ResourceStates value) {
                initialState = value;
                return *this;
            }
            constexpr TextureDesc& setKeepInitialState(bool value) {
                keepInitialState = value;
                return *this;
            }
            constexpr TextureDesc& setSharedResourceFlags(SharedResourceFlags value) {
                sharedResourceFlags = value;
                return *this;
            }

            // Equivalent to .setInitialState(initial_state).setKeepInitialState(true)
            constexpr TextureDesc& enableAutomaticStateTracking(ResourceStates initial_state) {
                initialState     = initial_state;
                keepInitialState = true;
                return *this;
            }
        };

        // Describes a 2D or 3D section of a single mip level, single array slice of a texture.
        struct TextureSlice {
            uint32_t x = 0;
            uint32_t y = 0;
            uint32_t z = 0;
            // -1 means the entire dimension is part of the region
            // resolve() below will translate these values into actual dimensions
            uint32_t width  = uint32_t(-1);
            uint32_t height = uint32_t(-1);
            uint32_t depth  = uint32_t(-1);

            MipLevel   mipLevel   = 0;
            ArraySlice arraySlice = 0;

            ATA_NODISCARD TextureSlice resolve(const TextureDesc& desc) const;

            constexpr TextureSlice& setOrigin(uint32_t vx = 0, uint32_t vy = 0, uint32_t vz = 0) {
                x = vx;
                y = vy;
                z = vz;
                return *this;
            }
            constexpr TextureSlice& setWidth(uint32_t value) {
                width = value;
                return *this;
            }
            constexpr TextureSlice& setHeight(uint32_t value) {
                height = value;
                return *this;
            }
            constexpr TextureSlice& setDepth(uint32_t value) {
                depth = value;
                return *this;
            }
            constexpr TextureSlice& setSize(uint32_t vx = uint32_t(-1), uint32_t vy = uint32_t(-1), uint32_t vz = uint32_t(-1)) {
                width  = vx;
                height = vy;
                depth  = vz;
                return *this;
            }
            constexpr TextureSlice& setMipLevel(MipLevel level) {
                mipLevel = level;
                return *this;
            }
            constexpr TextureSlice& setArraySlice(ArraySlice slice) {
                arraySlice = slice;
                return *this;
            }
        };

        struct TextureSubresourceSet {
            static constexpr MipLevel   ALL_MIP_LEVELS   = MipLevel(-1);
            static constexpr ArraySlice ALL_ARRAY_SLICES = ArraySlice(-1);

            MipLevel   baseMipLevel   = 0;
            MipLevel   numMipLevels   = 1;
            ArraySlice baseArraySlice = 0;
            ArraySlice numArraySlices = 1;

            TextureSubresourceSet() = default;

            TextureSubresourceSet(MipLevel base_mip_level, MipLevel num_mip_levels, ArraySlice base_array_slice, ArraySlice num_array_slices)
                : baseMipLevel(base_mip_level), numMipLevels(num_mip_levels), baseArraySlice(base_array_slice), numArraySlices(num_array_slices) {
            }

            ATA_NODISCARD TextureSubresourceSet resolve(const TextureDesc& desc, bool single_mip_level) const;
            ATA_NODISCARD bool                  isEntireTexture(const TextureDesc& desc) const;

            bool operator==(const TextureSubresourceSet& other) const {
                return baseMipLevel == other.baseMipLevel &&
                       numMipLevels == other.numMipLevels &&
                       baseArraySlice == other.baseArraySlice &&
                       numArraySlices == other.numArraySlices;
            }
            bool operator!=(const TextureSubresourceSet& other) const { return !(*this == other); }

            constexpr TextureSubresourceSet& setBaseMipLevel(MipLevel value) {
                baseMipLevel = value;
                return *this;
            }
            constexpr TextureSubresourceSet& setNumMipLevels(MipLevel value) {
                numMipLevels = value;
                return *this;
            }
            constexpr TextureSubresourceSet& setMipLevels(MipLevel base, MipLevel num) {
                baseMipLevel = base;
                numMipLevels = num;
                return *this;
            }
            constexpr TextureSubresourceSet& setBaseArraySlice(ArraySlice value) {
                baseArraySlice = value;
                return *this;
            }
            constexpr TextureSubresourceSet& setNumArraySlices(ArraySlice value) {
                numArraySlices = value;
                return *this;
            }
            constexpr TextureSubresourceSet& setArraySlices(ArraySlice base, ArraySlice num) {
                baseArraySlice = base;
                numArraySlices = num;
                return *this;
            }

            // see the bottom of this file for a specialization of std::hash<TextureSubresourceSet>
        };

        static const TextureSubresourceSet ALL_SUBRESOURCES = TextureSubresourceSet(0, TextureSubresourceSet::ALL_MIP_LEVELS, 0, TextureSubresourceSet::ALL_ARRAY_SLICES);

        class ITexture : public IResource {
        public:
            ATA_NODISCARD virtual const TextureDesc& getDesc() const = 0;

            // similar to getNativeObject, returns a native view for a specified set of subresources. Returns nullptr if unavailable.
            virtual Object getNativeView(ObjectType            object_type,
                                         Format                format           = Format::UNKNOWN,
                                         TextureSubresourceSet subresources     = ALL_SUBRESOURCES,
                                         TextureDimension      dimension        = TextureDimension::Unknown,
                                         bool                  is_read_only_dsv = false) = 0;
        };
        using TextureHandle = RefCountPtr<ITexture>;

        class IStagingTexture : public IResource {
        public:
            ATA_NODISCARD virtual const TextureDesc& getDesc() const = 0;
        };
        using StagingTextureHandle = RefCountPtr<IStagingTexture>;

        struct TiledTextureCoordinate {
            uint16_t mipLevel   = 0;
            uint16_t arrayLevel = 0;
            uint32_t x          = 0;
            uint32_t y          = 0;
            uint32_t z          = 0;
        };

        struct TiledTextureRegion {
            uint32_t tilesNum = 0;
            uint32_t width    = 0;
            uint32_t height   = 0;
            uint32_t depth    = 0;
        };

        struct TextureTilesMapping {
            TiledTextureCoordinate* tiledTextureCoordinates = nullptr;
            TiledTextureRegion*     tiledTextureRegions     = nullptr;
            uint64_t*               byteOffsets             = nullptr;
            uint32_t                numTextureRegions       = 0;
            IHeap*                  heap                    = nullptr;
        };

        struct PackedMipDesc {
            uint32_t numStandardMips                 = 0;
            uint32_t numPackedMips                   = 0;
            uint32_t numTilesForPackedMips           = 0;
            uint32_t startTileIndexInOverallResource = 0;
        };

        struct TileShape {
            uint32_t widthInTexels  = 0;
            uint32_t heightInTexels = 0;
            uint32_t depthInTexels  = 0;
        };

        struct SubresourceTiling {
            uint32_t widthInTiles                    = 0;
            uint32_t heightInTiles                   = 0;
            uint32_t depthInTiles                    = 0;
            uint32_t startTileIndexInOverallResource = 0;
        };

        enum SamplerFeedbackFormat : uint8_t {
            MIN_MIP_OPAQUE         = 0x0,
            MIP_REGION_USED_OPAQUE = 0x1,
        };

        struct SamplerFeedbackTextureDesc {
            SamplerFeedbackFormat samplerFeedbackFormat     = SamplerFeedbackFormat::MIN_MIP_OPAQUE;
            uint32_t              samplerFeedbackMipRegionX = 0;
            uint32_t              samplerFeedbackMipRegionY = 0;
            uint32_t              samplerFeedbackMipRegionZ = 0;
            ResourceStates        initialState              = ResourceStates::Unknown;
            bool                  keepInitialState          = false;
        };

        class ISamplerFeedbackTexture : public IResource {
        public:
            ATA_NODISCARD virtual const SamplerFeedbackTextureDesc& getDesc() const    = 0;
            virtual TextureHandle                                   getPairedTexture() = 0;
        };
        using SamplerFeedbackTextureHandle = RefCountPtr<ISamplerFeedbackTexture>;
    } // namespace rhi
} // namespace ata
