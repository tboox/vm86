/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "emulator.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the emulator impl type
typedef struct __vm86_impl_t
{
    // the text
    vm86_text_ref_t          text;

    // the data
    vm86_data_ref_t          data;

    // the stack
    vm86_stack_ref_t         stack;

    // the registers
    vm86_registers_t         registers;

    // the functions
    tb_hash_map_ref_t                   functions;

    // the lock
    tb_spinlock_t                   lock;

}vm86_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_handle_t vm86_instance_init(tb_cpointer_t* ppriv)
{
    // init it
#ifdef __vm_small__
    return vm86_init(1024, 1024);
#else
    return vm86_init(2048, 2048);
#endif
}
static tb_void_t vm86_instance_exit(tb_handle_t emulator, tb_cpointer_t priv)
{
    // exit it
    vm86_exit((vm86_ref_t)emulator);
}
/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
vm86_ref_t vm86()
{
    return (vm86_ref_t)tb_singleton_instance(TB_SINGLETON_TYPE_USER + 5, vm86_instance_init, vm86_instance_exit, tb_null, tb_null);
}
vm86_ref_t vm86_init(tb_size_t data_size, tb_size_t stack_size)
{
    // check
    tb_assert_and_check_return_val(data_size && stack_size, tb_null);

    // done
    tb_bool_t               ok = tb_false;
    vm86_impl_t*     impl = tb_null;
    do
    {
        // check
        tb_assert_static(sizeof(tb_pointer_t) <= sizeof(tb_uint32_t));

        // make emulator
        impl = tb_malloc0_type(vm86_impl_t);
        tb_assert_and_check_break(impl);

        // init lock
        if (!tb_spinlock_init(&impl->lock)) break;

        // init registers
        vm86_registers_clear(impl->registers);

        // make stack
        impl->stack = vm86_stack_init(stack_size, &impl->registers[VM86_REGISTER_ESP].u32);
        tb_assert_and_check_break(impl->stack);

        // make data
        impl->data = vm86_data_init(data_size);
        tb_assert_and_check_break(impl->data);

        // make text
        impl->text = vm86_text_init((vm86_ref_t)impl);
        tb_assert_and_check_break(impl->text);

        // make functions
        impl->functions = tb_hash_map_init(8, tb_element_str(tb_true), tb_element_ptr(tb_null, tb_null));
        tb_assert_and_check_break(impl->functions);

        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (impl) vm86_exit((vm86_ref_t)impl);
        impl = tb_null;
    }

    // ok?
    return (vm86_ref_t)impl;
}
tb_void_t vm86_exit(vm86_ref_t emulator)
{
    // check
    vm86_impl_t* impl = (vm86_impl_t*)emulator;
    tb_assert_and_check_return(impl);

    // enter
    tb_spinlock_enter(&impl->lock);

    // exit text
    if (impl->text) vm86_text_exit(impl->text);
    impl->text = tb_null;

    // exit data
    if (impl->data) vm86_data_exit(impl->data);
    impl->data = tb_null;

    // exit stack
    if (impl->stack) vm86_stack_exit(impl->stack);
    impl->stack = tb_null;

    // exit functions
    if (impl->functions) tb_hash_map_exit(impl->functions);
    impl->functions = tb_null;

    // leave
    tb_spinlock_leave(&impl->lock);

    // exit lock
    tb_spinlock_exit(&impl->lock);

    // exit it
    tb_free(impl);
}
tb_spinlock_ref_t vm86_lock(vm86_ref_t emulator)
{
    // check
    vm86_impl_t* impl = (vm86_impl_t*)emulator;
    tb_assert_and_check_return_val(impl, tb_null);

    // the lock
    return &impl->lock;
}
vm86_data_ref_t vm86_data(vm86_ref_t emulator)
{
    // check
    vm86_impl_t* impl = (vm86_impl_t*)emulator;
    tb_assert_and_check_return_val(impl, tb_null);

    // the data
    return impl->data;
}
vm86_text_ref_t vm86_text(vm86_ref_t emulator)
{
    // check
    vm86_impl_t* impl = (vm86_impl_t*)emulator;
    tb_assert_and_check_return_val(impl, tb_null);

    // the text
    return impl->text;
}
vm86_stack_ref_t vm86_stack(vm86_ref_t emulator)
{
    // check
    vm86_impl_t* impl = (vm86_impl_t*)emulator;
    tb_assert_and_check_return_val(impl, tb_null);

    // the stack
    return impl->stack;
}
vm86_registers_ref_t vm86_registers(vm86_ref_t emulator)
{
    // check
    vm86_impl_t* impl = (vm86_impl_t*)emulator;
    tb_assert_and_check_return_val(impl, tb_null);

    // the registers
    return impl->registers;
}
vm86_func_t vm86_function(vm86_ref_t emulator, tb_char_t const* name)
{
    // check
    vm86_impl_t* impl = (vm86_impl_t*)emulator;
    tb_assert_and_check_return_val(impl && name, tb_null);

    // the function
    return (vm86_func_t)tb_hash_map_get(impl->functions, name);
}
tb_void_t vm86_function_set(vm86_ref_t emulator, tb_char_t const* name, vm86_func_t func)
{
    // check
    vm86_impl_t* impl = (vm86_impl_t*)emulator;
    tb_assert_and_check_return(impl && name);

    // set the function
    if (func) tb_hash_map_insert(impl->functions, name, func);
    else tb_hash_map_remove(impl->functions, name);
}

