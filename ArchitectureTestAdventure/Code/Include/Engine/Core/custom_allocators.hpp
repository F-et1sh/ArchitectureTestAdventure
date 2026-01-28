/*=================================================

    Copyright (C) 2026 Farrakh. All Rights Reserved.
    
    This file is a part of farrakhfattahov_gmail-com\solution
    Check README.md for more information.
    
    File : custom_allocators.hpp
    
    Content : custom allocators : Arena, Pool, Slab

=================================================*/

#pragma once

#include <vector>
#include "attributes.hpp"

namespace ata {
    struct ArenaMarker {
        size_t offset{};
    };

    class Arena {
    public:
        explicit Arena(size_t capacity)
            : m_buffer(new uint8_t[capacity]), m_capacity(capacity) {}

        ~Arena() { delete[] m_buffer; }

        ATA_NODISCARD void* allocate(size_t size, size_t alignment = alignof(std::max_align_t)) {
            size_t aligned_offset = (m_offset + alignment - 1) & ~(alignment - 1);

            if (aligned_offset + size > m_capacity) {
                return nullptr;
            }

            void* ptr = m_buffer + aligned_offset;
            m_offset  = aligned_offset + size;
            return ptr;
        }

        void reset() { m_offset = 0; }

        constexpr size_t get_used() const noexcept { return m_offset; }
        constexpr size_t get_available() const noexcept { return m_capacity - m_offset; }

        ArenaMarker save() const noexcept {
            return { m_offset };
        }

        void restore(ArenaMarker marker) noexcept {
            m_offset = marker.offset;
        }

    private:
        uint8_t* m_buffer = nullptr;
        size_t   m_capacity;
        size_t   m_offset = 0;
    };

} // namespace ata
