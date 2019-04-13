/*!The x86 Script Instruction Virtual Machine
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * Copyright (C) 2014 - 2017, TBOOX Open Source Group.
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


