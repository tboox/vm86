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
 * @file        register.h
 *
 */
#ifndef VM86_REGISTER_H
#define VM86_REGISTER_H

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

/// the machine register type
typedef union __vm86_register_t
{
    /// the u8
    tb_uint8_t          u8[4];

    /// the u16
    tb_uint16_t         u16[2];

    /// the u32
    tb_uint32_t         u32;

}vm86_register_t;

/// the machine register enum
typedef enum __vm86_register_e
{
    VM86_REGISTER_EAX        = 0
,   VM86_REGISTER_EBX        = 1
,   VM86_REGISTER_ECX        = 2
,   VM86_REGISTER_EDX        = 3

,   VM86_REGISTER_AL         = 1 << 4
,   VM86_REGISTER_BL         = 5 << 4
,   VM86_REGISTER_CL         = 9 << 4
,   VM86_REGISTER_DL         = 13 << 4

,   VM86_REGISTER_AH         = 2 << 4
,   VM86_REGISTER_BH         = 6 << 4
,   VM86_REGISTER_CH         = 10 << 4
,   VM86_REGISTER_DH         = 14 << 4

,   VM86_REGISTER_AX         = 3 << 4
,   VM86_REGISTER_BX         = 7 << 4
,   VM86_REGISTER_CX         = 11 << 4
,   VM86_REGISTER_DX         = 15 << 4

,   VM86_REGISTER_ESP        = 4
,   VM86_REGISTER_EBP        = 5
,   VM86_REGISTER_ESI        = 6
,   VM86_REGISTER_EDI        = 7

,   VM86_REGISTER_CS         = 8
,   VM86_REGISTER_DS         = 9
,   VM86_REGISTER_SS         = 10
,   VM86_REGISTER_ES         = 11
,   VM86_REGISTER_FS         = 12
,   VM86_REGISTER_GS         = 13

,   VM86_REGISTER_EIP        = 14
,   VM86_REGISTER_EFLAGS     = 15

,   VM86_REGISTER_MAXN       = 16
,   VM86_REGISTER_MASK       = 0xf

}vm86_register_e;

/// the machine register eflag enum
typedef enum __vm86_register_eflag_e
{
    VM86_REGISTER_EFLAG_CF   = 1 << 0
,   VM86_REGISTER_EFLAG_PF   = 1 << 2
,   VM86_REGISTER_EFLAG_AF   = 1 << 4
,   VM86_REGISTER_EFLAG_ZF   = 1 << 6
,   VM86_REGISTER_EFLAG_SF   = 1 << 7
,   VM86_REGISTER_EFLAG_OF   = 1 << 11

}vm86_register_eflag_e;

/// the machine registers type
typedef vm86_register_t (vm86_registers_t)[VM86_REGISTER_MAXN];

/// the machine registers ref type
typedef vm86_register_t* vm86_registers_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */
static __tb_inline__ tb_void_t vm86_registers_clear(vm86_registers_ref_t registers)
{
    // check
    tb_assert(registers);

    // clear it
    tb_memset(registers, 0, VM86_REGISTER_MAXN * sizeof(vm86_register_t));
}
static __tb_inline__ tb_uint32_t vm86_registers_value(vm86_registers_ref_t registers, tb_uint8_t index)
{
    // check
    tb_assert(registers);

    // get the register
    tb_size_t r = index & VM86_REGISTER_MASK;
    tb_assert(r < VM86_REGISTER_MAXN);

    // done
    tb_uint32_t value = 0;
    switch ((index >> 4) & 3)
    {
    case 0: value = registers[r].u32;       break;
    case 1: value = registers[r].u8[0];     break;
    case 2: value = registers[r].u8[1];     break;
    case 3: value = registers[r].u16[0];    break;
    default:
        {
            tb_assert(0);
        }
    }

    // ok?
    return value;
}
static __tb_inline__ tb_void_t vm86_registers_value_set(vm86_registers_ref_t registers, tb_uint8_t index, tb_uint32_t value)
{
    // check
    tb_assert(registers);

    // get the register
    tb_size_t r = index & VM86_REGISTER_MASK;
    tb_assert(r < VM86_REGISTER_MAXN);

    // done
    switch ((index >> 4) & 3)
    {
    case 0: registers[r].u32 = value;       break;
    case 1: registers[r].u8[0] = value;     break;
    case 2: registers[r].u8[1] = value;     break;
    case 3: registers[r].u16[0] = value;    break;
    default:
        {
            tb_assert(0);
        }
    }
}
static __tb_inline__ tb_char_t const* vm86_registers_cstr(tb_uint8_t index)
{
    // done
    tb_char_t const* cstr = tb_null;
    switch (index)
    {
    case VM86_REGISTER_EAX: cstr = "eax"; break;
    case VM86_REGISTER_EBX: cstr = "ebx"; break;
    case VM86_REGISTER_ECX: cstr = "ecx"; break;
    case VM86_REGISTER_EDX: cstr = "edx"; break;

    case VM86_REGISTER_ESP: cstr = "esp"; break;
    case VM86_REGISTER_EBP: cstr = "ebp"; break;
    case VM86_REGISTER_ESI: cstr = "esi"; break;
    case VM86_REGISTER_EDI: cstr = "edi"; break;

    case VM86_REGISTER_EAX | VM86_REGISTER_AL: cstr = "al"; break;
    case VM86_REGISTER_EAX | VM86_REGISTER_AH: cstr = "ah"; break;
    case VM86_REGISTER_EAX | VM86_REGISTER_AX: cstr = "ax"; break;

    case VM86_REGISTER_EBX | VM86_REGISTER_BL: cstr = "bl"; break;
    case VM86_REGISTER_EBX | VM86_REGISTER_BH: cstr = "bh"; break;
    case VM86_REGISTER_EBX | VM86_REGISTER_BX: cstr = "bx"; break;

    case VM86_REGISTER_ECX | VM86_REGISTER_CL: cstr = "cl"; break;
    case VM86_REGISTER_ECX | VM86_REGISTER_CH: cstr = "ch"; break;
    case VM86_REGISTER_ECX | VM86_REGISTER_CX: cstr = "cx"; break;

    case VM86_REGISTER_EDX | VM86_REGISTER_DL: cstr = "dl"; break;
    case VM86_REGISTER_EDX | VM86_REGISTER_DH: cstr = "dh"; break;
    case VM86_REGISTER_EDX | VM86_REGISTER_DX: cstr = "dx"; break;

    default: cstr = "unk"; break;
    }

    // ok?
    return cstr;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__


#endif


