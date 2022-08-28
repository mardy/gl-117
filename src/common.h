#ifndef IS_COMMON_H
#define IS_COMMON_H

#include "config.h" // defines HAVE_SDL, HAVE_SDL_MIXER

// USE_GLUT means ONLY use GLUT
// HAVE_SDL means that SDL is available, so USE_GLUT will not be defined
// HAVE_SDL_MIXER means SDL and SDL_mixer are available
// HAVE_SDL_NET means SDL_net is available

//#define HAVE_SDL_NET // dummy
//#undef HAVE_SDL_MIXER // dummy
//#undef HAVE_SDL // dummy

#define USE_GLUT
#ifdef HAVE_SDL
#undef USE_GLUT
#else
#undef HAVE_SDL_MIXER
#endif

// APPLE path is different (thanks Minami)
#ifdef __APPLE__
#include <GLUT/glut.h>
#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"
#ifdef HAVE_SDL_MIXER
#include "SDL/SDL_mixer.h"
#endif
#ifdef HAVE_SDL_NET
#include "SDL/SDL_net.h"
#endif
#endif
#ifndef __APPLE__
#ifdef USE_GLUT
#include <GL/glut.h>
#else
#include <GL/glut.h>
#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"
#ifdef HAVE_SDL_MIXER
#include "SDL/SDL_mixer.h"
#endif
#ifdef HAVE_SDL_NET
#include "SDL/SDL_net.h"
#endif
#endif
#endif // __APPLE__

#define ZOOM 256
#define GLOBALSCALE 1
#define VERSIONSTRING "V1.3"

// data types
#ifdef USE_GLUT
#define Uint32 unsigned int
#define Uint16 unsigned short
#endif

#define LINEWIDTH(x) (float) x * height / 600.0F

// game states
#define GAME_INIT 0
#define GAME_PLAY 1
#define GAME_PAUSE 2
#define GAME_MENU 3
#define GAME_CREDITS 4
#define GAME_CREATE 5
#define GAME_JOIN 6
#define GAME_MISSION 7
#define GAME_STATS 8
#define GAME_FAME 9
#define GAME_FIGHTER 10
#define GAME_FINISH 11
#define GAME_QUIT 12

// weather
#define WEATHER_SUNNY 0
#define WEATHER_THUNDERSTORM 1
#define WEATHER_CLOUDY 2

// controls
#define CONTROLS_KEYBOARD 0
#define CONTROLS_MOUSE 1
#define CONTROLS_JOYSTICK 2

// view range borders
#define VIEW_MIN 30
#define VIEW_MAX 150

// preferences
extern int quality;
extern float view;
extern int width, height, bpp;
extern int wantwidth, wantheight, wantfullscreen; // requested values for next restart
extern int volumesound;
extern int volumemusic;
extern int dithering;
extern int antialiasing;
extern int specialeffects;
extern int dynamiclighting;

// current mouse coordinates
extern int mousex, mousey;

extern int debug; // debug mode (1=counter measures)
extern bool multiplayer, isserver;
extern int fullscreen;
extern int day;
extern int weather; // 0: sunny, 1: thunderstorm, 2: no atmosphere (moon)
extern float sungamma; // angle of sunrays dropping on horizon

extern int camera; // camera type (0=cockpit, 1=chase, 2=backward, ...)
extern float camx, camy, camz; // camera position
extern float camphi, camgamma, camtheta; // phi: heading, theta: roll, gamma: elevation
extern float sunlight; // dynamically adjustable light (0=black...1=default...brighter)

extern int lighting; // 1=on, 0=off (obsolete)
extern int mode; // always off=0, 1 would mean to calculate ROAM (obsolete)!

extern float getView (); // return view dependant on weather

extern int difficulty; // 0=easy, 1=normal, 2=hard
extern int physics; // 0=action, 1=realistic
extern int brightness; // brightness correction
extern int contrast; // contrast/gamma correction (not yet configurable)

extern bool sunblinding; // viewer gazing into the sun?

extern int clouds;

extern int controls; // see CONTROLS-constants

extern int game; // see GAME-constants

extern float blackout, redout; // high force

extern float view_x, view_y; // angles for joystick-controlled view

// maximum constants for objects
const int maxexplosion = 30, maxfighter = 30, maxlaser = 150, maxmissile = 30, maxstar = 70, maxgroundobj = 10, maxblacksmoke = 50, maxflare = 30, maxchaff = 30;

// sine and cosine table (obsolete)
//#define SIN(x) sine[(int)x]
//#define COS(x) cosi[(int)x]

// exact sine, cosine functions
#define SIN(x) sin(x*PI/180.0)
#define COS(x) cos(x*PI/180.0)

// define common GLUT/SDL keys
#ifdef USE_GLUT
  #define MOUSE_BUTTON_LEFT GLUT_LEFT_BUTTON+1 // LEFT MB=0, but we use 0 for "no button"
  #define MOUSE_BUTTON_MIDDLE GLUT_MIDDLE_BUTTON+1
  #define MOUSE_BUTTON_RIGHT GLUT_RIGHT_BUTTON+1
  #define MOUSE_UP GLUT_UP
  #define MOUSE_DOWN GLUT_DOWN
  #define KEY_F1 GLUT_KEY_F1
  #define KEY_F2 GLUT_KEY_F2
  #define KEY_F3 GLUT_KEY_F3
  #define KEY_F4 GLUT_KEY_F4
  #define KEY_F5 GLUT_KEY_F5
  #define KEY_F6 GLUT_KEY_F6
  #define KEY_F7 GLUT_KEY_F7
  #define KEY_F8 GLUT_KEY_F8
  #define KEY_F9 GLUT_KEY_F9
  #define KEY_F10 GLUT_KEY_F10
  #define KEY_UP GLUT_KEY_UP
  #define KEY_DOWN GLUT_KEY_DOWN
  #define KEY_LEFT GLUT_KEY_LEFT
  #define KEY_RIGHT GLUT_KEY_RIGHT
  #define KEY_PGUP GLUT_KEY_PAGE_UP
  #define KEY_PGDOWN GLUT_KEY_PAGE_DOWN
  #define KEY_LALT -1 // where in glut.h???
  #define KEY_LCTRL -2
  #define KEY_LSHIFT -3
  #define KEY_RSHIFT -4
  #define KEY_RALT -5
  #define KEY_RCTRL -6
  #define KEY_CAPSLOCK -7
  #define KEY_DELETE -8
  #define KEY_INSERT GLUT_KEY_INSERT
  #define KEY_HOME GLUT_KEY_HOME
  #define KEY_END GLUT_KEY_END
#else
  #define MOUSE_BUTTON_LEFT SDL_BUTTON_LEFT
  #define MOUSE_BUTTON_MIDDLE SDL_BUTTON_MIDDLE
  #define MOUSE_BUTTON_RIGHT SDL_BUTTON_RIGHT
  #define MOUSE_UP SDL_RELEASED
  #define MOUSE_DOWN SDL_PRESSED
  #define KEY_F1 SDLK_F1
  #define KEY_F2 SDLK_F2
  #define KEY_F3 SDLK_F3
  #define KEY_F4 SDLK_F4
  #define KEY_F5 SDLK_F5
  #define KEY_F6 SDLK_F6
  #define KEY_F7 SDLK_F7
  #define KEY_F8 SDLK_F8
  #define KEY_F9 SDLK_F9
  #define KEY_F10 SDLK_F10
  #define KEY_UP SDLK_UP
  #define KEY_DOWN SDLK_DOWN
  #define KEY_LEFT SDLK_LEFT
  #define KEY_RIGHT SDLK_RIGHT
  #define KEY_PGUP SDLK_PAGEUP
  #define KEY_PGDOWN SDLK_PAGEDOWN
  #define KEY_LALT SDLK_LALT
  #define KEY_RALT SDLK_RALT
  #define KEY_LCTRL SDLK_LCTRL
  #define KEY_RCTRL SDLK_RCTRL
  #define KEY_LSHIFT SDLK_LSHIFT
  #define KEY_RSHIFT SDLK_RSHIFT
  #define KEY_CAPSLOCK SDLK_CAPSLOCK
  #define KEY_DELETE SDLK_DELETE
  #define KEY_INSERT SDLK_INSERT
  #define KEY_HOME SDLK_HOME
  #define KEY_END SDLK_END
#endif

#define KEY_BACKSPACE 8
#define KEY_TAB 9
#define KEY_ENTER 13
#define KEY_SPACE 32

// log/debug level, look at common.cpp
#define LOG_NONE 0 // do not log anything
#define LOG_FATAL 1 // only show fatal (GL-117 will exit) errors (stderr)
#define LOG_ERROR 2 // show errors too (stderr)
#define LOG_WARN 3 // show warnings too (stdout)
#define LOG_MOST 4 // show important messages too (stdout)
#define LOG_ALL 5 // log everything (stdout)

// display log/debug message
extern void display (char *str, int level);

extern void display_exit ();

// display "out of memory" error and exit
extern void error_outofmemory ();

extern void key2string (int key, char *buf);
extern void joystick2string (int key, char *buf);

// current log/debug level (set to constants above)
extern int debuglevel;

// show collition detection
extern int showcollision;

#define STDSIZE 256 // typical temporary buffer size
#define PATHSIZE 4096 // maximum path/directory string size

// exit codes
#define EXIT_NORMAL 0
#define EXIT_LOADFILE 1
#define EXIT_INIT 2
#define EXIT_COMMAND 3
#define EXIT_ALLOC 4
#define EXIT_CONTEXT 5

// joystick constants
const int maxjaxis = 10;
const int maxjoysticks = 10;

// global normed timestep (higher values mean slower gameplay)
const int timestep = 34;

#endif
