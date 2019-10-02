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

#ifndef TRENCHBROOM_ABSTRACT_LINE_H
#define TRENCHBROOM_ABSTRACT_LINE_H

#include <vecmath/vec.h>

#include <cstddef>

namespace vm {
    /*
     * An abstract line is a generalization of lines in space. Thereby, a line can be bounded or unbounded in either
     * direction. The following concepts arise:
     *
     * - If the line is unbounded in both directions, it is just that, a line.
     * - If the line is bounded in one direction, it is a ray.
     * - If the line is bounded in both directions, it is a segment.
     *
     * In any case, the concept of an abstract line requires that the concrete line type has the following members:
     *
     * - Line::component_type, the component type of the origin and direction
     * - Line::size, the number of dimensions
     * - vec<component_type, size> Line::getOrigin() const, returns the line's origin
     * - vec<component_type, size> Line::getDirection() const, returns the line's direction
     *
     * The functions in this file operate on this concept.
     */

    /**
     * Computes the distance from the origin to the orthogonal projection of the given point onto the direction of the
     * given abstract line.
     *
     * @param abstract_line the line to project onto
     * @param point the point to project
     * @return the distance from the origin to the orthogonal projection of the given point
     */
    template <typename AL>
    constexpr typename AL::component_type distanceToProjectedPoint(const AL& abstract_line, const vec<typename AL::component_type, AL::size>& point) {
        return dot(point - abstract_line.getOrigin(), abstract_line.getDirection());
    }

    /**
     * Computes the point on the given abstract line at the given distance from the line's origin.
     *
     * @param abstract_line the line to compute the point on
     * @param distance the distance of the point
     * @return the point
     */
    template <typename AL>
    constexpr vec<typename AL::component_type, AL::size> pointAtDistance(const AL& abstract_line, const typename AL::component_type distance) {
        return abstract_line.getOrigin() + abstract_line.getDirection() * distance;
    }

    /**
     * Orthogonally projects the given point onto the given abstract line.
     *
     * @param abstract_line the line to project the given point onto
     * @param point the point to project
     * @return the projected point
     */
    template <typename AL>
    constexpr vec<typename AL::component_type, AL::size> projectPoint(const AL& abstract_line, const vec<typename AL::component_type, AL::size>& point) {
        return pointAtDistance(abstract_line, distanceToProjectedPoint(abstract_line, point));
    }
}

#endif //TRENCHBROOM_ABSTRACT_LINE_H
