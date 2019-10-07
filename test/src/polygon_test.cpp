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
#include <vecmath/vec.h>
#include <vecmath/vec_ext.h>
#include <vecmath/mat.h>
#include <vecmath/mat_ext.h>
#include <vecmath/polygon.h>

#include "test_utils.h"

#include <algorithm>
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
        polygon3d::getVertices(std::begin(ps), std::end(ps), std::back_inserter(act));

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
