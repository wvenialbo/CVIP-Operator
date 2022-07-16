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

#ifndef CVIP_CONFIG_PLATFORM_HPP
#define CVIP_CONFIG_PLATFORM_HPP

#pragma once


// OS Platform flags

#if defined(__unix__) or defined(__unix) or defined(__APPLE__) and defined(__MACH__)
#   define CVIP_TARGET_POSIX_BUILD 1
#elif defined(_WIN32) or defined(_WIN64)
#   define CVIP_TARGET_WINDOWS_BUILD
#   define NOMINMAX
#   define _USE_MATH_DEFINES
#else
#   error ERROR : Unknow Target!
#endif


// Compiler flags

#if defined(__clang__)
#   define CVIP_COMPILER_CLANG
#elif defined(__GNUC__) or defined(__GNUG__)
#   define CVIP_COMPILER_GNUC
#elif defined(_MSC_VER)
#   define CVIP_COMPILER_MSVC
#else
#   error ERROR : Unsupported compiler!
#endif // defined(__clang__)


#endif // !CVIP_CONFIG_PLATFORM_HPP
