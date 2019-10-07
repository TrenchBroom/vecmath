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

#ifndef VECMATH_VEC_EXT_H
#define VECMATH_VEC_EXT_H

#include "vec.h"

#include <array>
#include <vector>

namespace vm {
    /**
     * Adds the given vector to each of the vectors in the given vector.
     *
     * @tparam T the component type
     * @tparam S the number of components
     * @param lhs the vector of vectors
     * @param rhs the right hand vector
     * @return a vector containing the sum of each of the vectors in the given vector with the right hand vector
     */
    template <typename T, std::size_t S>
    std::vector<vec<T,S>> operator+(const std::vector<vec<T,S>>& lhs, const vec<T,S>& rhs) {
        std::vector<vec<T,S>> result;
        result.reserve(lhs.size());
        for (const auto& vec : lhs) {
            result.push_back(vec + rhs);
        }
        return result;
    }

    /**
     * Adds the given vector to each of the vectors in the given array.
     *
     * @tparam T the component type
     * @tparam S the number of components
     * @tparam N the number of elements in the given array
     * @param lhs the array of vectors
     * @param rhs the right hand vector
     * @return an array containing the sum of each of the vectors in the given array with the right hand vector
     */
    template <typename T, std::size_t S, std::size_t N>
    constexpr std::array<vec<T,S>, N> operator+(const std::array<vec<T,S>, N>& lhs, const vec<T,S>& rhs) {
        std::array<vec<T,S>, N> result;
        for (std::size_t i = 0; i < N; ++i) {
            result[i] = lhs[i] + rhs;
        }
        return result;
    }

    /**
     * Adds the given vector to each of the vectors in the given vector.
     *
     * @tparam T the component type
     * @tparam S the number of components
     * @param lhs the left hand vector
     * @param rhs the vector of vectors
     * @return a vector containing the sum of each of the vectors in the given vector with the left hand vector
     */
    template <typename T, std::size_t S>
    std::vector<vec<T,S>> operator+(const vec<T,S>& lhs, const std::vector<vec<T,S>>& rhs) {
        return rhs + lhs;
    }

    /**
     * Adds the given vector to each of the vectors in the given array.
     *
     * @tparam T the component type
     * @tparam S the number of components
     * @tparam N the number of elements in the given array
     * @param lhs the left hand vector
     * @param rhs the array of vectors
     * @return an array containing the sum of each of the vectors in the given array with the left hand vector
     */
    template <typename T, std::size_t S, std::size_t N>
    constexpr std::array<vec<T,S>, N> operator+(const vec<T,S>& lhs, const std::array<vec<T,S>, N>& rhs) {
        return rhs + lhs;
    }

    /**
     * Multiplies each vector in the given range by the given scalar.
     *
     * @tparam T the component type
     * @tparam S the number of components
     * @param lhs the range of vectors
     * @param rhs the scalar factor
     * @return a range containing the scalar product of each vector in the given vector with the given scalar
     */
    template <typename T, std::size_t S>
    std::vector<vec<T,S>> operator*(const std::vector<vec<T,S>>& lhs, const T rhs) {
        std::vector<vec<T,S>> result;
        result.reserve(lhs.size());
        for (const auto& vec : lhs) {
            result.push_back(vec * rhs);
        }
        return result;
    }

    /**
     * Multiplies each vector in the given array by the given scalar.
     *
     * @tparam T the component type
     * @tparam S the number of components
     * @param lhs the array of vectors
     * @param rhs the scalar factor
     * @return an array containing the scalar product of each vector in the given array with the given scalar
     */
    template <typename T, std::size_t S, std::size_t N>
    constexpr std::array<vec<T,S>, N> operator*(const std::array<vec<T,S>, N>& lhs, const T rhs) {
        std::array<vec<T,S>, N> result;
        for (std::size_t i = 0u; i < N; ++i) {
            result[i] = lhs[i] * rhs;
        }
        return result;
    }

    /**
     * Multiplies each vector in the given vector by the given scalar.
     *
     * @tparam T the component type
     * @tparam S the number of components
     * @param lhs the scalar factor
     * @param rhs the vector of vectors
     * @return a vector containing the scalar product of each vector in the given vector with the given scalar
     */
    template <typename T, std::size_t S>
    std::vector<vec<T,S>> operator*(const T lhs, const std::vector<vec<T,S>>& rhs) {
        return rhs * lhs;
    }

    /**
     * Multiplies each vector in the given array by the given scalar.
     *
     * @tparam T the component type
     * @tparam S the number of components
     * @param lhs the scalar factor
     * @param rhs the array of vectors
     * @return an array containing the scalar product of each vector in the given array with the given scalar
     */
    template <typename T, std::size_t S, std::size_t N>
    constexpr std::array<vec<T,S>, N> operator*(const T lhs, const std::array<vec<T,S>, N>& rhs) {
        return rhs * lhs;
    }
}

#endif //VECMATH_VEC_EXT_H
