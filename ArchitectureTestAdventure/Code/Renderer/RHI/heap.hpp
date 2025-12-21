/*=================================================

    Copyright (C) 2025 Farrakh.  All Rights Reserved.
    
    This source is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : heap.hpp
    
    Content : Heap - Part of RHI. Mostly a copy of NVRHI ( https://github.com/NVIDIA-RTX/NVRHI )

=================================================*/

#pragma once
#include "Renderer/Common/resource.hpp"

namespace ata {
    namespace rhi {
        enum class HeapType : uint8_t {
            DeviceLocal,
            Upload,
            Readback
        };

        struct HeapDesc {
            uint64_t    capacity = 0;
            HeapType    type;
            std::string debugName;

            constexpr HeapDesc& setCapacity(uint64_t value) {
                capacity = value;
                return *this;
            }
            constexpr HeapDesc& setType(HeapType value) {
                type = value;
                return *this;
            }
            HeapDesc& setDebugName(const std::string& value) {
                debugName = value;
                return *this;
            }
        };

        class IHeap : public IResource {
        public:
            virtual const HeapDesc& getDesc() = 0;
        };

        struct MemoryRequirements {
            uint64_t size      = 0;
            uint64_t alignment = 0;
        };

        using HeapHandle = RefCountPtr<IHeap>;
    } // namespace rhi
} // namespace ata
