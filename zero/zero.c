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
