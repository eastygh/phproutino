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
  | Author: easty (c) 2017                                               |
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
#include "php_routino_types.h"
#include "php_routino_props.h"

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

static void char_to_zval_array(char *name, char *val, zval *zval){
	if (val) {
		add_assoc_string(zval,name,val);
	} else {
		add_assoc_string(zval,name,"");
	}
}

static void set_last_routino_error( zval *object, long errn){
	zend_update_property_long(php_routino_ce, object, "lasterrorno", sizeof("lasterrorno") - 1, errn TSRMLS_CC);
}

//static void load_database(const *char dir,const *char preifx){
//	// do i need thread safe? make TODO
//}

//static void unload_database(){
//	// do i need thread safe? make TODO
//}

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

	set_last_routino_error(object, ROUTINO_ERROR_NO_DATABASE);
	
	if (r_obj->initialised==0){
		// set default properties value
		zend_update_property_string(php_routino_ce, object, "lng", sizeof("lng") - 1, "en" TSRMLS_CC); // by default we using English translation
		zend_update_property_string(php_routino_ce, object, "profile", sizeof("profile") - 1, "motorcar" TSRMLS_CC); // we trip on car by default))
	}


    if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "s|s", &dir, &dir_len,&prefix,&pfx_len)) {
        return;
	}

	if (r_obj->dbloaded==1 && r_obj->db){
		Routino_UnloadDatabase(r_obj->db);
		set_last_routino_error(object, Routino_errno);
		r_obj->db=NULL;
		r_obj->dbloaded = 0;
	}

	// update properties
	zend_update_property_string(php_routino_ce, object, "dir", sizeof("dir") - 1, dir TSRMLS_CC);
	zend_update_property_string(php_routino_ce, object, "prefix", sizeof("prefix") - 1, prefix TSRMLS_CC);
	r_obj->initialised=1;	

	r_obj->db=Routino_LoadDatabase(dir,prefix);
	set_last_routino_error(object, Routino_errno);	
	if (r_obj->db==NULL || Routino_errno!=ROUTINO_ERROR_NONE){
		r_obj->dbloaded = 0;
		RETURN_FALSE;
	}
	r_obj->dbloaded = 1;
	RETURN_TRUE;
}

// Routino::Close();
PHP_METHOD(Routino, close) {
	php_routino_object *r_obj;
	zval *object = getThis();

	r_obj = Z_ROUTINO_P(object);
	if (!r_obj || r_obj->initialised==0  || r_obj->dbloaded==0)	{
		RETURN_TRUE;
	}
	if (r_obj->dbloaded==1 && r_obj->db) {
		Routino_UnloadDatabase(r_obj->db);
		r_obj->db=NULL;
		r_obj->dbloaded = 0;
	}
	php_printf("closed\n");		
	RETURN_TRUE;
}

// Routino::Calculate(Array $waypoints);
PHP_METHOD(Routino, calculate) {
	php_routino_object *r_obj;
	zval *object = getThis();
	Routino_Waypoint **wps;
	Routino_Waypoint *wp;
	Routino_Output *route;
	Routino_Output *route_first;
	HashTable *waypoints;
	zval *data;
	zval *lat,*lon;
	zval lat1,lon1;
	zend_string *key1,*key2;
	int wp_count,i,wp_count2;
	float distance=0,tm=0;
	zval *prop_value,rarray,onepoint,rv;
	// external
	Routino_Translation *rt;
	Routino_Profile *prof;
	
	r_obj = Z_ROUTINO_P(object);
	if (!r_obj || r_obj->initialised==0 || !r_obj->dbloaded){
		zend_throw_exception(zend_ce_exception, "Object not initialised or DB not opened", 0);
		return;
	}

	if (FAILURE == zend_parse_parameters_throw(ZEND_NUM_ARGS(), "h", &waypoints) 
		|| 0==(wp_count=zend_hash_num_elements(waypoints)))
		return;

	// load and convert profile
	prop_value = zend_read_property(php_routino_ce, object, "profile", sizeof("profile") - 1,0,&rv);
	if (Z_TYPE_P(prop_value) == IS_STRING){
		convert_to_string(prop_value);
		prof=Routino_GetProfile(Z_STRVAL_P(prop_value));
		Routino_ValidateProfile(r_obj->db,prof);
		if (!prof || Routino_errno!=ROUTINO_ERROR_NONE){
			zend_throw_exception(zend_ce_exception, "Invalid transport profile", 0);
			return;		
		}		
	} else {
		zend_throw_exception(zend_ce_exception, "Invalid profile value", 0);
	}
	// load and convert transalation
	prop_value = zend_read_property(php_routino_ce, object, "lng", sizeof("lng") - 1,0,&rv);
	if (Z_TYPE_P(prop_value) == IS_STRING){
		convert_to_string(prop_value);
		rt=Routino_GetTranslation(Z_STRVAL_P(prop_value));
		if (!rt || Routino_errno!=ROUTINO_ERROR_NONE){
			zend_throw_exception(zend_ce_exception, "Invalid translations sets", 0);
			return;
		}		
	} else {
		zend_throw_exception(zend_ce_exception, "Invalid translations sets", 0);
	}
	key1 = zend_string_init("lat", strlen("lat"), 0);
	key2 = zend_string_init("lon", strlen("lon"), 0);
	// foreach array and prepare waypoints to calculate route
	wps=ecalloc(1,sizeof(char *)*wp_count);
	for (i = 0; i < wp_count; i++) // nullable ptrs array
		wps[i] = NULL;
	i=0;
	ZEND_HASH_FOREACH_VAL(waypoints, data)
		if (Z_TYPE_P(data) == IS_ARRAY){
			if ((lat=zend_hash_find(Z_ARRVAL_P(data),key1)) && (lon=zend_hash_find(Z_ARRVAL_P(data),key2))) {
				//convert_to_double(lat);
				ZVAL_COPY_VALUE(&lon1, lon);
				convert_to_double(&lon1);
				ZVAL_COPY_VALUE(&lat1, lat);
				convert_to_double(&lat1);
				double lond=Z_DVAL(lon1);
				double latd=Z_DVAL(lat1);
				wp=Routino_FindWaypoint(r_obj->db,prof,latd,lond);
				if (wp!=NULL){
					// add vlalid waypoint
					wps[i]=wp;
					i++;
				}
				php_printf("lat lon found %f %f \n",latd,lond);
			}
		}
	ZEND_HASH_FOREACH_END();
	
	if (i>1){
		// we have valid waypoints to calcuate route
		route=Routino_CalculateRoute(r_obj->db,prof,rt,wps,i,ROUTINO_ROUTE_SHORTEST | ROUTINO_ROUTE_LIST_TEXT_ALL,NULL);
		wp_count2=0;
		if (route!=NULL && Routino_errno==ROUTINO_ERROR_NONE){
			array_init(return_value); // lets form return  array
			array_init(&rarray);
			route_first=route;
			while(route!=NULL){
				array_init(&onepoint);
				// fill one point of route
				add_assoc_double(&onepoint,"lat",radians_to_degrees(route->lat));
				add_assoc_double(&onepoint,"lon",radians_to_degrees(route->lon));
				add_assoc_double(&onepoint,"latr",route->lat);
				add_assoc_double(&onepoint,"lonr",route->lon);
				add_assoc_double(&onepoint,"dist",route->dist);
				add_assoc_double(&onepoint,"time",route->time);
				add_assoc_double(&onepoint,"speed",route->speed);
				char_to_zval_array("name",route->name,&onepoint);
				char_to_zval_array("desc1",route->desc1,&onepoint);
				char_to_zval_array("desc2",route->desc2,&onepoint);
				char_to_zval_array("desc3",route->desc3,&onepoint);
				distance=route->dist;
				tm=route->time;
				route=route->next;
				wp_count2++;
				// link one point into route data array
				add_next_index_zval(&rarray,&onepoint);
			}
			add_assoc_zval(return_value, "route", &rarray);
			add_assoc_double(return_value,"distance",distance);
			add_assoc_double(return_value,"time",tm);
			add_assoc_long(return_value,"waypoints",wp_count2);
			// free allocated storages		
			Routino_DeleteRoute(route_first);
		} else {

		}
	}

	// free allocated storages
	i=0;
	while (wps[i]!=NULL && i<wp_count){
		free(wps[i]);
		i++;
	}
	efree(wps); // free waypoints array mem
	zend_string_release(key1);
	zend_string_release(key2);

}

// Array Routino::getLanguages();
PHP_METHOD(Routino, getlanguages) {
	array_init(return_value);
	char **lng=NULL;
	int	c=0;
	lng=Routino_GetTranslationLanguages();
	if (lng)
		while(lng[c]){
			add_next_index_string(return_value,lng[c]);
			c++;
		}
	else RETURN_FALSE;
}

// Array Routino::getLanguagesEx(); // array lng=>full_lng_name
PHP_METHOD(Routino, getlanguagesex) {
	array_init(return_value);
	char **lng=NULL;
	char **flng=NULL;
	int	c=0;
	lng=Routino_GetTranslationLanguages();
	flng=Routino_GetTranslationLanguageFullNames();
	if (lng)
		while(lng[c]){
			add_assoc_string(return_value,lng[c],flng[c]);
			c++;
		}
	else RETURN_FALSE;
}

// Array Routino->getProfiles();
PHP_METHOD(Routino, getprofiles) {
	array_init(return_value);
	char **profs=NULL;
	int	c=0;
	profs=Routino_GetProfileNames();
	if (profs)
		while(profs[c]){
			add_next_index_string(return_value,profs[c]);
			c++;
		}
	else RETURN_FALSE;
}


// Array Routino::setLngFile($lngfile);  set language file
PHP_METHOD(Routino, setlngfile) {
	php_printf("Static method called \n");
}

// Array Routino::setLngFile($lngfile);  set language file
PHP_METHOD(Routino, setlasterror) {
	php_printf("Called protected method \n");
}


/* {{{ routino_functions[]
 *
 * Every user visible function must have an entry in routino_functions[].
 */
 ZEND_BEGIN_ARG_INFO_EX(arginfo_void, 0, 0, 0)
 ZEND_END_ARG_INFO()
 
 ZEND_BEGIN_ARG_INFO_EX(arginfo_set, 0, 0, 1)
	 ZEND_ARG_INFO(0, value)
 ZEND_END_ARG_INFO()

static const zend_function_entry routino_methods[] = {
	PHP_ME(Routino, open, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Routino, close, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Routino, calculate, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Routino, getlanguages, NULL, ZEND_ACC_PUBLIC) // return array of string
	PHP_ME(Routino, getlanguagesex, NULL, ZEND_ACC_PUBLIC) // return array of string
	PHP_ME(Routino, getprofiles, NULL, ZEND_ACC_PUBLIC) // return array of string	
	/* Aliases */
	PHP_MALIAS(Routino,	__construct, open, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)

	/*  Static methods  */
	PHP_ME(Routino, setlngfile, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC) // return array of string

	/* Protected */
	PHP_ME(Routino, setlasterror, NULL, ZEND_ACC_PROTECTED) // setLastError property

	// properties
	PHP_ME(Routino, getdir, arginfo_void, ZEND_ACC_PUBLIC)
	PHP_ME(Routino, getprefix, arginfo_void, ZEND_ACC_PUBLIC)
	PHP_ME(Routino, getlng, arginfo_void, ZEND_ACC_PUBLIC)
	PHP_ME(Routino, getprofile, arginfo_void, ZEND_ACC_PUBLIC)
	PHP_ME(Routino, getlasterror, arginfo_void, ZEND_ACC_PUBLIC)
    PHP_FE_END
};
/* }}} */

// set default to user profile
void fill_dafult_user_profile(Routino_UserProfile *upf) /* {{{ */
	{
		if (!upf){
			return;
		}
		upf->transport=Transport_Motorcar;
		upf->oneway=1;
		upf->turns=1;
		upf->weight=1;
		upf->height=1.5;
		upf->width=1.2;
		upf->length=2;
		for(int i;i<14;i++){
			upf->highway[i]=0;
			upf->speed[i]=80;
		}
		for(int i;i<7;i++){
			upf->props[i]=0;
		}		
	}
/* }}} */

void reset_routino_object(php_routino_object *object){
	object->uprof=NULL;
	object->db=NULL;
	object->initialised=0;
	object->dbloaded=0;
}

// allocate memory for Routino Object
static zend_object *php_routino_object_new(zend_class_entry *class_type) /* {{{ */
	{
		php_routino_object *intern;
		char **lng;
	
		/* Allocate memory for it */
		intern = ecalloc(1, sizeof(php_routino_object) + zend_object_properties_size(class_type));
		reset_routino_object(intern);
		zend_object_std_init(&intern->zo, class_type); // init standart values for object
		object_properties_init(&intern->zo, class_type);
	
		intern->zo.handlers = &routino_object_handlers;
		intern->uprof=ecalloc(1, sizeof(Routino_UserProfile)); // profile memory allocated
		fill_dafult_user_profile(intern->uprof);

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
			intern->db=NULL;
			if (intern->uprof)
				efree(intern->uprof);
			intern->initialised = 0;
			intern->dbloaded = 0;
		}	
		zend_object_std_dtor(&intern->zo);
		efree(intern);
	}
/* }}} */

static void register_constants (int module_number){

}

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(routino)
{
	php_printf("entry %d \n",routino_module_entry.module_number);
	zend_class_entry ce;
	// standart handlers for object
	memcpy(&routino_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	/* Register Routino Class */
	INIT_CLASS_ENTRY(ce, "Routino", routino_methods);
	ce.create_object = php_routino_object_new; // my own fucntion for allocate memory
	routino_object_handlers.offset = XtOffsetOf(php_routino_object, zo);
	routino_object_handlers.clone_obj = NULL;
	routino_object_handlers.free_obj = php_routino_object_free_storage; // my own function for free storage
	php_routino_ce = zend_register_internal_class(&ce);	

#include "php_routino_constants.h"


	// load routino profiles and languages from standart files
	Routino_ParseXMLProfiles(PHP_ROUTINO_PROFILES_FILE);
	Routino_ParseXMLTranslations(PHP_ROUTINO_TRANSLATES_FILE);

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

	// unload some loaded data
	Routino_FreeXMLProfiles;
	Routino_FreeXMLTranslations;
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
	php_info_print_table_header(2, "Routino support", "enabled");
	php_info_print_table_end();
	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
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
	PHP_RINIT(routino),		/* Init module */
	PHP_RSHUTDOWN(routino),	/* Shutdown module */
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
