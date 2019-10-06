/*
Copyright (C) 2010-2019 Kristian Duske

This file is part of libvecmath.

libvecmath is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

libvecmath is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with libvecmath. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef VECMATH_CONSTEXPR_UTIL_h
#define VECMATH_CONSTEXPR_UTIL_h

#include <cstddef>
#include <iterator>

namespace vm {
    namespace detail {
        /**
         * Swaps the given values.
         *
         * @tparam T the value type
         * @param lhs the left hand side
         * @param rhs the right hand side
         */
        template<class T>
        constexpr void swap(T& lhs, T& rhs) {
            auto tmp = std::move(lhs);
            lhs = std::move(rhs);
            rhs = std::move(tmp);
        }

        /**
         * Swaps the contents of the given iterators.
         *
         * @tparam I the iterator type
         * @param lhs the left hand side
         * @param rhs the right hand side
         */
        template<class I>
        constexpr void iter_swap(I lhs, I rhs) {
            swap(*lhs, *rhs);
        }

        /**
         * Sorts the given range using the given comparator.
         *
         * @tparam I the iterator type, which must be a random access iterator
         * @tparam C the comparator type
         * @param left the beginning of the range
         * @param right the end of the range
         * @param cmp the comparator, which must induce a strict weak ordering on the elements in the given range
         */
        template <typename I, typename C>
        constexpr void sort(I left, I right, const C& cmp) {
            if (left < right) {
                auto mid = left;

                for (auto i = std::next(left); i < right; ++i) {
                    if (cmp(*i, *left)) {
                        iter_swap(++mid, i);
                    }
                }

                iter_swap(left, mid);
                sort(left, mid, cmp);
                sort(std::next(mid), right, cmp);
            }
        }
    }
}

#endif
