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

#ifndef CVIP_CORE_OPERATOR_HPP
#define CVIP_CORE_OPERATOR_HPP

#pragma once


#include <cvip/core/i_operator.hpp>

#if not defined(CVIP_CONFIG_LOADED)
#error ERROR: Missing config.hpp
#endif // defined(CVIP_CONFIG_LOADED)


namespace cvip
{

    namespace core
    {

        // Generic abstract base class for image matrix operators, partially
        // implements i_operator providing cloning facility via CRT Pattern.
        //

        template<typename ConcreteOperator>
        class base_operator : public i_operator
        {
        public:

            base_operator() noexcept = default;

            base_operator(base_operator const& src) noexcept = default;

            base_operator(base_operator&& src) noexcept = default;

            virtual ~base_operator() noexcept = default;

            base_operator& operator=(base_operator const& src) noexcept = default;

            base_operator& operator=(base_operator&& src) noexcept = default;


        protected:

            using operator_t = ConcreteOperator;

            virtual opnode_t clone() const override;

        };


        // Predicate based image operator
        //

        template<typename Predicate>
        class basic_operator : public base_operator< basic_operator<Predicate> >
        {
        public:

            template<typename ...Args>
            basic_operator(Args&& ...arg);

            template<typename ...Args>
            basic_operator& operator()(Args&& ...arg);


        protected:

            virtual void apply(matrix& dst, matrix& src, bool const first) override;


        private:

            using predicate_t = Predicate;

            static_assert(is_operator_predicate<predicate_t>::value,
                          "predicate_t does not implement i_operator_predicate");


        private:

            predicate_t m_operation = { };

        };

    }

}


#include "operator.inl"


#endif // !CVIP_CORE_OPERATOR_HPP
