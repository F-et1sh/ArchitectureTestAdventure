/*=================================================

    Copyright (C) 2025 Farrakh.  All Rights Reserved.
    
    This source is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : texture.cpp
    
    Content : Texture - Part of RHI. Mostly a copy of NVRHI ( https://github.com/NVIDIA-RTX/NVRHI )

=================================================*/

#include "pch.hpp"
#include "texture.hpp"

#undef min
#undef max

namespace ata {
    namespace rhi {
        TextureSlice TextureSlice::resolve(const TextureDesc& desc) const {
            TextureSlice ret(*this);

            assert(mipLevel < desc.mipLevels);

            if (width == uint32_t(-1))
                ret.width = std::max(desc.width >> mipLevel, 1u);

            if (height == uint32_t(-1))
                ret.height = std::max(desc.height >> mipLevel, 1u);

            if (depth == uint32_t(-1)) {
                if (desc.dimension == TextureDimension::Texture3D)
                    ret.depth = std::max(desc.depth >> mipLevel, 1u);
                else
                    ret.depth = 1;
            }

            return ret;
        }

        TextureSubresourceSet TextureSubresourceSet::resolve(const TextureDesc& desc, bool singleMipLevel) const {
            TextureSubresourceSet ret;
            ret.baseMipLevel = baseMipLevel;

            if (singleMipLevel) {
                ret.numMipLevels = 1;
            }
            else {
                int lastMipLevelPlusOne = std::min(baseMipLevel + numMipLevels, desc.mipLevels);
                ret.numMipLevels        = MipLevel(std::max(0u, lastMipLevelPlusOne - baseMipLevel));
            }

            switch (desc.dimension) {
                case TextureDimension::Texture1DArray:
                case TextureDimension::Texture2DArray:
                case TextureDimension::TextureCube:
                case TextureDimension::TextureCubeArray:
                case TextureDimension::Texture2DMSArray: {
                    ret.baseArraySlice        = baseArraySlice;
                    int lastArraySlicePlusOne = std::min(baseArraySlice + numArraySlices, desc.arraySize);
                    ret.numArraySlices        = ArraySlice(std::max(0u, lastArraySlicePlusOne - baseArraySlice));
                    break;
                }
                default:
                    ret.baseArraySlice = 0;
                    ret.numArraySlices = 1;
                    break;
            }

            return ret;
        }

        bool TextureSubresourceSet::isEntireTexture(const TextureDesc& desc) const {
            if (baseMipLevel > 0u || baseMipLevel + numMipLevels < desc.mipLevels)
                return false;

            switch (desc.dimension)
            {
                case TextureDimension::Texture1DArray:
                case TextureDimension::Texture2DArray:
                case TextureDimension::TextureCube:
                case TextureDimension::TextureCubeArray:
                case TextureDimension::Texture2DMSArray:
                    if (baseArraySlice > 0u || baseArraySlice + numArraySlices < desc.arraySize)
                        return false;
                default:
                    return true;
            }
        }
    } // namespace rhi
} // namespace ata
