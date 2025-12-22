/*=================================================

    Copyright (C) 2025 Farrakh.  All Rights Reserved.
    
    This source is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : buffer.hpp
    
    Content : Buffer - Part of RHI. Mostly a copy of NVRHI ( https://github.com/NVIDIA-RTX/NVRHI )

=================================================*/

#pragma once
#include "Renderer/RHI/basic_types.hpp"
#include "Renderer/RHI/rhiHLSL.hpp"

namespace ata {
    namespace rhi {
        struct BufferDesc {
            uint64_t    byteSize     = 0;
            uint32_t    structStride = 0; // if non-zero it's structured
            uint32_t    maxVersions  = 0; // only valid and required to be nonzero for volatile buffers on Vulkan
            std::string debugName;
            Format      format                  = Format::UNKNOWN; // for typed buffer views
            bool        canHaveUAVs             = false;
            bool        canHaveTypedViews       = false;
            bool        canHaveRawViews         = false;
            bool        isVertexBuffer          = false;
            bool        isIndexBuffer           = false;
            bool        isConstantBuffer        = false;
            bool        isDrawIndirectArgs      = false;
            bool        isAccelStructBuildInput = false;
            bool        isAccelStructStorage    = false;
            bool        isShaderBindingTable    = false;

            // a dynamic/upload buffer whose contents only live in the current command list
            bool isVolatile = false;

            // indicates that the buffer is created with no backing memory,
            // and memory is bound to the buffer later using bindBufferMemory.
            bool isVirtual = false;

            ResourceStates initialState = ResourceStates::Common;

            // see TextureDesc::keepInitialState
            bool keepInitialState = false;

            CpuAccessMode cpuAccess = CpuAccessMode::None;

            SharedResourceFlags sharedResourceFlags = SharedResourceFlags::None;

            constexpr BufferDesc& setByteSize(uint64_t value) {
                byteSize = value;
                return *this;
            }
            constexpr BufferDesc& setStructStride(uint32_t value) {
                structStride = value;
                return *this;
            }
            constexpr BufferDesc& setMaxVersions(uint32_t value) {
                maxVersions = value;
                return *this;
            }
            BufferDesc& setDebugName(const std::string& value) {
                debugName = value;
                return *this;
            }
            constexpr BufferDesc& setFormat(Format value) {
                format = value;
                return *this;
            }
            constexpr BufferDesc& setCanHaveUAVs(bool value) {
                canHaveUAVs = value;
                return *this;
            }
            constexpr BufferDesc& setCanHaveTypedViews(bool value) {
                canHaveTypedViews = value;
                return *this;
            }
            constexpr BufferDesc& setCanHaveRawViews(bool value) {
                canHaveRawViews = value;
                return *this;
            }
            constexpr BufferDesc& setIsVertexBuffer(bool value) {
                isVertexBuffer = value;
                return *this;
            }
            constexpr BufferDesc& setIsIndexBuffer(bool value) {
                isIndexBuffer = value;
                return *this;
            }
            constexpr BufferDesc& setIsConstantBuffer(bool value) {
                isConstantBuffer = value;
                return *this;
            }
            constexpr BufferDesc& setIsDrawIndirectArgs(bool value) {
                isDrawIndirectArgs = value;
                return *this;
            }
            constexpr BufferDesc& setIsAccelStructBuildInput(bool value) {
                isAccelStructBuildInput = value;
                return *this;
            }
            constexpr BufferDesc& setIsAccelStructStorage(bool value) {
                isAccelStructStorage = value;
                return *this;
            }
            constexpr BufferDesc& setIsShaderBindingTable(bool value) {
                isShaderBindingTable = value;
                return *this;
            }
            constexpr BufferDesc& setIsVolatile(bool value) {
                isVolatile = value;
                return *this;
            }
            constexpr BufferDesc& setIsVirtual(bool value) {
                isVirtual = value;
                return *this;
            }
            constexpr BufferDesc& setInitialState(ResourceStates value) {
                initialState = value;
                return *this;
            }
            constexpr BufferDesc& setKeepInitialState(bool value) {
                keepInitialState = value;
                return *this;
            }
            constexpr BufferDesc& setCpuAccess(CpuAccessMode value) {
                cpuAccess = value;
                return *this;
            }

            // equivalent to .setInitialState(_initialState).setKeepInitialState(true)
            constexpr BufferDesc& enableAutomaticStateTracking(ResourceStates _initialState) {
                initialState     = _initialState;
                keepInitialState = true;
                return *this;
            }
        };

        struct BufferRange {
            uint64_t byteOffset = 0;
            uint64_t byteSize   = 0;

            BufferRange() = default;

            BufferRange(uint64_t _byteOffset, uint64_t _byteSize)
                : byteOffset(_byteOffset), byteSize(_byteSize) {}

            ATA_NODISCARD BufferRange    resolve(const BufferDesc& desc) const;
            ATA_NODISCARD constexpr bool isEntireBuffer(const BufferDesc& desc) const { return (byteOffset == 0) && (byteSize == ~0ull || byteSize == desc.byteSize); }
            constexpr bool               operator==(const BufferRange& other) const { return byteOffset == other.byteOffset && byteSize == other.byteSize; }

            constexpr BufferRange& setByteOffset(uint64_t value) {
                byteOffset = value;
                return *this;
            }
            constexpr BufferRange& setByteSize(uint64_t value) {
                byteSize = value;
                return *this;
            }
        };

        static const BufferRange EntireBuffer = BufferRange(0, ~0ull);

        class IBuffer : public IResource {
        public:
            ATA_NODISCARD virtual const BufferDesc& getDesc() const              = 0;
            ATA_NODISCARD virtual GpuVirtualAddress getGpuVirtualAddress() const = 0;
        };

        using BufferHandle = RefCountPtr<IBuffer>;
    } // namespace rhi
} // namespace ata
