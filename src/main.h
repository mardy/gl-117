/*
    GL-117
    Copyright 2001, 2002 Thomas A. Drexl aka heptargon

    This file is part of GL-117.

    GL-117 is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    GL-117 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GL-117; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* This file includes the main program. */

#ifndef IS_MAIN_H
#define IS_MAIN_H

// dummy debug definitions
//#define HAVE_SDL_MIXER
//#define USE_GLUT

#include <math.h>
#include <stdio.h>
#include <string.h>

/* Load definitions of GNU autoconf:
    USE_GLUT: no SDL
    HAVE_SDL: SDL installed
    HAVE_SDL_MIXER: SDL_mixer installed
    HAVE_SDL_NET: SDL_net installed */

#include "aiobject.h" // ok
#include "model.h" // ok
#include "3ds.h" // ok
#include "effects.h" // ok
#include "audio.h" // ok
#include "pilots.h" // ok

extern Font *font1, *font2;

extern CTexture *texsun, *texflare1, *texflare2, *texflare3, *texflare4, *texmoon, *texcross, *texcross2, *texranks, *texmedals;
extern CTexture *texradar1, *texradar2, *texarrow;

extern PilotList *pilots; // other pilots of the Eagle Squad

extern CExplosion *explosion [maxexplosion];
extern CBlackSmoke *blacksmoke [maxblacksmoke];

extern CLoad3DS g_Load3ds;
extern CModel model_fig; // fighter models
extern CModel model_figa;
extern CModel model_figb;
extern CModel model_figc;
extern CModel model_figd;
extern CModel model_fige;
extern CModel model_figf;
extern CModel model_figg;
extern CModel model_figh;
extern CModel model_figi;
extern CModel model_figt;

extern CModel model_cannon1;
extern CModel model_cannon2;
extern CModel model_cannon1b;
extern CModel model_cannon2b;
extern CModel model_flare1;
extern CModel model_chaff1;
extern CModel model_missile1;
extern CModel model_missile2;
extern CModel model_missile3;
extern CModel model_missile4;
extern CModel model_missile5;
extern CModel model_missile6;
extern CModel model_missile7;
extern CModel model_missile8;
extern CModel model_flak1;
extern CModel model_flarak1;
extern CModel model_tent1;
extern CModel model_ship1;
extern CModel model_ship2;
extern CModel model_gl117;
extern CModel model_tank1;
extern CModel model_container1;
extern CModel model_truck1;
extern CModel model_truck2;
extern CModel model_trsam;
extern CModel model_pickup1;
extern CModel model_pickup2;
extern CModel model_tank2;
extern CModel model_tent4;
extern CModel model_hall1;
extern CModel model_hall2;
extern CModel model_oilrig;
extern CModel model_egg;
extern CModel model_radar;
extern CModel model_mine1;
extern CModel model_aster1;
extern CModel model_base1;
extern CModel model_barrier1;
extern CModel model_rubble1;
extern CModel model_depot1;
extern CModel model_house1;

extern DynamicObj *flare [maxflare];
extern DynamicObj *chaff [maxchaff];
extern AIObj *missile [maxmissile];
extern DynamicObj *laser [maxlaser];
extern AIObj *fighter [maxfighter];
extern Star *star [maxstar]; // the stars at night
extern DynamicObj *groundobj [maxgroundobj];
extern Flash *flash1; // the thunderstorm flash

extern Space *space; // the scene (everything) without the landscape

extern AIObj *fplayer; // the player itself

extern SoundSystem *sound;

// get name of a keyboard's key (ENTER, SPACE)
extern char *getKeyString (int key, char *str);

// own export: main.h may be included in the cpp files
extern CModel *getModel (int id);

#endif

