dnl Process this file with autoconf to produce a configure script.
AC_INIT(Indri, 2.2, indri-info@ciir.cs.umass.edu)
AC_CONFIG_SRCDIR(indri.sln)
AC_CONFIG_FILES(MakeDefns Makefile.app)
dnl To use a header file instead of command line -D defs, use
dnl AC_CONFIG_HEADERS(config.h)
dnl canonical host
AC_CANONICAL_HOST

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_LEX
AC_PROG_INSTALL
AC_PATH_PROGS(AR, ar gar)
if test -z "$AR" ; then
   AC_MSG_ERROR([cannot find the program ar on your path. Add AR=/path/to/ar to your configure command line])
fi
AC_SUBST(SHARED)

dnl Checks for libraries.
AC_CHECK_LIB(m, log)
AC_CHECK_LIB(pthread, pthread_create)

case $host in
   *-*-solaris*) 
     AC_CHECK_LIB(nsl, gethostbyname)
     AC_CHECK_LIB(socket, socket)
     SHARED="-shared -fPIC"
     SWIG_SHARED="-G"
     ;;
   *linux*) 
     SHARED="-shared -fPIC"
     SWIG_SHARED="-shared"
     ;;
   *-*-darwin*)
     SHARED=
     SWIG_SHARED=
     ;;
   *);;
esac

AC_CHECK_LIB(z, deflate)
AC_CHECK_LIB(iberty, mkstemps)

dnl Checks for header files.
dnl Checks for typedefs, structures, and compiler characteristics.
dnl AC_CHECK_SIZEOF(int)
dnl AC_CHECK_SIZEOF(void *)
dnl AC_CHECK_SIZEOF(unsigned long)
dnl AC_CHECK_SIZEOF(unsigned long long)
AC_C_BIGENDIAN
AC_SYS_LARGEFILE
AC_FUNC_FSEEKO

dnl Checks for library functions.
AC_CHECK_FUNCS(mkstemp)
AC_CHECK_FUNCS(mkstemps)

dnl check for optional components.

AC_ARG_ENABLE(php,
	    AC_HELP_STRING([--enable-php],
	                   [Enable php shared library generation]))
if test "$enable_php" = "yes" ; then
  WITH_PHP=1
else
  WITH_PHP=0
fi
AC_ARG_ENABLE(java,
	    AC_HELP_STRING([--enable-java],
	                   [Enable java shared library generation]))
if test "$enable_java" = "yes" ; then
  WITH_JAVA=1
else
  WITH_JAVA=0
fi

AC_ARG_ENABLE(assert,
	    AC_HELP_STRING([--enable-assert],
	                   [Enable assert statements in code]),
			   [if test "${enable_assert}" = "no" ; then
			    AC_DEFINE(NDEBUG, 1, [Turn off assert statements])
			   fi],
			   AC_DEFINE(NDEBUG, 1, [Turn off assert statements]))

dnl Look for java
if test "$enable_java" = "yes" ; then
   AC_ARG_WITH(javahome, [  --with-javahome=path       Set location of JAVAHOME],[JAVAHOME="$withval"; JAVACBIN=$JAVAHOME/bin/javac; JAVAINCDIR=$JAVAHOME/include; JAR=$JAVAHOME/bin/jar], [JAVAHOME=])
   
   if test -z "$JAVACBIN" ; then
     AC_PATH_PROGS(JAVAC, javac)
     AC_PATH_PROGS(JAR, jar)
   else
     JAVAC="$JAVACBIN"
   fi
   AC_MSG_CHECKING(for java include file jni.h)
   if test -z "$JAVAINCDIR"; then
     JAVAINCDIR="/usr/j2sdk*/include /usr/local/j2sdk*/include /usr/jdk*/include /usr/local/jdk*/include /opt/j2sdk*/include /opt/jdk*/include /usr/java/include /usr/java/j2sdk*/include /usr/java/jdk*/include /usr/local/java/include /opt/java/include /usr/include/java /usr/local/include/java /usr/lib/java/include /System/Library/Frameworks/JavaVM.framework/Headers/"
   fi
   JAVAINC=""
   for d in $JAVAINCDIR ; do
     if test -r $d/jni.h ; then
       AC_MSG_RESULT($d)
       JAVAINCDIR=$d
       JAVAINC="-I$d"
       break
     fi
   done
dnl only disable if no java includes or no javac
   if test "$JAVAINC" = "" -o -z "$JAVAC" ; then
      AC_MSG_RESULT(not found disabling swig java wrappers)
      WITH_JAVA=0
      enable_java=no
   else
dnl now look for <arch>/jni_md.h
      AC_MSG_CHECKING(for java include file jni_md.h)
      JAVAMDDIR=`find $JAVAINCDIR -follow -name jni_md.h -print`
      if test "$JAVAMDDIR" = "" ; then
	 AC_MSG_RESULT(not found)
	 WITH_JAVA=0
         enable_java=no
      else
	 JAVAMDDIR=`dirname $JAVAMDDIR`
	 JAVAINC="${JAVAINC} -I$JAVAMDDIR"
	 AC_MSG_RESULT($JAVAMDDIR)
	 WITH_JAVA=1
      fi
   fi
else
   JAVAC=
   JAR=
   JAVAINC=
   JAVAHOME=
   WITH_JAVA=0
fi
AC_SUBST(JAVAC)
AC_SUBST(JAR)
AC_SUBST(JAVAINC)
AC_SUBST(JAVAHOME)
AC_SUBST(WITH_JAVA)

dnl Look for php
if test "$enable_php" = "yes" ; then
   AC_ARG_WITH(php-config, [  --with-php-config=path       Set location of php-config],[PHPCONFIG="$withval"], [PHPCONFIG=])
   if test -z "$PHPCONFIG" ; then
      AC_PATH_PROG(PHPCONFIG, php-config)
   fi
   if test "$PHPCONFIG" = ""; then
      AC_MSG_WARN($PHPCONFIG not found disabling swig php wrappers)
      WITH_PHP=0
      enable_php=no
      PHPINCLUDE=
   else
      AC_MSG_CHECKING(for php includes)
      PHPINCLUDE=`$PHPCONFIG --includes`
      AC_MSG_RESULT(found)
      WITH_PHP=1
   fi
else
   PHPINCLUDE=
   WITH_PHP=0
fi
AC_SUBST(PHPINCLUDE)
AC_SUBST(WITH_PHP)

dnl Look for swig
SWIG=
if test "$enable_java" = yes -o "$enable_php" = "yes" ; then
  WITH_SWIG=1
  AC_ARG_WITH(swig, [  --with-swig=path       Set location of swig executable],[SWIG="$withval"])
  if test -z "$SWIG" ; then
     AC_PATH_PROGS(SWIG, swig)
  fi
else
  WITH_SWIG=0
fi
AC_SUBST(SWIG)
AC_SUBST(WITH_SWIG)
AC_SUBST(SWIG_SHARED)
AC_OUTPUT
