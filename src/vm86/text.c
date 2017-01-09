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
 * @file        text.c
 *
 */
/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "machine.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the machine text text type
typedef struct __vm86_text_t
{
    // the machine
    vm86_machine_ref_t      machine;

    // the procs
    tb_hash_map_ref_t       procs;

}vm86_text_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_void_t vm86_text_proc_exit(tb_element_ref_t func, tb_pointer_t buff)
{
    // check
    tb_assert_and_check_return(buff);

    // the proc
    vm86_proc_ref_t proc = *((vm86_proc_ref_t*)buff);
    tb_assert_and_check_return(proc);

    // exit it
    vm86_proc_exit(proc);
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
vm86_text_ref_t vm86_text_init(vm86_machine_ref_t machine)
{
    // check
    tb_assert_and_check_return_val(machine, tb_null);

    // done
    tb_bool_t       ok = tb_false;
    vm86_text_t*    text = tb_null;
    do
    {
        // make text
        text = tb_malloc0_type(vm86_text_t);
        tb_assert_and_check_break(text);

        // save machine
        text->machine = machine;

        // init procs
        text->procs = tb_hash_map_init(8, tb_element_str(tb_true), tb_element_ptr(vm86_text_proc_exit, tb_null));
        tb_assert_and_check_break(text->procs);

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (text) vm86_text_exit((vm86_text_ref_t)text);
        text = tb_null;
    }

    // ok?
    return (vm86_text_ref_t)text;
}
tb_void_t vm86_text_exit(vm86_text_ref_t self)
{
    // check
    vm86_text_t* text = (vm86_text_t*)self;
    tb_assert_and_check_return(text);

    // exit procs
    if (text->procs) tb_hash_map_exit(text->procs);
    text->procs = tb_null;

    // exit it
    tb_free(text);
}
vm86_proc_ref_t vm86_text_compile(vm86_text_ref_t self, tb_char_t const* code, tb_size_t size)
{
    // check
    vm86_text_t* text = (vm86_text_t*)self;
    tb_assert_and_check_return_val(text && text->machine && text->procs && code && size, tb_null);

    // done
    tb_bool_t       ok = tb_false;
    vm86_proc_ref_t proc = tb_null;
    do
    {
        // make proc
        proc = vm86_proc_init(text->machine, code, size);
        tb_assert_and_check_break(proc);

        // the proc name
        tb_char_t const* name = vm86_proc_name(proc);
        tb_assert_and_check_break(name);

        // save proc
        tb_hash_map_insert(text->procs, name, proc);

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (proc) vm86_proc_exit(proc);
        proc = tb_null;
    }

    // ok?
    return proc;
}
vm86_proc_ref_t vm86_text_proc(vm86_text_ref_t self, tb_char_t const* name)
{
    // check
    vm86_text_t* text = (vm86_text_t*)self;
    tb_assert_and_check_return_val(text && name, tb_null);

    // find proc
    return (vm86_proc_ref_t)tb_hash_map_get(text->procs, name);
}

