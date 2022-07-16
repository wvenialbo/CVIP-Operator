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

#ifndef CVIP_CONFIG_CONFIG_HPP
#define CVIP_CONFIG_CONFIG_HPP

#pragma once


#include "platform.hpp"

#if not defined(CVIP_CONFIG_PLATFORM_HPP)
#error ERROR: Missing platform.hpp
#endif // defined(CVIP_CONFIG_CONFIG_HPP)


// This library build's version info
// (Not the ideal to be here though...)
//
static auto constexpr cvip_ver_number =  20220713L;
static auto constexpr cvip_ver_string = "20220713";
static auto constexpr __cvip          = cvip_ver_number;
static auto constexpr __cvip_s        = cvip_ver_string;


// Tags for signal the begin and end of implementation detail code
#define CVIP_IMPLEMENTATION_DETAILS(Namespace)          namespace Namespace::detail
#define CVIP_BEGIN_IMPLEMENTATION_DETAILS(Namespace)    namespace Namespace::detail {
#define CVIP_END_IMPLEMENTATION_DETAILS(Namespace)      }


#define CVIP_CONFIG_LOADED


#endif // !CVIP_CONFIG_CONFIG_HPP
