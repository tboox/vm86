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
 * @file        parser.h
 *
 */
#ifndef VM86_PARSER_H
#define VM86_PARSER_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "machine.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/* get variable name
 *
 * @param pp                the code pointer
 * @param e                 the code pointer tail
 * @param name              the variable name
 * @param maxn              the variable name maxn
 *
 * @return                  tb_true or tb_false
 */
tb_bool_t                   vm86_parser_get_variable_name(tb_char_t const** pp, tb_char_t const* e, tb_char_t* name, tb_size_t maxn);

/* get segment name
 *
 * @param pp                the code pointer
 * @param e                 the code pointer tail
 * @param name              the segment name
 * @param maxn              the segment name maxn
 *
 * @return                  tb_true or tb_false
 */
tb_bool_t                   vm86_parser_get_segment_name(tb_char_t const** pp, tb_char_t const* e, tb_char_t* name, tb_size_t maxn);

/* get instruction name
 *
 * @param pp                the code pointer
 * @param e                 the code pointer tail
 * @param name              the instruction name
 * @param maxn              the instruction name maxn
 *
 * @return                  tb_true or tb_false
 */
tb_bool_t                   vm86_parser_get_instruction_name(tb_char_t const** pp, tb_char_t const* e, tb_char_t* name, tb_size_t maxn);

/* get register
 *
 * @param pp                the code pointer
 * @param e                 the code pointer tail
 * @param r                 the register pointer
 *
 * @return                  tb_true or tb_false
 */
tb_bool_t                   vm86_parser_get_register(tb_char_t const** pp, tb_char_t const* e, tb_uint16_t* r);

/* get value
 *
 * @param pp                the code pointer
 * @param e                 the code pointer tail
 * @param value             the value pointer
 * @param proc_locals       the proc locals
 * @param proc_labels       the proc labels
 * @param data              the .data segment
 *
 * @return                  tb_true or tb_false
 */
tb_bool_t                   vm86_parser_get_value(tb_char_t const** pp, tb_char_t const* e, tb_uint32_t* value, tb_hash_map_ref_t proc_locals, tb_hash_map_ref_t proc_labels, vm86_data_ref_t data);

/* get number value
 *
 * @param pp                the code pointer
 * @param e                 the code pointer tail
 * @param value             the value pointer
 *
 * @return                  tb_true or tb_false
 */
tb_bool_t                   vm86_parser_get_number_value(tb_char_t const** pp, tb_char_t const* e, tb_uint32_t* value);

/* get local value
 *
 * @param pp                the code pointer
 * @param e                 the code pointer tail
 * @param value             the value pointer
 * @param proc_locals       the proc locals
 *
 * @return                  tb_true or tb_false
 */
tb_bool_t                   vm86_parser_get_local_value(tb_char_t const** pp, tb_char_t const* e, tb_uint32_t* value, tb_hash_map_ref_t proc_locals);

/* get offset value
 *
 * @param pp                the code pointer
 * @param e                 the code pointer tail
 * @param value             the value pointer
 * @param proc_labels       the proc labels
 * @param data              the .data segment
 *
 * @return                  tb_true or tb_false
 */
tb_bool_t                   vm86_parser_get_offset_value(tb_char_t const** pp, tb_char_t const* e, tb_uint32_t* value, tb_hash_map_ref_t proc_labels, vm86_data_ref_t data);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


