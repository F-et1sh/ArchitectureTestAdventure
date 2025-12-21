/*=================================================

    Copyright (C) 2025 Farrakh.  All Rights Reserved.
    
    This source is a part of ArchitectureTestAdventure.
    Check README.md for more information.
    
    File : misc.hpp
    
    Content : Some useful functions

=================================================*/

#pragma once
#include <cstdint>
#include "attributes.hpp"

namespace ata {
    template <typename T>
    ATA_NODISCARD T align(T size, T alignment) {
        return (size + alignment - 1) & ~(alignment - 1);
    }

    template <typename T, typename U>
    ATA_NODISCARD bool arraysAreDifferent(const T& a, const U& b) {
        if (a.size() != b.size())
            return true;

        for (uint32_t i = 0; i < uint32_t(a.size()); i++) {
            if (a[i] != b[i])
                return true;
        }

        return false;
    }

    template <typename T, typename U>
    ATA_NODISCARD uint32_t arrayDifferenceMask(const T& a, const U& b) {
        assert(a.size() <= 32);
        assert(b.size() <= 32);

        if (a.size() != b.size())
            return ~0u;

        uint32_t mask = 0;
        for (uint32_t i = 0; i < uint32_t(a.size()); i++) {
            if (a[i] != b[i])
                mask |= (1 << i);
        }

        return mask;
    }

    ATA_NODISCARD inline uint32_t hash_to_u32(size_t hash) {
        return uint32_t(hash) ^ (uint32_t(hash >> 32));
    }

    // a type cast that is safer than static_cast in debug builds, and is a simple static_cast in release builds.
    // used for downcasting various ISomething* pointers to their implementation classes in the backends.
    template <typename T, typename U>
    ATA_NODISCARD T checked_cast(U u) {
        static_assert(!std::is_same_v<T, U>, "Redundant checked_cast");
#ifdef _DEBUG
        if (!u) return nullptr;
        T t = dynamic_cast<T>(u);
        if (!t) assert(!"Invalid type cast");
        return t;
#else
        return static_cast<T>(u);
#endif
    }
} // namespace ata
