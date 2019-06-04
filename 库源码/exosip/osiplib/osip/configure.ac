AC_PREREQ([2.69])
AC_INIT([libosip2],[5.0.0])
AC_CONFIG_SRCDIR([include/osip2/osip.h])

AC_MSG_NOTICE([libosip2                The GNU Open SIP library.])
AC_MSG_NOTICE([Copyright (C) 2001-2015 Aymeric MOIZARD - <amoizard@antisip.com>])

AC_SUBST(LIBOSIP_SO_VERSION, 12:0:0)
AC_SUBST(OSIP_VERSION, $VERSION)

AC_MSG_RESULT([Configuring ${PACKAGE} ${VERSION}])
OS=`uname|sed -e 'y/ABCDEFGHIJKLMNOPQRSTUVWXYZ/abcdefghijklmnopqrstuvwxyz/'`
AC_MSG_RESULT([Building Package on ${OS}])


dnl *********************************************************************
dnl Initialize automake stuff
AC_CONFIG_AUX_DIR(scripts)
AC_CONFIG_HEADERS(osip-config.h)
AM_INIT_AUTOMAKE
AC_CONFIG_MACRO_DIR([scripts])

AC_CANONICAL_HOST                                                               

dnl Checks for programs.

AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL


dnl Initialize libtool
AC_PROG_SED
LT_INIT

dnl declare --enable-* args and collect ac_help strings

AC_ARG_ENABLE(mpatrol,
  [AS_HELP_STRING([--enable-mpatrol],
    [turn on memory leak detection with patrol @<:@default=no@:>@])],
  enable_mpatrol=$enableval,enable_mpatrol="no")

AC_ARG_ENABLE(gprof,
  [AS_HELP_STRING([--enable-gprof],
    [turn on profiling support @<:@default=no@:>@])],
  enable_gprof=$enableval,enable_gprof="no")

AC_ARG_ENABLE(trace,
  [AS_HELP_STRING([--enable-trace],
    [turn on trace @<:@default=yes@:>@])],
  enable_trace=$enableval,enable_trace="yes")

dnl minimize size of code.
AC_ARG_ENABLE(minisize,
  [AS_HELP_STRING([--enable-minisize],
    [only compile minimal voip related code @<:@default=no@:>@])], 
  enable_minisize=$enableval,enable_minisize="no")

dnl support for linux-thread or posix thread (pthread.h)
AC_ARG_ENABLE(pthread,
  [AS_HELP_STRING([--enable-pthread=@<:@autodetect|force@:>@],
    [autodetect POSIX threads or force -DHAVE_PTHREAD @<:@default=autodetect@:>@])],
  enable_pthread=$enableval,enable_pthread="autodetect")

dnl support for semaphore.h (linux/sun...)
dnl support for sysV semaphore in sys/sem.h (BSD/linux...)
AC_ARG_ENABLE(semaphore,
  [AS_HELP_STRING([--enable-semaphore=@<:@autodetect|semaphore|sysv@:>@],
    [select your prefered semaphore @<:@default=autodetect@:>@.])],
  enable_semaphore=$enableval,enable_semaphore="autodetect")

dnl support for gperf.
AC_ARG_ENABLE(gperf,
  [AS_HELP_STRING([--enable-gperf],
    [enable support for gperf (improve the parser speed) @<:@default=no@:>@])],
  enable_gperf=$enableval,enable_gperf="no")

dnl libdict support.
AC_ARG_ENABLE(hashtable,
  [AS_HELP_STRING([--enable-hashtable],
    [compile with hashtable (libdict) support @<:@default=no@:>@])],
  enable_hashtable=$enableval,enable_hashtable="no")

dnl build with multithreaded support (need semaphore).
AC_ARG_ENABLE(mt,
  [AS_HELP_STRING([--enable-mt],
    [compile with multi-thread support @<:@default=yes@:>@])],
  enable_mt=$enableval,enable_mt="yes")

dnl support for test suite.
AC_ARG_ENABLE(test,
  [AS_HELP_STRING([--enable-test],
    [enable building test programs @<:@default=no@:>@])],
  enable_test=$enableval,enable_test="no")

dnl compile with mt support
if test "x$enable_mt" = "xyes"; then
  if test "x$enable_pthread" = "xforce"; then
    AC_DEFINE(HAVE_PTHREAD)
  else
    AX_PTHREAD([], [AC_MSG_ERROR([Could not configure pthreads support -use --enable-mt=no to disable or --enable-pthread=force to define HAVE_PTHREAD])])
    LIBS="$PTHREAD_LIBS $LIBS"
    CFLAGS="$CFLAGS $PTHREAD_CFLAGS"
    CC="$PTHREAD_CC"
  fi
else
  SIP_FSM_FLAGS="-DOSIP_MONOTHREAD"
fi

AM_CONDITIONAL(BUILD_MT, test x$enable_mt = xyes)

if test "x$enable_gperf" = "xyes"; then
  SIP_PARSER_FLAGS="$SIP_PARSER_FLAGS -DUSE_GPERF"
fi

AM_CONDITIONAL(COMPILE_TESTS, test "x$enable_test" != "xno")

if test "x$enable_semaphore" = "xsemaphore"; then
  AC_DEFINE(HAVE_SEMAPHORE_H)
elif test "x$enable_semaphore" = "xsysv"; then
  AC_DEFINE(HAVE_SYS_SEM_H)
else
  AC_CHECK_HEADERS(semaphore.h)
  AC_CHECK_HEADERS(sys/sem.h)
fi

if test "x$host_cpu" = "x$build_cpu"; then
  case $OS in
   linux*)
     SIP_EXTRA_FLAGS="$SIP_EXTRA_FLAGS"
     ;;
   irix*)
     ;;
   hpux* | hp-ux*)
     ;;
   aix*)
     ;;
   osf*)
     ;;
   sunos*)
     ;;
   darwin*)
     ;;
   *)
     ;;
 esac
else
 AC_MSG_NOTICE([Cross compiling osip.])
fi

case $host_cpu in
  arc*)
    ;;
  *)
    ;;
esac

if test "x$enable_hashtable" = "xyes"; then
   AC_CHECK_HEADERS([dict/dict.h],
                   [AC_DEFINE([HAVE_DICT_DICT_H], [], [Define to 1 if you have <dict/dict.h> header file])],
                   [AC_MSG_ERROR([libdict library not installed. Please install or use --disable-hashtable])])
   EXTRA_LIB="$EXTRA_LIB -ldict"
fi

if test "x$enable_trace" = "xyes"; then
  SIP_EXTRA_FLAGS="$SIP_EXTRA_FLAGS -DENABLE_TRACE"
fi

if test "x$enable_mpatrol" = "xyes"; then
  SIP_EXTRA_FLAGS="$SIP_EXTRA_FLAGS -DENABLE_MPATROL"
  EXTRA_LIB="$EXTRA_LIB -lmpatrolmt -lelf -lpthread"
fi

if test "x$enable_gprof" = "xyes"; then
  SIP_EXTRA_FLAGS="$SIP_EXTRA_FLAGS -pg"
  EXTRA_LIB="$EXTRA_LIB -lc_p"
fi

if test "x$enable_minisize" = "xyes"; then
  SIP_EXTRA_FLAGS="$SIP_EXTRA_FLAGS -DMINISIZE"
fi
AM_CONDITIONAL(BUILD_MAXSIZE, test x$enable_minisize = xno)

dnl Checks for libraries. (those one are needed for sun)
AC_CHECK_LIB(posix4,sem_open,[FSM_LIB="$FSM_LIB -lposix4 -mt"])
AC_CHECK_LIB(nsl,nis_add,[FSM_LIB="$FSM_LIB -lnsl"])
AC_CHECK_LIB(socket,sendto,[FSM_LIB="$FSM_LIB -lsocket"])
AC_CHECK_LIB(c, lrand48, AC_DEFINE([HAVE_LRAND48], [], [Defined if you have lrand48]))
AC_CHECK_LIB(rt,clock_gettime,[FSM_LIB="$FSM_LIB -lrt"])

AC_CACHE_CHECK([whether -Wall works], Wall_cv_flag, [
  saved_CPPFLAGS=$CPPFLAGS
  CPPFLAGS="-Wall"
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]], [[]])],[Wall_cv_flag=yes],[Wall_cv_flag=no])
  CPPFLAGS=$saved_CPPFLAGS
])

if test "x$Wall_cv_flag" = xyes; then
  CPPFLAGS="$CPPFLAGS -Wall"
fi

AC_CACHE_CHECK([whether -Wcast-align works], Wcast_align_cv_flag, [
  saved_CPPFLAGS=$CPPFLAGS
  CPPFLAGS="-Wcast-align"
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]], [[]])],[Wcast_align_cv_flag=yes],[Wcast_align_cv_flag=no])
  CPPFLAGS=$saved_CPPFLAGS
])

if test "x$Wcast_align_cv_flag" = xyes; then
  CPPFLAGS="$CPPFLAGS -Wcast-align"
fi

AC_CACHE_CHECK([whether -Wchar-subscripts works], Wchar_subscripts_cv_flag, [
  saved_CPPFLAGS=$CPPFLAGS
  CPPFLAGS="-Wchar-subscripts"
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]], [[]])],[Wchar_subscripts_cv_flag=yes],[Wchar_subscripts_cv_flag=no])
  CPPFLAGS=$saved_CPPFLAGS
])

if test "x$Wchar_subscripts_cv_flag" = xyes; then
  CPPFLAGS="$CPPFLAGS -Wchar-subscripts"
fi

AC_CACHE_CHECK([whether -Wformat works], Wformat_cv_flag, [
  saved_CPPFLAGS=$CPPFLAGS
  CPPFLAGS="-Wformat"
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]], [[]])],[Wformat_cv_flag=yes],[Wformat_cv_flag=no])
  CPPFLAGS=$saved_CPPFLAGS
])

if test "x$Wformat_cv_flag" = xyes; then
  CPPFLAGS="$CPPFLAGS -Wformat"
fi

AC_CACHE_CHECK([whether -Winline works], Winline_cv_flag, [
  saved_CPPFLAGS=$CPPFLAGS
  CPPFLAGS="-Winline"
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]], [[]])],[Winline_cv_flag=yes],[Winline_cv_flag=no])
  CPPFLAGS=$saved_CPPFLAGS
])

if test "x$Winline_cv_flag" = xyes; then
  CPPFLAGS="$CPPFLAGS -Winline"
fi

AC_CACHE_CHECK([whether -Wmissing-declarations works], Wmissing_declarations_cv_flag, [
  saved_CPPFLAGS=$CPPFLAGS
  CPPFLAGS="-Wmissing-declarations"
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]], [[]])],[Wmissing_declarations_cv_flag=yes],[Wmissing_declarations_cv_flag=no])
  CPPFLAGS=$saved_CPPFLAGS
])

if test "x$Wmissing_declarations_cv_flag" = xyes; then
  CPPFLAGS="$CPPFLAGS -Wmissing-declarations"
fi

AC_CACHE_CHECK([whether -Wmissing-prototypes works], Wmissing_prototypes_cv_flag, [
  saved_CPPFLAGS=$CPPFLAGS
  CPPFLAGS="-Wmissing-prototypes"
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]], [[]])],[Wmissing_prototypes_cv_flag=yes],[Wmissing_prototypes_cv_flag=no])
  CPPFLAGS=$saved_CPPFLAGS
])

if test "x$Wmissing_prototypes_cv_flag" = xyes; then
  CPPFLAGS="$CPPFLAGS -Wmissing-prototypes"
fi

AC_CACHE_CHECK([whether -Wnested-externs works], Wnested_externs_cv_flag, [
  saved_CPPFLAGS=$CPPFLAGS
  CPPFLAGS="-Wnested-externs"
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]], [[]])],[Wnested_externs_cv_flag=yes],[Wnested_externs_cv_flag=no])
  CPPFLAGS=$saved_CPPFLAGS
])

if test "x$Wnested_externs_cv_flag" = xyes; then
  CPPFLAGS="$CPPFLAGS -Wnested-externs"
fi

AC_CACHE_CHECK([whether -Wpointer-arith works], Wpointer_arith_cv_flag, [
  saved_CPPFLAGS=$CPPFLAGS
  CPPFLAGS="-Wpointer-arith"
  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]], [[]])],[Wpointer_arith_cv_flag=yes],[Wpointer_arith_cv_flag=no])
  CPPFLAGS=$saved_CPPFLAGS
])

if test "x$Wpointer_arith_cv_flag" = xyes; then
  CPPFLAGS="$CPPFLAGS -Wpointer-arith"
fi

dnl SIP_CFLAGS="$SIP_CFLAGS $CFLAGS"

AC_SUBST(SIP_CFLAGS)

AC_SUBST(SIP_EXTRA_FLAGS)
AC_SUBST(SIP_PARSER_FLAGS)
AC_SUBST(SIP_FSM_FLAGS)

AC_SUBST(EXTRA_LIB)
AC_SUBST(PARSER_LIB)
AC_SUBST(FSM_LIB)

dnl Checks for header files.

dnl This is to be removed for autoconf2.50
AC_HEADER_STDC
AC_CHECK_HEADERS(string.h)
AC_CHECK_HEADERS(strings.h)
AC_CHECK_HEADERS(stdlib.h)
AC_CHECK_HEADERS(unistd.h)
AC_CHECK_HEADERS(sys/types.h)
dnl !This is to be removed for autoconf2.50

AC_CHECK_HEADERS(sys/unistd.h)
AC_CHECK_HEADERS(syslog.h)
AC_CHECK_HEADERS(ctype.h)
AC_CHECK_HEADERS(stdio.h)
AC_CHECK_HEADERS(stdarg.h)
AC_CHECK_HEADERS(varargs.h)
AC_CHECK_HEADERS(fcntl.h)
AC_CHECK_HEADERS(time.h)
AC_CHECK_HEADERS(sys/time.h)
AC_CHECK_HEADERS(sys/select.h)
AC_CHECK_HEADERS(assert.h)
AC_CHECK_HEADERS(signal.h)
AC_CHECK_HEADERS(sys/signal.h)
AC_CHECK_HEADERS(malloc.h)

AC_CHECK_TYPES([struct timeval],,,[
     #if TIME_WITH_SYS_TIME
     # include <sys/time.h>
     # include <time.h>
     #else
     # if HAVE_SYS_TIME_H
     #  include <sys/time.h>
     # else
     #  include <time.h>
     # endif
     #endif
])

AC_CONFIG_FILES([libosip2.pc
Makefile
scripts/Makefile
src/Makefile
src/osipparser2/Makefile
src/osip2/Makefile
src/test/Makefile
include/Makefile
include/osip2/Makefile
include/osipparser2/Makefile
include/osipparser2/headers/Makefile
help/Makefile
help/man/Makefile
help/doxygen/Makefile
platform/Makefile
platform/rpm/Makefile
platform/vsnet/Makefile
include/osip2/doxygen.dox])
AC_OUTPUT

