/*=================================================

    Copyright (C) 2026 Farrakh. All Rights Reserved.
    
    This file is a part of ArchitectureTestAdventure
    Check README.md for more information.
    
    File : custom_allocators.hpp
    
    Content : custom allocators : Arena and Pool

=================================================*/

#pragma once

#include <cassert>
#include <type_traits>
#include <memory>

#include "attributes.hpp"

namespace ata {
    struct ArenaMarker {
        size_t offset{};
    };

    class Arena { // mostly per-frame container
    public:
        explicit Arena(size_t capacity) : m_capacity(capacity) {
            m_buffer = static_cast<std::byte*>(::operator new(capacity, std::align_val_t{ alignof(std::max_align_t) }));
        }

        ~Arena() { ::operator delete[](m_buffer, std::align_val_t{ alignof(std::max_align_t) }); }

        ATA_CLASS_NONCOPYABLE(Arena)
        ATA_CLASS_MOVABLE(Arena)

        ATA_NODISCARD std::byte* allocate(size_t size, size_t alignment = alignof(std::max_align_t)) {
            assert(alignment > 0 && (alignment & (alignment - 1)) == 0); // check that it's a power of two

            size_t aligned_offset = (m_offset + alignment - 1) & ~(alignment - 1);

            if (aligned_offset + size > m_capacity) {
                return nullptr;
            }

            std::byte* ptr = m_buffer + aligned_offset;
            m_offset       = aligned_offset + size;
            return ptr;
        }

        void reset() { m_offset = 0; }

        ATA_NODISCARD constexpr size_t get_used_memory() const noexcept { return m_offset; }
        ATA_NODISCARD constexpr size_t get_available_memory() const noexcept { return m_capacity - m_offset; }

        ATA_NODISCARD ArenaMarker save() const noexcept {
            return { m_offset };
        }

        void restore(ArenaMarker marker) noexcept {
            assert(marker.offset <= m_offset);
            m_offset = marker.offset;
        }

    private:
        std::byte*   m_buffer   = nullptr;
        const size_t m_capacity = 0;
        size_t       m_offset   = 0;
    };

    template <typename _Ty>
    class Pool {
        struct FreeNode {
            FreeNode* next = nullptr;
        };

    public:
        explicit Pool(size_t count) {
            static_assert(sizeof(_Ty) >= sizeof(FreeNode*), "Object too small for pool");

            m_capacity = count;
            m_buffer   = ::operator new(count * sizeof(_Ty), std::align_val_t{ alignof(_Ty) });

            for (size_t i = 0; i < count; i++) {
                auto* node  = reinterpret_cast<FreeNode*>(m_buffer + i * sizeof(_Ty));
                node->next  = m_free_list;
                m_free_list = node;
            }
        }

        ~Pool() { ::operator delete[](m_buffer, std::align_val_t{ alignof(_Ty) }); }

        ATA_CLASS_NONCOPYABLE(Pool)

        ATA_NODISCARD _Ty* allocate() {
            if (!m_free_list) return nullptr;

            std::byte* ptr = m_free_list;
            m_free_list    = m_free_list->next;
            return static_cast<_Ty*>(ptr);
        }

        void deallocate(_Ty* ptr) {
            auto* node  = reinterpret_cast<FreeNode*>(ptr);
            node->next  = m_free_list;
            m_free_list = node;
        }

        // don't forget about Pool::destroy()
        template <typename... Args>
        ATA_NODISCARD _Ty* create(Args&&... args) {
            _Ty* ptr = allocate();
            std::construct_at(ptr, std::forward<Args>(args)...);
            return ptr;
        }

        void destroy(_Ty* ptr) {
            std::destroy_at(ptr);
            deallocate(ptr);
        }

    private:
        std::byte* m_buffer    = nullptr;
        size_t     m_capacity  = 0;
        FreeNode*  m_free_list = nullptr;
    };

} // namespace ata
