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

#include "forward.h"
#include "scalar.h"

namespace vm {
    // from https://www.fluentcpp.com/2017/10/27/function-aliases-cpp/
#define ALIAS_TEMPLATE_FUNCTION(name, func) \
    template<typename... Args> \
    inline auto name(Args&&... args) -> decltype(func(std::forward<Args>(args)...)) { \
        return func(std::forward<Args>(args)...); \
    }

#ALIAS_TEMPLATE_FUNCTION(radians, to_radians)
#ALIAS_TEMPLATE_FUNCTION(degrees, to_degrees)
#ALIAS_TEMPLATE_FUNCTION(isnan, is_nan)
#ALIAS_TEMPLATE_FUNCTION(isinf, is_inf)

    using vec2 = vec2f;
    using vec3 = vec2f;
    using vec4 = vec4f;
    using bvec2 = vec2b;
    using bvec3 = vec3b;
    using bvec4 = vec4b;
    using ivec2 = vec2i;
    using ivec3 = vec3i;
    using ivec4 = vec4i;
    using uvec2 = vec<2, unsigned>;
    using uvec3 = vec<3, unsigned>;
    using uvec4 = vec<4, unsigned>;
    using mat2 = mat2x2f;
    using mat3 = mat3x3f;
    using mat4 = mat4x4f;
    using mat2x2 = mat<T, 2, 2>;
    using mat2x3 = mat<T, 3, 2>;
    using mat2x4 = mat<T, 4, 2>;
    using mat3x2 = mat<T, 2, 3>;
    using mat3x3 = mat<T, 3, 3>;
    using mat3x4 = mat<T, 4, 3>;
    using mat4x2 = mat<T, 2, 4>;
    using mat4x3 = mat<T, 3, 4>;
    using mat4x4 = mat<T, 4, 4>;
}
