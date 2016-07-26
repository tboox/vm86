/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TB_TRACE_MODULE_NAME            "emulator_proc"
#define TB_TRACE_MODULE_DEBUG           (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "emulator.h"
#include "instruction.h"
#include "parser.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the emulator proc impl type
typedef struct __vm86_proc_impl_t
{
    // the name
    tb_char_t*                          name;

    // the labels
    tb_hash_map_ref_t                       labels;

    // the locals
    tb_hash_map_ref_t                       locals;

    // the emulator
    vm86_ref_t                   emulator;

    // the instructions
    vm86_instruction_ref_t       instructions;

    // the instruction count
    tb_size_t                           instructions_count;

    // the last data name
    tb_char_t                           last_data_name[8192];

}vm86_proc_impl_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * compiler implementation
 */
static tb_char_t const* vm86_proc_compiler_find_name(vm86_proc_impl_t* impl, tb_char_t const* p, tb_char_t const* e)
{
    // save the start position
    tb_char_t const* b = p;
    do
    {
        // done
        p = tb_strnistr(p, e - p, "proc");
        if (p)
        {
            // save the proc position
            tb_char_t const* proc = p;

            // find the near
            p += 4;
            p = tb_strnistr(p, e - p, "near");
            if (p)
            {
                // seek the name last
                tb_char_t const* t = proc - 1;
                while (t >= b && tb_isspace(*t)) t--;
                if (t < b) 
                {
                    // failed
                    p = tb_null;
                    break;
                }
                t++;

                // seek the name head
                tb_char_t const* h = t - 1;
                while (h >= b && !tb_isspace(*h)) h--;
                if (h < b) h = b;
                else h++;

                // save name
                impl->name = tb_strndup(h, t - h);
                
                // trace
                tb_trace_d("proc: %s", impl->name);

                // ok
                p += 4;
                break;
            }
        }

        // failed
        p = tb_null;

    } while (0);

    // ok?
    return p;
}
static tb_char_t const* vm86_proc_compiler_read_line(tb_char_t const* p, tb_char_t const* e, tb_char_t* line, tb_size_t* size)
{
    // done
    tb_char_t   ch = 0;
    tb_char_t*  q = line;
    tb_bool_t   is_space = tb_true;
    tb_bool_t   is_comment = tb_false;
    tb_size_t   maxn = *size;
    while (p < e)
    {
        // read char
        ch = *p++;

        // is line?
        if (ch == '\n') 
        {
            // finish line
            if (q > line && q[-1] == '\r')
                q--;
            *q = '\0';

            // save size
            *size = q - line;
    
            // ok
            return p;
        }
        // is comment?
        else if (ch == ';') is_comment = tb_true;
        // append char to line
        else if (!is_comment)
        {
            // exists non-space character
            if (!tb_isspace(ch)) is_space = tb_false;

            // append it
            if ((q - line) < maxn - 1 && (!tb_isspace(ch) || !is_space)) *q++ = ch;

            // no data?
            if (!ch) break;
        }
    }

    // end
    *q = '\0';

    // save size
    *size = q - line;
    
    // ok?
    return p;
}
static tb_char_t const* vm86_proc_compiler_read_data(vm86_proc_impl_t* impl, tb_char_t const* p, tb_char_t const* e, tb_byte_t* data, tb_size_t* size)
{
    // check
    tb_assert_and_check_return_val(impl && impl->labels, tb_null);

    // done
    tb_bool_t   ok = tb_false;
    tb_byte_t*  qb = data;
    tb_byte_t*  qe = data + *size;
    do
    {
        // skip the space
        while (p < e && tb_isspace(*p)) p++;

        // save base
        tb_char_t const* b = p;

        // init type
        tb_char_t type[16] = {0};
        while (p < e && !tb_isspace(*p)) p++;
        tb_check_break(p < e && p - b < sizeof(type));
        tb_memcpy(type, b, p - b);

        // trace
        tb_trace_d("type: %s", type);

        // byte?
        if (!tb_stricmp(type, "db"))
        {
            // done
            tb_char_t ch;
            tb_bool_t in_str = tb_false;
            while (p < e && qb < qe)
            {
                // the character
                ch = *p;

                // in string?
                if (!in_str)
                {
                    // space or ,? skip it
                    if (tb_isspace(ch) || ch == ',') p++;
                    // string?
                    else if (ch == '\"' || ch == '\'')
                    {
                        // enter string
                        in_str = tb_true;
                        p++;
                    }
                    // value?
                    else if (tb_isdigit16(ch))
                    {
                        // get value
                        tb_uint32_t value = 0;
                        if (!vm86_parser_get_number_value(&p, e, &value)) break;
                        tb_assert(value >= TB_MINU8 && value <= TB_MAXU8);

                        // append data
                        *qb++ = (tb_byte_t)value;
                    }
                    // comment? end it
                    else if (ch == ';') p = e;
                    // invalid character
                    else 
                    {
                        // trace
                        tb_trace_e("invalid character: %c", ch);
                        tb_assert(0);
                    }
                }
                else
                {
                    // leave?
                    if (ch == '\"' || ch == '\'')
                    {
                        // leave string
                        in_str = tb_false;
                        p++;
                    }
                    // append data
                    else *qb++ = *p++;
                }
            }
        }
        // dword?
        else if (!tb_stricmp(type, "dd"))
        {
            // done
            tb_char_t ch;
            while (p < e && qb < qe)
            {
                // the character
                ch = *p;

                // space or ,? skip it
                if (tb_isspace(ch) || ch == ',') p++;
                // offset?
                else if (p + 6 < e && !tb_strnicmp(p, "offset", 6))
                {
                    // seek to the name
                    p += 6;
                    while (p < e && tb_isspace(*p)) p++;

                    // get the offset value
                    tb_uint32_t value = 0;
                    if (!vm86_parser_get_offset_value(&p, e, &value, impl->labels, vm86_data(impl->emulator))) break;

                    // append data
                    tb_bits_set_u32_ne(qb, value);
                    qb += 4;
                }
                // value?
                else if (tb_isdigit16(ch))
                {
                    // get value
                    tb_uint32_t value = 0;
                    if (!vm86_parser_get_number_value(&p, e, &value)) break;
                    tb_assert(value >= TB_MINU32 && value <= TB_MAXU32);

                    // append data
                    tb_bits_set_u32_ne(qb, value);
                    qb += 4;
                }
                // comment? end it
                else if (ch == ';') p = e;
                // invalid character
                else 
                {
                    // trace
                    tb_trace_e("invalid character: %c", ch);
                    tb_assert(0);
                }
            }
        }
        // word?
        else if (!tb_stricmp(type, "dw"))
        { 
            // TODO
            tb_assert(0);
        }
        else
        {
            // TODO
            tb_assert(0);
        }

        // ok
        ok = tb_true;

    } while (0);

    // ok?
    if (ok) 
    {
        // save size
        *size = qb - data;
    }
    else p = tb_null;

    // ok?
    return p;
}
static tb_bool_t vm86_proc_compiler_prepare_locals(vm86_proc_impl_t* impl, tb_char_t const* p, tb_char_t const* e)
{
    // check
    tb_assert_and_check_return_val(impl && impl->locals, tb_false);

    // done
    tb_bool_t ok = tb_false;
    do
    {
        // save base
        tb_char_t const* b = p;

        // init name
        tb_char_t name[8192] = {0};
        if (!vm86_parser_get_variable_name(&p, e, name, sizeof(name))) break;

        // find '='
        while (p < e && *p != '=') p++;
        tb_check_break(p < e && *p == '=');
        p++;

        // skip space
        while (p < e && tb_isspace(*p)) p++;

        // init type
        tb_char_t type[8192] = {0};
        for (b = p; p < e && !tb_isspace(*p); p++);
        tb_check_break(p < e && p - b < sizeof(type));
        tb_memcpy(type, b, p - b);

        // check type
        if (    tb_stricmp(type, "dword")
            &&  tb_stricmp(type, "word")
            &&  tb_stricmp(type, "byte")
            &&  tb_stricmp(type, "qword"))
        {
            break;
        }

        // seek to the value
        while (p < e && !tb_isdigit16(*p) && *p != '-') p++;

        // init value
        tb_uint32_t value = 0;
        if (!vm86_parser_get_number_value(&p, e, &value)) break;

        // save local
        tb_hash_map_insert(impl->locals, name, (tb_pointer_t)value);

        // trace
        tb_trace_d("local: %s, type: %s, value: %d", name, type, value);

        // ok
        ok = tb_true;

    } while (0);

    // ok?
    return ok;
}
static tb_bool_t vm86_proc_compiler_prepare_label(vm86_proc_impl_t* impl, tb_char_t const* p, tb_char_t const* e, tb_size_t offset)
{
    // check
    tb_assert_and_check_return_val(impl && impl->labels, tb_false);

    // done
    tb_bool_t ok = tb_false;
    do
    {
        // save base
        tb_char_t const* b = p;

        // init name
        tb_char_t name[8192] = {0};
        while (p < e && !tb_isspace(*p) && *p != ':') p++;
        tb_check_break(p < e && p - b < sizeof(name) && *p == ':');
        tb_memcpy(name, b, p - b);

        // save offset
        tb_hash_map_insert(impl->labels, name, (tb_cpointer_t)offset);

        // trace
        tb_trace_d("label: %s => %lu", name, offset);

        // ok
        ok = tb_true;

    } while (0);

    // ok?
    return ok;
}
static tb_char_t const* vm86_proc_compiler_prepare(vm86_proc_impl_t* impl, tb_char_t const* p, tb_char_t const* e, tb_size_t* instructions_count)
{
    // trace
    tb_trace_d("");
    tb_trace_d("prepare: ..");

    // done
    tb_char_t           line[8192];
    tb_size_t           count = 0;
    tb_bool_t           is_data = tb_false;
    tb_char_t const*    start = tb_null;
    while (p < e)
    {
        // save the base
        tb_char_t const* b = p;

        // read line
        tb_size_t size = sizeof(line);
        p = vm86_proc_compiler_read_line(p, e, line, &size);

        // check
        tb_check_continue(size);
        tb_assert(size < sizeof(line));

        // attempt to prepare locals
        if (vm86_proc_compiler_prepare_locals(impl, line, line + size)) continue ;

        // save the start position
        if (!start) start = b;

        // is data or code segment?
        if (!tb_strnicmp(line, ".data", 5))
        {
            is_data = tb_true;
            continue ;
        }
        else if (!tb_strnicmp(line, ".code", 5))
        {
            is_data = tb_false;
            continue ;
        }

        // is data?
        tb_check_continue(!is_data);

        // attempt to prepare label
        if (vm86_proc_compiler_prepare_label(impl, line, line + size, count)) continue ;

        // is end?
        if (tb_strnistr(line, size, "endp")) break;

        // trace
//        tb_trace_d("line: %s", line);

        // update the instructions count
        count++;
    }

    // trace
    tb_trace_d("prepare: count: %lu", count);

    // save the instructions count
    *instructions_count = count;

    // ok?
    return start;
}
static tb_bool_t vm86_proc_compiler_compile_skip_label(vm86_proc_impl_t* impl, tb_char_t const* p, tb_char_t const* e)
{
    // check
    tb_assert_and_check_return_val(impl && impl->labels, tb_false);

    // done
    tb_bool_t ok = tb_false;
    do
    {
        // init name
        while (p < e && !tb_isspace(*p) && *p != ':') p++;
        tb_check_break(p < e && *p == ':');

        // ok
        ok = tb_true;

    } while (0);

    // ok?
    return ok;
}
static tb_bool_t vm86_proc_compiler_compile_data(vm86_proc_impl_t* impl, tb_char_t const* p, tb_char_t const* e)
{
    // check
    tb_assert_and_check_return_val(impl && impl->emulator, tb_false);

    // the data
    vm86_data_ref_t data = vm86_data(impl->emulator);
    tb_assert_and_check_return_val(data, tb_false);

    // done
    tb_bool_t ok = tb_false;
    do
    {
        // save base
        tb_char_t const* b = p;

        // init name
        tb_char_t name[8192] = {0};
        while (p < e && !tb_isspace(*p)) p++;
        tb_check_break(p < e && p - b < sizeof(name));
        tb_memcpy(name, b, p - b);

        // only data? restore the pointer offset
        tb_bool_t only_data = tb_false;
        if (!tb_stricmp(name, "dd") || !tb_stricmp(name, "dw") || !tb_stricmp(name, "db"))
        {
            p -= 2;
            only_data = tb_true;
        }

        // done
        tb_byte_t   buff[8192];
        tb_size_t   maxn = sizeof(buff);
        tb_byte_t*  base = buff;
        while (p < e)
        {
            // read data
            tb_size_t read = maxn;
            p = vm86_proc_compiler_read_data(impl, p, e, base, &read);
            tb_assert(p && read);

            // update the buffer
            maxn -= read;
            base += read;
        }

        // check
        tb_assert(base > buff);

        // only data? append to the last data
        if (only_data)
        {
            // add data
            vm86_data_add(data, impl->last_data_name, buff, base - buff);
        }
        // exists name? add new data
        else 
        {
            // add data
            vm86_data_add(data, name, buff, base - buff);

            // save the last data name
            tb_strlcpy(impl->last_data_name, name, sizeof(impl->last_data_name));
        }

        // ok
        ok = tb_true;

    } while (0);

    // ok?
    return ok;
}
static tb_bool_t vm86_proc_compiler_compile_code(vm86_proc_impl_t* impl, tb_char_t const* p, tb_char_t const* e, vm86_instruction_ref_t instruction)
{
    // check
    tb_assert_and_check_return_val(impl, tb_false);

    // compile this instruction
    return vm86_instruction_compile(instruction, p, e - p, impl->emulator, impl->labels, impl->locals);
}
static tb_size_t vm86_proc_compiler_compile_done(vm86_proc_impl_t* impl, tb_char_t const* p, tb_char_t const* e)
{
    // check
    tb_assert_and_check_return_val(impl, 0);

    // trace
    tb_trace_d("");
    tb_trace_d("compile: ..");

    // done
    tb_char_t line[8192];
    tb_size_t count = 0;
    tb_bool_t is_data = tb_false;
    while (p < e)
    {
        // read line
        tb_size_t size = sizeof(line);
        p = vm86_proc_compiler_read_line(p, e, line, &size);

        // check
        tb_check_continue(size);
        tb_assert(size < sizeof(line));

        // is data or code segment?
        if (!tb_strnicmp(line, ".data", 5))
        {
            // switch to the data segment
            is_data = tb_true;
            continue ;
        }
        else if (!tb_strnicmp(line, ".code", 5))
        {
            // dump data
#ifdef __vm_debug__
            if (is_data) vm86_data_dump(vm86_data(impl->emulator));
#endif

            // switch to the code segment
            is_data = tb_false;
            continue ;
        }

        // is data?
        if (is_data)
        {
            // compile data
            if (!vm86_proc_compiler_compile_data(impl, line, line + size)) break ;
        }
        // is code?
        else
        {
            // skip the label
            if (vm86_proc_compiler_compile_skip_label(impl, line, line + size)) continue ;

            // is end?
            if (tb_strnistr(line, size, "endp")) break;

            // trace
            tb_trace_d("");
            tb_trace_d("line: %s", line);

            // compile code
            if (!vm86_proc_compiler_compile_code(impl, line, line + size, &impl->instructions[count])) break ;

            // update the instructions count
            count++;
        }
    }

    // trace
    tb_trace_d("compile: count: %lu", count);

    // ok?
    return count;
}
static tb_bool_t vm86_proc_compile(vm86_proc_impl_t* impl, tb_char_t const* code, tb_size_t size)
{
    // trace
    tb_trace_d("=====================================================================");

    // done
    tb_bool_t           ok = tb_false;
    tb_char_t const*    p = code;
    tb_char_t const*    e = code + size;
    do
    {
        // find the proc name
        p = vm86_proc_compiler_find_name(impl, p, e);
        tb_assert_and_check_break(p && impl->name);

        // prepare some data and labels first before compiling code
        p = vm86_proc_compiler_prepare(impl, p, e, &impl->instructions_count);
        tb_assert_and_check_break(p < e && p && impl->instructions_count);

        // make instructions
        impl->instructions = tb_nalloc0_type(impl->instructions_count, vm86_instruction_t);
        tb_assert_and_check_break(impl->instructions);

        // convert the labels offset to the instructions address
        tb_for_all_if (tb_hash_map_item_t*, item, impl->labels, item)
        {
            // check
            tb_assert((tb_uint32_t)item->data < impl->instructions_count);

            // the offset
            tb_uint32_t offset = (tb_uint32_t)(impl->instructions + (tb_uint32_t)item->data);

            // update the offset
            tb_iterator_copy(impl->labels, item_itor, (tb_pointer_t)offset);
        }

        // compile it
        tb_size_t count = vm86_proc_compiler_compile_done(impl, p, e);
        tb_assert_and_check_break(count == impl->instructions_count);

        // ok
        ok = tb_true;

    } while (0);

    // ok?
    return ok;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
vm86_proc_ref_t vm86_proc_init(vm86_ref_t emulator, tb_char_t const* code, tb_size_t size)
{
    // check
    tb_assert_and_check_return_val(emulator && code && size, tb_null);

    // done
    tb_bool_t                   ok = tb_false;
    vm86_proc_impl_t*    impl = tb_null;
    do
    {
        // make proc
        impl = tb_malloc0_type(vm86_proc_impl_t);
        tb_assert_and_check_break(impl);

        // save emulator
        impl->emulator = emulator;

        // init labels
        impl->labels = tb_hash_map_init(8, tb_element_str(tb_true), tb_element_uint32());
        tb_assert_and_check_break(impl->labels);

        // init locals
        impl->locals = tb_hash_map_init(8, tb_element_str(tb_true), tb_element_uint32());
        tb_assert_and_check_break(impl->locals);

        // compile code
        ok = vm86_proc_compile(impl, code, size);

    } while (0);

    // failed?
    if (!ok)
    {
        // exit it
        if (impl) vm86_proc_exit((vm86_proc_ref_t)impl);
        impl = tb_null;
    }

    // ok?
    return (vm86_proc_ref_t)impl;
}
tb_void_t vm86_proc_exit(vm86_proc_ref_t proc)
{
    // check
    vm86_proc_impl_t* impl = (vm86_proc_impl_t*)proc;
    tb_assert_and_check_return(impl);

    // exit name
    if (impl->name) tb_free(impl->name);
    impl->name = tb_null;

    // exit labels
    if (impl->labels) tb_hash_map_exit(impl->labels);
    impl->labels = tb_null;

    // exit locals
    if (impl->locals) tb_hash_map_exit(impl->locals);
    impl->locals = tb_null;

    // exit instructions
    if (impl->instructions) 
    {
        // exit cstring
        vm86_instruction_ref_t p = impl->instructions;
        vm86_instruction_ref_t e = impl->instructions + impl->instructions_count;
        while (p < e) 
        {
            // exists?
            if (p->is_cstr && p->v0.cstr) 
            {
                // exit cstring
                tb_free(p->v0.cstr);
                p->v0.cstr = tb_null;
            }

            // next
            p++;
        }

        // exit it
        tb_free(impl->instructions);
        impl->instructions = tb_null;
    }

    // exit it
    tb_free(impl);
}
tb_char_t const* vm86_proc_name(vm86_proc_ref_t proc)
{
    // check
    vm86_proc_impl_t* impl = (vm86_proc_impl_t*)proc;
    tb_assert_and_check_return_val(impl, tb_null);

    // the name
    return impl->name;
}
tb_void_t vm86_proc_done(vm86_proc_ref_t proc)
{
    // check
    vm86_proc_impl_t* impl = (vm86_proc_impl_t*)proc;
    tb_assert_and_check_return(impl && impl->name);

    // trace
    tb_trace_d("=====================================================================");
    tb_trace_d("done: %s", impl->name);

    // the emulator
    vm86_ref_t emulator = impl->emulator;
    tb_assert_and_check_return(emulator);

    // the stack
    vm86_stack_ref_t stack = vm86_stack(emulator);
    tb_assert_and_check_return(stack);

    // push the stub return address
    vm86_stack_push(stack, 0xbeaf);

    // done it
    vm86_instruction_ref_t p = impl->instructions;
    vm86_instruction_ref_t e = impl->instructions + impl->instructions_count;
    while (p < e && p) 
    {
        // check
        tb_assert(p->done);

        // execute it
        p = p->done(p, emulator);
    }
}
