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


