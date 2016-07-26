#ifndef VM86_DATA_H
#define VM86_DATA_H

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

/// the emulator data ref type
typedef struct{}*           vm86_data_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! init data
 *
 * @param size              the data size
 *
 * @return                  the data
 */
vm86_data_ref_t      vm86_data_init(tb_size_t size);

/*! exit data 
 *
 * @param data              the data
 */
tb_void_t                   vm86_data_exit(vm86_data_ref_t data);

/*! is data?
 *
 * @param data              the data
 * @param name              the data name
 *
 * @return                  tb_true or tb_false
 */
tb_bool_t                   vm86_data_is(vm86_data_ref_t data, tb_char_t const* name);

/*! get data from the given name 
 *
 * @param data              the data
 * @param name              the data name
 * @param psize             the data size pointer
 *
 * @return                  the data offset
 */
tb_uint32_t                 vm86_data_get(vm86_data_ref_t data, tb_char_t const* name, tb_size_t* psize);

/*! add data with the given name and size 
 *
 * @param data              the data
 * @param name              the data name
 * @param buff              the data buff
 * @param size              the data size
 */
tb_void_t                   vm86_data_add(vm86_data_ref_t data, tb_char_t const* name, tb_byte_t const* buff, tb_size_t size);

#ifdef __vm_debug__
/*! dump data 
 *
 * @param data              the data
 */
tb_void_t                   vm86_data_dump(vm86_data_ref_t data);
#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

#endif


