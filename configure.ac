dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(linkgen, 0.1.4, code@eth.si)


AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE()
AM_PROG_CC_C_O

AC_CONFIG_FILES(Makefile src/Makefile)

AC_CONFIG_SRCDIR([src/])
#AC_CONFIG_HEADERS([config.h])

# Checks for programs.

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lpthread':
AC_CHECK_LIB([pthread], [pthread_create],,
			AC_MSG_ERROR([pthread library is missing!]))

# FIXME: Replace `main' with a function in `-lrt':
AC_CHECK_LIB([rt], [clock_gettime],,
			AC_MSG_ERROR([rt library is missing!]))
				
# Checks for header files.
AC_CHECK_HEADERS([stdio.h stdarg.h pthread.h unistd.h errno.h dirent.h time.h \
				  	signal.h sys/syscall.h string.h regex.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UNSIGNED_LONG_LONG_INT
AC_TYPE_LONG_LONG_INT
AC_TYPE_SIGNAL


# Checks for library functions.
AC_CHECK_FUNCS([pthread_create pthread_cancel pthread_join \
				bzero clock_gettime memcpy memmove memset \
				regcomp strerror usleep sleep syscall \
				printf sprintf vsprintf vprintf fflush daemon \
				regcomp regexec pthread_mutex_unlock \
				pthread_mutex_lock signal readdir opendir fopen \
				fclose],,
				AC_MSG_ERROR([function not found!]))


AC_OUTPUT

