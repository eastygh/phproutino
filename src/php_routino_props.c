
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"

#include "php_routino.h"
#include "php_routino_structs.h"
#include "php_routino_types.h"

#include "zend_exceptions.h"
#include "zend_interfaces.h"


// read current dir property
PHP_METHOD(Routino, getdir) {
    zval *prop_value,rv;
	zval *object = getThis();

    prop_value = zend_read_property(php_routino_ce, object, "dir", sizeof("dir") - 1,0,&rv);
    
    RETURN_ZVAL(prop_value, 1, 0);    
}

// read current prefix property
PHP_METHOD(Routino, getprefix) {
    zval *prop_value,rv;
	zval *object = getThis();

    prop_value = zend_read_property(php_routino_ce, object, "prefix", sizeof("prefix") - 1,0,&rv);
    
    RETURN_ZVAL(prop_value, 1, 0);    
}

// get current translation
PHP_METHOD(Routino, getlng) {
    zval *prop_value,rv;
	zval *object = getThis();

    prop_value = zend_read_property(php_routino_ce, object, "lng", sizeof("lng") - 1,0,&rv);
    
    RETURN_ZVAL(prop_value, 1, 0);    
}


// set current translation
PHP_METHOD(Routino, setlng) {
	zval *object = getThis();
    
}

// get current profile
PHP_METHOD(Routino, getprofile) {
    zval *prop_value,rv;
	zval *object = getThis();

    prop_value = zend_read_property(php_routino_ce, object, "profile", sizeof("profile") - 1,0,&rv);
    
    RETURN_ZVAL(prop_value, 1, 0);    
}

// set current profiles
PHP_METHOD(Routino, setprofile) {
	zval *object = getThis();
   
}