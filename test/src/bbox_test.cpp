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

#include <gtest/gtest.h>

#include <vecmath/forward.h>
#include <vecmath/bbox.h>
#include <vecmath/vec.h>
#include <vecmath/mat.h>
#include <vecmath/mat_ext.h>

#include "test_utils.h"

#include <vector>

namespace vm {
    TEST(bbox_test, constructor_default) {
        constexpr auto bounds = bbox3f();
        CER_ASSERT_EQ(vec3f::zero(), bounds.min)
        CER_ASSERT_EQ(vec3f::zero(), bounds.max)
    }

    TEST(bbox_test, constructor_with_min_max_points) {
        constexpr auto min = vec3f(-1, -2, -3);
        constexpr auto max = vec3f( 1,  2,  3);

        constexpr auto bounds = bbox3f(min, max);
        CER_ASSERT_EQ(min, bounds.min)
        CER_ASSERT_EQ(max, bounds.max)
    }

    TEST(bbox_test, constructor_with_min_max_values) {
        constexpr auto min = -16.f;
        constexpr auto max = +32.0f;

        constexpr auto bounds =  bbox3f(min, max);
        CER_ASSERT_EQ(vec3f::fill(min), bounds.min)
        CER_ASSERT_EQ(vec3f::fill(max), bounds.max)
    }

    TEST(bbox_test, constructor_with_minmax_value) {
        constexpr auto minMax = 16.f;

        constexpr auto bounds = bbox3f(minMax);
        CER_ASSERT_EQ(-vec3f::fill(minMax), bounds.min)
        CER_ASSERT_EQ(+vec3f::fill(minMax), bounds.max)
    }

    TEST(bbox_test, merge_all) {
        constexpr auto points = std::array<vec3d, 6>{
            vec3d(-32, -16, - 8), vec3d(  0, - 4, -4),
            vec3d(+ 4, + 8, -16), vec3d(+32, +16, -4),
            vec3d(+16, + 4, - 8), vec3d(+24, +32, +4)
        };

        constexpr auto min = vm::min(points[0], points[1], points[2], points[3], points[4], points[5]);
        constexpr auto max = vm::max(points[0], points[1], points[2], points[3], points[4], points[5]);

        constexpr auto merged = bbox3d::merge_all(std::begin(points), std::end(points));
        CER_ASSERT_EQ(min, merged.min)
        CER_ASSERT_EQ(max, merged.max)
    }

    TEST(bbox_test, is_valid) {
        CER_ASSERT_TRUE(bbox3d::is_valid(vec3d::zero(), vec3d::zero()))
        CER_ASSERT_TRUE(bbox3d::is_valid(vec3d(-1, -1, -1), vec3d(+1, +1, +1)))
        CER_ASSERT_FALSE(bbox3d::is_valid(vec3d(+1, -1, -1), vec3d(-1, +1, +1)))
        CER_ASSERT_FALSE(bbox3d::is_valid(vec3d(-1, +1, -1), vec3d(+1, -1, +1)))
        CER_ASSERT_FALSE(bbox3d::is_valid(vec3d(-1, -1, +1), vec3d(+1, +1, -1)))
    }

    TEST(bbox_test, is_empty) {
        CER_ASSERT_TRUE(bbox3d().is_empty())
        CER_ASSERT_FALSE(bbox3d(1.0).is_empty())
        CER_ASSERT_TRUE(bbox3d(vec3d(-1, 0, -1), vec3d(+1, 0, +1)).is_empty())
    }

    TEST(bbox_test, center) {
        constexpr auto min = vec3f(-1, -2, -3);
        constexpr auto max = vec3f( 1,  4,  5);
        constexpr auto bounds = bbox3f(min, max);

        CER_ASSERT_EQ(vec3f(0, 1, 1), bounds.center())
    }

    TEST(bbox_test, size) {
        constexpr auto min = vec3f(-1, -2, -3);
        constexpr auto max = vec3f( 1,  3,  5);
        constexpr auto bounds = bbox3f(min, max);

        CER_ASSERT_EQ(vec3f(2, 5, 8), bounds.size())
    }

    TEST(bbox_test, volume) {
        CER_ASSERT_DOUBLE_EQ(0.0, bbox3d().volume())
        CER_ASSERT_DOUBLE_EQ(4.0 * 4.0 * 4.0, bbox3d(2.0).volume())
    }

    TEST(bbox_test, contains_point) {
        constexpr auto bounds = bbox3f(vec3f(-12, -3,  4), vec3f( 8, 9, 8));
        CER_ASSERT_TRUE(bounds.contains(vec3f(2, 1, 7)))
        CER_ASSERT_TRUE(bounds.contains(vec3f(-12, -3, 7)))
        CER_ASSERT_FALSE(bounds.contains(vec3f(-13, -3, 7)))
    }

    TEST(bbox_test, contains_bbox) {
        constexpr auto bounds1 = bbox3f(vec3f(-12, -3,  4), vec3f( 8, 9, 8));
        constexpr auto bounds2 = bbox3f(vec3f(-10, -2,  5), vec3f( 7, 8, 7));
        constexpr auto bounds3 = bbox3f(vec3f(-13, -2,  5), vec3f( 7, 8, 7));
        CER_ASSERT_TRUE(bounds1.contains(bounds1))
        CER_ASSERT_TRUE(bounds1.contains(bounds2))
        CER_ASSERT_FALSE(bounds1.contains(bounds3))
    }

    TEST(bbox_test, encloses) {
        constexpr auto bounds1 = bbox3f(vec3f(-12, -3,  4), vec3f( 8, 9, 8));
        constexpr auto bounds2 = bbox3f(vec3f(-10, -2,  5), vec3f( 7, 8, 7));
        constexpr auto bounds3 = bbox3f(vec3f(-10, -3,  5), vec3f( 7, 8, 7));
        CER_ASSERT_FALSE(bounds1.encloses(bounds1))
        CER_ASSERT_TRUE(bounds1.encloses(bounds2))
        CER_ASSERT_FALSE(bounds1.encloses(bounds3))
    }

    TEST(bbox_test, intersects) {
        constexpr auto bounds1 = bbox3f(vec3f(-12.0f, -3.0f,  4.0f), vec3f(  8.0f,  9.0f,  8.0f));
        constexpr auto bounds2 = bbox3f(vec3f(-10.0f, -2.0f,  5.0f), vec3f(  7.0f,  8.0f,  7.0f));
        constexpr auto bounds3 = bbox3f(vec3f(-13.0f, -2.0f,  5.0f), vec3f(  7.0f,  8.0f,  7.0f));
        constexpr auto bounds4 = bbox3f(vec3f(-15.0f, 10.0f,  9.0f), vec3f(-13.0f, 12.0f, 10.0f));
        constexpr auto bounds5 = bbox3f(vec3f(-15.0f, 10.0f,  9.0f), vec3f(-12.0f, 12.0f, 10.0f));
        CER_ASSERT_TRUE(bounds1.intersects(bounds1))
        CER_ASSERT_TRUE(bounds1.intersects(bounds2))
        CER_ASSERT_TRUE(bounds1.intersects(bounds3))
        CER_ASSERT_FALSE(bounds1.intersects(bounds4))
        CER_ASSERT_FALSE(bounds1.intersects(bounds5))
    }

    TEST(bbox_test, constrain) {
        constexpr auto bounds = bbox3d(1024.0);
        CER_ASSERT_VEC_EQ(vec3d::zero(), bounds.constrain(vec3d::zero()))
        CER_ASSERT_VEC_EQ(bounds.min, bounds.constrain(bounds.min))
        CER_ASSERT_VEC_EQ(bounds.min, bounds.constrain(bounds.min + vec3d::neg_x()))
        CER_ASSERT_VEC_EQ(bounds.min, bounds.constrain(bounds.min + vec3d::neg_y()))
        CER_ASSERT_VEC_EQ(bounds.min, bounds.constrain(bounds.min + vec3d::neg_z()))
        CER_ASSERT_VEC_EQ(bounds.max, bounds.constrain(bounds.max + vec3d::pos_x()))
        CER_ASSERT_VEC_EQ(bounds.max, bounds.constrain(bounds.max + vec3d::pos_y()))
        CER_ASSERT_VEC_EQ(bounds.max, bounds.constrain(bounds.max + vec3d::pos_z()))
    }

    TEST(bbox_test, corner) {
        constexpr auto min = vec3f(-1.0f, -2.0f, -3.0f);
        constexpr auto max = vec3f( 1.0f,  3.0f,  5.0f);
        constexpr auto bounds = bbox3f(min, max);

        CER_ASSERT_VEC_EQ(vec3f(-1.0f, -2.0f, -3.0f), bounds.corner(bbox3f::Corner::min, bbox3f::Corner::min, bbox3f::Corner::min))
        CER_ASSERT_VEC_EQ(vec3f(-1.0f, -2.0f,  5.0f), bounds.corner(bbox3f::Corner::min, bbox3f::Corner::min, bbox3f::Corner::max))
        CER_ASSERT_VEC_EQ(vec3f(-1.0f,  3.0f, -3.0f), bounds.corner(bbox3f::Corner::min, bbox3f::Corner::max, bbox3f::Corner::min))
        CER_ASSERT_VEC_EQ(vec3f(-1.0f,  3.0f,  5.0f), bounds.corner(bbox3f::Corner::min, bbox3f::Corner::max, bbox3f::Corner::max))
        CER_ASSERT_VEC_EQ(vec3f( 1.0f, -2.0f, -3.0f), bounds.corner(bbox3f::Corner::max, bbox3f::Corner::min, bbox3f::Corner::min))
        CER_ASSERT_VEC_EQ(vec3f( 1.0f, -2.0f,  5.0f), bounds.corner(bbox3f::Corner::max, bbox3f::Corner::min, bbox3f::Corner::max))
        CER_ASSERT_VEC_EQ(vec3f( 1.0f,  3.0f, -3.0f), bounds.corner(bbox3f::Corner::max, bbox3f::Corner::max, bbox3f::Corner::min))
        CER_ASSERT_VEC_EQ(vec3f( 1.0f,  3.0f,  5.0f), bounds.corner(bbox3f::Corner::max, bbox3f::Corner::max, bbox3f::Corner::max))
    }

    TEST(bbox_test, relative_position) {
        constexpr auto bounds = bbox3f(vec3f(-12.0f, -3.0f,  4.0f), vec3f( 8.0f, 9.0f, 8.0f));
        constexpr auto point1 = vec3f(-1.0f, 0.0f, 0.0f);
        constexpr auto pos1 = bounds.relative_position(point1);
        CER_ASSERT_EQ(bbox3f::Range::within, pos1[0])
        CER_ASSERT_EQ(bbox3f::Range::within, pos1[1])
        CER_ASSERT_EQ(bbox3f::Range::less,   pos1[2])
    }

    TEST(bbox_test, expand) {
        constexpr auto bounds   =  bbox3f(vec3f(-12.0f, -3.0f,  4.0f), vec3f( 8.0f,  9.0f,  8.0f));
        constexpr auto expanded =  bbox3f(vec3f(-14.0f, -5.0f,  2.0f), vec3f(10.0f, 11.0f, 10.0f));
        CER_ASSERT_EQ(expanded, bounds.expand(2.0f))
    }

    TEST(bbox_test, translate) {
        constexpr auto bounds     = bbox3f(vec3f(-12.0f, -3.0f,  4.0f), vec3f( 8.0f, 9.0f, 8.0f));
        constexpr auto translated = bbox3f(vec3f(-10.0f, -4.0f,  1.0f), vec3f(10.0f, 8.0f, 5.0f));
        CER_ASSERT_EQ(translated, bounds.translate(vec3f(2.0f, -1.0f, -3.0f)))
    }

    TEST(bbox_test, transform) {
        constexpr auto bounds = bbox3d(-2.0, +10.0);
        constexpr auto transform = scaling_matrix(vec3d(0.5, 2, 3));
        constexpr auto points = bounds.vertices();
        constexpr auto transformedPoints = transform * points;
        constexpr auto transformed = bbox3d::merge_all(std::begin(transformedPoints), std::end(transformedPoints));
        CER_ASSERT_VEC_EQ(transformed.min, bounds.transform(transform).min)
        CER_ASSERT_VEC_EQ(transformed.max, bounds.transform(transform).max)
    }

    TEST(bbox_test, operator_equal) {
        constexpr auto min =  vec3f(-1, -2, -3);
        constexpr auto max =  vec3f( 1,  2,  3);
        constexpr auto bounds1 =  bbox3f(min, max);
        constexpr auto bounds2 =  bbox3f(min, max);
        constexpr auto bounds3 =  bbox3f(22.0f);

        CER_ASSERT_TRUE(bounds1 == bounds2)
        CER_ASSERT_FALSE(bounds1 == bounds3)
    }

    TEST(bbox_test, operator_not_equal) {
        constexpr auto min = vec3f(-1, -2, -3);
        constexpr auto max = vec3f( 1,  2,  3);
        constexpr auto bounds1 = bbox3f(min, max);
        constexpr auto bounds2 = bbox3f(min, max);
        constexpr auto bounds3 = bbox3f(22.0f);

        CER_ASSERT_FALSE(bounds1 != bounds2)
        CER_ASSERT_TRUE(bounds1 != bounds3)
    }

    TEST(bbox_test, is_equal) {
        constexpr auto bounds1 = bbox3f(vec3f(-12, -3, 4), vec3f(7, 8, 9));
        constexpr auto bounds2 = bbox3f(vec3f(-12, -3, 4), vec3f(7, 8, 10));

        CER_ASSERT_TRUE(is_equal(bounds1, bounds1, 0.0f))
        CER_ASSERT_TRUE(is_equal(bounds2, bounds2, 0.0f))
        CER_ASSERT_FALSE(is_equal(bounds1, bounds2, 0.0f))
        CER_ASSERT_FALSE(is_equal(bounds1, bounds2, 0.999f))
        CER_ASSERT_TRUE(is_equal(bounds1, bounds2, 1.0f))
    }

    TEST(bbox_test, repair) {
        auto actual = bbox3d(0.0);
        actual.min = vec3d(+8, -8, +8);
        actual.max = vec3d(-8, +8, -8);
        ASSERT_EQ(bbox3d(8.0), repair(actual));
    }

    TEST(bbox_test, merge_with_bbox) {
        constexpr auto bounds1 = bbox3f(vec3f(-12, -3, 4), vec3f(7, 8, 9));
        constexpr auto bounds2 = bbox3f(vec3f(-10, -5, 3), vec3f(9, 9, 5));
        constexpr auto merged  = bbox3f(vec3f(-12, -5, 3), vec3f(9, 9, 9));

        CER_ASSERT_EQ(merged, merge(bounds1, bounds2))
    }

    TEST(bbox_test, merge_with_vec) {
        constexpr auto bounds = bbox3f(vec3f(-12, -3, 4), vec3f(7, 8,  9));
        constexpr auto vec    = vec3f(-10, -6, 10);
        constexpr auto merged = bbox3f(vec3f(-12, -6, 4), vec3f(7, 8, 10));

        CER_ASSERT_EQ(merged, merge(bounds, vec))
    }

    TEST(bbox_test, intersect) {
        constexpr auto b1 = bbox3d(vec3d(-10, -10, -10), vec3d(10, 10, 10));
        constexpr auto b2 = bbox3d(vec3d(-5, -5, -5), vec3d(20, 5, 10));
        constexpr auto b3 = bbox3d(vec3d(12, 12, 12), vec3d(15, 15, 15));

        CER_ASSERT_EQ(bbox3d(vec3d(-5, -5, -5), vec3d(10, 5, 10)), intersect(b1, b2))
        CER_ASSERT_EQ(bbox3d(vec3d(-5, -5, -5), vec3d(10, 5, 10)), intersect(b2, b1))
        CER_ASSERT_EQ(bbox3d(vec3d(0, 0, 0), vec3d(0, 0, 0)), intersect(b1, b3))
        CER_ASSERT_EQ(bbox3d(vec3d(0, 0, 0), vec3d(0, 0, 0)), intersect(b3, b1))
        CER_ASSERT_EQ(bbox3d(vec3d(0, 0, 0), vec3d(0, 0, 0)), intersect(b2, b3))
    }

    TEST(bbox_builder_test, empty) {
        constexpr auto builder = vm::bbox3f::builder();
        CER_ASSERT_FALSE(builder.initialized())
    }

    TEST(bbox_builder_test, add_one_point) {
        const auto point = vm::vec3f(10, 20, 30);

        vm::bbox3f::builder builder;
        builder.add(point);

        ASSERT_TRUE(builder.initialized());
        ASSERT_EQ(vm::bbox3f(point, point), builder.bounds());
    }

    TEST(bbox_builder_test, twoPoints) {
        const auto point1 = vm::vec3f(10, 20, 30);
        const auto point2 = vm::vec3f(100, 200, 300);

        vm::bbox3f::builder builder;
        builder.add(point1);
        builder.add(point2);

        ASSERT_TRUE(builder.initialized());
        ASSERT_EQ(vm::bbox3f(point1, point2), builder.bounds());
    }

    TEST(bbox_builder_test, twoPointsReverseOrder) {
        const auto point1 = vm::vec3f(10, 20, 30);
        const auto point2 = vm::vec3f(100, 200, 300);

        vm::bbox3f::builder builder;
        builder.add(point2);
        builder.add(point1);

        ASSERT_TRUE(builder.initialized());
        ASSERT_EQ(vm::bbox3f(point1, point2), builder.bounds());
    }

    TEST(bbox_builder_test, add_one_bbox) {
        const auto bbox = vm::bbox3f(vec3f(2, 3, 4), vec3f(5, 6, 7));

        vm::bbox3f::builder builder;
        builder.add(bbox);

        ASSERT_TRUE(builder.initialized());
        ASSERT_EQ(bbox, builder.bounds());
    }
}
