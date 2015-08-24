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

void inspect(zval* value);

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

PHP_FUNCTION(array_delete)
{
    zval* array;
    zval** item;
    int long_index;
    char* string_index;
    int string_index_len;
    char buffer[1024];

    array_init(return_value);

    if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "al", &array, &long_index) == SUCCESS) {
        if (!PZVAL_IS_REF(array)) {
            php_error(E_WARNING, "The array parameter must be passed by reference");
            return;
        }

        // delete item indexed by integer, long_index
        zend_hash_index_del(Z_ARRVAL_P(array), long_index);
    } else if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS() TSRMLS_CC, "as", &array, &string_index, &string_index_len) == SUCCESS) {
        if (!PZVAL_IS_REF(array)) {
            php_error(E_WARNING, "The array parameter must be passed by reference");
            return;
        }
        
        // delete item indexed by assoc string, string index
        zend_hash_del(Z_ARRVAL_P(array), string_index, string_index_len + 1);
    } else {
        php_error(E_WARNING, "usage %s(&array, [string|integer])", get_active_function_name(TSRMLS_C));
        RETURN_FALSE;
    }
}

PHP_FUNCTION(test_inspect)
{
    zval *value;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &value) == FAILURE) {
        return;
    }

    inspect(value);
}

PHP_FUNCTION(return_object)
{
    zval* array;
    zval* emb_obj;

    object_init(return_value);

    add_property_long(return_value, "long_property", 1);
    add_property_unset(return_value, "unset_property");
    add_property_bool(return_value, "bool_property", 1);
    add_property_double(return_value, "double_property", 3.14);
    add_property_string(return_value, "string_property", "This is a string", 1);
    MAKE_STD_ZVAL(emb_obj);
    object_init(emb_obj);

    add_property_long(emb_obj, "long_property", 2);
    add_property_unset(emb_obj, "unset_property");
    add_property_bool(emb_obj, "bool_property", 0);
    add_property_double(emb_obj, "double_property", 2.71);
    add_property_string(emb_obj, "string_property", "This is a string in an embedded sub-object", 1);

    add_property_zval(return_value, "object_property", emb_obj);

    MAKE_STD_ZVAL(array);
    array_init(array);
    
    add_next_index_long(array, 1);
    add_next_index_long(array, 3);
    add_next_index_long(array, 5);

    add_property_zval(return_value, "array_property", array);
}

PHP_FUNCTION(traverse_object)
{
    zval* object;
    zval** item;
    int count, i;
    char buffer[1024];

    array_init(return_value);

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "o", &object) == FAILURE) {
        return;
    }

    // get number of elements in the object
    count = zend_hash_num_elements(Z_OBJPROP_P(object));
    sprintf(buffer, "object property count = %d", count);
    add_next_index_string(return_value, buffer, 1);

    // move to the begining of the object properties
    zend_hash_internal_pointer_reset(Z_OBJPROP_P(object));
    for (i = 0; i < count; i++) {
        char* key;
        int ind;

        // get the data in the current property and coerce into a string
        zend_hash_get_current_data(Z_OBJPROP_P(object), (void **)&item);
        convert_to_string_ex(item);

        // get the key (note this function returns key type)
        if (zend_hash_get_current_key(Z_OBJPROP_P(object), &key, &ind, 0) == HASH_KEY_IS_STRING) {
            sprintf(buffer, "object->%s = %s", key, Z_STRVAL_PP(item));
            add_next_index_string(return_value, buffer, 1);
        } else {
            sprintf(buffer, "object->%d = %s", ind, Z_STRVAL_PP(item));
            add_next_index_string(return_value, buffer, 1);
        }

        zend_hash_move_forward(Z_OBJPROP_P(object));
    }
}

PHP_FUNCTION(obj_prop_find)
{
    zval* object;
    zval** item;
    char* string_index;
    int string_index_len;
    char buffer[1024];

    array_init(return_value);
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "os", &object, &string_index, &string_index_len) != SUCCESS) {
        return;
    }

    // find property with name = string_index
    if (zend_hash_find(Z_OBJPROP_P(object), string_index, string_index_len + 1, (void**) item) == SUCCESS) {
        convert_to_string_ex(item);
        sprintf(buffer, "obj->%s found containing data %s", string_index, Z_STRVAL_PP(item));
        add_next_index_string(return_value, buffer, 1);
    } else {
        sprintf(buffer, "object->%s not found", string_index);
        add_next_index_string(return_value, buffer, 1);
    }
}

PHP_FUNCTION(obj_prop_delete)
{
    zval* object;
    zval** item;
    char* string_index;
    int string_index_len;
    char buffer[1024];

    array_init(return_value);

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "os", &object, &string_index, &string_index_len) != SUCCESS) {
        return;
    }

    if (!PZVAL_IS_REF(object)) {
        php_error(E_WARNING, "The object parameter must be passed by reference");
        return;
    }

    zend_hash_del(Z_OBJPROP_P(object), string_index, string_index_len + 1);
}

PHP_FUNCTION(open_resource)
{
    zero_resource* resource;
    int rscid;

    // this function accepts no arguments and returns a resource
    resource = emalloc(sizeof(zero_resource));
    resource->fp = fopen("/etc/hosts", "r");

    rscid = ZEND_REGISTER_RESOURCE(return_value, resource, le_zero);
    RETVAL_RESOURCE(rscid);
}

PHP_FUNCTION(read_resource)
{
    zero_resource* resource;
    zval* arg1;
    char fileline[1024];

    // this function accepts a resource and returns a single line from the file abstracted by the resource
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &arg1) == FAILURE) {
        return;
    }

    ZEND_FETCH_RESOURCE(resource, zero_resource*, &arg1, -1, le_zero_name, le_zero);

    if (resource) {
        if (fgets(fileline, 1024, resource->fp)) {
            RETURN_STRING(fileline, 1);
        } else {
            RETURN_FALSE;
        }
    }
}

PHP_FUNCTION(close_resource)
{
    zval* arg1;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &arg1) == FAILURE) {
        return;
    }

    zend_list_delete(Z_RESVAL_P(arg1));
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
    zend_printf("In PHP_MINIT_FUNCTION\n");
    // ZEND_INIT_MODULE_GLOBALS(zero, php_zero_init_globals, NULL);
    // zend_printf("global_value = %d\n", ZERO_G(global_value));
    // REGISTER_INI_ENTRIES();
    // zend_printf("global_value = %d\n", ZERO_G(global_value));
    le_zero = zend_register_list_destructor_ex(zero_destruction_handler, NULL, le_zero_name, module_number);

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
    PHP_FE(array_delete,   NULL)
    PHP_FE(test_inspect,   NULL)
    PHP_FE(return_object,   NULL)
    PHP_FE(traverse_object,   NULL)
    PHP_FE(obj_prop_find,   NULL)
    PHP_FE(obj_prop_delete,   NULL)
    PHP_FE(open_resource,   NULL)
    PHP_FE(read_resource,   NULL)
    PHP_FE(close_resource,   NULL)
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

void inspect(zval* value)
{
    switch (Z_TYPE_P(value)) {
        case IS_NULL:
            php_printf("The variable passed is of type IS_NULL\n");
            break;
        case IS_LONG:
            php_printf("The variable passed is of type IS_LONG\n");
            php_printf("The content is %ld\n", Z_LVAL_P(value));
            break;
        case IS_DOUBLE:
            php_printf("The variable passed is of type IS_DOUBLE\n");
            php_printf("The content is %f\n", Z_DVAL_P(value));
            break;
#if PHP_VERSION_ID >= 70000
        case IS_TRUE:
            php_printf("The variable passed is of type IS_TRUE\n");
            break;
        case IS_FALSE:
            php_printf("The variable passed is of type IS_FALSE\n");
            break;
#else
        case IS_BOOL:
            php_printf("The variable passed is of type IS_BOOL\n");
            break;
#endif
        case IS_OBJECT:
            php_printf("The variable passed is of type IS_OBJECT\n");
            break;
        case IS_STRING:
            php_printf("The variable passed is of type IS_STRING\n");
            php_printf("The content is \"%s\"\n", Z_STRVAL_P(value));
            php_printf("The content reversed is \"");
            char *p = Z_STRVAL_P(value) + Z_STRLEN_P(value);
            while (--p >= Z_STRVAL_P(value))
                php_printf("%c", *p);
            php_printf("\"\n");
            break;
        case IS_RESOURCE:
            php_printf("The variable is of type IS_RESOURCE\n");
            php_printf("The number of resource is: %ld\n",
#if PHP_VERSION_ID >= 7000
                    Z_RES_HANDLE_P(value)
#else
                    Z_RESVAL_P(value);
#endif
                    );
            break;
        case IS_ARRAY:
            php_printf("The variable passed is type of IS_ARRAY\n");
            php_printf("There is %ld direct elements in the array\n", zend_hash_num_elements(Z_ARRVAL_P(value)));
            break;
        default:
            php_printf("unkown type");
            break;
    }
}

void general_destruction_handler(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
    zero_resource* resource;

    zend_printf("In zero_destruction_handler\n");

    resource = (zero_resource*) rsrc->ptr;
    fclose(resource->fp);
    efree(resource);
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
