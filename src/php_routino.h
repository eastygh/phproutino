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

#ifndef PHP_ROUTINO_H
#define PHP_ROUTINO_H

extern zend_module_entry routino_module_entry;
#define phpext_routino_ptr &routino_module_entry

#define PHP_ROUTINO_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_ROUTINO_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_ROUTINO_API __attribute__ ((visibility("default")))
#else
#	define PHP_ROUTINO_API
#endif


// default files
#ifdef PHP_WIN32
#	define PHP_ROUTINO_TRANSLATES_FILE "c:/Program Files/Routino/xml/translations.xml"
#	define PHP_ROUTINO_PROFILES_FILE "c:/Program Files/Routino/xml/profiles.xml"
#else
#	define PHP_ROUTINO_TRANSLATES_FILE "/usr/local/share/routino/translations.xml"
#	define PHP_ROUTINO_PROFILES_FILE "/usr/local/share/routino/profiles.xml"
#endif

#ifdef ZTS
#include "TSRM.h"
#endif


zend_class_entry *php_routino_ce;

ZEND_BEGIN_MODULE_GLOBALS(routino)
  char  *pfname;
  int pfloaded;
  char  *lngfname;
  int lngloaded;
ZEND_END_MODULE_GLOBALS(routino)

/* Always refer to the globals in your function as ROUTINO_G(variable).
   You are encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/
#define ROUTINO_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(routino, v)

#if defined(ZTS) && defined(COMPILE_DL_ROUTINO)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif	/* PHP_ROUTINO_H */