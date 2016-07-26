#ifndef VM86_H
#define VM86_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "data.h"
#include "text.h"
#include "stack.h"
#include "register.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/*! the emulator func type
 *
 * @param emulator          the emulator
 */
typedef tb_void_t           (*vm86_func_t)(vm86_ref_t emulator);

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! the emulator instance
 *
 * @return                  the emulator
 */
vm86_ref_t           vm86(tb_noarg_t);

/*! init emulator
 *
 * @param data_size         the data size
 * @param stack_size        the stack size
 *
 * @return                  the emulator
 */
vm86_ref_t           vm86_init(tb_size_t data_size, tb_size_t stack_size);

/*! exit emulator 
 *
 * @param emulator          the emulator
 */
tb_void_t                   vm86_exit(vm86_ref_t emulator);

/*! the emulator lock
 *
 * @param emulator          the emulator
 */
tb_spinlock_ref_t           vm86_lock(vm86_ref_t emulator);

/*! the emulator text
 *
 * @param emulator          the emulator
 *
 * @return                  the text
 */
vm86_text_ref_t      vm86_text(vm86_ref_t emulator);

/*! the emulator data
 *
 * @param emulator          the emulator
 *
 * @return                  the data
 */
vm86_data_ref_t      vm86_data(vm86_ref_t emulator);

/*! the emulator stack
 *
 * @param emulator          the emulator
 *
 * @return                  the stack
 */
vm86_stack_ref_t     vm86_stack(vm86_ref_t emulator);

/*! the emulator registers
 *
 * @param emulator          the emulator
 *
 * @return                  the registers
 */
vm86_registers_ref_t vm86_registers(vm86_ref_t emulator);

/*! get function from the emulator 
 *
 * @param emulator          the emulator
 * @param name              the function name
 *
 * @return                  the function address
 */
vm86_func_t          vm86_function(vm86_ref_t emulator, tb_char_t const* name);

/*! set function to the emulator 
 *
 * @param emulator          the emulator
 * @param name              the function name
 * @param func              the function address
 */
tb_void_t                   vm86_function_set(vm86_ref_t emulator, tb_char_t const* name, vm86_func_t func);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


