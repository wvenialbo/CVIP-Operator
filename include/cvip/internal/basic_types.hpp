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

#ifndef CVIP_CORE_BASIC_TYPES_HPP
#define CVIP_CORE_BASIC_TYPES_HPP

#pragma once


#include "../config/config.hpp"
#include <opencv2/core.hpp>
#include <cstdint>

#if not defined(CVIP_CONFIG_LOADED)
#error ERROR: Missing config.hpp
#endif // defined(CVIP_CONFIG_LOADED)


namespace cvip
{

    // Aliases for most used types from std
    //

    using uint_t  = std::uint32_t;
    using upix_t  = std::uint8_t;
    using wpix_t  = std::uint16_t;


    // Aliases for most common types in cv

    using vscalar = double;
    using mscalar = cv::Scalar;

    using matrix  = cv::Mat;

    using point   = cv::Point2i;


    // Algoritms/Scanning execution model
    //

    enum class execution_model : int
    {
        parallel, sequential
    };

}


#endif // !CVIP_CORE_BASIC_TYPES_HPP
