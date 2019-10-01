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

#include <array>
#include <cstddef>

namespace vm {
    namespace detail {
        template <typename T, typename U, std::size_t S, std::size_t... I>
        constexpr std::array<T, sizeof...(I)> cast_array_helper(const std::array<U, S>& a, const std::index_sequence<I...>) {
            return { static_cast<T>(a[I])... };
        }

        template <typename T, typename U, std::size_t S>
        constexpr std::array<T, S> cast_array(const std::array<U, S>& a) {
            return cast_array_helper<T>(a, std::make_index_sequence<S>{});
        }

        template <typename T, typename U, std::size_t R, std::size_t C, std::size_t... I>
        constexpr std::array<vec<T, R>, C> cast_matrix_rows_helper(const std::array<vec<U, R>, C>& a, const std::index_sequence<I...>) {
            return { vec<T, R>(cast_array<T>(a[I].v))... };
        }

        template <typename T, typename U, std::size_t R, std::size_t C>
        constexpr std::array<vec<T, R>, C> cast_matrix_rows(const std::array<vec<U, R>, C>& a) {
            return cast_matrix_rows_helper<T>(a, std::make_index_sequence<C>{});
        }

        template <typename T, std::size_t S1, std::size_t... I1, std::size_t S2, std::size_t... I2>
        constexpr std::array<T, S1+S2> concat_helper(
            const std::array<T, S1>& a1, const std::index_sequence<I1...>,
            const std::array<T, S2>& a2, const std::index_sequence<I2...>) {
            return { a1[I1]..., a2[I2]... };
        }

        template <typename T, std::size_t S1, std::size_t S2>
        constexpr std::array<T, S1+S2> concat(
            const std::array<T, S1>& a1,
            const std::array<T, S2>& a2) {
            return concat_helper(a1, std::make_index_sequence<S1>{}, a2, std::make_index_sequence<S2>{});
        }

        template<typename T, std::size_t S, std::size_t... I>
        constexpr std::array<T, S> to_array_helper(
            std::initializer_list<T> v,
            std::index_sequence<I...>) {
            return { *(v.begin() + I)... };
        }

        template<typename T, std::size_t S>
        constexpr std::array<T, S> to_array(std::initializer_list<T> v) {
            return to_array_helper<T, S>(v, std::make_index_sequence<S>());
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

        template <std::size_t... I>
        constexpr auto index_sequence(std::integer_sequence<std::size_t, I...>) noexcept -> std::array<std::size_t, sizeof...(I)> {
            return { { I... } };
        }

        template <std::size_t S>
        constexpr std::array<std::size_t, S> index_sequence() noexcept {
            return index_sequence(std::make_integer_sequence<std::size_t, S>{});
        }

        template <typename T, std::size_t S>
        constexpr std::array<T,S> get_elements(const std::array<T, S>& a, const std::array<std::size_t, S>& s) {
            auto result = std::array<T,S>();
            for (std::size_t i = 0; i < S; ++i) {
                result[i] = a[s[i]];
            }
            return result;
        }

        template<class T>
        constexpr void swap(T& l, T& r) {
            T tmp = std::move(l);
            l = std::move(r);
            r = std::move(tmp);
        }

        template <typename T, std::size_t S, typename C>
        constexpr void sort_helper(const std::array<T, S>& a, std::array<std::size_t, S>& s, const std::size_t l, const std::size_t r, const C& cmp) {
            if (l < r) {
                auto m = l;

                for (auto i = l + 1; i < r; ++i) {
                    if (cmp(a[s[i]], a[s[l]])) {
                        swap(s[++m], s[i]);
                    }
                }

                swap(s[l], s[m]);

                sort_helper(a, s, l,     m, cmp);
                sort_helper(a, s, m + 1, r, cmp);
            }
        }

        template <typename T, std::size_t S, typename C>
        constexpr std::array<std::size_t, S> sort(const std::array<T, S>& a, const C& cmp) {
            auto s = index_sequence<S>();
            sort_helper(a, s, 0, S, cmp);
            return s;
        }
    }
}

#endif
