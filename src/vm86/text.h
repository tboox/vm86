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
 * @file        text.h
 *
 */
#ifndef VM86_TEXT_H
#define VM86_TEXT_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "proc.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/// the machine text ref type
typedef struct{}*           vm86_text_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! init text
 *
 * @param machine           the machine
 *
 * @return                  the text
 */
vm86_text_ref_t             vm86_text_init(vm86_machine_ref_t machine);

/*! exit text 
 *
 * @param text              the text
 */
tb_void_t                   vm86_text_exit(vm86_text_ref_t text);

/*! compile proc 
 *
 * @param text              the text
 * @param code              the proc code
 *
 * @return                  the proc 
 */
vm86_proc_ref_t             vm86_text_compile(vm86_text_ref_t text, tb_char_t const* code, tb_size_t size);

/*! get the compiled proc 
 *
 * @param text              the text
 * @param name              the proc name
 *
 * @return                  the proc 
 */
vm86_proc_ref_t             vm86_text_proc(vm86_text_ref_t text, tb_char_t const* name);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


