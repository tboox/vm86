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
 * @file        data.c
 *
 */
/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "machine.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the machine data chunk type
typedef struct __vm86_data_chunk_t
{
    // the offset
    tb_uint32_t                     offset;

    // the size
    tb_uint32_t                     size;

}vm86_data_chunk_t, *vm86_data_chunk_ref_t;

// the machine data data type
typedef struct __vm86_data_t
{
    // the data
    tb_byte_t*                      data;

    // the size
    tb_uint32_t                     size;

    // the base
    tb_uint32_t                     base;

    // the labels
    tb_hash_map_ref_t               labels;

}vm86_data_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
vm86_data_ref_t vm86_data_init(tb_size_t size)
{
    // check
    tb_assert_and_check_return_val(size, tb_null);

    // done
    tb_bool_t       ok = tb_false;
    vm86_data_t*    data = tb_null;
    do
    {
        // make data
        data = tb_malloc0_type(vm86_data_t);
        tb_assert_and_check_break(data);

        // make data
        data->data = tb_malloc0_bytes(size);
        tb_assert_and_check_break(data->data);

        // init labels
        data->labels = tb_hash_map_init(8, tb_element_str(tb_true), tb_element_mem(sizeof(vm86_data_chunk_t), tb_null, tb_null));
        tb_assert_and_check_break(data->labels);

        // init data
        data->size  = size;
        data->base  = 0;
    
        // ok
        ok = tb_true;

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (data) vm86_data_exit((vm86_data_ref_t)data);
        data = tb_null;
    }

    // ok?
    return (vm86_data_ref_t)data;
}
tb_void_t vm86_data_exit(vm86_data_ref_t self)
{
    // check
    vm86_data_t* data = (vm86_data_t*)self;
    tb_assert_and_check_return(data);

    // exit data
    if (data->data) tb_free(data->data);
    data->data = tb_null;

    // exit labels
    if (data->labels) tb_hash_map_exit(data->labels);
    data->labels = tb_null;

    // exit it
    tb_free(data);
}
tb_bool_t vm86_data_is(vm86_data_ref_t self, tb_char_t const* name)
{
    // check
    vm86_data_t* data = (vm86_data_t*)self;
    tb_assert_and_check_return_val(data && data->labels && name, tb_false);

    // is data?
    return tb_hash_map_find(data->labels, name) != tb_iterator_tail(data->labels);
}
tb_uint32_t vm86_data_get(vm86_data_ref_t self, tb_char_t const* name, tb_size_t* psize)
{
    // check
    vm86_data_t* data = (vm86_data_t*)self;
    tb_assert_and_check_return_val(data && data->labels && name, 0);

    // get the data chunk
    vm86_data_chunk_ref_t chunk = (vm86_data_chunk_ref_t)tb_hash_map_get(data->labels, name);
    tb_assert(chunk && chunk->offset < data->size);

    // save size
    if (psize) *psize = chunk->size;

    // the data offset
    return tb_p2u32(data->data + chunk->offset);
}
tb_void_t vm86_data_add(vm86_data_ref_t self, tb_char_t const* name, tb_byte_t const* buff, tb_size_t size)
{
    // check
    vm86_data_t* data = (vm86_data_t*)self;
    tb_assert_and_check_return(data && data->labels && name);

    // get the data chunk
    vm86_data_chunk_ref_t chunk = (vm86_data_chunk_ref_t)tb_hash_map_get(data->labels, name);
    if (chunk)
    {
        // must be the last chunk
        tb_assert(chunk->offset + chunk->size == data->base);

        // check
        tb_assert(data->base + size <= data->size);

        // add data
        if (buff && size) tb_memcpy(data->data + data->base, buff, size);

        // update size
        chunk->size += size;

        // update base
        data->base += size;
    }
    else
    {
        // check
        tb_assert(data->base + size <= data->size);

        // init chunk
        vm86_data_chunk_t item = {data->base, size};

        // add chunk
        tb_hash_map_insert(data->labels, name, &item);

        // add data
        if (buff && size) tb_memcpy(data->data + data->base, buff, size);

        // update base
        data->base += size;
    }
}
#ifdef __vm_debug__
tb_void_t vm86_data_dump(vm86_data_ref_t self)
{
    // check
    vm86_data_t* data = (vm86_data_t*)self;
    tb_assert_and_check_return(data && data->labels);

    // trace
    tb_trace_i("");
    tb_trace_i(".data");

    // done
    tb_for_all_if (tb_hash_map_item_t*, item, data->labels, item && item->data)
    {
        // the chunk
        vm86_data_chunk_ref_t chunk = (vm86_data_chunk_ref_t)item->data;
       
        // dump data
        tb_dump_data(data->data + chunk->offset, chunk->size);
 
        // trace
        tb_trace_i("data: name: %s, offset: %lu, size: %lu", item->name, chunk->offset, chunk->size);
    }
}
#endif
