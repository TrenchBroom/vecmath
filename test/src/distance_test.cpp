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

#include "test_utils.h"

#include <vecmath/forward.h>
#include <vecmath/vec.h>
#include <vecmath/distance.h>

namespace vm {
    TEST(distance_test, distance_ray_point) {
        constexpr auto ray = ray3f(vec3f::zero(), vec3f::pos_z());

        // point is behind ray
        CER_ASSERT_FLOAT_EQ(0.0f, squared_distance(ray, vec3f(-1.0f, -1.0f, -1.0f)).position)
        CER_ASSERT_FLOAT_EQ(3.0f, squared_distance(ray, vec3f(-1.0f, -1.0f, -1.0f)).distance)

        // point is in front of ray
        CER_ASSERT_FLOAT_EQ(1.0f, squared_distance(ray, vec3f(1.0f, 1.0f, 1.0f)).position)
        CER_ASSERT_FLOAT_EQ(2.0f, squared_distance(ray, vec3f(1.0f, 1.0f, 1.0f)).distance)

        // point is on ray
        CER_ASSERT_FLOAT_EQ(1.0f, squared_distance(ray, vec3f(0.0f, 0.0f, 1.0f)).position)
        CER_ASSERT_FLOAT_EQ(0.0f, squared_distance(ray, vec3f(0.0f, 0.0f, 1.0f)).distance)
    }

    TEST(distance_test, distance_segment_point) {
        constexpr auto segment = segment3f(vec3f::zero(), vec3f::pos_z());

        // point is below start
        ASSERT_FLOAT_EQ(0.0f, squared_distance(segment, vec3f(-1.0f, -1.0f, -1.0f)).position);
        ASSERT_FLOAT_EQ(3.0f, squared_distance(segment, vec3f(-1.0f, -1.0f, -1.0f)).distance);

        // point is within segment
        ASSERT_FLOAT_EQ(1.0f, squared_distance(segment, vec3f(1.0f, 1.0f, 1.0f)).position);
        ASSERT_FLOAT_EQ(2.0f, squared_distance(segment, vec3f(1.0f, 1.0f, 1.0f)).distance);

        // point is above end
        ASSERT_FLOAT_EQ(1.0f, squared_distance(segment, vec3f(0.0f, 0.0f, 2.0f)).position);
        ASSERT_FLOAT_EQ(1.0f, squared_distance(segment, vec3f(0.0f, 0.0f, 2.0f)).distance);
    }

    TEST(distance_test, distance_ray_segment) {
        constexpr auto ray = ray3f(vec3f::zero(), vec3f::pos_z());
        line_distance<float> segDist;

        segDist = squared_distance(ray, segment3f(vec3f(0.0f, 0.0f, 0.0f), vec3f(0.0f, 0.0f, 1.0f)));
        ASSERT_TRUE(segDist.parallel);
        ASSERT_FLOAT_EQ(0.0f, segDist.distance);

        segDist = squared_distance(ray, segment3f(vec3f(1.0f, 1.0f, 0.0f), vec3f(1.0f, 1.0f, 1.0f)));
        ASSERT_TRUE(segDist.parallel);
        ASSERT_FLOAT_EQ(2.0f, segDist.distance);

        segDist = squared_distance(ray, segment3f(vec3f(1.0f, 0.0f, 0.0f), vec3f(0.0f, 1.0f, 0.0f)));
        ASSERT_FALSE(segDist.parallel);
        ASSERT_FLOAT_EQ(0.0f, segDist.position1);
        ASSERT_FLOAT_EQ(0.5f, segDist.distance);
        ASSERT_FLOAT_EQ(0.70710677f, segDist.position2);

        segDist = squared_distance(ray, segment3f(vec3f(1.0f, 0.0f, 0.0f), vec3f(2.0f, -1.0f, 0.0f)));
        ASSERT_FALSE(segDist.parallel);
        ASSERT_FLOAT_EQ(0.0f, segDist.position1);
        ASSERT_FLOAT_EQ(1.0f, segDist.distance);
        ASSERT_FLOAT_EQ(0.0f, segDist.position2);

        segDist = distance(ray, segment3f(vec3f(-1.0f, 1.5f, 2.0f), vec3f(+1.0f, 1.5f, 2.0f)));
        ASSERT_FALSE(segDist.parallel);
        ASSERT_FLOAT_EQ(2.0f, segDist.position1);
        ASSERT_FLOAT_EQ(1.5f, segDist.distance);
        ASSERT_FLOAT_EQ(1.0f, segDist.position2);
    }

    TEST(distance_test, distance_ray_ray) {
        constexpr auto ray1 = ray3f(vec3f::zero(), vec3f::pos_z());

        constexpr auto segDist1 = squared_distance(ray1, ray1);
        CER_ASSERT_TRUE(segDist1.parallel)
        CER_ASSERT_NEAR(0.0f, segDist1.distance, 0.001f)

        constexpr auto segDist2 = squared_distance(ray1, ray3f(vec3f(1.0f, 1.0, 0.0f), vec3f::pos_z()));
        CER_ASSERT_TRUE(segDist2.parallel)
        CER_ASSERT_NEAR(2.0f, segDist2.distance, 0.001f)

        constexpr auto segDist3 = squared_distance(ray1, ray3f(vec3f(1.0f, 1.0f, 0.0f), normalize_c(vec3f(1.0f, 1.0f, 1.0f))));
        CER_ASSERT_FALSE(segDist3.parallel)
        CER_ASSERT_NEAR(0.0f, segDist3.position1, 0.001f)
        CER_ASSERT_NEAR(2.0f, segDist3.distance, 0.001f)
        CER_ASSERT_NEAR(0.0f, segDist3.position2, 0.001f)

        constexpr auto segDist4 = squared_distance(ray1, ray3f(vec3f(1.0f, 1.0f, 0.0f), normalize_c(vec3f(-1.0f, -1.0f, +1.0f))));
        CER_ASSERT_FALSE(segDist4.parallel)
        CER_ASSERT_NEAR(1.0f, segDist4.position1, 0.001f)
        CER_ASSERT_NEAR(0.0f, segDist4.distance, 0.001f)
        CER_ASSERT_NEAR(length(vec3f(1.0f, 1.0f, 1.0f)), segDist4.position2, 0.001f)

        constexpr auto segDist5 = squared_distance(ray1, ray3f(vec3f(1.0f, 1.0f, 0.0f), normalize_c(vec3f(-1.0f, 0.0f, +1.0f))));
        CER_ASSERT_FALSE(segDist5.parallel)
        CER_ASSERT_NEAR(1.0f, segDist5.position1, 0.001f)
        CER_ASSERT_NEAR(1.0f, segDist5.distance, 0.001f)
        CER_ASSERT_NEAR(length(vec3f(1.0f, 0.0f, 1.0f)), segDist5.position2, 0.001f)
    }

    TEST(distance_test, distance_ray_line) {
        constexpr auto ray = ray3f(vec3f::zero(), vec3f::pos_z());

        constexpr auto segDist1 = squared_distance(ray, line3f(vec3f(0.0f, 0.0f, 0.0f), vec3f::pos_z()));
        CER_ASSERT_TRUE(segDist1.parallel)
        CER_ASSERT_FLOAT_EQ(0.0f, segDist1.distance)

        constexpr auto segDist2 = squared_distance(ray, line3f(vec3f(1.0f, 1.0f, 0.0f), vec3f::pos_z()));
        CER_ASSERT_TRUE(segDist2.parallel)
        CER_ASSERT_FLOAT_EQ(2.0f, segDist2.distance)

        constexpr auto segDist3 = squared_distance(ray, line3f(vec3f(1.0f, 0.0f, 0.0f), normalize_c(vec3f(-1.0f, 1.0f, 0.0f))));
        CER_ASSERT_FALSE(segDist3.parallel)
        CER_ASSERT_FLOAT_EQ(0.0f, segDist3.position1)
        CER_ASSERT_FLOAT_EQ(0.5f, segDist3.distance)
        CER_ASSERT_FLOAT_EQ(sqrt_c(2.0f) / 2.0f, segDist3.position2)

        constexpr auto segDist4 = squared_distance(ray, line3f(vec3f(1.0f, 0.0f, 0.0f), normalize_c(vec3f(1.0f, -1.0f, 0.0f))));
        CER_ASSERT_FALSE(segDist4.parallel)
        CER_ASSERT_FLOAT_EQ(0.0f, segDist4.position1)
        CER_ASSERT_FLOAT_EQ(0.5f, segDist4.distance)
        CER_ASSERT_FLOAT_EQ(-sqrt_c(2.0f) / 2.0f, segDist4.position2)
    }
}
