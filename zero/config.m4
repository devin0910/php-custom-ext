dnl $Id$
dnl config.m4 for extension zero

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(zero, for zero support,
dnl Make sure that the comment is aligned:
dnl [  --with-zero             Include zero support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(zero, whether to enable zero support,
dnl Make sure that the comment is aligned:
[  --enable-zero           Enable zero support])

if test "$PHP_ZERO" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-zero -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/zero.h"  # you most likely want to change this
  dnl if test -r $PHP_ZERO/$SEARCH_FOR; then # path given as parameter
  dnl   ZERO_DIR=$PHP_ZERO
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for zero files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       ZERO_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$ZERO_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the zero distribution])
  dnl fi

  dnl # --with-zero -> add include path
  dnl PHP_ADD_INCLUDE($ZERO_DIR/include)

  dnl # --with-zero -> check for lib and symbol presence
  dnl LIBNAME=zero # you may want to change this
  dnl LIBSYMBOL=zero # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $ZERO_DIR/$PHP_LIBDIR, ZERO_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_ZEROLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong zero lib version or lib not found])
  dnl ],[
  dnl   -L$ZERO_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(ZERO_SHARED_LIBADD)

  PHP_NEW_EXTENSION(zero, zero.c, $ext_shared)
fi
