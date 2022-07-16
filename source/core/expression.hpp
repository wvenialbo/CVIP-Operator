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

#ifndef CVIP_CORE_EXPRESSION_HPP
#define CVIP_CORE_EXPRESSION_HPP

#pragma once


#include <cvip/core/i_operator.hpp>
#include <list>
#include <memory>

#if not defined(CVIP_CONFIG_LOADED)
#error ERROR: Missing config.hpp
#endif // defined(CVIP_CONFIG_LOADED)


namespace cvip
{

    // Operator expressions
    //
    // The built-int multiplication operator (operator*) is  used  to  indicate
    // operations among image operators and image matrices.
    //
    // Arithmetic operation among operators and matrices is done from right  to
    // left, nevertheless, operation among operators are gathered from  left to
    // right within an operator expression.
    //
    // Applying an operator, P, on a matrix, M1, produces a new matrix, M2:
    //
    //      P * M1 -> M2.
    //
    // Applying an operator, P2, on another operator, P1, produces an  operator
    // expression that gather  both  operators  for  later  application  in the
    // appropriate order:
    //
    //      P2 * P1 -> EX(P1, P2).
    //
    // Applying an operator expression on a matrix, M1, produces a new  matrix,
    // M2, after applying the encapsulated operators in the correct order:
    //
    //      EX(P1, P2) * M1 -> M2.
    //
    // Applying an operator expression on  an  operator,  P3,  produces  a  new
    // operator expression concatenating it with the  existing operators in the
    // proper order:
    //
    //      EX(P1, P2) * P3 -> EX(P3, P1, P2).
    //
    // Applying an operator expression on another operator expression, produces
    // a new operator expression concatenating the existing  operators  in  the
    // proper order:
    //
    //      EX(P1, P2) * EX(P3, P4) -> EX(P3, P4, P1, P2).
    //
    // The commutative property  does  not  hold  for  operators  and  operator
    // expressions:
    //
    //      P2 * P1 -> EX(P1, P2),
    //      P1 * P2 -> EX(P2, P1),
    //
    //      EX(P1, P2) * P3 -> EX(P3, P1, P2),
    //      P3 * EX(P1, P2) -> EX(P1, P2, P3),
    //
    //      EX(P1, P2) * EX(P3, P4) -> EX(P3, P4, P1, P2),
    //      EX(P3, P4) * EX(P1, P2) -> EX(P1, P2, P3, P4).
    //
    // Associative property among operators and operator  expressions  is  just
    // syntactic sugar and  is  implemented  for  semantic  purposes,  e.g. for
    // grouping or emphasise related  operations.  The following operations are
    // the same:
    //
    //      (P1 * P2) * P3 * M  ===  P1 * P2 * P3 * M,
    //
    // however, the next operations are equivalent,  i.e. they produce the same
    // result but the intermediate products are not the same:
    //
    //      (P1 * P2) * P3 * M  ==  P1 * (P2 * P3) * M,
    //
    // but
    //
    //      (P1 * P2) * P3 * M  !==  P1 * (P2 * P3) * M.
    //
    // Listing the intermediate results, we have:
    //
    // Example 1.
    //
    //  1) P3 * P2 * P1 * M1     ... or (P3 * P2) * P1 * M1
    //  2) EX(P2, P3) * P1 * M1
    //  3) EX(P1, P2, P3) * M1   ... operators are applied in the order
    //  4) M2                    ... P1, P2, and P3
    //
    // Example 2.
    //
    //  1) P3 * (P2 * P1) * M1
    //  2) P3 * EX(P1, P2) * M1
    //  3) EX(P1, P2, P3) * M1   ... operators are applied in the order
    //  4) M2                    ... P1, P2, and P3
    //
    // Example 3.
    //
    //  1) P4 * P3 * (P2 * P1) * M1
    //  2) P4 * P3 * EX(P1, P2) * M1
    //  3) EX(P3, P4) * EX(P1, P2) * M1
    //  4) EX(P1, P2, P3, P4) * M1
    //  5) M2
    //
    // Example 4.
    //
    //  1) (P4 * P3) * (P2 * P1) * M1
    //  2) EX(P3, P4) * (P2 * P1) * M1
    //  3) EX(P3, P4) * EX(P1, P2) * M1
    //  4) EX(P1, P2, P3, P4) * M1
    //  5) M2
    //
    // Provided the operators  are  equivalent,  Example 1  and  Example 2, and
    // Example 3 and Example 4, respectively, produce the same result.
    //
    // Applying association to operators or operator expressions and  matrices,
    // force the application  of  the  gathered  operators  to  the  image  and
    // produces a new intermediate image before continuing  with  the  remaning
    // operators:
    //
    // Example 5.
    //
    //  1) P3 * P2 * (P1 * M1)
    //  2) P3 * P2 * M2
    //  3) EX(P2, P3) * M2
    //  4) M3
    //
    // Even though, if the operators  are equivalent,  Example 1 and Example 5,
    // produce the same result.
    //

    namespace core
    {

        //class operator_expression;

        //operator_expression operator*(i_operator const& lhs_op, i_operator const& rhs_op);

        //operator_expression operator*(operator_expression&& lhs_ex, i_operator const& rhs_op);

        //operator_expression operator*(i_operator const& lhs_op, operator_expression&& rhs_ex);

        //operator_expression operator*(operator_expression&& lhs_ex, operator_expression&& rhs_ex);

        //matrix operator*(operator_expression& lhs_ex, matrix const& rhs_im);

        // Operator expression
        //
        // Allows operator semantics for  image matrix  operations.  Operations
        // among image  operators  and image  matrices  are right  associative,
        // while operations among operators are left associative.
        //

        class operator_expression
        {
        public:

            operator_expression() = delete;

            operator_expression(operator_expression const& src) noexcept = default;

            operator_expression(operator_expression&& src) noexcept = default;

            ~operator_expression() noexcept = default;

            operator_expression& operator=(operator_expression const& src) noexcept = default;

            operator_expression& operator=(operator_expression&& src) noexcept = default;


        private:

            operator_expression(i_operator const& lhs_op, i_operator const& rhs_op);


        private:

            matrix apply(matrix const& rhs_im);

            void emplace_back(operator_expression&& lhs_ex);

            void push_back(i_operator const& lhs_op);

            void push_front(i_operator const& rhs_op);


        private:

            friend operator_expression operator*(i_operator const& lhs_op, i_operator const& rhs_op);

            friend operator_expression operator*(operator_expression&& lhs_ex, i_operator const& rhs_op);

            friend operator_expression operator*(i_operator const& lhs_op, operator_expression&& rhs_ex);

            friend operator_expression operator*(operator_expression&& lhs_ex, operator_expression&& rhs_ex);

            friend matrix operator*(operator_expression& lhs_ex, matrix const& rhs_im);


        private:

            using opnode_t  = i_operator::opnode_t;
            using opchain_t = std::list<opnode_t>;
            using exdata_t  = std::shared_ptr<opchain_t>;


        private:

            exdata_t construct_data(i_operator const& lhs_op, i_operator const& rhs_op);


        private:

            exdata_t m_data = { };

        };

    }

}


#include "expression.inl"


#endif // !CVIP_CORE_EXPRESSION_HPP
