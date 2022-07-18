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

#ifndef CVIP_CORE_OPERATOR_INL
#define CVIP_CORE_OPERATOR_INL

#pragma once


#include "../operator.hpp"
#include <memory>
#include <utility>

#if not defined(CVIP_CONFIG_LOADED)
#error ERROR: Missing config.hpp
#endif // defined(CVIP_CONFIG_LOADED)


namespace cvip
{

    namespace core
    {

        // base_operator<ConcreteOperator>
        //

        template<typename ConcreteOperator>
        inline typename base_operator<ConcreteOperator>::opnode_t base_operator<ConcreteOperator>::clone() const
        {
            static_assert(is_operator<operator_t>::value, "operator_t is not an i_operator");
            return std::make_shared<operator_t>(*static_cast<operator_t const*>(this));
        }


        // basic_operator<Predicate>
        //

        template<typename Predicate> template<typename ...Args>
        basic_operator<Predicate>::basic_operator(Args&& ...arg) :
            m_operation{ std::forward<Args>(arg)... }
        {
            // NOOP
        }

        template<typename Predicate> template<typename ...Args>
        inline basic_operator<Predicate>& cvip::core::basic_operator<Predicate>::operator()(Args&& ...arg)
        {
            return (m_operation.reset(std::forward<Args>(arg)...), *this);
        }

        template<typename Predicate>
        inline void basic_operator<Predicate>::apply(matrix& dst, matrix& src, bool const first)
        {
            m_operation.do_apply(dst, src, first);
        }


        // image operator operations
        //

        inline matrix operator*(i_operator&& lhs_op, matrix const& rhs_im)
        {
            return lhs_op * rhs_im;
        }

    }

}

#endif // !CVIP_CORE_OPERATOR_INL
