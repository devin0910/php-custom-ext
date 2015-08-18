/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2015 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_zero.h"

/* If you declare any globals in php_zero.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(zero)
*/

/* True global resources - no need for thread safety here */
static int le_zero;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("zero.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_zero_globals, zero_globals)
    STD_PHP_INI_ENTRY("zero.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_zero_globals, zero_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_zero_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_zero_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "zero", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/

/* {{{ proto double calcpi(int iterations)
   Calculate Pi */
PHP_FUNCTION(calcpi)
{
	int argc = ZEND_NUM_ARGS();
	long iterations;
    int index, hits;
    double randx, randy, distance, value;

	if (zend_parse_parameters(argc TSRMLS_CC, "l", &iterations) == FAILURE) 
		return;

    hits = 0;
    for (index = 0; index < iterations; index++) {
        randx = rand();
        randy = rand();

        randx /= RAND_MAX;
        randy /= RAND_MAX;

        distance = sqrt((randx * randx) + (randy * randy));
        if (distance <= 1.0) {
            hits++;
        }
        value = ((double) hits / (double) index);
        value *= 4.0;
    }

    value = ((double) hits / (double) iterations);
    value *= 4.0;
    RETVAL_DOUBLE(value);
}
/* }}} */

/* {{{ proto string reverse(string input)
   Reverse the input string */
PHP_FUNCTION(reverse)
{
	char *input = NULL;
	int argc = ZEND_NUM_ARGS();
	int input_len;
    char *workstr;
    int index;

	if (zend_parse_parameters(argc TSRMLS_CC, "s", &input, &input_len) == FAILURE) 
		return;
    
    workstr = (char*) emalloc(input_len + 1);
    memset(workstr, 0, input_len + 1);
    for (index = 0; index < input_len; index++) {
        workstr[index] = input[input_len - (index + 1)];
    }

    RETVAL_STRING(workstr, 1);
    efree(workstr);
}
/* }}} */

/* {{{ proto array uniquechars(string input [, bool case_sensitive])
   Return the unique characters in the input string */
PHP_FUNCTION(uniquechars)
{
	char *input = NULL;
	int argc = ZEND_NUM_ARGS();
	int input_len;
	zend_bool case_sensitive;
    char* workbuf;
    int index, work_index;

	if (zend_parse_parameters(argc TSRMLS_CC, "s|b", &input, &input_len, &case_sensitive) == FAILURE) 
		return;

    if (argc == 1) {
        case_sensitive = 1;
    }

    work_index = 0;
    workbuf = (char*) emalloc(input_len + 1);
    memset(workbuf, 0, input_len + 1);

    for (index = 0; index < input_len; index++) {
        if (case_sensitive) {
            if (!strchr(workbuf, input[index])) {
                workbuf[work_index] = input[index];
                work_index++;
            }
        } else {
            if (!strchr(workbuf, tolower(input[index]))) {
                workbuf[work_index] = tolower(input[index]);
                work_index++;
            }
        }
    }

    array_init(return_value);
    for (index = 0; index < input_len; index++) {
        if (workbuf[index] != '\0') {
            add_next_index_stringl(return_value, &workbuf[index], 1, 1);
        }
    }

    efree(workbuf);
}
/* }}} */

/* {{{ proto void add_variables()
 *    Add local or global variables */
PHP_FUNCTION(add_variables)
{
    zval *new1, *new2;

    MAKE_STD_ZVAL(new1);
    MAKE_STD_ZVAL(new2);

    ZVAL_LONG(new1, 100);
    ZVAL_LONG(new2, 500);

    ZEND_SET_SYMBOL(EG(active_symbol_table), "new_local_var", new1);
    ZEND_SET_SYMBOL(&EG(symbol_table), "new_global_var", new2);

    RETURN_NULL();
}
/* }}} */

/* {{{ proto void add_variables2()
 *    Add local or global variables */
PHP_FUNCTION(add_variables2)
{
    zval *new1, *new2;

    MAKE_STD_ZVAL(new1);
    MAKE_STD_ZVAL(new2);

    ZVAL_LONG(new1, 100);
    ZVAL_LONG(new2, 500);

    zend_hash_update(EG(active_symbol_table),
            "new_local_var",
            strlen("new_local_var") + 1,
            &new1,
            sizeof(zval*),
            NULL
    );

    zend_hash_update(&EG(symbol_table),
            "new_global_var",
            strlen("new_global_var") + 1,
            &new2,
            sizeof(zval*),
            NULL
    );

    RETURN_NULL();
}
/* }}} */

/* {{{ proto array return_array1()
 *    Test returning array #1 */
PHP_FUNCTION(return_array1)
{
    if (ZEND_NUM_ARGS() != 0) {
        WRONG_PARAM_COUNT;
    }

    array_init(return_value);

    add_next_index_long(return_value, 5);
    add_next_index_long(return_value, 4);
    add_next_index_long(return_value, 3);
    add_next_index_long(return_value, 2);
    add_next_index_long(return_value, 1);
}
/* }}} */

/* {{{ proto array return_array2()
 *    Test returning array #2 */
PHP_FUNCTION(return_array2)
{
    zval *sub_array1;
    zval *sub_array2;

    if (ZEND_NUM_ARGS() != 0) {
        WRONG_PARAM_COUNT;
    }

    array_init(return_value);
    add_next_index_long(return_value, 1);
    add_next_index_unset(return_value);
    add_next_index_bool(return_value, 1);
    add_next_index_double(return_value, 3.1415);
    add_next_index_string(return_value, "Testing", 1);

    /* create s sub array to put within the return value */
    MAKE_STD_ZVAL(sub_array1);
    array_init(sub_array1);
    add_next_index_long(sub_array1, 2);
    add_next_index_string(sub_array1, "This is the second element in the first sub_array", 1);

    /* create a sub array to put within the first sub array */
    MAKE_STD_ZVAL(sub_array2);
    array_init(sub_array2);
    add_next_index_long(sub_array2, 2);
    add_next_index_string(sub_array2, "This is the second element in the second sub_array", 1);

    /* insert the sub arrays into their parent arrays */
    add_next_index_zval(sub_array1, sub_array2);
    add_next_index_zval(return_value, sub_array1);
}
/* }}} */

/* {{{ proto array return_array2()
 *    Test returning array #3 */
PHP_FUNCTION(return_array3)
{
    if (ZEND_NUM_ARGS() != 0) {
        WRONG_PARAM_COUNT;
    }

    array_init(return_value);
    add_index_long(return_value, 7, 1);
    add_next_index_long(return_value, 2);

    add_index_string(return_value, 300, "Hello world!", 1);
    add_next_index_string(return_value, "Next index insert", 1);
}
/* }}} */

/* {{{ proto array return_array4()
 *    Test returning array #4 */
PHP_FUNCTION(return_array4)
{
    if (ZEND_NUM_ARGS() != 0) {
        WRONG_PARAM_COUNT;
    }

    array_init(return_value);

    add_assoc_string(return_value, "apple", "red", 1);
    add_assoc_string(return_value, "banana", "yellow", 1);
    add_assoc_string(return_value, "orange", "orange", 1);

    add_assoc_long(return_value, "one", 1);
    add_assoc_long(return_value, "two", 2);
    add_assoc_long(return_value, "apple", 3);
}
/* }}} */

PHP_FUNCTION(traverse_array)
{
    zval* array;
    zval** item;
    int count, i;
    char buffer[1024];

    array_init(return_value);

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &array) == FAILURE) {
        return;
    }

    // get number of elements in the array
    count = zend_hash_num_elements(Z_ARRVAL_P(array));
    sprintf(buffer, "array_size = %d", count);
    add_next_index_string(return_value, buffer, 1);

    // move to the begining of the array
    zend_hash_internal_pointer_reset(Z_ARRVAL_P(array));
    for (i = 0; i < count; i++) {
        char* key;
        int ind;

        // get the data in the current array element and coerce into a string
        zend_hash_get_current_data(Z_ARRVAL_P(array), (void **) &item);
        convert_to_string_ex(item);

        // get the key (note this function returns key type)
        if (zend_hash_get_current_key(Z_ARRVAL_P(array), &key, &ind, 0) == HASH_KEY_IS_STRING) {
            sprintf(buffer, "array[%s] = %s", key, Z_STRVAL_PP(item));
            add_next_index_string(return_value, buffer, 1);
        } else {
           sprintf(buffer, "array[%d] = %s", ind, Z_STRVAL_PP(item));
           add_next_index_string(return_value, buffer, 1);
       }

       zend_hash_move_forward(Z_ARRVAL_P(array));
    }
}

PHP_FUNCTION(traverse_array_r)
{
    zval* array;
    zval** item;
    int count, i;
    char buffer[1024];

    array_init(return_value);

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a", &array) == FAILURE) {
        return;
    }

    // get number of elements in the array
    count = zend_hash_num_elements(Z_ARRVAL_P(array));
    sprintf(buffer, "array_size = %d", count);
    add_next_index_string(return_value, buffer, 1);

    // move to the begining of the array
    zend_hash_internal_pointer_end(Z_ARRVAL_P(array));
    for (i = 0; i < count; i++) {
        char* key;
        int ind;

        // get the data in the current array element and coerce into a string
        zend_hash_get_current_data(Z_ARRVAL_P(array), (void **) &item);
        convert_to_string_ex(item);

        // get the key (note this function returns key type)
        if (zend_hash_get_current_key(Z_ARRVAL_P(array), &key, &ind, 0) == HASH_KEY_IS_STRING) {
            sprintf(buffer, "array[%s] = %s", key, Z_STRVAL_PP(item));
            add_next_index_string(return_value, buffer, 1);
        } else {
           sprintf(buffer, "array[%d] = %s", ind, Z_STRVAL_PP(item));
           add_next_index_string(return_value, buffer, 1);
       }

       zend_hash_move_backwards(Z_ARRVAL_P(array));
    }
}

PHP_FUNCTION(array_find)
{
    zval* array;
    zval** item;
    int long_index;
    char* string_index;
    int string_index_len;
    char buffer[1024];

    array_init(return_value);

    if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "al", &array, &long_index) == SUCCESS) {
        // find item indexed by integer, long index
        if (zend_hash_index_find(Z_ARRVAL_P(array), long_index, (void **)&item) == SUCCESS) {
            convert_to_string_ex(item);
            sprintf(buffer, "index [%d] found containing data %s", long_index, Z_STRVAL_PP(item));
            add_next_index_string(return_value, buffer, 1);
            zend_hash_get_current_data(Z_ARRVAL_P(array), (void**) &item);
            convert_to_string_ex(item);
            sprintf(buffer, "data at current array location: %s", Z_STRVAL_PP(item));
            add_next_index_string(return_value, buffer, 1);
        } else {
            sprintf(buffer, "index [%d] not found in array", long_index);
            add_next_index_string(return_value, buffer, 1);
        }
    } else if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "as" , &array, &string_index, &string_index_len) == SUCCESS) {
            // find item indexed by assoc string
            if (zend_hash_find(Z_ARRVAL_P(array), string_index, string_index_len + 1, (void **)&item) == SUCCESS) {
                convert_to_string_ex(item);
                sprintf(buffer, "index [%s] found containing data %s", string_index, Z_STRVAL_PP(item));
                add_next_index_string(return_value, buffer, 1);
                zend_hash_get_current_data(Z_ARRVAL_P(array), (void**) &item);
                convert_to_string_ex(item);
                sprintf(buffer, "data at current array location: %s", Z_STRVAL_PP(item));
                add_next_index_string(return_value, buffer, 1);
            } else {
                sprintf(buffer, "index [%s] nout found in array", string_index);
                add_next_index_string(return_value, buffer, 1);
            }
    } else {
       php_error(E_WARNING, "usage %s(array, [string|integer])", get_active_function_name(TSRMLS_C));
       RETURN_FALSE;
   }
}

/* {{{ php_zero_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_zero_init_globals(zend_zero_globals *zero_globals)
{
	zero_globals->global_value = 0;
	zero_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(zero)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(zero)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(zero)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(zero)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(zero)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "zero support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ zero_functions[]
 *
 * Every user visible function must have an entry in zero_functions[].
 */
const zend_function_entry zero_functions[] = {
	PHP_FE(confirm_zero_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(calcpi,	NULL)
	PHP_FE(reverse,	NULL)
	PHP_FE(uniquechars,	NULL)
    PHP_FE(add_variables,   NULL)
    PHP_FE(add_variables2,   NULL)
    PHP_FE(return_array1,   NULL)
    PHP_FE(return_array2,   NULL)
    PHP_FE(return_array3,   NULL)
    PHP_FE(return_array4,   NULL)
    PHP_FE(traverse_array,   NULL)
    PHP_FE(traverse_array_r,   NULL)
    PHP_FE(array_find,   NULL)
	PHP_FE_END	/* Must be the last line in zero_functions[] */
};
/* }}} */

/* {{{ zero_module_entry
 */
zend_module_entry zero_module_entry = {
	STANDARD_MODULE_HEADER,
	"zero",
	zero_functions,
	PHP_MINIT(zero),
	PHP_MSHUTDOWN(zero),
	PHP_RINIT(zero),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(zero),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(zero),
	PHP_ZERO_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_ZERO
ZEND_GET_MODULE(zero)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
