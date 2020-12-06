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

#pragma once

#include "mat.h"

#include <ostream>

namespace vm {
    /**
     * Prints a textual representation of the given matrix on the given stream.
     *
     * @tparam T the component type
     * @tparam R the number of rows
     * @tparam C the number of columns
     * @param stream the stream to print to
     * @param mat the matrix to print
     * @return the given stream
     */
    template <typename T, size_t R, size_t C>
    std::ostream& operator<<(std::ostream& stream, const mat<T,R,C>& mat) {
        stream << "[\n";
        for (size_t r = 0u; r < R; ++r) {
            stream << "  ";
            for (size_t c = 0u; c < C; ++c) {
                stream << mat[c][r];
                if (c < C-1u) {
                    stream << ", ";
                }
            }
            if (r < R-1u) {
                stream << ",";
            }
            stream << "\n";
        }
        stream << "]";
        return stream;
    }
}

#endif //VECMATH_MAT_IO_H
