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

#include <vecmath/forward.h>
#include <vecmath/vec.h>
#include <vecmath/vec_ext.h>
#include <vecmath/mat.h>
#include <vecmath/mat_ext.h>
#include <vecmath/polygon.h>

#include "test_utils.h"

#include <iterator>
#include <vector>

namespace vm {
    TEST(polygon_test, constructor_default) {
        ASSERT_EQ(0u, polygon3d().vertices().size());
    }

    TEST(polygon_test, constructor_with_initializer_list) {
        const auto expected = std::vector<vec3d> {
            vec3d(-1, -1, 0),
            vec3d(-1, +1, 0),
            vec3d(+1, +1, 0),
            vec3d(+1, -1, 0)
        };
        ASSERT_EQ(expected,
            polygon3d({
                vec3d(+1, +1, 0),
                vec3d(+1, -1, 0),
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0)
            }).vertices());
    }

    TEST(polygon_test, construct_with_vertex_list) {
        const auto vertices = std::vector<vec3d> {
            vec3d(+1, +1, 0),
            vec3d(+1, -1, 0),
            vec3d(-1, -1, 0),
            vec3d(-1, +1, 0)
        };
        const auto expected = std::vector<vec3d> {
            vec3d(-1, -1, 0),
            vec3d(-1, +1, 0),
            vec3d(+1, +1, 0),
            vec3d(+1, -1, 0)
        };
        ASSERT_EQ(expected, polygon3d(vertices).vertices());
    }

    TEST(polygon_test, has_vertex) {
        const auto vertices = std::vector<vec3d> {
            vec3d(+1, +1, 0),
            vec3d(+1, -1, 0),
            vec3d(-1, -1, 0),
            vec3d(-1, +1, 0)
        };
        const auto p = polygon3d(vertices);

        for (const auto& v : vertices) {
            ASSERT_TRUE(p.hasVertex(v));
        }

        ASSERT_FALSE(p.hasVertex(vec3d::one()));
    }

    TEST(polygon_test, vertex_count) {
        const auto vertices = std::vector<vec3d> {
            vec3d(+1, +1, 0),
            vec3d(+1, -1, 0),
            vec3d(-1, -1, 0),
            vec3d(-1, +1, 0)
        };
        const auto p = polygon3d(vertices);
        ASSERT_EQ(4u, p.vertexCount());
        ASSERT_EQ(0u, polygon3d().vertexCount());
    }

    TEST(polygon_test, vertices) {
        const auto vertices = std::vector<vec3d> {
            vec3d(-1, -1, 0),
            vec3d(-1, +1, 0),
            vec3d(+1, +1, 0),
            vec3d(+1, -1, 0)
        };
        const auto p = polygon3d(vertices);
        ASSERT_EQ(vertices, p.vertices());
    }

    TEST(polygon_test, center) {
        const auto vertices = std::vector<vec3d> {
            vec3d(-1, -1, 0),
            vec3d(-1, +1, 0),
            vec3d(+1, +1, 0),
            vec3d(+1, -1, 0)
        };
        const auto p = polygon3d(vertices);
        ASSERT_VEC_EQ(vec3d::zero(), p.center());
    }

    TEST(polygon_test, invert) {
        const auto p = polygon3d({
            vec3d(-1, -1, 0),
            vec3d(-1, +1, 0),
            vec3d(+1, +1, 0),
            vec3d(+1, -1, 0),
        });

        const auto exp = std::vector<vec3d> {
            vec3d(-1, -1, 0),
            vec3d(+1, -1, 0),
            vec3d(+1, +1, 0),
            vec3d(-1, +1, 0),
        };

        ASSERT_EQ(exp, p.invert().vertices());
    }

    TEST(polygon_test, translate) {
        const auto p = polygon3d({
            vec3d(+1, +1, 0),
            vec3d(+1, -1, 0),
            vec3d(-1, -1, 0),
            vec3d(-1, +1, 0)
        });
        const auto t = vec3d(1, 2, 3);
        ASSERT_EQ(p.vertices() + t, p.translate(t).vertices());
    }

    TEST(polygon_test, transform) {
        const auto p = polygon3d({
            vec3d(+1, +1, 0),
            vec3d(+1, -1, 0),
            vec3d(-1, -1, 0),
            vec3d(-1, +1, 0)
        });
        const auto t = rotation_matrix(to_radians(14.0), to_radians(13.0), to_radians(44.0)) * translation_matrix(vec3d(1,2,3));
        const auto exp = polygon3d(t * p.vertices());
        ASSERT_EQ(exp.vertices(), p.transform(t).vertices());
    }

    TEST(polygon_test, get_vertices) {
        const auto p1 = polygon3d({
            vec3d(+1, +1, 0),
            vec3d(+1, -1, 0),
            vec3d(-1, -1, 0),
            vec3d(-1, +1, 0)
        });
        const auto p2 = p1.translate(vec3d(1,2,3));
        const auto ps = std::vector<polygon3d>{ p1, p2 };

        auto exp = p1.vertices();
        exp.insert(std::end(exp), std::begin(p2), std::end(p2));

        auto act = std::vector<vec3d>();
        polygon3d::get_vertices(std::begin(ps), std::end(ps), std::back_inserter(act));

        ASSERT_EQ(exp, act);
    }

    TEST(polygon_test, compare) {
        ASSERT_TRUE(
            compare(
                polygon3d(),
                polygon3d()
            ) == 0
        );

        ASSERT_TRUE(
            compare(
                polygon3d {
                    vec3d(-1, -1, 0),
                    vec3d(-1, +1, 0),
                    vec3d(+1, +1, 0),
                    vec3d(+1, -1, 0)
                },
                polygon3d {
                    vec3d(-1, -1, 0),
                    vec3d(-1, +1, 0),
                    vec3d(+1, +1, 0),
                    vec3d(+1, -1, 0)
                }
            ) == 0
        );

        ASSERT_TRUE(
            compare(
                polygon3d {
                    vec3d(-1, -1, 0),
                    vec3d(-1, +1, 0),
                    vec3d(+1, +1, 0),
                    vec3d(+1, -1, 0)
                },
                polygon3d {
                    vec3d(-2, -1, 0),
                    vec3d(-1, +1, 0),
                    vec3d(+1, +1, 0),
                    vec3d(+1, -1, 0)
                },
                2.0
            ) == 0
        );

        ASSERT_TRUE(
            compare(
                polygon3d {
                    vec3d(-1, -1, 0),
                    vec3d(-1, +1, 0),
                    vec3d(+1, +1, 0)
                },
                polygon3d {
                    vec3d(-1, -1, 0),
                    vec3d(-1, +1, 0),
                    vec3d(+1, +1, 0),
                    vec3d(+1, -1, 0)
                }
            ) < 0
        );

        ASSERT_TRUE(
            compare(
                polygon3d {
                    vec3d(-1, -1, 0),
                    vec3d(-1, +1, 0),
                    vec3d(+1, +1, 0),
                    vec3d(+1, -1, 0)
                },
                polygon3d {
                    vec3d(-1, -1, 0),
                    vec3d(-1, +1, 0),
                    vec3d(+1, +1, 0)
                }
            ) > 0
        );

        ASSERT_TRUE(
            compare(
                polygon3d {
                    vec3d(-1, -1, 0),
                    vec3d(-1, +1, 0),
                    vec3d(+1, +1, 0)
                },
                polygon3d {
                    vec3d(+1, -1, 0),
                    vec3d(-1, +1, 0),
                    vec3d(+1, +1, 0),
                    vec3d(+1, -1, 0)
                }
            ) < 0
        );

        ASSERT_TRUE(
            compare(
                polygon3d {
                    vec3d(-1, -1, 0),
                    vec3d(-1, +1, 0),
                    vec3d(+1, +1, 0)
                },
                polygon3d {
                    vec3d(+1, -1, 0),
                    vec3d(-1, +1, 0),
                }
            ) < 0
        );

        ASSERT_TRUE(
            compare(
                polygon3d {
                    vec3d(+1, -1, 0),
                    vec3d(-1, +1, 0),
                    vec3d(+1, +1, 0),
                    vec3d(+1, -1, 0)
                },
                polygon3d {
                    vec3d(-1, -1, 0),
                    vec3d(-1, +1, 0),
                    vec3d(+1, +1, 0)
                }
            ) > 0
        );

        ASSERT_TRUE(
            compare(
                polygon3d {
                    vec3d(+1, -1, 0),
                    vec3d(-1, +1, 0),
                },
                polygon3d {
                    vec3d(-1, -1, 0),
                    vec3d(-1, +1, 0),
                    vec3d(+1, +1, 0)
                }
            ) > 0
        );
    }

    TEST(polygon_test, operator_equal) {
        ASSERT_TRUE(
            polygon3d() ==
            polygon3d()
        );

        ASSERT_TRUE(
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0),
                vec3d(+1, -1, 0)
            }) ==
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0),
                vec3d(+1, -1, 0)
            })
        );

        ASSERT_FALSE(
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0)
            }) ==
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0),
                vec3d(+1, -1, 0)
            })
        );

        ASSERT_FALSE(
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0),
                vec3d(+1, -1, 0)
            }) ==
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0)
            })
        );

        ASSERT_FALSE(
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0)
            }) ==
            polygon3d ({
                vec3d(+1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0),
                vec3d(+1, -1, 0)
            })
        );

        ASSERT_FALSE(
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0)
            }) ==
            polygon3d ({
                vec3d(+1, -1, 0),
                vec3d(-1, +1, 0),
            })
        );

        ASSERT_FALSE(
            polygon3d ({
                vec3d(+1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0),
                vec3d(+1, -1, 0)
            }) ==
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0)
            })
        );

        ASSERT_FALSE(
            polygon3d ({
                vec3d(+1, -1, 0),
                vec3d(-1, +1, 0),
            }) ==
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0)
            })
        );
    }

    TEST(polygon_test, operator_not_equal) {
        ASSERT_FALSE(
            polygon3d() !=
            polygon3d()
        );

        ASSERT_FALSE(
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0),
                vec3d(+1, -1, 0)
            }) !=
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0),
                vec3d(+1, -1, 0)
            })
        );

        ASSERT_TRUE(
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0)
            }) !=
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0),
                vec3d(+1, -1, 0)
            })
        );

        ASSERT_TRUE(
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0),
                vec3d(+1, -1, 0)
            }) !=
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0)
            })
        );

        ASSERT_TRUE(
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0)
            }) !=
            polygon3d ({
                vec3d(+1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0),
                vec3d(+1, -1, 0)
            })
        );

        ASSERT_TRUE(
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0)
            }) !=
            polygon3d ({
                vec3d(+1, -1, 0),
                vec3d(-1, +1, 0),
            })
        );

        ASSERT_TRUE(
            polygon3d ({
                vec3d(+1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0),
                vec3d(+1, -1, 0)
            }) !=
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0)
            })
        );

        ASSERT_TRUE(
            polygon3d ({
                vec3d(+1, -1, 0),
                vec3d(-1, +1, 0),
            }) !=
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0)
            })
        );
    }

    TEST(polygon_test, operator_less_than) {
        ASSERT_FALSE(
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0),
                vec3d(+1, -1, 0)
            }) <
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0),
                vec3d(+1, -1, 0)
            })
        );

        ASSERT_TRUE(
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0)
            }) <
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0),
                vec3d(+1, -1, 0)
            })
        );

        ASSERT_FALSE(
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0),
                vec3d(+1, -1, 0)
            }) <
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0),
            })
        );
    }

    TEST(polygon_test, operator_less_than_or_equal) {
        ASSERT_TRUE(
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0),
                vec3d(+1, -1, 0)
            }) <=
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0),
                vec3d(+1, -1, 0)
            })
        );

        ASSERT_TRUE(
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0)
            }) <=
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0),
                vec3d(+1, -1, 0)
            })
        );

        ASSERT_FALSE(
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0),
                vec3d(+1, -1, 0)
            }) <=
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0),
            })
        );
    }

    TEST(polygon_test, operator_greater_than) {
        ASSERT_FALSE(
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0),
                vec3d(+1, -1, 0)
            }) >
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0),
                vec3d(+1, -1, 0)
            })
        );

        ASSERT_FALSE(
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0)
            }) >
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0),
                vec3d(+1, -1, 0)
            })
        );

        ASSERT_TRUE(
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0),
                vec3d(+1, -1, 0)
            }) >
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0),
            })
        );
    }

    TEST(polygon_test, operator_greater_than_or_equal) {
        ASSERT_TRUE(
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0),
                vec3d(+1, -1, 0)
            }) >=
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0),
                vec3d(+1, -1, 0)
            })
        );

        ASSERT_FALSE(
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0)
            }) >=
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0),
                vec3d(+1, -1, 0)
            })
        );

        ASSERT_TRUE(
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0),
                vec3d(+1, -1, 0)
            }) >=
            polygon3d ({
                vec3d(-1, -1, 0),
                vec3d(-1, +1, 0),
                vec3d(+1, +1, 0),
            })
        );
    }

    TEST(polygon_test, compare_unoriented_empty_polygon) {
        polygon3d p1{};
        ASSERT_EQ(compareUnoriented(p1, polygon3d{}), 0);
        ASSERT_EQ(compareUnoriented(p1, polygon3d{vec3d::zero()}), -1);

        polygon3d p2{vec3d::zero()};
        ASSERT_EQ(compareUnoriented(p2, p1), +1);
        ASSERT_EQ(compareUnoriented(p2, polygon3d{vec3d::zero()}), 0);
    }

    TEST(polygon_test, testBackwardComparePolygonWithOneVertex) {
        polygon3d p2{vec3d::zero()};
        ASSERT_EQ(compareUnoriented(p2, polygon3d{vec3d::zero()}), 0);
        ASSERT_EQ(compareUnoriented(p2, polygon3d{vec3d::zero(), vec3d::zero()}), -1);
    }

    TEST(polygon_test, compare_unoriented) {
        polygon3d p1{
                vec3d(-1.0, -1.0, 0.0),
                vec3d(+1.0, -1.0, 0.0),
                vec3d(+1.0, +1.0, 0.0),
                vec3d(-1.0, +1.0, 0.0),
        };
        polygon3d p2{
                vec3d(-1.0, +1.0, 0.0),
                vec3d(+1.0, +1.0, 0.0),
                vec3d(+1.0, -1.0, 0.0),
                vec3d(-1.0, -1.0, 0.0),
        };
        ASSERT_EQ(compareUnoriented(p1, p1), 0);
        ASSERT_EQ(compareUnoriented(p1, p2), 0);
        ASSERT_EQ(compareUnoriented(p2, p1), 0);
        ASSERT_EQ(compareUnoriented(p2, p2), 0);
    }
}
