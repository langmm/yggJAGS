# ===========================================================================
#
# SYNOPSIS
#
#   AX_YGGINTERFACE([ACTION-IF-FOUND[, ACTION-IF-NOT-FOUND]])
#
# DESCRIPTION
#
#   This macro looks for a library that implements the YggInterface model
#   interface. On success, it sets the YGGINTERFACE_LIBS output variable
#   to hold the requisite library linkages.
#
#   The user may also use --with-ygginterface=<lib> in order to use some 
#   specific YggInterface library <lib>. In order to link successfully,
#   however, be aware that you will probably need to use the same C++
#   compiler as was used to compile the YggInterface library.
#
#   ACTION-IF-FOUND is a list of shell commands to run if a YggInterface
#   library is found, and ACTION-IF-NOT-FOUND is a list of commands to
#   run it if it is not found. If ACTION-IF-FOUND is not specified, the
#   default action will define HAVE_YGGINTERFACE.
#
# LICENSE
#
#   Copyright (c) 2026 Meagan Lang <langmm.astro@gmail.com>
#
#   This program is free software: you can redistribute it and/or modify it
#   under the terms of the GNU General Public License as published by the
#   Free Software Foundation, either version 3 of the License, or (at your
#   option) any later version.
#
#   This program is distributed in the hope that it will be useful, but
#   WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
#   Public License for more details.
#
#   You should have received a copy of the GNU General Public License along
#   with this program. If not, see <http://www.gnu.org/licenses/>.
#
#   As a special exception, the respective Autoconf Macro's copyright owner
#   gives unlimited permission to copy, distribute and modify the configure
#   scripts that are the output of Autoconf when processing the Macro. You
#   need not follow the terms of the GNU General Public License when using
#   or distributing such scripts, even though portions of the text of the
#   Macro appear in them. The GNU General Public License (GPL) does govern
#   all other use of the material that constitutes the Autoconf Macro.
#
#   This special exception to the GPL applies to versions of the Autoconf
#   Macro released by the Autoconf Archive. When you make and distribute a
#   modified version of the Autoconf Macro, you may extend this special
#   exception to the GPL to apply to your modified version as well.

#serial 7

AU_ALIAS([ACX_YGGINTERFACE], [AX_YGGINTERFACE])
AC_DEFUN([AX_YGGINTERFACE], [
ax_ygginterface_ok=no

AC_ARG_WITH(ygginterface,
        [AS_HELP_STRING([--with-ygginterface=<lib>], [use YGGINTERFACE library <lib>])])
case $with_ygginterface in
        yes | "") ;;
        no) ax_ygginterface_ok=disable ;;
        -* | */* | *.a | *.so | *.so.* | *.o) YGGINTERFACE_LIBS="$with_ygginterface" ;;
        *) YGGINTERFACE_LIBS="-l$with_ygginterface" ;;
esac

# Name of YGGINTERFACE function to check for.
yggfunc=yggStateInterface

# First, check YGGINTERFACE_LIBS environment variable
if test "x$YGGINTERFACE_LIBS" != x; then
        save_LIBS="$LIBS"; LIBS="$YGGINTERFACE_LIBS $LIBS"
        AC_MSG_CHECKING([for $yggfunc in $YGGINTERFACE_LIBS])
        AC_TRY_LINK_FUNC($yggfunc, [ax_ygginterface_ok=yes], [YGGINTERFACE_LIBS=""])
        AC_MSG_RESULT($ax_ygginterface_ok)
        LIBS="$save_LIBS"
        if test $ax_ygginterface_ok = no; then
                YGGINTERFACE_LIBS=""
        fi
fi

# Generic YGGINTERFACE library?
for ygginterface in ygginterface; do
        if test $ax_ygginterface_ok = no; then
                save_LIBS="$LIBS"; LIBS="$LIBS"
                AC_CHECK_LIB($ygginterface, $yggfunc,
                    [ax_ygginterface_ok=yes; YGGINTERFACE_LIBS="-l$ygginterface"], [], [])
                LIBS="$save_LIBS"
        fi
done

# TODO: Do this correctly via pkg-config
ax_ygginterface_ok=yes
YGGINTERFACE_LIBS="/Users/langmm/yggcpp/lib/libYggInterface.dylib"
YGGINTERFACE_INCL="/Users/langmm/yggcpp/cpp/include"
AC_SUBST(YGGINTERFACE_LIBS)
AC_SUBST(YGGINTERFACE_INCL)

# Finally, execute ACTION-IF-FOUND/ACTION-IF-NOT-FOUND:
if test x"$ax_ygginterface_ok" = xyes; then
        ifelse([$1],,AC_DEFINE(HAVE_YGGINTERFACE,1,[Define if you have YGGINTERFACE library.]),[$1])
        :
else
        ax_ygginterface_ok=no
        $2
fi
])dnl AX_YGGINTERFACE
