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


