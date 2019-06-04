AC_PREREQ([2.69])
AC_INIT([libeXosip2],[5.0.0])
AC_CONFIG_SRCDIR([include/eXosip2/eXosip.h])

AC_MSG_NOTICE([libeXosip2              The Extended Open SIP library.])
AC_MSG_NOTICE([Copyright (C) 2002-2015 Aymeric MOIZARD - <amoizard@antisip.com>])

AC_SUBST(LIBEXOSIP_SO_VERSION, 12:0:0)
AC_SUBST(EXOSIP_VERSION, $VERSION)

AC_MSG_RESULT([Configuring ${PACKAGE} ${EXOSIP_VERSION}])
OS=`uname|sed -e 'y/ABCDEFGHIJKLMNOPQRSTUVWXYZ/abcdefghijklmnopqrstuvwxyz/'`
AC_MSG_RESULT([Building Package on ${OS}])


dnl *********************************************************************
dnl Initialize automake stuff
AC_CONFIG_AUX_DIR(scripts)
AC_CONFIG_HEADERS(exosip-config.h)
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

AC_ARG_ENABLE(openssl,
  [AS_HELP_STRING([--enable-openssl],
    [enable support for openssl @<:@default=yes@:>@])],
  enable_openssl=$enableval,enable_openssl="yes")

dnl build with multithreaded support (need semaphore).
AC_ARG_ENABLE(mt,
  [AS_HELP_STRING([--enable-mt],
    [compile with multi-thread support @<:@default=yes@:>@])],
  enable_mt=$enableval,enable_mt="yes")

AC_ARG_ENABLE(srvrec,
  [AS_HELP_STRING([--enable-srvrec],
    [enable support for SRV records DNS queries @<:@default=yes@:>@])],
  enable_srvrec=$enableval,enable_srvrec="yes")

AC_ARG_ENABLE(tools,
  [AS_HELP_STRING([--enable-tools],
    [compile tools programs @<:@default=yes@:>@])],
  enable_tools=$enableval,enable_tools="yes")

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
    EXOSIP_FLAGS="$EXOSIP_FLAGS -DOSIP_MONOTHREAD"
fi

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
     ;;
   irix*)
     ;;
   hpux* | hp-ux*)
     ;;
   aix*)
     ;;
   osf*)
     AC_CHECK_LIB(rt,sem_open,[EXOSIP_LIB="$EXOSIP_LIB -lrt"])
     ;;
   sunos*)
     ;;
   darwin*)
     ;;
   *mingw*)
     EXOSIP_FLAGS="$EXOSIP_FLAGS  -D_WIN32_WINNT=0x0501"
     EXOSIP_LIB="EXOSIP_LIB -lws2_32 -liphlpapi -ldnsapi"
dnl tools are not ported to win32
     enable_tools=no
     ;;
   *)
     ;;
 esac
else
 AC_MSG_NOTICE([Cross compiling exosip.])
fi

if test "x$enable_trace" = "xyes"; then
  EXOSIP_FLAGS="$EXOSIP_FLAGS -DENABLE_TRACE"
fi

if test "x$enable_mpatrol" = "xyes"; then
  EXOSIP_FLAGS="$EXOSIP_FLAGS -DENABLE_MPATROL"
  EXOSIP_LIB="$EXOSIP_LIB -lmpatrolmt -lelf"
fi

if test "x$enable_gprof" = "xyes"; then
  EXOSIP_FLAGS="$EXOSIP_FLAGS -pg"
fi

if test "x$enable_minisize" = "xyes"; then
  EXOSIP_FLAGS="$EXOSIP_FLAGS -DMINISIZE"
fi

AM_CONDITIONAL(BUILD_MAXSIZE, test x$enable_minisize = xno)
AM_CONDITIONAL(COMPILE_TOOLS, test "x$enable_tools" = "xyes")

dnl Checks for libraries.
AC_CHECK_LIB(posix4,sem_open,[EXOSIP_LIB="$EXOSIP_LIB -lposix4 -mt"])
AC_CHECK_LIB(nsl,nis_add,[EXOSIP_LIB="$EXOSIP_LIB -lnsl"])
AC_CHECK_LIB(socket,sendto,[EXOSIP_LIB="$EXOSIP_LIB -lsocket"])
AC_CHECK_LIB(rt,clock_gettime,[EXOSIP_LIB="$EXOSIP_LIB -lrt"])

case $OS in
  openbsd*)
     ;;
  *)
     AC_CHECK_LIB(resolv,res_query,[EXOSIP_LIB="$EXOSIP_LIB -lresolv"])
     AC_CHECK_LIB(resolv,__res_query,[EXOSIP_LIB="$EXOSIP_LIB -lresolv"])
     ;;
esac

AC_CHECK_LIB([cares], [ares_init],
     [AC_CHECK_HEADERS(ares.h,
	[EXOSIP_LIB="$EXOSIP_LIB -lcares"],
	[AC_MSG_WARN([c-ares header files are not installed])])
     ],
     [AC_MSG_WARN([c-ares library is not installed])]
)

PKG_CHECK_MODULES(OSIP, libosip2 >= 5.0, [], [
AC_MSG_WARN([assuming osip can be found in -I$includedir and -L$libdir])
OSIP_CFLAGS='-I$(includedir) -I$(prefix)/include'
OSIP_LIBS='-L$(libdir) -L$(prefix)/lib -losip2 -losipparser2'
])

EXOSIP_FLAGS="$EXOSIP_FLAGS"

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

AC_SUBST(EXOSIP_FLAGS)
AC_SUBST(EXOSIP_LIB)

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(ctype.h)
AC_CHECK_HEADERS(string.h)
AC_CHECK_HEADERS(strings.h)
AC_CHECK_HEADERS(stdio.h)
AC_CHECK_HEADERS(stdlib.h)
AC_CHECK_HEADERS(limits.h)
AC_CHECK_HEADERS(unistd.h)
AC_CHECK_HEADERS(stdarg.h)
AC_CHECK_HEADERS(varargs.h)
AC_CHECK_HEADERS(sys/time.h)
AC_CHECK_HEADERS(assert.h)
AC_CHECK_HEADERS(signal.h)
AC_CHECK_HEADERS(sys/signal.h)
AC_CHECK_HEADERS(malloc.h)
AC_CHECK_HEADERS(sys/select.h)
AC_CHECK_HEADERS(sys/types.h)
AC_CHECK_HEADERS(fcntl.h)
AC_CHECK_HEADERS(sys/soundcard.h)
AC_CHECK_HEADERS(arpa/inet.h)
AC_CHECK_HEADERS(arpa/nameser.h)
AC_CHECK_HEADERS(netdb.h)
AC_CHECK_HEADERS(nameser8_compat.h)
AC_CHECK_HEADERS(arpa/nameser_compat.h)
AC_CHECK_HEADERS(netinet/tcp.h)

AC_CHECK_HEADERS([resolv.h], [], [],
[#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#if HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#if HAVE_ARPA_NAMESER_H
#include <arpa/nameser.h>
#endif
])

dnl check if we have the getifaddrs() sytem call
AC_CHECK_FUNCS(getifaddrs)

AS_IF([test "$enable_openssl" = "yes" -o "$enable_openssl" = "auto"],
      [PKG_CHECK_MODULES([SSL], [openssl],
                         [EXOSIP_LIB="$EXOSIP_LIB $SSL_LIBS"]
                         AC_CHECK_HEADERS(openssl/ssl.h),
                         [AS_IF([test "$enable_openssl" = "yes"],
                                [AC_MSG_WARN([can not find openssl with pkgconfig])
				AC_CHECK_HEADERS(openssl/ssl.h)
				AC_CHECK_LIB(ssl,SSL_CTX_new,[EXOSIP_LIB="$EXOSIP_LIB -lssl"])
				AC_CHECK_LIB(crypto,MD5,[EXOSIP_LIB="$EXOSIP_LIB -lcrypto"])
				])
                         ])
      ])

if test "x$enable_srvrec" = "xyes" ; then
  AC_DEFINE([SRV_RECORD], [1], [Defined to 1 if compiled with NAPTR and SRV support.])
fi

AC_CONFIG_FILES([Makefile
 scripts/Makefile
 platform/Makefile
 platform/vsnet/Makefile
 include/Makefile
 include/eXosip2/Makefile
 src/Makefile
 tools/Makefile 
 help/Makefile
 help/doxygen/Makefile
 help/doxygen/doxygen.dox
])
AC_OUTPUT

