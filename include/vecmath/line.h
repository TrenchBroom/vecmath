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

#ifndef TRENCHBROOM_LINE_DECL
#define TRENCHBROOM_LINE_DECL

#include "vec.h"
#include "mat.h"

namespace vm {
    /**
     * An infinite line represented by a point and a direction.
     *
     * @tparam T the component type
     * @tparam S the number of components
     */
    template <typename T, size_t S>
    class line {
    public:
        using component_type = T;
        static constexpr std::size_t size = S;
    public:
        vec<T,S> point;
        vec<T,S> direction;

        constexpr line() :
        point(vec<T,S>::zero()),
        direction(vec<T,S>::zero()) {}

        // Copy and move constructors
        line(const line<T,S>& line) = default;
        line(line<T,S>&& other) noexcept = default;

        // Assignment operators
        line<T,S>& operator=(const line<T,S>& other) = default;
        line<T,S>& operator=(line<T,S>&& other) noexcept = default;

        /**
         * Converts the given line by converting its component type using static_cast.
         *
         * @tparam U the component type of the given line
         * @param line the line to convert
         */
        template <typename U>
        explicit constexpr line(const line<U,S>& line) :
        point(line.point),
        direction(line.direction) {}

        /**
         * Creates a new line with the given point and direction.
         *
         * @param i_point the point
         * @param i_direction the direction
         */
        constexpr line(const vec<T,S>& i_point, const vec<T,S>& i_direction) :
        point(i_point),
        direction(i_direction) {}

        /**
         * Returns the origin of this line.
         */
        constexpr vec<T,S> getOrigin() const {
            return point;
        }

        /**
         * Returns the direction of this line.
         */
        constexpr vec<T,S> getDirection() const {
            return direction;
        }

        /**
         * Transforms this line using the given transformation matrix. The translational part is not applied to the
         * direction, and the direction is normalized after the transformation was applied.
         *
         * @param transform the transformation to apply
         * @return the transformed line
         */
        line<T,S> transform(const mat<T,S+1,S+1>& transform) const {
            return line<T,S>(transform * point, normalize(strip_translation(transform) * direction));
        }

        /**
         * Transforms this line using the given transformation matrix at compile time. The translational part is not
         * applied to the direction, and the direction is normalized after the transformation was applied.
         *
         * @param transform the transformation to apply
         * @return the transformed line
         */
        constexpr line<T,S> transform_c(const mat<T,S+1,S+1>& transform) const {
            return line<T,S>(transform * point, normalize_c(strip_translation(transform) * direction));
        }

        /**
         * Returns a canonical representation of the given line. Since a line could be represented by any point on it
         * plus its direction, every line has an infinite number of representations. This function maps each representation
         * onto a unique representation.
         *
         * @return the canonical representation of this line
         */
        constexpr line<T,S> makeCanonical() const {
            // choose the point such that its support vector is orthogonal to
            // the direction of this line
            const auto distance = dot(point, direction);
            const auto newPoint = (point - distance * direction);

            // make sure the first nonzero component of the direction is positive
            auto newDirection = direction;
            for (size_t i = 0; i < S; ++i) {
                if (direction[i] != T(0)) {
                    if (direction[i] < T(0)) {
                        newDirection = -newDirection;
                    }
                    break;
                }
            }

            return line<T,S>(newPoint, newDirection);
        }
    };

    /**
     * Checks whether the given lines have equal components.
     *
     * @tparam T the component type
     * @tparam S the number of components
     * @param lhs the first line
     * @param rhs the second line
     * @param epsilon the epsilon value
     * @return true if all components of the given lines are equal, and false otherwise
     */
    template <typename T, size_t S>
    constexpr bool is_equal(const line<T,S>& lhs, const line<T,S>& rhs, const T epsilon) {
        return is_equal(lhs.point, rhs.point, epsilon) && is_equal(lhs.direction, rhs.direction, epsilon);
    }

    /**
     * Checks whether the two given lines are identical.
     *
     * @tparam T the component type
     * @tparam S the number of components
     * @param lhs the first line
     * @param rhs the second line
     * @return true if the given lines are identical, and false otherwise
     */
    template <typename T, size_t S>
    constexpr bool operator==(const line<T,S>& lhs, const line<T,S>& rhs) {
        const auto lhsC = lhs.makeCanonical();
        const auto rhsC = rhs.makeCanonical();
        return lhsC.point == rhsC.point && lhsC.direction == rhsC.direction;
    }

    /**
     * Checks whether the two given lines are identical.
     *
     * @tparam T the component type
     * @tparam S the number of components
     * @param lhs the first line
     * @param rhs the second line
     * @return false if the given lines are identical, and true otherwise
     */
    template <typename T, size_t S>
    constexpr bool operator!=(const line<T,S>& lhs, const line<T,S>& rhs) {
        const auto lhsC = lhs.makeCanonical();
        const auto rhsC = rhs.makeCanonical();
        return lhsC.point != rhsC.point || lhsC.direction != rhsC.direction;
    }

    /**
     * Prints a textual representation of the given line to the given stream.
     *
     * @tparam T the component type
     * @tparam S the number of components
     * @param stream the stream to print to
     * @param line the line to print
     * @return the given stream
     */
    template <typename T, size_t S>
    std::ostream& operator<<(std::ostream& stream, const line<T,S>& line) {
        stream << "{ point: (" << line.point << "), direction: (" << line.direction << ") }";
        return stream;
    }
}

#endif
