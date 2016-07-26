#ifndef VM86_PROC_H
#define VM86_PROC_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/// the emulator proc ref type
typedef struct{}*           vm86_proc_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! init proc
 *
 * @param emulator          the emulator
 * @param code              the proc code
 * @param size              the proc size
 *
 * @return                  the proc
 */
vm86_proc_ref_t      vm86_proc_init(vm86_ref_t emulator, tb_char_t const* code, tb_size_t size);

/*! exit proc 
 *
 * @param proc              the proc
 */
tb_void_t                   vm86_proc_exit(vm86_proc_ref_t proc);

/*! the proc name
 *
 * @param proc              the proc
 *
 * @return                  the name
 */
tb_char_t const*            vm86_proc_name(vm86_proc_ref_t proc);

/*! done proc
 *
 * @param proc              the proc
 */
tb_void_t                   vm86_proc_done(vm86_proc_ref_t proc);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


