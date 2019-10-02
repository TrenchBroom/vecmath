/*
 Copyright (C) 2010-2017 Kristian Duske

 This file is part of TrenchBroom.

 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with TrenchBroom. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TRENCHBROOM_CONSTEXPR_MAT_UTIL_H
#define TRENCHBROOM_CONSTEXPR_MAT_UTIL_H

#include "constexpr_vec_util.h"

#include <array>
#include <cstddef>
#include <utility>

namespace vm {
    namespace detail {
        template <typename T, typename U, std::size_t R, std::size_t C, std::size_t... I>
        constexpr std::array<vec<T, R>, C> cast_matrix_rows_helper(const std::array<vec<U, R>, C>& a, const std::index_sequence<I...>) {
            return { vec<T, R>(cast_array<T>(a[I].v))... };
        }

        template <typename T, typename U, std::size_t R, std::size_t C>
        constexpr std::array<vec<T, R>, C> cast_matrix_rows(const std::array<vec<U, R>, C>& a) {
            return cast_matrix_rows_helper<T>(a, std::make_index_sequence<C>{});
        }

        template <typename T, std::size_t R, std::size_t C, std::size_t CI, std::size_t... RI>
        constexpr std::array<T, R> to_vec_helper(
            std::initializer_list<T> v,
            std::index_sequence<RI...>) {
            return { *(v.begin() + CI + RI * C)... };
        }

        template <typename T, std::size_t R, std::size_t C, std::size_t... CI>
        constexpr std::array<vec<T, R>, C> to_vec_array_helper(
            std::initializer_list<T> v,
            std::index_sequence<CI...>) {
            return { vec<T, R>(to_vec_helper<T, R, C, CI>(v, std::make_index_sequence<R>()))... };
        }

        template <typename T, std::size_t R, std::size_t C>
        constexpr std::array<vec<T, R>, C> to_vec_array(std::initializer_list<T> v) {
            return to_vec_array_helper<T, R, C>(v, std::make_index_sequence<C>());
        }
    }
}

#endif
