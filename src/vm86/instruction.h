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


