/*
 Copyright 2010-2019 Kristian Duske
 Copyright 2015-2019 Eric Wasylishen

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
 rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit
 persons to whom the Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
 Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
