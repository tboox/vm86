/*!The x86 Script Instruction Virtual Machine
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
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


