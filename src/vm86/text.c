/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "emulator.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the emulator text impl type
typedef struct __vm86_text_impl_t
{
    // the emulator
    vm86_ref_t       emulator;

    // the procs
    tb_hash_map_ref_t           procs;

}vm86_text_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_void_t vm86_text_proc_exit(tb_element_t* func, tb_pointer_t buff)
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
vm86_text_ref_t vm86_text_init(vm86_ref_t emulator)
{
    // check
    tb_assert_and_check_return_val(emulator, tb_null);

    // done
    tb_bool_t                   ok = tb_false;
    vm86_text_impl_t*    impl = tb_null;
    do
    {
        // make text
        impl = tb_malloc0_type(vm86_text_impl_t);
        tb_assert_and_check_break(impl);

        // save emulator
        impl->emulator = emulator;

        // init procs
        impl->procs = tb_hash_map_init(8, tb_element_str(tb_true), tb_element_ptr(vm86_text_proc_exit, tb_null));
        tb_assert_and_check_break(impl->procs);

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (impl) vm86_text_exit((vm86_text_ref_t)impl);
        impl = tb_null;
    }

    // ok?
    return (vm86_text_ref_t)impl;
}
tb_void_t vm86_text_exit(vm86_text_ref_t text)
{
    // check
    vm86_text_impl_t* impl = (vm86_text_impl_t*)text;
    tb_assert_and_check_return(impl);

    // exit procs
    if (impl->procs) tb_hash_map_exit(impl->procs);
    impl->procs = tb_null;

    // exit it
    tb_free(impl);
}
vm86_proc_ref_t vm86_text_compile(vm86_text_ref_t text, tb_char_t const* code, tb_size_t size)
{
    // check
    vm86_text_impl_t* impl = (vm86_text_impl_t*)text;
    tb_assert_and_check_return_val(impl && impl->emulator && impl->procs && code && size, tb_null);

    // done
    tb_bool_t                   ok = tb_false;
    vm86_proc_ref_t      proc = tb_null;
    do
    {
        // make proc
        proc = vm86_proc_init(impl->emulator, code, size);
        tb_assert_and_check_break(proc);

        // the proc name
        tb_char_t const* name = vm86_proc_name(proc);
        tb_assert_and_check_break(name);

        // save proc
        tb_hash_map_insert(impl->procs, name, proc);

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
vm86_proc_ref_t vm86_text_proc(vm86_text_ref_t text, tb_char_t const* name)
{
    // check
    vm86_text_impl_t* impl = (vm86_text_impl_t*)text;
    tb_assert_and_check_return_val(impl && name, tb_null);

    // find proc
    return (vm86_proc_ref_t)tb_hash_map_get(impl->procs, name);
}

