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
 * @file        parser.c
 *
 */
/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "machine_parser"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "parser.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tb_long_t vm86_parser_comp_register(tb_iterator_ref_t iterator, tb_cpointer_t item, tb_cpointer_t name)
{
    // check
    tb_assert(item);

    // hacking for register 
    return tb_stricmp(*((tb_char_t const**)item), (tb_char_t const*)name);
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_bool_t vm86_parser_get_variable_name(tb_char_t const** pp, tb_char_t const* e, tb_char_t* name, tb_size_t maxn)
{
    // check
    tb_assert(pp && e && name && maxn);

    // done
    tb_bool_t           ok = tb_false;
    tb_char_t const*    p = *pp;
    do
    {
        // save base
        tb_char_t const* b = p;

        // check
        tb_check_break(p < e && (tb_isalpha(*p) || *p == '_'));
        p++;

        // get name
        while (p < e && (tb_isalpha(*p) || *p == '_' || tb_isdigit(*p))) p++;
        tb_check_break(p <= e && p - b < maxn);
        tb_memcpy(name, b, p - b);

        // end
        name[p - b] = '\0';

        // skip the space
        while (p < e && tb_isspace(*p)) p++;

        // ok
        ok = tb_true;

    } while (0);

    // update the code pointer if ok
    if (ok) *pp = p;

    // ok?
    return ok;
}
tb_bool_t vm86_parser_get_segment_name(tb_char_t const** pp, tb_char_t const* e, tb_char_t* name, tb_size_t maxn)
{
    // check
    tb_assert(pp && e && name && maxn);

    // done
    tb_bool_t           ok = tb_false;
    tb_char_t const*    p = *pp;
    do
    {
        // save base
        tb_char_t const* b = p;

        // get name
        while (p < e && tb_isalpha(*p)) p++;
        tb_check_break(p < e && p - b < maxn && *p == ':');
        tb_memcpy(name, b, p - b);

        // end
        name[p - b] = '\0';

        // skip ":"
        p++;

        // skip the space
        while (p < e && tb_isspace(*p)) p++;

        // ok
        ok = tb_true;

    } while (0);

    // update the code pointer if ok
    if (ok) *pp = p;

    // ok?
    return ok;
}
tb_bool_t vm86_parser_get_instruction_name(tb_char_t const** pp, tb_char_t const* e, tb_char_t* name, tb_size_t maxn)
{
    // check
    tb_assert(pp && e && name && maxn);

    // done
    tb_bool_t           ok = tb_false;
    tb_char_t const*    p = *pp;
    do
    {
        // save base
        tb_char_t const* b = p;

        // skip name
        while (p < e && tb_isalpha(*p)) p++;
        tb_check_break(p <= e && p - b < maxn);

        // not instruction name?
        if (p < e && !tb_isspace(*p)) break;

        // save name
        tb_memcpy(name, b, p - b);

        // end
        name[p - b] = '\0';

        // skip the space
        while (p < e && tb_isspace(*p)) p++;

        // ok
        ok = tb_true;

    } while (0);

    // update the code pointer if ok
    if (ok) *pp = p;

    // ok?
    return ok;
}
tb_bool_t vm86_parser_get_register(tb_char_t const** pp, tb_char_t const* e, tb_uint16_t* r)
{
    // check
    tb_assert(pp && e && r);

    // done
    tb_bool_t           ok = tb_false;
    tb_char_t const*    p = *pp;
    do
    {
        // save base
        tb_char_t const* b = p;

        // get instruction name
        tb_char_t name[64] = {0};
        while (p < e && tb_isalpha(*p)) p++;
        tb_check_break(p <= e && p - b < sizeof(name));
        tb_memcpy(name, b, p - b);

        // skip the space
        while (p < e && tb_isspace(*p)) p++;

        // the register entry type
        typedef struct __vm86_register_entry_t
        {
            // the register name
            tb_char_t const*        name;

            // the register index
            tb_uint8_t              index;

        }vm86_register_entry_t, *vm86_register_entry_ref_t;

        // the registers
        static vm86_register_entry_t s_registers[] =
        {
            { "ah",     VM86_REGISTER_EAX | VM86_REGISTER_AH }
        ,   { "al",     VM86_REGISTER_EAX | VM86_REGISTER_AL }
        ,   { "ax",     VM86_REGISTER_EAX | VM86_REGISTER_AX }
        ,   { "bh",     VM86_REGISTER_EBX | VM86_REGISTER_BH }
        ,   { "bl",     VM86_REGISTER_EBX | VM86_REGISTER_BL }
        ,   { "bx",     VM86_REGISTER_EBX | VM86_REGISTER_BX }
        ,   { "ch",     VM86_REGISTER_ECX | VM86_REGISTER_CH }
        ,   { "cl",     VM86_REGISTER_ECX | VM86_REGISTER_CL }
        ,   { "cx",     VM86_REGISTER_ECX | VM86_REGISTER_CX }
        ,   { "dh",     VM86_REGISTER_EDX | VM86_REGISTER_DH }
        ,   { "dl",     VM86_REGISTER_EDX | VM86_REGISTER_DL }
        ,   { "dx",     VM86_REGISTER_EDX | VM86_REGISTER_DX }
        ,   { "eax",    VM86_REGISTER_EAX }
        ,   { "ebp",    VM86_REGISTER_EBP }
        ,   { "ebx",    VM86_REGISTER_EBX }
        ,   { "ecx",    VM86_REGISTER_ECX }
        ,   { "edi",    VM86_REGISTER_EDI }
        ,   { "edx",    VM86_REGISTER_EDX }
        ,   { "esi",    VM86_REGISTER_ESI }
        ,   { "esp",    VM86_REGISTER_ESP }
        };

        // init iterator
        tb_array_iterator_t array_iterator;
        tb_iterator_ref_t   iterator = tb_iterator_make_for_mem(&array_iterator, s_registers, tb_arrayn(s_registers), sizeof(vm86_register_entry_t));

        // find register by the binary search
        tb_size_t itor = tb_binary_find_all_if(iterator, vm86_parser_comp_register, name);
        tb_check_break(itor != tb_iterator_tail(iterator));

        // get the register
        vm86_register_entry_ref_t entry = (vm86_register_entry_ref_t)tb_iterator_item(iterator, itor);
        tb_assert_and_check_break(entry && (entry->index & VM86_REGISTER_MASK) < VM86_REGISTER_MAXN);

        // save register
        *r = entry->index;

        // trace
        tb_trace_d("register: %s: %x", name, entry->index);

        // ok 
        ok = tb_true;

    } while (0);

    // update the code pointer if ok
    if (ok) *pp = p;

    // ok?
    return ok;
}
tb_bool_t vm86_parser_get_value(tb_char_t const** pp, tb_char_t const* e, tb_uint32_t* value, tb_hash_map_ref_t proc_locals, tb_hash_map_ref_t proc_labels, vm86_data_ref_t data)
{
    // check
    tb_assert(pp && e && value && proc_labels);

    // try to get the number value first
    if (vm86_parser_get_number_value(pp, e, value)) return tb_true;

    // try to get the local value again
    if (vm86_parser_get_local_value(pp, e, value, proc_locals)) return tb_true;

    // get the offset value last
    return vm86_parser_get_offset_value(pp, e, value, proc_labels, data);
}
tb_bool_t vm86_parser_get_number_value(tb_char_t const** pp, tb_char_t const* e, tb_uint32_t* value)
{ 
    // check
    tb_assert(pp && e && value);

    // done
    tb_bool_t           ok = tb_false;
    tb_char_t const*    p = *pp;
    do
    {
        // check
        tb_check_break(p < e && (tb_isdigit16(*p) || *p == '-'));

        // is sign?
        tb_bool_t is_sign = tb_false;
        if (*p == '-')
        {
            is_sign = tb_true;
            p++;
        }

        // save the base
        tb_char_t const* b = p;

        // skip the value
        while (p < e && tb_isdigit16(*p)) p++;
        
        // is hex?
        tb_bool_t is_hex = tb_false;
        if (p < e && *p == 'h') 
        {
            is_hex = tb_true;
            p++;
        }

        // check end, exclude variable name
        tb_check_break(!tb_isalpha(*p) && *p != '_');

        // save value
        *value = is_hex? tb_s16tou32(b) : tb_s10tou32(b);
        if (is_sign) *value = (tb_uint32_t)(((tb_sint32_t)*value) * -1);

        // skip the space
        while (p < e && tb_isspace(*p)) p++;

        // ok
        ok = tb_true;

    } while (0);

    // update the code pointer if ok
    if (ok) *pp = p;

    // ok?
    return ok;
}
tb_bool_t vm86_parser_get_local_value(tb_char_t const** pp, tb_char_t const* e, tb_uint32_t* value, tb_hash_map_ref_t proc_locals)
{
    // check
    tb_assert(pp && e && value && proc_locals);

    // done
    tb_bool_t           ok = tb_false;
    tb_char_t const*    p = *pp;
    do
    {
        // get instruction name
        tb_char_t name[256] = {0};
        if (!vm86_parser_get_variable_name(&p, e, name, sizeof(name))) break;

        // get value
        if (tb_hash_map_find(proc_locals, name) != tb_iterator_tail(proc_locals))
            *value = (tb_uint32_t)tb_hash_map_get(proc_locals, name);
        else break;

        // trace
        tb_trace_d("local: %s: %u", name, *value);

        // ok
        ok = tb_true;

    } while (0);

    // update the code pointer if ok
    if (ok) *pp = p;

    // ok?
    return ok;
}
tb_bool_t vm86_parser_get_offset_value(tb_char_t const** pp, tb_char_t const* e, tb_uint32_t* value, tb_hash_map_ref_t proc_labels, vm86_data_ref_t data)
{
    // check
    tb_assert(pp && e && value && proc_labels);

    // done
    tb_bool_t           ok = tb_false;
    tb_char_t const*    p = *pp;
    do
    {
        // attempt to get segment name
        tb_char_t segment[16] = {0};
        tb_bool_t has_segment = vm86_parser_get_segment_name(&p, e, segment, sizeof(segment));

        // skip "short ..."
        if (p + 6 < e && !tb_strnicmp(p, "short ", 6)) p += 6;

        // skip the space
        while (p < e && tb_isspace(*p)) p++;

        // get instruction name
        tb_char_t name[256] = {0};
        if (!vm86_parser_get_variable_name(&p, e, name, sizeof(name))) break;

        // is .data segment?
        if (has_segment && !tb_stricmp(segment, "ds"))
            *value = vm86_data_get(data, name, tb_null);
        // is .code segment?
        else if (has_segment && !tb_stricmp(segment, "cs"))
            *value = (tb_uint32_t)tb_hash_map_get(proc_labels, name);
        else
        {
            // get value
            if (tb_hash_map_find(proc_labels, name) != tb_iterator_tail(proc_labels))
                *value = (tb_uint32_t)tb_hash_map_get(proc_labels, name);
            else if (vm86_data_is(data, name)) 
                *value = vm86_data_get(data, name, tb_null);
            else break;
        }

        // check
        tb_assert(*value < TB_MAXU32);

        // trace
        tb_trace_d("offset: %s: %x", name, *value);

        // ok
        ok = tb_true;

    } while (0);

    // update the code pointer if ok
    if (ok) *pp = p;

    // ok?
    return ok;
}
