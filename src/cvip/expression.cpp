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

#include <cvip/expression.hpp>
#include <cvip/internal/basic_imports.hpp>
#include <initializer_list>


namespace cvip
{

    namespace core
    {

        operator_expression::operator_expression(i_operator const& lhs_op, i_operator const& rhs_op) :
            m_data{ construct_data(lhs_op, rhs_op) }
        {
            // NOOP
        }

        matrix operator_expression::apply(matrix const& rhs_im)
        {
            auto src = matrix{ rhs_im };
            auto dst = matrix{ };

            auto first = true;

            for (auto& op : *m_data)
            {
                op->apply(dst, src, first);

                cvip::swap(dst, src);

                first = false;
            }

            // REMARK: The result is in src due to the swap
            //         at the end of each iteration!

            return src;
        }

        inline operator_expression::exdata_t operator_expression::construct_data(i_operator const& lhs_op,
                                                                                 i_operator const& rhs_op)
        {
            return std::make_shared<opchain_t>(opchain_t{ rhs_op.clone(), lhs_op.clone() });
        }

    }

}
