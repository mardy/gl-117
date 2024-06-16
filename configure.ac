AC_INIT(src/main.cpp)
AM_INIT_AUTOMAKE(gl-117,1.0,200308190000)
AM_CONFIG_HEADER(config.h)

AC_PATH_X
AC_PATH_XTRA

AH_TEMPLATE([HAVE_SDL],[Use SDL library])
AH_TEMPLATE([HAVE_SDL_MIXER],[Use SDL mixer library])
AH_TEMPLATE([HAVE_GL],[Use GL library])
AH_TEMPLATE([HAVE_GLU],[Use GLU library])
AH_TEMPLATE([HAVE_GLUT],[Use GLUT library])

AC_PROG_CXX
AC_PROG_CC
AC_LANG_C
AC_PROG_INSTALL
AC_PROG_MAKE_SET

AC_C_BIGENDIAN()

AC_SUBST(EXEEXT, ".elf")

PKG_PROG_PKG_CONFIG

PKG_CHECK_MODULES(SDL, [sdl], AC_DEFINE(HAVE_SDL),
    AC_MSG_WARN(SDL.h not found - no sounds and no joystick available))
PKG_CHECK_MODULES(GL, [opengl glut glu], [
    AC_DEFINE(HAVE_GL)
    AC_DEFINE(HAVE_GLU)
    AC_DEFINE(HAVE_GLUT)
], AC_MSG_ERROR(GL,GLU or glut not found - please install OpenGL- or Mesa-devel))
PKG_CHECK_MODULES(SDL_MIXER, [SDL_mixer], AC_DEFINE(HAVE_SDL_MIXER),
    AC_MSG_WARN(SDL_mixer.h not found - no advanced sounds available))


CFLAGS="$CFLAGS $GL_CFLAGS $SDL_MIXER_CFLAGS $SDL_CFLAGS"
CPPFLAGS="-DDATADIR='\"$datadir\"' -Wall -pedantic $CPPFLAGS"
CXXFLAGS="$CXXFLAGS"
LIBS="$LIBS $GL_LIBS $SDL_MIXER_LIBS $SDL_LIBS"

AC_OUTPUT([ \
Makefile \
src/Makefile \
])
