m4_define([MAJOR_V], [0])
m4_define([MINOR_V], [1])
m4_define([PATCH_V], [0])

AC_INIT([libigni], MAJOR_V.MINOR_V.PATCH_V)
CFLAGS="$CFLAGS -DMAJ_V="MAJOR_V" -DMIN_V="MINOR_V" -DPATCH_V="PATCH_V
AC_PREFIX_DEFAULT(/usr)

AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])

AC_PROG_CC
AC_PROG_RANLIB

AM_PROG_AR

AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_FILES([
	Makefile
	src/Makefile
	libigni.pc
])

AC_OUTPUT

