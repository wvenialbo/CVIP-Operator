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

#ifndef CVIP_CORE_BASIC_IMPORTS_HPP
#define CVIP_CORE_BASIC_IMPORTS_HPP

#pragma once


#include "../config/config.hpp"
#include <opencv2/core.hpp>

#if not defined(CVIP_CONFIG_LOADED)
#error ERROR: Missing config.hpp
#endif // defined(CVIP_CONFIG_LOADED)


namespace cvip
{


    // Importing miscellaneous symbols
    //

    using cv::swap;

}


#endif // !CVIP_CORE_BASIC_IMPORTS_HPP
