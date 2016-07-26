/*!The x86 Script Instruction Virtual Machine
 * 
 * vm86 is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 * 
 * vm86 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with vm86; 
 * If not, see <a href="http://www.gnu.org/licenses/"> http://www.gnu.org/licenses/</a>
 * 
 * Copyright (C) 2014 - 2016, ruki All rights reserved.
 *
 * @author      ruki
 * @file        version.h
 *
 */
#ifndef VM86_PREFIX_VERSION_H
#define VM86_PREFIX_VERSION_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "config.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

/// the major version
#define VM86_VERSION_MAJOR            VM86_CONFIG_VERSION_MAJOR

/// the minor version
#define VM86_VERSION_MINOR            VM86_CONFIG_VERSION_MINOR

/// the alter version
#define VM86_VERSION_ALTER            VM86_CONFIG_VERSION_ALTER

/// the build version
#define VM86_VERSION_BUILD            VM86_CONFIG_VERSION_BUILD

/// the build version string
#define VM86_VERSION_BUILD_STRING     __tb_mstring_ex__(VM86_CONFIG_VERSION_BUILD)

/// the version string
#define VM86_VERSION_STRING           __tb_mstrcat6__("vm86_", __tb_mstring_ex__(__tb_mconcat8_ex__(v, VM86_VERSION_MAJOR, _, VM86_VERSION_MINOR, _, VM86_VERSION_ALTER, _, VM86_CONFIG_VERSION_BUILD)), "_", TB_ARCH_VERSION_STRING, " by ", TB_COMPILER_VERSION_STRING)

/// the short version string
#define VM86_VERSION_SHORT_STRING     __tb_mstrcat__("vm86_", __tb_mstring_ex__(__tb_mconcat8_ex__(v, VM86_VERSION_MAJOR, _, VM86_VERSION_MINOR, _, VM86_VERSION_ALTER, _, VM86_CONFIG_VERSION_BUILD)))

#endif


