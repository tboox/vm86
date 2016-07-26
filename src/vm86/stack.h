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
 * @file        stack.h
 *
 */
#ifndef VM86_STACK_H
#define VM86_STACK_H

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

/// the machine stack ref type
typedef struct{}*           vm86_stack_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! init stack
 *
 * @param size              the stack size
 * @param esp               the esp register
 *
 * @return                  the stack
 */
vm86_stack_ref_t            vm86_stack_init(tb_size_t size, tb_uint32_t* esp);

/*! exit stack 
 *
 * @param stack             the stack
 */
tb_void_t                   vm86_stack_exit(vm86_stack_ref_t stack);

/*! clear stack 
 *
 * @param stack             the stack
 */
tb_void_t                   vm86_stack_clear(vm86_stack_ref_t stack);

/*! the top address
 *
 * @param stack             the stack
 * @param pdata             the data pointer
 * @param index             the data top index
 */
tb_void_t                   vm86_stack_top(vm86_stack_ref_t stack, tb_uint32_t* pdata, tb_size_t index);

/*! push data to stack
 *
 * @param stack             the stack
 * @param data              the data
 */
tb_void_t                   vm86_stack_push(vm86_stack_ref_t stack, tb_uint32_t data);

/*! pop data from stack
 *
 * @param stack             the stack
 * @param pdata             the data pointer
 */
tb_void_t                   vm86_stack_pop(vm86_stack_ref_t stack, tb_uint32_t* pdata);

#ifdef __vm_debug__
/*! dump stack 
 *
 * @param stack             the stack
 */
tb_void_t                   vm86_stack_dump(vm86_stack_ref_t stack);
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


