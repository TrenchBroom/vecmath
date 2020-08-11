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

#include <vecmath/abstract_line.h>
#include <vecmath/constants.h>
#include <vecmath/forward.h>
#include <vecmath/vec.h>
#include <vecmath/mat.h>
#include <vecmath/distance.h>
#include <vecmath/util.h>

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

    template <class A, class B>
    static void assert_line_distance_squared_invariants(const line_distance<float>& linedist, const A& lhs, const B& rhs) {
        const vec3f lhsPoint = point_at_distance(lhs, linedist.position1);
        const vec3f rhsPoint = point_at_distance(rhs, linedist.position2);
        const float dist = squared_distance(lhsPoint, rhsPoint);

        ASSERT_NEAR(dist, linedist.distance, constants<float>::almost_zero());
    }

    template <class A, class B>
    static void assert_line_distance_invariants(const line_distance<float>& linedist, const A& lhs, const B& rhs) {
        const vec3f lhsPoint = point_at_distance(lhs, linedist.position1);
        const vec3f rhsPoint = point_at_distance(rhs, linedist.position2);
        const float dist = distance(lhsPoint, rhsPoint);

        ASSERT_NEAR(dist, linedist.distance, constants<float>::almost_zero());
    }

    TEST(distance_test, distance_ray_segment) {
        constexpr auto ray = ray3f(vec3f::zero(), vec3f::pos_z());
        line_distance<float> segDist;
        segment3f seg;

        // segment overlapping ray
        seg = segment3f(vec3f(0.0f, 0.0f, 0.0f), vec3f(0.0f, 0.0f, 1.0f));
        segDist = squared_distance(ray, seg);
        ASSERT_TRUE(segDist.parallel);
        ASSERT_FLOAT_EQ(0.0f, segDist.position1);
        ASSERT_FLOAT_EQ(0.0f, segDist.distance);
        ASSERT_FLOAT_EQ(0.0f, segDist.position2);
        assert_line_distance_squared_invariants(segDist, ray, seg);

        // segment parallel to ray with XY offset
        seg = segment3f(vec3f(1.0f, 1.0f, 0.0f), vec3f(1.0f, 1.0f, 1.0f));
        segDist = squared_distance(ray, seg);
        ASSERT_TRUE(segDist.parallel);
        ASSERT_FLOAT_EQ(0.0f, segDist.position1);
        ASSERT_FLOAT_EQ(2.0f, segDist.distance);
        ASSERT_FLOAT_EQ(0.0f, segDist.position2);
        assert_line_distance_squared_invariants(segDist, ray, seg);

        // segment parallel, in front of ray
        seg = segment3f(vec3f(1.0f, 1.0f, 5.0f), vec3f(1.0f, 1.0f, 6.0f));
        segDist = squared_distance(ray, seg);
        ASSERT_TRUE(segDist.parallel);
        ASSERT_FLOAT_EQ(5.0f, segDist.position1);
        ASSERT_FLOAT_EQ(2.0f, segDist.distance);
        ASSERT_FLOAT_EQ(0.0f, segDist.position2);
        assert_line_distance_squared_invariants(segDist, ray, seg);

        // flip previous test case
        seg = seg.transform(mat4x4f::mirror_z());
        segDist = squared_distance(ray.transform(mat4x4f::mirror_z()), seg);
        assert_line_distance_squared_invariants(segDist, ray.transform(mat4x4f::mirror_z()), seg);

        // segment parallel, behind ray
        seg = segment3f(vec3f(1.0f, 1.0f, -2.0f), vec3f(1.0f, 1.0f, -1.0f));
        segDist = squared_distance(ray, seg);
        ASSERT_TRUE(segDist.parallel);
        ASSERT_FLOAT_EQ(0.0f, segDist.position1);
        ASSERT_FLOAT_EQ(3.0f, segDist.distance);
        ASSERT_FLOAT_EQ(1.0f, segDist.position2);
        assert_line_distance_squared_invariants(segDist, ray, seg);

        // flip previous test case
        seg = seg.transform(mat4x4f::mirror_z());
        segDist = squared_distance(ray.transform(mat4x4f::mirror_z()), seg);
        assert_line_distance_squared_invariants(segDist, ray.transform(mat4x4f::mirror_z()), seg);

        // segment parallel, in front of ray, degenerate segment
        seg = segment3f(vec3f(1.0f, 1.0f, 5.0f), vec3f(1.0f, 1.0f, 5.0f));
        segDist = squared_distance(ray, seg);
        ASSERT_TRUE(segDist.parallel);
        ASSERT_FLOAT_EQ(5.0f, segDist.position1);
        ASSERT_FLOAT_EQ(2.0f, segDist.distance);
        ASSERT_FLOAT_EQ(0.0f, segDist.position2);
        //assert_line_distance_squared_invariants(segDist, ray, seg); // can't do this check on a degenerate segment

        // segment parallel, behind ray, degenerate segment
        seg = segment3f(vec3f(1.0f, 1.0f, -1.0f), vec3f(1.0f, 1.0f, -1.0f));
        segDist = squared_distance(ray, seg);
        ASSERT_TRUE(segDist.parallel);
        ASSERT_FLOAT_EQ(0.0f, segDist.position1);
        ASSERT_FLOAT_EQ(3.0f, segDist.distance);
        ASSERT_FLOAT_EQ(0.0f, segDist.position2);
        //assert_line_distance_squared_invariants(segDist, ray, seg); // can't do this check on a degenerate segment

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
        seg = segment3f(vec3f(1.0f, 0.0f, 0.0f), vec3f(0.0f, 1.0f, 0.0f));
        segDist = squared_distance(ray, seg);
        ASSERT_FALSE(segDist.parallel);
        ASSERT_FLOAT_EQ(0.0f, segDist.position1);        // the ray origin is the closest point on R
        ASSERT_FLOAT_EQ(0.5f, segDist.distance);         // R to c distance, squared
        ASSERT_FLOAT_EQ(0.70710677f, segDist.position2); // s to c distance
        assert_line_distance_squared_invariants(segDist, ray, seg);

        // same as previous, but segment is below ray start
        seg = segment3f(vec3f(1.0f, 0.0f, -1.0f), vec3f(0.0f, 1.0f, -1.0f));
        segDist = squared_distance(ray, seg);
        ASSERT_FALSE(segDist.parallel);
        ASSERT_FLOAT_EQ(0.0f, segDist.position1);        // the ray origin is the closest point on R
        ASSERT_FLOAT_EQ(1.5f, segDist.distance);         // R to c distance, squared
        ASSERT_FLOAT_EQ(0.70710677f, segDist.position2); // s to c distance
        assert_line_distance_squared_invariants(segDist, ray, seg);

        seg = segment3f(vec3f(1.0f, 0.0f, 0.0f), vec3f(2.0f, -1.0f, 0.0f));
        segDist = squared_distance(ray, seg);
        ASSERT_FALSE(segDist.parallel);
        ASSERT_FLOAT_EQ(0.0f, segDist.position1);
        ASSERT_FLOAT_EQ(1.0f, segDist.distance);
        ASSERT_FLOAT_EQ(0.0f, segDist.position2);
        assert_line_distance_squared_invariants(segDist, ray, seg);

        seg = segment3f(vec3f(-1.0f, 1.5f, 2.0f), vec3f(+1.0f, 1.5f, 2.0f));
        segDist = distance(ray, seg);
        ASSERT_FALSE(segDist.parallel);
        ASSERT_FLOAT_EQ(2.0f, segDist.position1);
        ASSERT_FLOAT_EQ(1.5f, segDist.distance);
        ASSERT_FLOAT_EQ(1.0f, segDist.position2);
        assert_line_distance_invariants(segDist, ray, seg);
    }

    TEST(distance_test, distance_ray_ray) {
        constexpr auto ray1 = ray3f(vec3f::zero(), vec3f::pos_z());

        // parallel, ray with itself
        constexpr auto segDist1 = squared_distance(ray1, ray1);
        CER_ASSERT_TRUE(segDist1.parallel)
        CER_ASSERT_NEAR(0.0f, segDist1.position1, 0.001f)
        CER_ASSERT_NEAR(0.0f, segDist1.distance, 0.001f)
        CER_ASSERT_NEAR(0.0f, segDist1.position2, 0.001f)
        assert_line_distance_squared_invariants(segDist1, ray1, ray1);

        // parallel, XY offset
        constexpr auto segDist2Ray = ray3f(vec3f(1.0f, 1.0, 0.0f), vec3f::pos_z());
        constexpr auto segDist2 = squared_distance(ray1, segDist2Ray);
        CER_ASSERT_TRUE(segDist2.parallel)
        CER_ASSERT_NEAR(0.0f, segDist2.position1, 0.001f)
        CER_ASSERT_NEAR(2.0f, segDist2.distance, 0.001f)
        CER_ASSERT_NEAR(0.0f, segDist2.position2, 0.001f)
        assert_line_distance_squared_invariants(segDist2, ray1, segDist2Ray);

        constexpr auto segDist3Ray = ray3f(vec3f(1.0f, 1.0f, 0.0f), normalize_c(vec3f(1.0f, 1.0f, 1.0f)));
        constexpr auto segDist3 = squared_distance(ray1, segDist3Ray);
        CER_ASSERT_FALSE(segDist3.parallel)
        CER_ASSERT_NEAR(0.0f, segDist3.position1, 0.001f)
        CER_ASSERT_NEAR(2.0f, segDist3.distance, 0.001f)
        CER_ASSERT_NEAR(0.0f, segDist3.position2, 0.001f)
        assert_line_distance_squared_invariants(segDist3, ray1, segDist3Ray);

        constexpr auto segDist4Ray = ray3f(vec3f(1.0f, 1.0f, 0.0f), normalize_c(vec3f(-1.0f, -1.0f, +1.0f)));
        constexpr auto segDist4 = squared_distance(ray1, segDist4Ray);
        CER_ASSERT_FALSE(segDist4.parallel)
        CER_ASSERT_NEAR(1.0f, segDist4.position1, 0.001f)
        CER_ASSERT_NEAR(0.0f, segDist4.distance, 0.001f)
        CER_ASSERT_NEAR(length(vec3f(1.0f, 1.0f, 1.0f)), segDist4.position2, 0.001f)
        assert_line_distance_squared_invariants(segDist4, ray1, segDist4Ray);

        constexpr auto segDist5Ray = ray3f(vec3f(1.0f, 1.0f, 0.0f), normalize_c(vec3f(-1.0f, 0.0f, +1.0f)));
        constexpr auto segDist5 = squared_distance(ray1, segDist5Ray);
        CER_ASSERT_FALSE(segDist5.parallel)
        CER_ASSERT_NEAR(1.0f, segDist5.position1, 0.001f)
        CER_ASSERT_NEAR(1.0f, segDist5.distance, 0.001f)
        CER_ASSERT_NEAR(length(vec3f(1.0f, 0.0f, 1.0f)), segDist5.position2, 0.001f)
        assert_line_distance_squared_invariants(segDist5, ray1, segDist5Ray);

        // parallel, second ray is in front
        constexpr auto segDist6Ray = ray3f(vec3f(1.0f, 1.0, 1.0f), vec3f::pos_z());
        constexpr auto segDist6 = squared_distance(ray1, segDist6Ray);
        CER_ASSERT_TRUE(segDist6.parallel)
        CER_ASSERT_NEAR(1.0f, segDist6.position1, 0.001f)
        CER_ASSERT_NEAR(2.0f, segDist6.distance, 0.001f)
        CER_ASSERT_NEAR(0.0f, segDist6.position2, 0.001f)
        assert_line_distance_squared_invariants(segDist6, ray1, segDist6Ray);

        // parallel, second ray is behind
        constexpr auto segDist7Ray = ray3f(vec3f(1.0f, 1.0, -1.0f), vec3f::pos_z());
        constexpr auto segDist7 = squared_distance(ray1, segDist7Ray);
        CER_ASSERT_TRUE(segDist7.parallel)
        CER_ASSERT_NEAR(0.0f, segDist7.position1, 0.001f)
        CER_ASSERT_NEAR(2.0f, segDist7.distance, 0.001f)
        CER_ASSERT_NEAR(1.0f, segDist7.position2, 0.001f)
        assert_line_distance_squared_invariants(segDist7, ray1, segDist7Ray);
    }

    TEST(distance_test, distance_ray_line) {
        constexpr auto ray = ray3f(vec3f::zero(), vec3f::pos_z());

        constexpr auto segDist1Line = line3f(vec3f(0.0f, 0.0f, 0.0f), vec3f::pos_z());
        constexpr auto segDist1 = squared_distance(ray, segDist1Line);
        CER_ASSERT_TRUE(segDist1.parallel)
        CER_ASSERT_FLOAT_EQ(0.0f, segDist1.position1)
        CER_ASSERT_FLOAT_EQ(0.0f, segDist1.distance)
        CER_ASSERT_FLOAT_EQ(0.0f, segDist1.position2)
        assert_line_distance_squared_invariants(segDist1, ray, segDist1Line);

        constexpr auto segDist2Line = line3f(vec3f(1.0f, 1.0f, 0.0f), vec3f::pos_z());
        constexpr auto segDist2 = squared_distance(ray, segDist2Line);
        CER_ASSERT_TRUE(segDist2.parallel)
        CER_ASSERT_FLOAT_EQ(0.0f, segDist2.position1)
        CER_ASSERT_FLOAT_EQ(2.0f, segDist2.distance)
        CER_ASSERT_FLOAT_EQ(0.0f, segDist2.position2)
        assert_line_distance_squared_invariants(segDist2, ray, segDist2Line);

        constexpr auto segDist3Line = line3f(vec3f(1.0f, 0.0f, 0.0f), normalize_c(vec3f(-1.0f, 1.0f, 0.0f)));
        constexpr auto segDist3 = squared_distance(ray, segDist3Line);
        CER_ASSERT_FALSE(segDist3.parallel)
        CER_ASSERT_FLOAT_EQ(0.0f, segDist3.position1)
        CER_ASSERT_FLOAT_EQ(0.5f, segDist3.distance)
        CER_ASSERT_FLOAT_EQ(sqrt_c(2.0f) / 2.0f, segDist3.position2)
        assert_line_distance_squared_invariants(segDist3, ray, segDist3Line);

        constexpr auto segDist4Line = line3f(vec3f(1.0f, 0.0f, 0.0f), normalize_c(vec3f(1.0f, -1.0f, 0.0f)));
        constexpr auto segDist4 = squared_distance(ray, segDist4Line);
        CER_ASSERT_FALSE(segDist4.parallel)
        CER_ASSERT_FLOAT_EQ(0.0f, segDist4.position1)
        CER_ASSERT_FLOAT_EQ(0.5f, segDist4.distance)
        CER_ASSERT_FLOAT_EQ(-sqrt_c(2.0f) / 2.0f, segDist4.position2)
        assert_line_distance_squared_invariants(segDist4, ray, segDist4Line);

        // parallel, ray is in front of line
        constexpr auto segDist5Line = line3f(vec3f(1.0f, 1.0f, -1.0f), vec3f::pos_z());
        constexpr auto segDist5 = squared_distance(ray, segDist5Line);
        CER_ASSERT_TRUE(segDist5.parallel)
        CER_ASSERT_FLOAT_EQ(0.0f, segDist5.position1)
        CER_ASSERT_FLOAT_EQ(2.0f, segDist5.distance)
        CER_ASSERT_FLOAT_EQ(1.0f, segDist5.position2) // we use the ray origin as the closest point, so this is the corresponding position on the line
        assert_line_distance_squared_invariants(segDist5, ray, segDist5Line);

        // parallel, ray is behind line
        constexpr auto segDist6Line = line3f(vec3f(1.0f, 1.0f, 1.0f), vec3f::pos_z());
        constexpr auto segDist6 = squared_distance(ray, segDist6Line);
        CER_ASSERT_TRUE(segDist6.parallel)
        CER_ASSERT_FLOAT_EQ(0.0f, segDist6.position1)
        CER_ASSERT_FLOAT_EQ(2.0f, segDist6.distance)
        CER_ASSERT_FLOAT_EQ(-1.0f, segDist6.position2) // we use the ray origin as the closest point, so this is the corresponding position on the line
        assert_line_distance_squared_invariants(segDist6, ray, segDist6Line);
    }
}
