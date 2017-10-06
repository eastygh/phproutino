dnl $Id$
dnl config.m4 for extension routino

PHP_ARG_WITH(routino, for routino support,
Make sure that the comment is aligned:
[  --with-routino             Include routino support])

if test "$PHP_ROUTINO" != "no"; then
PHP_ADD_INCLUDE($ROUTINO_DIR/include)
PHP_ADD_INCLUDE(/usr/local/include)
PHP_ADD_LIBRARY_WITH_PATH(routino, /usr/local/lib, ROUTINO_SHARED_LIBADD)
PHP_SUBST(ROUTINO_SHARED_LIBADD)
AC_DEFINE(HAVE_ROUTINOLIB,1,[ ])
PHP_NEW_EXTENSION(routino, src/php_routino.c src/php_routino_props.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
