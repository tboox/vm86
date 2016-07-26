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
 * @file        stack.c
 *
 */
/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "machine_stack"
#define TB_TRACE_MODULE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "machine.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the machine stack impl type
typedef struct __vm86_stack_impl_t
{
    // the data
    tb_uint32_t*                    data;

    // the size
    tb_size_t                       size;

    // the top
    tb_uint32_t**                   top;

}vm86_stack_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
vm86_stack_ref_t vm86_stack_init(tb_size_t size, tb_uint32_t* esp)
{
    // check
    tb_assert_and_check_return_val(size && esp, tb_null);

    // done
    tb_bool_t                   ok = tb_false;
    vm86_stack_impl_t*   impl = tb_null;
    do
    {
        // make stack
        impl = tb_malloc0_type(vm86_stack_impl_t);
        tb_assert_and_check_break(impl);

        // make data
        impl->data = tb_nalloc0_type(size, tb_uint32_t);
        tb_assert_and_check_break(impl->data);

        // init stack
        impl->size  = size;

        // init top
        impl->top   = (tb_uint32_t**)esp;
        *impl->top  = impl->data + size;
    
        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (impl) vm86_stack_exit((vm86_stack_ref_t)impl);
        impl = tb_null;
    }

    // ok?
    return (vm86_stack_ref_t)impl;
}
tb_void_t vm86_stack_exit(vm86_stack_ref_t stack)
{
    // check
    vm86_stack_impl_t* impl = (vm86_stack_impl_t*)stack;
    tb_assert_and_check_return(impl);

    // exit data
    if (impl->data) tb_free(impl->data);
    impl->data = tb_null;

    // exit it
    tb_free(impl);
}
tb_void_t vm86_stack_clear(vm86_stack_ref_t stack)
{
    // check
    vm86_stack_impl_t* impl = (vm86_stack_impl_t*)stack;
    tb_assert_and_check_return(impl && impl->top && impl->data && impl->size);

    // clear data
    tb_memset(impl->data, 0, impl->size * sizeof(tb_uint32_t));

    // reset it
    *impl->top = impl->data + impl->size;
}
tb_void_t vm86_stack_top(vm86_stack_ref_t stack, tb_uint32_t* pdata, tb_size_t index)
{
    // check
    vm86_stack_impl_t* impl = (vm86_stack_impl_t*)stack;
    tb_assert_and_check_return(impl && pdata);

    // check
    tb_assert(impl->top && *impl->top);
    tb_assert(*impl->top + index < impl->data + impl->size);

    // save data
    *pdata = (*impl->top)[index];
}
tb_void_t vm86_stack_push(vm86_stack_ref_t stack, tb_uint32_t data)
{
    // check
    vm86_stack_impl_t* impl = (vm86_stack_impl_t*)stack;
    tb_assert_and_check_return(impl);

    // check
    tb_assert(impl->top && *impl->top);
    tb_assert(*impl->top > impl->data);

    // push it
    *--(*impl->top) = data;
}
tb_void_t vm86_stack_pop(vm86_stack_ref_t stack, tb_uint32_t* pdata)
{
    // check
    vm86_stack_impl_t* impl = (vm86_stack_impl_t*)stack;
    tb_assert_and_check_return(impl);

    // check
    tb_assert(impl->top && *impl->top);
    tb_assert(*impl->top < impl->data + impl->size);

    // save data
    if (pdata) *pdata = **impl->top;

    // pop it
    (*impl->top)++;
}
#ifdef __vm_debug__
tb_void_t vm86_stack_dump(vm86_stack_ref_t stack)
{
    // check
    vm86_stack_impl_t* impl = (vm86_stack_impl_t*)stack;
    tb_assert_and_check_return(impl);

    // trace
    tb_trace_i("");
    tb_trace_i("stack: ");

    // done
    tb_uint32_t* top = (*impl->top);
    tb_uint32_t* end = impl->data + impl->size;
    while (top < end)
    {
        // trace
        tb_trace_i("%p: %#x", top, *top);

        // next
        top++;
    }
}
#endif
