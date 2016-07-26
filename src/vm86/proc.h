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
 * @file        proc.h
 *
 */
#ifndef VM86_PROC_H
#define VM86_PROC_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/// the machine proc ref type
typedef struct{}*           vm86_proc_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! init proc
 *
 * @param machine          the machine
 * @param code              the proc code
 * @param size              the proc size
 *
 * @return                  the proc
 */
vm86_proc_ref_t      vm86_proc_init(vm86_machine_ref_t machine, tb_char_t const* code, tb_size_t size);

/*! exit proc 
 *
 * @param proc              the proc
 */
tb_void_t                   vm86_proc_exit(vm86_proc_ref_t proc);

/*! the proc name
 *
 * @param proc              the proc
 *
 * @return                  the name
 */
tb_char_t const*            vm86_proc_name(vm86_proc_ref_t proc);

/*! done proc
 *
 * @param proc              the proc
 */
tb_void_t                   vm86_proc_done(vm86_proc_ref_t proc);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


