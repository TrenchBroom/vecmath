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

        // point is in front of ray
        CER_ASSERT_FLOAT_EQ(2.0f, squared_distance(ray, vec3f(1.0f, 1.0f, 2.0f)).position) // NOTE: position is not squared
        CER_ASSERT_FLOAT_EQ(2.0f, squared_distance(ray, vec3f(1.0f, 1.0f, 2.0f)).distance)

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

        // segment overlapping ray
        segDist = squared_distance(ray, segment3f(vec3f(0.0f, 0.0f, 0.0f), vec3f(0.0f, 0.0f, 1.0f)));
        ASSERT_TRUE(segDist.parallel);
        ASSERT_FLOAT_EQ(0.0f, segDist.position1);
        ASSERT_FLOAT_EQ(0.0f, segDist.distance);
        ASSERT_FLOAT_EQ(0.0f, segDist.position2);

        // segment parallel to ray with XY offset
        segDist = squared_distance(ray, segment3f(vec3f(1.0f, 1.0f, 0.0f), vec3f(1.0f, 1.0f, 1.0f)));
        ASSERT_TRUE(segDist.parallel);
        ASSERT_FLOAT_EQ(0.0f, segDist.position1);
        ASSERT_FLOAT_EQ(2.0f, segDist.distance);
        ASSERT_FLOAT_EQ(0.0f, segDist.position2);

        // segment parallel, in front of ray
        segDist = squared_distance(ray, segment3f(vec3f(1.0f, 1.0f, 5.0f), vec3f(1.0f, 1.0f, 6.0f)));
        ASSERT_TRUE(segDist.parallel);
        ASSERT_FLOAT_EQ(5.0f, segDist.position1);
        ASSERT_FLOAT_EQ(2.0f, segDist.distance);
        ASSERT_FLOAT_EQ(0.0f, segDist.position2);

        // segment parallel, behind ray
        segDist = squared_distance(ray, segment3f(vec3f(1.0f, 1.0f, -2.0f), vec3f(1.0f, 1.0f, -1.0f)));
        ASSERT_TRUE(segDist.parallel);
        ASSERT_FLOAT_EQ(0.0f, segDist.position1);
        ASSERT_FLOAT_EQ(3.0f, segDist.distance);
        ASSERT_FLOAT_EQ(1.0f, segDist.position2);

        // segment parallel, in front of ray, degenerate segment
        segDist = squared_distance(ray, segment3f(vec3f(1.0f, 1.0f, 5.0f), vec3f(1.0f, 1.0f, 5.0f)));
        ASSERT_TRUE(segDist.parallel);
        ASSERT_FLOAT_EQ(5.0f, segDist.position1);
        ASSERT_FLOAT_EQ(2.0f, segDist.distance);
        ASSERT_FLOAT_EQ(0.0f, segDist.position2);

        // segment parallel, behind ray, degenerate segment
        segDist = squared_distance(ray, segment3f(vec3f(1.0f, 1.0f, -1.0f), vec3f(1.0f, 1.0f, -1.0f)));
        ASSERT_TRUE(segDist.parallel);
        ASSERT_FLOAT_EQ(0.0f, segDist.position1);
        ASSERT_FLOAT_EQ(3.0f, segDist.distance);
        ASSERT_FLOAT_EQ(0.0f, segDist.position2);

        // segment perpendicular to ray
        //   R = ray
        //   c = closest point
        //   s = segment start s
        //   e = segment end
        //
        // ^  s
        // |    c
        // Y  R   e
        //
        //    X ->
        //
        segDist = squared_distance(ray, segment3f(vec3f(1.0f, 0.0f, 0.0f), vec3f(0.0f, 1.0f, 0.0f)));
        ASSERT_FALSE(segDist.parallel);
        ASSERT_FLOAT_EQ(0.0f, segDist.position1);        // the ray origin is the closest point on R
        ASSERT_FLOAT_EQ(0.5f, segDist.distance);         // R to c distance, squared
        ASSERT_FLOAT_EQ(0.70710677f, segDist.position2); // s to c distance

        // same as previous, but segment is below ray start
        segDist = squared_distance(ray, segment3f(vec3f(1.0f, 0.0f, -1.0f), vec3f(0.0f, 1.0f, -1.0f)));
        ASSERT_FALSE(segDist.parallel);
        ASSERT_FLOAT_EQ(0.0f, segDist.position1);        // the ray origin is the closest point on R
        ASSERT_FLOAT_EQ(1.5f, segDist.distance);         // R to c distance, squared
        ASSERT_FLOAT_EQ(0.70710677f, segDist.position2); // s to c distance

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

        // parallel, ray with itself
        constexpr auto segDist1 = squared_distance(ray1, ray1);
        CER_ASSERT_TRUE(segDist1.parallel)
        CER_ASSERT_NEAR(0.0f, segDist1.position1, 0.001f)
        CER_ASSERT_NEAR(0.0f, segDist1.distance, 0.001f)
        CER_ASSERT_NEAR(0.0f, segDist1.position2, 0.001f)

        // parallel, XY offset
        constexpr auto segDist2 = squared_distance(ray1, ray3f(vec3f(1.0f, 1.0, 0.0f), vec3f::pos_z()));
        CER_ASSERT_TRUE(segDist2.parallel)
        CER_ASSERT_NEAR(0.0f, segDist2.position1, 0.001f)
        CER_ASSERT_NEAR(2.0f, segDist2.distance, 0.001f)
        CER_ASSERT_NEAR(0.0f, segDist2.position2, 0.001f)

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

        // parallel, second ray is in front
        constexpr auto segDist6 = squared_distance(ray1, ray3f(vec3f(1.0f, 1.0, 1.0f), vec3f::pos_z()));
        CER_ASSERT_TRUE(segDist6.parallel)
        CER_ASSERT_NEAR(1.0f, segDist6.position1, 0.001f)
        CER_ASSERT_NEAR(2.0f, segDist6.distance, 0.001f)
        CER_ASSERT_NEAR(0.0f, segDist6.position2, 0.001f)

        // parallel, second ray is behind
        constexpr auto segDist7 = squared_distance(ray1, ray3f(vec3f(1.0f, 1.0, -1.0f), vec3f::pos_z()));
        CER_ASSERT_TRUE(segDist7.parallel)
        CER_ASSERT_NEAR(0.0f, segDist7.position1, 0.001f)
        CER_ASSERT_NEAR(2.0f, segDist7.distance, 0.001f)
        CER_ASSERT_NEAR(1.0f, segDist7.position2, 0.001f)
    }

    TEST(distance_test, distance_ray_line) {
        constexpr auto ray = ray3f(vec3f::zero(), vec3f::pos_z());

        constexpr auto segDist1 = squared_distance(ray, line3f(vec3f(0.0f, 0.0f, 0.0f), vec3f::pos_z()));
        CER_ASSERT_TRUE(segDist1.parallel)
        CER_ASSERT_FLOAT_EQ(0.0f, segDist1.position1)
        CER_ASSERT_FLOAT_EQ(0.0f, segDist1.distance)
        CER_ASSERT_FLOAT_EQ(0.0f, segDist1.position2)

        constexpr auto segDist2 = squared_distance(ray, line3f(vec3f(1.0f, 1.0f, 0.0f), vec3f::pos_z()));
        CER_ASSERT_TRUE(segDist2.parallel)
        CER_ASSERT_FLOAT_EQ(0.0f, segDist2.position1)
        CER_ASSERT_FLOAT_EQ(2.0f, segDist2.distance)
        CER_ASSERT_FLOAT_EQ(0.0f, segDist2.position2)

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

        // parallel, ray is in front of line
        constexpr auto segDist5 = squared_distance(ray, line3f(vec3f(1.0f, 1.0f, -1.0f), vec3f::pos_z()));
        CER_ASSERT_TRUE(segDist5.parallel)
        CER_ASSERT_FLOAT_EQ(0.0f, segDist5.position1)
        CER_ASSERT_FLOAT_EQ(2.0f, segDist5.distance)
        CER_ASSERT_FLOAT_EQ(1.0f, segDist5.position2) // we use the ray origin as the closest point, so this is the corresponding position on the line

        // parallel, ray is behind line
        constexpr auto segDist6 = squared_distance(ray, line3f(vec3f(1.0f, 1.0f, 1.0f), vec3f::pos_z()));
        CER_ASSERT_TRUE(segDist6.parallel)
        CER_ASSERT_FLOAT_EQ(0.0f, segDist6.position1)
        CER_ASSERT_FLOAT_EQ(2.0f, segDist6.distance)
        CER_ASSERT_FLOAT_EQ(-1.0f, segDist6.position2) // we use the ray origin as the closest point, so this is the corresponding position on the line
    }
}
