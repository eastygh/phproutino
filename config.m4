dnl $Id$
dnl config.m4 for extension routino

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(routino, for routino support,
Make sure that the comment is aligned:
[  --with-routino             Include routino support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(routino, whether to enable routino support,
dnl Make sure that the comment is aligned:
dnl [  --enable-routino           Enable routino support])

if test "$PHP_ROUTINO" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-routino -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/routino.h"  # you most likely want to change this
  dnl if test -r $PHP_ROUTINO/$SEARCH_FOR; then # path given as parameter
  dnl   ROUTINO_DIR=$PHP_ROUTINO
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for routino files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       ROUTINO_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$ROUTINO_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the routino distribution])
  dnl fi

  dnl # --with-routino -> add include path

PHP_ADD_INCLUDE($ROUTINO_DIR/include)
PHP_ADD_INCLUDE(/usr/local/include)
PHP_ADD_LIBRARY_WITH_PATH(routino, /usr/local/lib, ROUTINO_SHARED_LIBADD)
PHP_SUBST(ROUTINO_SHARED_LIBADD)

  dnl # --with-routino -> check for lib and symbol presence
  dnl LIBNAME=routino # you may want to change this
  dnl LIBSYMBOL=routino # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $ROUTINO_DIR/$PHP_LIBDIR, ROUTINO_SHARED_LIBADD)
AC_DEFINE(HAVE_ROUTINOLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong routino lib version or lib not found])
  dnl ],[
  dnl   -L$ROUTINO_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(ROUTINO_SHARED_LIBADD)

  PHP_NEW_EXTENSION(routino, src/php_routino.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
