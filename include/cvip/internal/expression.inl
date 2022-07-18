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

#ifndef CVIP_CORE_EXPRESSION_INL
#define CVIP_CORE_EXPRESSION_INL

#pragma once


#include "../expression.hpp"
#include <utility>

#if not defined(CVIP_CONFIG_LOADED)
#error ERROR: Missing config.hpp
#endif // defined(CVIP_CONFIG_LOADED)


namespace cvip
{

    namespace core
    {

        // operator_expression
        //

        inline void operator_expression::emplace_back(operator_expression&& lhs_ex)
        {
            m_data->splice(m_data->end(), std::move(*lhs_ex.m_data));
        }

        inline void operator_expression::push_back(i_operator const& lhs_op)
        {
            m_data->emplace_back(lhs_op.clone());
        }

        inline void operator_expression::push_front(i_operator const& rhs_op)
        {
            m_data->emplace_front(rhs_op.clone());
        }


        // operator_expression operator* (op * op)
        //

        inline operator_expression operator*(i_operator const& lhs_op, i_operator const& rhs_op)
        {
            return { lhs_op, rhs_op };
        }


        // operator_expression operator* (ex * op, op * ex)
        //

        inline operator_expression operator*(operator_expression&& lhs_ex, i_operator const& rhs_op)
        {
            return (lhs_ex.push_front(rhs_op), std::move(lhs_ex));
        }

        inline operator_expression operator*(i_operator const& lhs_op, operator_expression&& rhs_ex)
        {
            return (rhs_ex.push_back(lhs_op), std::move(rhs_ex));
        }

        inline operator_expression operator*(operator_expression const& lhs_ex, i_operator const& rhs_op)
        {
            return operator_expression{ lhs_ex } * rhs_op;
        }

        inline operator_expression operator*(i_operator const& lhs_op, operator_expression const& rhs_ex)
        {
            return lhs_op * operator_expression{ rhs_ex };
        }


        // operator_expression operator* (ex * ex)
        //

        inline operator_expression operator*(operator_expression&& lhs_ex, operator_expression&& rhs_ex)
        {
            return (rhs_ex.emplace_back(std::move(lhs_ex)), std::move(rhs_ex));
        }

        inline operator_expression operator*(operator_expression&& lhs_ex, operator_expression const& rhs_ex)
        {
            return std::move(lhs_ex) * operator_expression{ rhs_ex };
        }

        inline operator_expression operator*(operator_expression const& lhs_ex, operator_expression&& rhs_ex)
        {
            return operator_expression{ lhs_ex } * std::move(rhs_ex);
        }

        inline operator_expression operator*(operator_expression const& lhs_ex, operator_expression const& rhs_ex)
        {
            return operator_expression{ lhs_ex } * operator_expression{ rhs_ex };
        }

        inline matrix operator*(operator_expression& lhs_ex, matrix const& rhs_im)
        {
            return lhs_ex.apply(rhs_im);
        }

        inline matrix operator*(operator_expression&& lhs_ex, matrix const& rhs_im)
        {
            return lhs_ex * rhs_im;
        }

    }

}


#endif // !CVIP_CORE_EXPRESSION_INL
