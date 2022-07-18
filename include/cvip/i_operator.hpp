//
// Copyright 2021, Waldemar Villamayor-Venialbo. All Rights Reserved.
//
// This file is part of the AsterAID Project. See README for details.
//
// AsterAID is a trademark of the copyright owner. Other trademarks
// may be the property of their respective owners.
//
// The content of this source code is licensed under the BSD License.
// See LICENSE file in the project root for full license information.
//

#ifndef CVIP_CORE_I_OPERATOR_HPP
#define CVIP_CORE_I_OPERATOR_HPP

#pragma once


#include "internal/basic_types.hpp"
#include <memory>
#include <type_traits>

#if not defined(CVIP_CONFIG_LOADED)
#error ERROR: Missing config.hpp
#endif // defined(CVIP_CONFIG_LOADED)


namespace cvip
{

    namespace core
    {

        // Interface for image operator classes
        //

        class i_operator
        {
        protected:

            // operator chain node type
            //
            using opnode_t = std::shared_ptr<i_operator>;

            // apply operator on matrix
            //
            // rhs : A modifiable reference to a smart-reference  image  container.
            //       On input,  it holds a reference  to  the  original (right hand
            //       side) operand.  On output,  it holds  a reference  to an image
            //       with  the  same  depth and size of the input image; this image
            //       will be used as temporary processing  buffer  in the  chain of
            //       operators in an expression. Must be a non empty image.
            //
            // dst : A modifiable reference to a smart-reference  image  container.
            //       On input, assumed to be an empty matrix for the first operator
            //       in the chain of operators in an expression; for remaing calls,
            //       a reference  to the buffer  returned by  the first operator in
            //       rhs. On output, the result of a successful operation.
            //
            virtual void apply(matrix& dst, matrix& src, bool const first) = 0;

            // clone this operator
            //
            virtual opnode_t clone() const = 0;

            friend matrix operator*(i_operator& lhs_op, matrix const& rhs_im);

            friend class operator_expression;

        };


        // Tool for checking i_operator implementation
        //

        template<typename T>
        class is_operator
        {
        public:

            static auto constexpr value = std::is_base_of<i_operator, T>::value;

        };


        // Interface for operator predicates
        //

        class i_operator_predicate
        {
        public:

            //virtual bool supported(matrix const& mat) const noexcept = 0;

            virtual void do_apply(matrix& dst, matrix& src, bool const first) = 0;

            template<typename ...Args>
            void reset(Args&& ...arg);

        };


        // Tool for checking i_operator_predicate implementation
        //

        template<typename P>
        class is_operator_predicate
        {
        private:

            struct has_member { char z[1]; };
            struct no_member  { char z[2]; };

            template<typename T> static has_member has_do_apply_member(decltype(&T::do_apply));

            template<typename T> static no_member  has_do_apply_member(...);

            static auto constexpr yes           = sizeof(has_member);
            static auto constexpr has_do_apply  = sizeof(has_do_apply_member<P>(nullptr));
            static auto constexpr is_predicate  = std::is_base_of<i_operator_predicate, P>::value;

        public:

            static auto constexpr value = is_predicate or has_do_apply == yes;

        };

    }

}


#endif // !CVIP_CORE_I_OPERATOR_HPP
