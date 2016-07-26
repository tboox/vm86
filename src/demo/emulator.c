/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "vm86/emulator.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * test
 */
static tb_void_t vm86_demo_proc_func_printf(vm86_ref_t emulator)
{
    // check
    tb_assert(emulator);

    // the stack
    vm86_stack_ref_t stack = vm86_stack(emulator);
    tb_assert(stack);

    // get the format
    tb_uint32_t format = 0;
    vm86_stack_top(stack, &format, 0);
    tb_assert(format);

    // get the value
    tb_uint32_t value = 0;
    vm86_stack_top(stack, &value, 1);

    // done it
    tb_printf((tb_char_t const*)format, value);
}
static tb_void_t vm86_demo_proc_done_hello(tb_uint32_t value)
{
    // the code
    static tb_char_t const s_code_sub_hello[] = 
    {
"sub_hello	proc near \n\
arg_0		= dword	ptr  8 \n\
.data \n\
        format db \"hello: %x\", 0ah, 0dh, 0 \n\
 \n\
off_5A74B0	dd offset loc_6B2B50	; DATA XREF: sub_589100+1832r \n\
		dd offset loc_58A945	; jump table for switch	statement \n\
 \n\
.code \n\
        ; hi\n\
        push	ebp ;hello \n\
		mov	ebp, esp \n\
 \n\
    loc_6B2B50:				; CODE XREF: sub_6B2B40+8j\n\
        push    eax \n\
		mov	eax, [ebp+arg_0] \n\
        push eax \n\
        mov eax, offset format \n\
        push eax \n\
        call printf \n\
        add esp, 4 \n\
        pop eax \n\
        \n\
        mov ecx, 1\n\
        jmp ds:off_5A74B0[ecx*4]\n\
 \n\
loc_58A945:\n\
        push    eax \n\
		mov	eax, [ebp+arg_0] \n\
        push eax \n\
        mov eax, offset format \n\
        push eax \n\
        call printf \n\
        add esp, 4 \n\
        pop eax \n\
        \n\
  end:\n\
        mov	esp, ebp \n\
		pop	ebp \n\
        retn \n\
sub_hello    endp \n\
    "
    };

    // the emulator
    vm86_ref_t emulator = vm86();
    if (emulator)
    {
        // the lock
        tb_spinlock_ref_t lock = vm86_lock(emulator);

        // enter
        tb_spinlock_enter(lock);

        // the stack
        vm86_stack_ref_t stack = vm86_stack(emulator);

        // compile proc
        vm86_proc_ref_t proc = vm86_text_compile(vm86_text(emulator), s_code_sub_hello, sizeof(s_code_sub_hello));
        if (proc)
        {
            // add function
            vm86_function_set(emulator, "printf", vm86_demo_proc_func_printf);

            // init arguments
            vm86_stack_push(stack, value);

            // done proc
            vm86_proc_done(proc);

            // restore stack
            vm86_stack_pop(stack, tb_null);

            // trace
            tb_trace_i("sub_hello(%x)", value);
        }

        // leave
        tb_spinlock_leave(lock);
    } 
}
static tb_void_t vm86_demo_proc_done_sub_6B2B40(tb_uint64_t a1, tb_uint8_t a2)
{
    // the code
    static tb_char_t const s_code_sub_6B2B40[] = 
    {
        "\n\
            ; ... \n\
    sub_6B2B40	proc near		; CODE XREF: sub_6B2B40+E83p\n\
                        ; sub_526E70+3C7Ap ...\n\
            cmp	cl, 40h\n\
            jnb	short loc_6B2B5A\n\
            cmp	cl, 20h\n\
            jnb	short loc_6B2B50\n\
            shrd	eax, edx, cl\n\
            shr	edx, cl\n\
            retn\n\
    ; ---------------------------------------------------------------------------\n\
    \
    loc_6B2B50:				; CODE XREF: sub_6B2B40+8j\n\
            mov	eax, edx\n\
            xor	edx, edx\n\
            and	cl, 1Fh\n\
            shr	eax, cl\n\
            retn\n\
    ; ---------------------------------------------------------------------------\n\
    \n\
    loc_6B2B5A:				; CODE XREF: sub_6B2B40+3j\n\
            xor	eax, eax\n\
            xor	edx, edx\n\
            retn\n\
    sub_6B2B40	endp\n\
    "
    };

    // the emulator
    vm86_ref_t emulator = vm86();
    if (emulator)
    {
        // the lock
        tb_spinlock_ref_t lock = vm86_lock(emulator);

        // enter
        tb_spinlock_enter(lock);

        // the registers
        vm86_registers_ref_t registers = vm86_registers(emulator);

        // compile proc
        vm86_proc_ref_t proc = vm86_text_compile(vm86_text(emulator), s_code_sub_6B2B40, sizeof(s_code_sub_6B2B40));
        if (proc)
        {
            // init arguments
            registers[VM86_REGISTER_EAX].u32 = (tb_uint32_t)a1;
            registers[VM86_REGISTER_EDX].u32 = (tb_uint32_t)(a1 >> 32);
            registers[VM86_REGISTER_ECX].u8[0] = a2;

            // done proc
            vm86_proc_done(proc);

            // the result
            tb_uint64_t result = ((tb_uint64_t)registers[VM86_REGISTER_EDX].u32 << 32) | registers[VM86_REGISTER_EAX].u32;

            // trace
            tb_trace_i("sub_6B2B40(%llx, %u): %llx", a1, a2, result);
        }

        // leave
        tb_spinlock_leave(lock);
    } 
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * main
 */
tb_int_t main(tb_int_t argc, tb_char_t** argv)
{
    // init tbox
    if (!tb_init(tb_null, tb_null)) return 0;

    // done
    vm86_demo_proc_done_hello(0x31415926);
    vm86_demo_proc_done_sub_6B2B40((0x123ULL << 32) | 0x321, 8);
    vm86_demo_proc_done_sub_6B2B40((0x123ULL << 32) | 0x321, 16);
    vm86_demo_proc_done_sub_6B2B40((0x123ULL << 32) | 0x321, 32);

    // exit tbox
    tb_exit();
    return 0;
}

