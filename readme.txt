  ________    __                        __        __    __________
 /   ___  \  /\ \                      /  \      /  \  /\________ \
/\  /___/\_\ \ \ \                    /    \    /    \ \/_______/\ \
\ \ \   \/_/_ \ \ \         _______  /\__/\ \  /\__/\ \         \/ /
 \ \ \  /\__ \ \ \ \       /\______\ \/_/\ \ \ \/_/\ \ \        / /
  \ \ \ \/_/\ \ \ \ \      \/______/      \ \ \     \ \ \      / /
   \ \ \___\/  \ \ \ \_______              \ \ \     \ \ \    /\ \
    \ \________/  \ \________\              \ \_\     \ \ \   \ \_\
     \/_______/    \/________/               \/_/      \/_/    \/_/


GL-117 is an action flight simulator written in C++ and OpenGL.
It is distributed unter the terms of the GNU Public License (GPL).


MINIMUM REQUIREMENTS:

- Linux or MSWindows
- GL, GLU (or compatible), GLUT
- SDL, SDL_mixer (both optional but strongly recommended)


MSWINDOWS INSTALLATION:

You might have to install GL, GLU, GLUT, and SDL, SDL_mixer
Look into your system directory, that is
 \WINDOWS\SYSTEM for Win9x/ME
 \WINDOWS\SYSTEM32 for WinNT/2000/XP.
You need the files opengl32.dll, glu32.dll, glut32.dll, and sdl.dll,
sdl_mixer.dll stored there!
If one is missing, search the internet.
That's it. Execute the binary gl-117.exe in the windows directory!


UNIX/LINUX INSTALLATION:

If you got a binary gl-117 for your OS in the linux directory,
you will only need
	GL, GLU, GLUT, and SDL, SDL_mixer
installed on your system.
If you have to compile the sources, you will need
	GL-devel, GLU-devel, GLUT-devel, SDL-devel, SDL_mixer
OR:	mesa-devel, mesaglu-devel, mesaglut-devel, SDL-devel, SDL_mixer
as well.

To compile gl-117, just execute
	./configure
	make
The new binary is in the src directory and may be executed.
As root enter
	make install
to properly install all game files.
Then you may start the game just executing
	gl-117
Never execute the game as root!


STARTUP:

When starting the game, it will test for a valid screen resolution
and save the startup settings in a file called "conf".
MSWindows users will locate this file in the "saves"-directory,
please execute "wordpad" to view the file.
On Linux/Unix the game will expect this file in the directory
"$HOME/.gl-117".
The configuration options are quite self-explanatory, adjust them
to your system.
If you lack a hardware accelerated video card, please set the
quality option to 0 or 1. Further acceleration can be achieved
negligating fullscreen mode and making the window smaller.


Please look at the file FAQ for further questions.
