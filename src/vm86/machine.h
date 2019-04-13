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
 * @file        machine.h
 *
 */
#ifndef VM86_MACHINE_H
#define VM86_MACHINE_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "data.h"
#include "text.h"
#include "stack.h"
#include "register.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/*! the machine func type
 *
 * @param machine               the machine
 */
typedef tb_void_t               (*vm86_machine_func_t)(vm86_machine_ref_t machine);

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! the machine instance
 *
 * @return                      the machine
 */
vm86_machine_ref_t              vm86_machine(tb_noarg_t);

/*! init machine
 *
 * @param data_size             the data size
 * @param stack_size            the stack size
 *
 * @return                      the machine
 */
vm86_machine_ref_t              vm86_machine_init(tb_size_t data_size, tb_size_t stack_size);

/*! exit machine 
 *
 * @param machine               the machine
 */
tb_void_t                       vm86_machine_exit(vm86_machine_ref_t machine);

/*! the machine lock
 *
 * @param machine               the machine
 */
tb_spinlock_ref_t               vm86_machine_lock(vm86_machine_ref_t machine);

/*! the machine text
 *
 * @param machine               the machine
 *
 * @return                      the text
 */
vm86_text_ref_t                 vm86_machine_text(vm86_machine_ref_t machine);

/*! the machine data
 *
 * @param machine               the machine
 *
 * @return                      the data
 */
vm86_data_ref_t                 vm86_machine_data(vm86_machine_ref_t machine);

/*! the machine stack
 *
 * @param machine               the machine
 *
 * @return                      the stack
 */
vm86_stack_ref_t                vm86_machine_stack(vm86_machine_ref_t machine);

/*! the machine registers
 *
 * @param machine               the machine
 *
 * @return                      the registers
 */
vm86_registers_ref_t            vm86_machine_registers(vm86_machine_ref_t machine);

/*! get function from the machine 
 *
 * @param machine               the machine
 * @param name                  the function name
 *
 * @return                      the function address
 */
vm86_machine_func_t             vm86_machine_function(vm86_machine_ref_t machine, tb_char_t const* name);

/*! set function to the machine 
 *
 * @param machine               the machine
 * @param name                  the function name
 * @param func                  the function address
 */
tb_void_t                       vm86_machine_function_set(vm86_machine_ref_t machine, tb_char_t const* name, vm86_machine_func_t func);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


