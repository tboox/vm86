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

// the emulator instruction done ref type
struct __vm86_instruction_t;
typedef struct __vm86_instruction_t* (*vm86_instruction_done_ref_t)(struct __vm86_instruction_t* instruction, vm86_ref_t emulator);

// the emulator instruction type
typedef struct __vm86_instruction_t
{
    // the registers
    tb_uint8_t                              r0;
    tb_uint8_t                              r1;
    tb_uint8_t                              r2;

    // is cstr? need free it
    tb_uint8_t                              is_cstr : 1;

    // the op: +, -, *
    tb_char_t                               op;

    // the hint 
    tb_char_t                               hint[3];

    // the values
    tb_value_t                              v0;
    tb_value_t                              v1;

    // done it
    vm86_instruction_done_ref_t      done;

}vm86_instruction_t, *vm86_instruction_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/* compile the instruction 
 *
 * @param instruction       the instruction
 * @param code              the instruction code
 * @param size              the instruction size
 * @param emulator          the emulator
 * @param proc_labels       the proc labels
 * @param proc_locals       the proc labels
 */
tb_bool_t                   vm86_instruction_compile(vm86_instruction_ref_t instruction, tb_char_t const* code, tb_size_t size, vm86_ref_t emulator, tb_hash_map_ref_t proc_labels, tb_hash_map_ref_t proc_locals);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


