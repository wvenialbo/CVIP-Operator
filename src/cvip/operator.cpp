//
// Copyright 2022, Waldemar Villamayor-Venialbo. All Rights Reserved.
//
// This file is part of the AsterAID Project. See README for details.
//
// AsterAID is a trademark of the copyright owner. Other trademarks
// may be the property of their respective owners.
//
// The content of this source code is licensed under the BSD License.
// See LICENSE file in the project root for full license information.
//

#include <cvip/i_operator.hpp>


namespace cvip
{

    namespace core
    {

        matrix operator*(i_operator& lhs_op, matrix const& rhs_im)
        {
            auto src = matrix{ rhs_im };
            auto dst = matrix{ };

            lhs_op.apply(dst, src, true);

            return dst;
        }

    }

}
