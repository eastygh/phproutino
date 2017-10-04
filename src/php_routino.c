/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2017 The PHP Group                                |
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
#include "php_routino.h"
#include "php_routino_structs.h"
#include "routino.h"
#include "zend_exceptions.h"
#include "zend_interfaces.h"

/* If you declare any globals in php_routino.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(routino)
*/

/* True global resources - no need for thread safety here */
static int le_routino;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("routino.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_routino_globals, routino_globals)
    STD_PHP_INI_ENTRY("routino.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_routino_globals, routino_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_routino_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_routino_compiled)
{
	char *arg = NULL;
	size_t arg_len, len;
	zend_string *strg;
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
		return;
	}
	strg = strpprintf(0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "routino", arg);

	RETURN_STR(strg);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/


/* {{{ php_routino_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_routino_init_globals(zend_routino_globals *routino_globals)
{
	routino_globals->global_value = 0;
	routino_globals->global_string = NULL;
}
*/
/* }}} */

/* Handlers */
static zend_object_handlers routino_object_handlers;


// Routino object
// Routino::Open(dir,prefix);
// dir - databasedirectory
// prefix - prefix of data files
PHP_METHOD(Routino, open) {
	// two arguments dir and prefix
	char *dir = NULL;
	char *prefix = NULL;
	size_t dir_len,pfx_len;
	php_routino_object *r_obj;

	zval *object = getThis();
	r_obj = Z_ROUTINO_P(object);

    if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s|s", &dir, &dir_len,&prefix,&pfx_len)) {
        return;
	}

	if (r_obj->initialised) {
		zend_throw_exception(zend_ce_exception, "Already initialised routino object", 0);
		return;
	}

	r_obj->db=Routino_LoadDatabase(dir,prefix);
	if (r_obj->db==NULL){
		RETURN_FALSE;
	}
	r_obj->initialised=1;
}

// Routino::Close();
PHP_METHOD(Routino, close) {
	php_routino_object *r_obj;
	zval *object = getThis();

	r_obj = Z_ROUTINO_P(object);
	if (!r_obj && !r_obj->initialised)	{
		zend_throw_exception(zend_ce_exception, "Object not initialised", 0);
		return;
	}
	if (r_obj->initialised && r_obj->db) {
		Routino_UnloadDatabase(r_obj->db);
		r_obj->db=NULL;
		r_obj->initialised = 0;
	}
	php_printf("closed\n");		
	RETURN_TRUE;
}


/* {{{ routino_functions[]
 *
 * Every user visible function must have an entry in routino_functions[].
 */
static const zend_function_entry routino_methods[] = {
	PHP_ME(Routino, open, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Routino, close, NULL, ZEND_ACC_PUBLIC)
	/* Aliases */
	PHP_MALIAS(Routino,	__construct, open, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
    PHP_FE_END
};
/* }}} */

// allocate memory for Routino Object
static zend_object *php_routino_object_new(zend_class_entry *class_type) /* {{{ */
	{
		php_routino_object *intern;
	
		/* Allocate memory for it */
		intern = ecalloc(1, sizeof(php_routino_object) + zend_object_properties_size(class_type));
	
		zend_object_std_init(&intern->zo, class_type);
		object_properties_init(&intern->zo, class_type);
	
		intern->zo.handlers = &routino_object_handlers;
		php_printf("new object \n");
		return &intern->zo;
	}
/* }}} */

// free object
static void php_routino_object_free_storage(zend_object *object) /* {{{ */
	{
		php_routino_object *intern = php_routino_from_object(object);
		if (!intern) {
			return;
		}
		if (intern->initialised && intern->db) {
			Routino_UnloadDatabase(intern->db);
			intern->initialised = 0;
		}	
		zend_object_std_dtor(&intern->zo);
	}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(routino)
{

	zend_class_entry ce;

	// standart handlers for object
	memcpy(&routino_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	/* Register Routino Class */
	INIT_CLASS_ENTRY(ce, "Routino", routino_methods);
	ce.create_object = php_routino_object_new;
	routino_object_handlers.offset = XtOffsetOf(php_routino_object, zo);
	routino_object_handlers.clone_obj = NULL;
	routino_object_handlers.free_obj = php_routino_object_free_storage;
//	php_routino_entry = zend_register_internal_class(&ce);	
	zend_register_internal_class(&ce);

	//REGISTER_INI_ENTRIES();
	return SUCCESS;
}
/* }}} */


/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(routino)
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
PHP_RINIT_FUNCTION(routino)
{
#if defined(COMPILE_DL_ROUTINO) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(routino)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(routino)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "routino support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ proto string hello_world(string arg)
   Say Hello World to everyone */
   PHP_FUNCTION(hello_world)
   {
		zend_string	*str;
		
		str = strpprintf(0,"routino module api version %d checked params of function %d\n",Routino_APIVersion,ZEND_NUM_ARGS());		
		RETURN_STR(str);
   }
   /* }}} */

   /* {{{ proto string hello_world(string arg)
   Say Hello World to everyone */
   PHP_FUNCTION(routino_opendatabase)
   {
	char *dir = NULL;
	char *prefix = NULL;
	size_t dir_len,pfx_len;
	zend_string *str;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s|s", &dir, &dir_len,&prefix,&pfx_len) == FAILURE) {
        RETURN_NULL();
	}

	php_printf("dir is  %s\n", dir);	
	php_printf("prefix is  %s\n", prefix);
	
	Routino_Database *db=Routino_LoadDatabase(dir,prefix);
	if (db==NULL){
		RETURN_FALSE;
	}
	php_printf("ong 80 \n");
	Routino_UnloadDatabase(db);
	RETURN_TRUE;	
   }
   /* }}} */   



/* {{{ routino_module_entry
 */
zend_module_entry routino_module_entry = {
	STANDARD_MODULE_HEADER,
	"routino",
	routino_methods,
	PHP_MINIT(routino),
	PHP_MSHUTDOWN(routino),
	PHP_RINIT(routino),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(routino),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(routino),
	PHP_ROUTINO_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_ROUTINO
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(routino)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
