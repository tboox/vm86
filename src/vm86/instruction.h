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
 * @file        instruction.h
 *
 */
#ifndef VM86_INSTRUCTION_H
#define VM86_INSTRUCTION_H

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

// the machine instruction done ref type
struct __vm86_instruction_t;
typedef struct __vm86_instruction_t* (*vm86_instruction_done_ref_t)(struct __vm86_instruction_t* instruction, vm86_machine_ref_t machine);

// the machine instruction type
typedef struct __vm86_instruction_t
{
    // the registers
    tb_uint8_t                      r0;
    tb_uint8_t                      r1;
    tb_uint8_t                      r2;

    // is cstr? need free it
    tb_uint8_t                      is_cstr : 1;

    // the op: +, -, *
    tb_char_t                       op;

    // the hint 
    tb_char_t                       hint[3];

    // the values
    tb_value_t                      v0;
    tb_value_t                      v1;

    // done it
    vm86_instruction_done_ref_t     done;

}vm86_instruction_t, *vm86_instruction_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! compile the instruction 
 *
 * @param instruction       the instruction
 * @param code              the instruction code
 * @param size              the instruction size
 * @param machine           the machine
 * @param proc_labels       the proc labels
 * @param proc_locals       the proc labels
 */
tb_bool_t                   vm86_instruction_compile(vm86_instruction_ref_t instruction, tb_char_t const* code, tb_size_t size, vm86_machine_ref_t machine, tb_hash_map_ref_t proc_labels, tb_hash_map_ref_t proc_locals);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


