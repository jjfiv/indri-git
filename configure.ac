dnl Process this file with autoconf to produce a configure script.
AC_INIT(Indri, 1.0, indri-info@ciir.cs.umass.edu)
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
AC_CHECK_PROGS(AR, ar gar)

AC_SUBST(HOST_OS)
dnl Checks for libraries.

dnl If/when we add client server
case $host in
   *-*-solaris*) 
     AC_CHECK_LIB(nsl, gethostbyname)
     AC_CHECK_LIB(socket, socket)
     HOST_OS=solaris
     AC_DEFINE(HOST_OS, solaris, [osname for jni build])
     ;;
   *linux*) 
     HOST_OS=linux
     AC_DEFINE(HOST_OS, linux, [osname for jni build])
     ;;
   *);;
esac
AC_CHECK_LIB(z, deflate)

dnl Checks for header files.

dnl Checks for typedefs, structures, and compiler characteristics.

AC_SYS_LARGEFILE

dnl Checks for library functions.

dnl Finish up and output.
AC_SUBST(WITH_SWIG)

AC_ARG_ENABLE(swig,
	    AC_HELP_STRING([--enable-swig],
	                   [Enable swig wrapper generation]))
if test "$enable_swig" = "yes" ; then
  WITH_SWIG=1
  AC_DEFINE(WITH_SWIG, 1, [Enable swig wrapper generation])
else
  WITH_SWIG=0
  AC_DEFINE(WITH_SWIG, 0, [Enable swig wrapper generation])
fi

AC_ARG_ENABLE(assert,
	    AC_HELP_STRING([--enable-assert],
	                   [Enable assert statements in code]),
			   [if test "${enable_assert}" = "no" ; then
			    AC_DEFINE(NDEBUG, 1, [Turn off assert statements])
			   fi],
			   AC_DEFINE(NDEBUG, 1, [Turn off assert statements]))

if test "$enable_swig" = "yes" ; then
# Look for swig
  AC_ARG_WITH(swig, [  --with-swig=path       Set location of swig executable],[SWIG="$withval"], [SWIG=])

  if test -z "$SWIG" ; then
     AC_CHECK_PROGS(SWIG, swig)
  fi
fi
AC_SUBST(SWIG)

#----------------------------------------------------------------
# Look for java
#----------------------------------------------------------------
if test "$enable_swig" = "yes" ; then
   AC_ARG_WITH(javahome, [  --with-javahome=path       Set location of JAVAHOME],[JAVAHOME="$withval"; JAVACBIN=$JAVAHOME/bin/javac; JAVAINCDIR=$JAVAHOME/include; JAR=$JAVAHOME/bin/jar], [JAVAHOME=])
   
   if test -z "$JAVACBIN" ; then
     AC_CHECK_PROGS(JAVAC, javac)
   else
     JAVAC="$JAVACBIN"
   fi
   AC_MSG_CHECKING(for java include file jni.h)
   if test -z "$JAVAINCDIR"; then
     JAVAINCDIR="/usr/j2sdk*/include /usr/local/j2sdk*/include /usr/jdk*/include /usr/local/jdk*/include /opt/j2sdk*/include /opt/jdk*/include /usr/java/include /usr/java/j2sdk*/include /usr/java/jdk*/include /usr/local/java/include /opt/java/include /usr/include/java /usr/local/include/java /usr/lib/java/include"
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
## only disable if no java or php
   if test "$JAVAINC" = "" ; then
      AC_MSG_RESULT(not found disabling swig java wrappers)
      WITH_JAVA=0
      AC_DEFINE(WITH_JAVA, 0, [Enable swig java wrapper generation])
   else
      # now look for <arch>/jni_md.h
      AC_MSG_CHECKING(for java include file jni_md.h)
      JAVAMDDIR=`find $JAVAINCDIR -follow -name jni_md.h -print`
      if test "$JAVAMDDIR" = "" ; then
	 AC_MSG_RESULT(not found)
	 WITH_JAVA=0
	 AC_DEFINE(WITH_JAVA, 0, [Enable swig java wrapper generation])
      else
	 JAVAMDDIR=`dirname $JAVAMDDIR`
	 JAVAINC="${JAVAINC} -I$JAVAMDDIR"
	 AC_MSG_RESULT($JAVAMDDIR)
	 WITH_JAVA=1
	 AC_DEFINE(WITH_JAVA, 1, [Enable swig java wrapper generation])
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

#----------------------------------------------------------------
# Look for php
#----------------------------------------------------------------
if test "$enable_swig" = "yes" ; then
   AC_ARG_WITH(php-config, [  --with-php-config=path       Set location of php-config],[PHPCONFIG="$withval"], [PHPCONFIG=])
   if test -z "$PHPCONFIG" ; then
      AC_CHECK_PROGS(PHPCONFIG, php-config)
   fi
   AC_MSG_CHECKING(for php includes)
   if test "$PHPCONFIG" = "" ; then
      AC_MSG_RESULT(not found disabling swig php wrappers)
      WITH_PHP=0
      PHPINCLUDE=""
      AC_DEFINE(WITH_PHP, 0, [Enable swig php wrapper generation])
   else
      PHPINCLUDE=`$PHPCONFIG --includes`
      AC_MSG_RESULT(found)
      WITH_PHP=1
      AC_DEFINE(WITH_PHP, 1, [Enable swig php wrapper generation])
   fi
else
   PHPINCLUDE=
   WITH_PHP=0
fi
AC_SUBST(PHPINCLUDE)
AC_SUBST(WITH_PHP)
##
AC_OUTPUT
