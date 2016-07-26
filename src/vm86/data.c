/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "emulator.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the emulator data chunk type
typedef struct __vm86_data_chunk_t
{
    // the offset
    tb_uint32_t                     offset;

    // the size
    tb_uint32_t                     size;

}vm86_data_chunk_t, *vm86_data_chunk_ref_t;

// the emulator data impl type
typedef struct __vm86_data_impl_t
{
    // the data
    tb_byte_t*                      data;

    // the size
    tb_uint32_t                     size;

    // the base
    tb_uint32_t                     base;

    // the labels
    tb_hash_map_ref_t                   labels;

}vm86_data_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
vm86_data_ref_t vm86_data_init(tb_size_t size)
{
    // check
    tb_assert_and_check_return_val(size, tb_null);

    // done
    tb_bool_t                   ok = tb_false;
    vm86_data_impl_t*    impl = tb_null;
    do
    {
        // make data
        impl = tb_malloc0_type(vm86_data_impl_t);
        tb_assert_and_check_break(impl);

        // make data
        impl->data = tb_malloc0_bytes(size);
        tb_assert_and_check_break(impl->data);

        // init labels
        impl->labels = tb_hash_map_init(8, tb_element_str(tb_true), tb_element_mem(sizeof(vm86_data_chunk_t), tb_null, tb_null));
        tb_assert_and_check_break(impl->labels);

        // init data
        impl->size  = size;
        impl->base  = 0;
    
        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (impl) vm86_data_exit((vm86_data_ref_t)impl);
        impl = tb_null;
    }

    // ok?
    return (vm86_data_ref_t)impl;
}
tb_void_t vm86_data_exit(vm86_data_ref_t data)
{
    // check
    vm86_data_impl_t* impl = (vm86_data_impl_t*)data;
    tb_assert_and_check_return(impl);

    // exit data
    if (impl->data) tb_free(impl->data);
    impl->data = tb_null;

    // exit labels
    if (impl->labels) tb_hash_map_exit(impl->labels);
    impl->labels = tb_null;

    // exit it
    tb_free(impl);
}
tb_bool_t vm86_data_is(vm86_data_ref_t data, tb_char_t const* name)
{
    // check
    vm86_data_impl_t* impl = (vm86_data_impl_t*)data;
    tb_assert_and_check_return_val(impl && impl->labels && name, tb_false);

    // is data?
    return tb_hash_map_find(impl->labels, name) != tb_iterator_tail(impl->labels);
}
tb_uint32_t vm86_data_get(vm86_data_ref_t data, tb_char_t const* name, tb_size_t* psize)
{
    // check
    vm86_data_impl_t* impl = (vm86_data_impl_t*)data;
    tb_assert_and_check_return_val(impl && impl->labels && name, 0);

    // get the data chunk
    vm86_data_chunk_ref_t chunk = (vm86_data_chunk_ref_t)tb_hash_map_get(impl->labels, name);
    tb_assert(chunk && chunk->offset < impl->size);

    // save size
    if (psize) *psize = chunk->size;

    // the data offset
    return tb_p2u32(impl->data + chunk->offset);
}
tb_void_t vm86_data_add(vm86_data_ref_t data, tb_char_t const* name, tb_byte_t const* buff, tb_size_t size)
{
    // check
    vm86_data_impl_t* impl = (vm86_data_impl_t*)data;
    tb_assert_and_check_return(impl && impl->labels && name);

    // get the data chunk
    vm86_data_chunk_ref_t chunk = (vm86_data_chunk_ref_t)tb_hash_map_get(impl->labels, name);
    if (chunk)
    {
        // must be the last chunk
        tb_assert(chunk->offset + chunk->size == impl->base);

        // check
        tb_assert(impl->base + size <= impl->size);

        // add data
        if (buff && size) tb_memcpy(impl->data + impl->base, buff, size);

        // update size
        chunk->size += size;

        // update base
        impl->base += size;
    }
    else
    {
        // check
        tb_assert(impl->base + size <= impl->size);

        // init chunk
        vm86_data_chunk_t item = {impl->base, size};

        // add chunk
        tb_hash_map_insert(impl->labels, name, &item);

        // add data
        if (buff && size) tb_memcpy(impl->data + impl->base, buff, size);

        // update base
        impl->base += size;
    }
}
#ifdef __vm_debug__
tb_void_t vm86_data_dump(vm86_data_ref_t data)
{
    // check
    vm86_data_impl_t* impl = (vm86_data_impl_t*)data;
    tb_assert_and_check_return(impl && impl->labels);

    // trace
    tb_trace_i("");
    tb_trace_i(".data");

    // done
    tb_for_all_if (tb_hash_map_item_t*, item, impl->labels, item && item->data)
    {
        // the chunk
        vm86_data_chunk_ref_t chunk = (vm86_data_chunk_ref_t)item->data;
       
        // dump data
        tb_dump_data(impl->data + chunk->offset, chunk->size);
 
        // trace
        tb_trace_i("data: name: %s, offset: %lu, size: %lu", item->name, chunk->offset, chunk->size);
    }
}
#endif
