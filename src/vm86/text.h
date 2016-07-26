#ifndef VM86_TEXT_H
#define VM86_TEXT_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "proc.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/// the emulator text ref type
typedef struct{}*           vm86_text_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! init text
 *
 * @param emulator          the emulator
 *
 * @return                  the text
 */
vm86_text_ref_t      vm86_text_init(vm86_ref_t emulator);

/*! exit text 
 *
 * @param text              the text
 */
tb_void_t                   vm86_text_exit(vm86_text_ref_t text);

/*! compile proc 
 *
 * @param text              the text
 * @param code              the proc code
 *
 * @return                  the proc 
 */
vm86_proc_ref_t      vm86_text_compile(vm86_text_ref_t text, tb_char_t const* code, tb_size_t size);

/*! get the compiled proc 
 *
 * @param text              the text
 * @param name              the proc name
 *
 * @return                  the proc 
 */
vm86_proc_ref_t      vm86_text_proc(vm86_text_ref_t text, tb_char_t const* name);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


