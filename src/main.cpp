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

/* To fully understand the GL-117 code, please read the methods from bottom to top! */

/*
TODO:
- options => video => resolution and fullscreen (both with restart)
- correct transport2 (left side corrupt in VRML) and add mission with transport2
- southern seashore landscape (additional missions)
- alpine snow landscape (additional missions)
- tree colors (fall, spring), draw more tree textures
- particle systems: rain, clouds to fly through
*/

#ifndef IS_MAIN_H

#include "main.h"
#include "menu.h"
#include "maploader.h"
#include "dirs.h"
#include "gl.h"
#include "land.h"
#include "net.h"
#include "mathtab.h"
#include "cockpit.h"
#include "conf.h"
#include "mission.h"
#include "glland.h"
#include "common.h"

#include <ctype.h>

int mousex, mousey;

int debug = 1;
bool multiplayer = false, isserver = false;
int day = 1;
int weather = WEATHER_SUNNY;
float sungamma = 45.0;

int camera = 0;
float camx = 0, camy = 0, camz = 0, camphi = 0, camgamma = 0, camtheta = 0;
float view_x = 0, view_y = 0;
float sunlight = 1.0, sunlight_dest = 1.0;

float blackout = 0, redout = 0;

int lighting = 1;
int mode = 0;

// pre-defined screen resolutions (x, y, bpp, fullscreen)
int resolution [4] [4] =
        { { 1024, 768, 32, 1 },
          { 800, 600, 32, 1 },
          { 640, 480, 32, 1 },
          { 640, 480, 32, 0 } };

float nearclippingplane = 0.25; // do NOT lower this!

bool sunblinding = false;

Dirs *dirs;
Server *server = NULL;
Client *client = NULL;

/*#ifdef HAVE_SDL
SDL_Thread *threadnet = NULL;
#endif*/

int game = GAME_INIT;

int debuglevel = LOG_MOST;
int showcollision = 0;
int brightness = 0;
int contrast = 10;

SoundSystem *sound = NULL;

GL *gl;

float getView ()
{
  if (weather == WEATHER_THUNDERSTORM && view > 40.0)
    return 40.0;
  return view;
}

int clouds = 0;

CTexture *texradar1, *texradar2, *texarrow;//, *texcounter;

MapLoader *maploader;

float visibleangle = 80.0F;







CTexture *texsun, *texflare1, *texflare2, *texflare3, *texflare4, *texmoon = NULL, *texcross, *texcross2, *texranks, *texmedals;
CTexture *texclouds1, *texclouds2, *texclouds3;

PilotList *pilots;

CExplosion *explosion [maxexplosion];
CBlackSmoke *blacksmoke [maxblacksmoke];
GLLandscape *l = NULL;
Font *font1, *font2;

CTexture *textitle;

Uint32 lasttime = 0;





CLoad3DS g_Load3ds;
CModel model_fig;
CModel model_figa;
CModel model_figb;
CModel model_figc;
CModel model_figd;
CModel model_fige;
CModel model_figf;
CModel model_figg;
CModel model_figh;
CModel model_figi;
CModel model_figt;
CModel model_figu;
CModel model_cannon1;
CModel model_cannon2;
CModel model_cannon1b;
CModel model_cannon2b;
CModel model_flare1;
CModel model_chaff1;
CModel model_missile1;
CModel model_missile2;
CModel model_missile3;
CModel model_missile4;
CModel model_missile5;
CModel model_missile6;
CModel model_missile7;
CModel model_missile8;
CModel model_flak1;
CModel model_flarak1;
CModel model_tent1;
CModel model_ship1;
CModel model_ship2;
CModel model_gl117;
CModel model_tank1;
CModel model_container1;
CModel model_truck1;
CModel model_truck2;
CModel model_trsam;
CModel model_pickup1;
CModel model_pickup2;
CModel model_tank2;
CModel model_tent4;
CModel model_hall1;
CModel model_hall2;
CModel model_oilrig;
CModel model_egg;
CModel model_radar;
CModel model_mine1;
CModel model_aster1;
CModel model_base1;
CModel model_barrier1;
CModel model_rubble1;
CModel model_depot1;
CModel model_house1;

DynamicObj *flare [maxflare];
DynamicObj *chaff [maxchaff];
AIObj *missile [maxmissile];
DynamicObj *laser [maxlaser];
AIObj *fighter [maxfighter];
Star *star [maxstar];
DynamicObj *groundobj [maxgroundobj];
Flash *flash1;

Space *space;

AIObj *fplayer;
CSpaceObj *sphere;
CSphere *objsphere;
HighClouds *highclouds;
HighClouds *highclouds2;

CModel *obj, *objlaser, *objmissile;
CVector3 *clip1, *clip2, *tlnull, *tlinf, *tlminf;
CRotation *rotnull, *rotmissile;

GLenum polygonMode = GL_FILL;

GLfloat light_position0[] = { 0.0, 0.5, -1.0, 0 };

#define max(x,y) ((x)>(y)?(x):(y))

int NX, NY;

int joysticks;

Mission *mission = NULL;
Mission *missionnew = NULL;

int keyb_elev = 0, keyb_roll = 0, keyb_rudder = 0;
int keyb_lshift = 0;

Container allmenus;
Container famemenu;
Container fightermenu;
Container missionmenu;
Container quitmenu;
Container statsmenu;
Component *currentsubmenu = NULL;
Component *currentoptmenu = NULL;

Container *mainmenu, *submenu [10], *optmenu [5], *controlsmenu [5];
Button *mainbutton [10];
Button *optbutton [10];



bool firststart = false;

bool missionactive = false;

CColor colorwhite (255, 255, 255, 255);
CColor colorblue (50, 50, 255, 255);
CColor colorgreen (100, 255, 100, 255);
CColor colororange (255, 150, 100, 255);
CColor colorred (255, 0, 0, 255);
CColor coloryellow (255, 255, 0, 200);
CColor colorgrey (150, 150, 150, 200);
CColor colorlightgrey (210, 210, 210, 255);


void drawRank (float xp, float yp, float zp, int rank, float zoom)
{
  rank --;
  if (rank < 0) return;
  float x = xp / 10.0, y = yp / 10.0, z = zp;
  float tx1 = 0.5 * (rank % 2);
  float tx2 = tx1 + 0.5;
  float ty1 = 0.755 - 0.25 * (rank / 2);
  float ty2 = ty1 + 0.24;
  zoom /= 10;
  gl->enableTextures (texranks->textureID);
  if (antialiasing) gl->enableLinearTexture (texranks->textureID);
  else gl->disableLinearTexture (texranks->textureID);
  gl->enableAlphaBlending ();
  glEnable (GL_ALPHA_TEST);
  glAlphaFunc (GL_GEQUAL, 0.35);

  // Example how to pass the glBegin()...glEnd() code using vertex lists
/*  float vertex [sizeof (float) * 9];
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer (3,	GL_FLOAT,	sizeof (float) * 9, &vertex [0]);
	glColorPointer (4, GL_FLOAT, sizeof (float) * 9, &vertex [3]);
	glTexCoordPointer (2, GL_FLOAT, sizeof (float) * 9, &vertex [7]);

  int mz = 0;
  vertex [mz ++] = x; vertex [mz ++] = y; vertex [mz ++] = z;
  vertex [mz ++] = 1; vertex [mz ++] = 1; vertex [mz ++] = 1; vertex [mz ++] = 200.0F/255.0F;
  vertex [mz ++] = tx1; vertex [mz ++] = ty1;
  vertex [mz ++] = x + zoom * 2; vertex [mz ++] = y; vertex [mz ++] = z;
  vertex [mz ++] = 1; vertex [mz ++] = 1; vertex [mz ++] = 1; vertex [mz ++] = 200.0F/255.0F;
  vertex [mz ++] = tx2; vertex [mz ++] = ty1;
  vertex [mz ++] = x + zoom * 2; vertex [mz ++] = y + zoom; vertex [mz ++] = z;
  vertex [mz ++] = 1; vertex [mz ++] = 1; vertex [mz ++] = 1; vertex [mz ++] = 200.0F/255.0F;
  vertex [mz ++] = tx2; vertex [mz ++] = ty2;
  vertex [mz ++] = x; vertex [mz ++] = y + zoom; vertex [mz ++] = z;
  vertex [mz ++] = 1; vertex [mz ++] = 1; vertex [mz ++] = 1; vertex [mz ++] = 200.0F/255.0F;
  vertex [mz ++] = tx1; vertex [mz ++] = ty2;

  glDrawArrays (GL_QUADS, 0, 4);*/

  glBegin (GL_QUADS);
  glColor4ub (255, 255, 255, 200);
  glTexCoord2f (tx1, ty1);
  glVertex3f (x, y, z);
  glTexCoord2f (tx2, ty1);
  glVertex3f (x + zoom * 2, y, z);
  glTexCoord2f (tx2, ty2);
  glVertex3f (x + zoom * 2, y + zoom, z);
  glTexCoord2f (tx1, ty2);
  glVertex3f (x, y + zoom, z);
  glEnd ();

  glDisable (GL_ALPHA_TEST);
  gl->disableAlphaBlending ();
  glDisable (GL_TEXTURE_2D);
}

void drawMedal (float xp, float yp, float zp, int medal, float zoom, int missionid)
{
  if (medal < 0) return;
  if (missionid < MISSION_CAMPAIGN1 || missionid > MISSION_CAMPAIGN2) return;
  float x = xp / 10.0 - zoom / 20, y = yp / 10.0 - zoom / 20, z = zp;
  float tx1 = 0.5 * (medal % 2);
  float tx2 = tx1 + 0.5;
  float ty1 = 0.5 * (medal / 2);
  float ty2 = ty1 + 0.5;
  zoom /= 10;
  gl->enableTextures (texmedals->textureID);
  if (antialiasing) gl->enableLinearTexture (texmedals->textureID);
  else gl->disableLinearTexture (texmedals->textureID);
  gl->enableAlphaBlending ();
  glEnable (GL_ALPHA_TEST);
  glAlphaFunc (GL_GEQUAL, 0.1);
  glBegin (GL_QUADS);
  glColor4ub (255, 255, 255, 200);
  glTexCoord2f (tx1, ty1);
  glVertex3f (x, y, z);
  glTexCoord2f (tx2, ty1);
  glVertex3f (x + zoom, y, z);
  glTexCoord2f (tx2, ty2);
  glVertex3f (x + zoom, y + zoom, z);
  glTexCoord2f (tx1, ty2);
  glVertex3f (x, y + zoom, z);
  glEnd ();
  glDisable (GL_ALPHA_TEST);
  gl->disableAlphaBlending ();
  glDisable (GL_TEXTURE_2D);
}

void drawMouseCursor ()
{
  glPushMatrix ();
  glMatrixMode (GL_PROJECTION);
  glPushMatrix ();
  glLoadIdentity ();
  gluOrtho2D (0, width, 0, height);
  glScalef (1, -1, 1);
  glTranslatef (0, -height, 0);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  gl->enableAlphaBlending ();
  glEnable (GL_ALPHA_TEST);
  glAlphaFunc (GL_GEQUAL, 0.1);
  glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  gl->enableTextures (texcross2->textureID);
  int crossradius = width / 35;

  glTranslatef (mousex, mousey, 0);
  glBegin (GL_QUADS);
  glColor4ub (255, 255, 255, 255);
  glTexCoord2i (0, 0);
  glVertex2d (-crossradius, -crossradius);
  glTexCoord2i (0, 1);
  glVertex2d (-crossradius, crossradius);
  glTexCoord2i (1, 1);
  glVertex2d (crossradius, crossradius);
  glTexCoord2i (1, 0);
  glVertex2d (crossradius, -crossradius);
  glEnd ();
  glDisable (GL_ALPHA_TEST);
  glDisable (GL_TEXTURE_2D);
  gl->disableAlphaBlending ();
  glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glMatrixMode (GL_PROJECTION);
  glPopMatrix ();
  glMatrixMode (GL_MODELVIEW);
  glPopMatrix ();

  glPopMatrix ();
}

int getMedal (int score)
{
  if (score >= 350) return 1;
  else if (score >= 300) return 0;
  else if (score >= 250) return 3;
  else if (score >= 200) return 2;
  return -1;
}

void gl117_rotateColors (int inittimer_gl117)
{
  int i;
  for (i = 0; i < model_gl117.numObjects; i ++)
  {
    for (int i2 = 0; i2 < model_gl117.object [i]->numVertices; i2 ++)
    {
      model_gl117.object [i]->vertex [i2].color.c [0] = (int) (75.0F * SIN(i2 * 100 + inittimer_gl117 / 2) + 155.0F);
      model_gl117.object [i]->vertex [i2].color.c [1] = model_gl117.object [i]->vertex [i2].color.c [0];
      model_gl117.object [i]->vertex [i2].color.c [2] = 100;
    }
  }
}

void adjustBrightness ()
{
  // adjust brightness setting (blending)
  if (brightness < 0)
  {
    glColor4ub (0, 0, 0, -brightness);
    float xf = 2.0, yf = 1.5, zf = 1.0;
    glDisable (GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin (GL_QUADS);
    glVertex3f (-xf, -yf, -zf);
    glVertex3f (-xf, yf, -zf);
    glVertex3f (xf, yf, -zf);
    glVertex3f (xf, -yf, -zf);
    glEnd ();
    glDisable (GL_BLEND);
//    glEnable (GL_DEPTH_TEST);
  }
  else if (brightness > 0)
  {
    glColor4ub (255, 255, 255, brightness);
    float xf = 2.0, yf = 1.5, zf = 1.0;
    glDisable (GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin (GL_QUADS);
    glVertex3f (-xf, -yf, -zf);
    glVertex3f (-xf, yf, -zf);
    glVertex3f (xf, yf, -zf);
    glVertex3f (xf, -yf, -zf);
    glEnd ();
    glDisable (GL_BLEND);
//    glEnable (GL_DEPTH_TEST);
  }
}



#ifndef USE_GLUT
SDL_Joystick *sdljoystick [maxjoysticks];
int sdljoystickaxes [maxjoysticks];
#endif

int sdldisplay = true;
int sdlreshape = true;
Cockpit *cockpit;
float fps = 30;

int glutwindow;



bool initing = true;
int flash = 0;
int lastshield = 0;

int getTrainingIdFromValue (int n)
{
  int z = 0;
  if (n == z ++) return MISSION_TUTORIAL;
  else if (n == z ++) return MISSION_TUTORIAL2;
  else if (n == z ++) return MISSION_DOGFIGHT;
  else if (n == z ++) return MISSION_TUTORIAL3;
  else if (n == z ++) return MISSION_FREEFLIGHT1;
  else if (n == z ++) return MISSION_DEATHMATCH1;
  else if (n == z ++) return MISSION_DEATHMATCH2;
  else if (n == z ++) return MISSION_DEATHMATCH3;
  else if (n == z ++) return MISSION_TEAMBASE1;
  else if (n == z ++) return MISSION_WAVES1;
  else
  {
    return MISSION_CUSTOM1 + n - 10;
  }
  return 0;
}

int getCampaignIdFromValue (int n)
{
  int z = 0;
  if (n == z ++) return MISSION_TEST1;
  else if (n == z ++) return MISSION_TEST2;
  else if (n == z ++) return MISSION_TRANSPORT;
  else if (n == z ++) return MISSION_CONVOY;
  else if (n == z ++) return MISSION_DOGFIGHT2;
  else if (n == z ++) return MISSION_AIRBATTLE;
  else if (n == z ++) return MISSION_SADEFENSE;
  else if (n == z ++) return MISSION_SCOUT;
  else if (n == z ++) return MISSION_BASE;
  else if (n == z ++) return MISSION_DEPOT;
  else if (n == z ++) return MISSION_DEFEND1;
  else if (n == z ++) return MISSION_DOGFIGHT3;
  else if (n == z ++) return MISSION_TANK1;
  else if (n == z ++) return MISSION_CONVOY2;
  else if (n == z ++) return MISSION_SHIP1;
  else if (n == z ++) return MISSION_SHIP2;
  else if (n == z ++) return MISSION_SHIP3;
  else if (n == z ++) return MISSION_CANYON1;
  else if (n == z ++) return MISSION_CANYON2;
  else if (n == z ++) return MISSION_TUNNEL1;
  else if (n == z ++) return MISSION_CANYON3;
  else if (n == z ++) return MISSION_MOON1;
  else if (n == z ++) return MISSION_MOONBATTLE;
  else if (n == z ++) return MISSION_MOON2;
  else if (n == z ++) return MISSION_MOON3;
  return 0;
}

int getCampaignValueFromId (int n)
{
  int z = 0;
  if (n == MISSION_TEST1) z = 0;
  else if (n == MISSION_TEST2) z = 1;
  else if (n == MISSION_TRANSPORT) z = 2;
  else if (n == MISSION_CONVOY) z = 3;
  else if (n == MISSION_DOGFIGHT2) z = 4;
  else if (n == MISSION_AIRBATTLE) z = 5;
  else if (n == MISSION_SADEFENSE) z = 6;
  else if (n == MISSION_SCOUT) z = 7;
  else if (n == MISSION_BASE) z = 8;
  else if (n == MISSION_DEPOT) z = 9;
  else if (n == MISSION_DEFEND1) z = 10;
  else if (n == MISSION_DOGFIGHT3) z = 11;
  else if (n == MISSION_TANK1) z = 12;
  else if (n == MISSION_CONVOY2) z = 13;
  else if (n == MISSION_SHIP1) z = 14;
  else if (n == MISSION_SHIP2) z = 15;
  else if (n == MISSION_SHIP3) z = 16;
  else if (n == MISSION_CANYON1) z = 17;
  else if (n == MISSION_CANYON2) z = 18;
  else if (n == MISSION_TUNNEL1) z = 19;
  else if (n == MISSION_CANYON3) z = 20;
  else if (n == MISSION_MOON1) z = 21;
  else if (n == MISSION_MOONBATTLE) z = 22;
  else if (n == MISSION_MOON2) z = 23;
  else if (n == MISSION_MOON3) z = 24;
  return z;
}

void setPlaneVolume ()
{
  if (game == GAME_PLAY)
  {
    int lev = (int) ((float) 128 * fplayer->thrust / fplayer->maxthrust) - 32;
    sound->stop (SOUND_PLANE1);
    sound->engine = (int) (fplayer->thrust / fplayer->maxthrust * 20) - 10;
    sound->setVolume (SOUND_PLANE1, lev);
    sound->playLoop (SOUND_PLANE1);
  }
  else
  {
    sound->setVolume (SOUND_PLANE1, 0);
  }
}

void setLightSource (int gamma)
{
  light_position0 [0] = -cosi [gamma];
  light_position0 [1] = sine [gamma];
  light_position0 [2] = 0;
  glLightfv (GL_LIGHT0, GL_POSITION, light_position0);
}

CModel *getModel (int id)
{
  if (id == FIGHTER_FALCON) return &model_fig;
  else if (id == FIGHTER_SWALLOW) return &model_figa;
  else if (id == FIGHTER_HAWK) return &model_figb;
  else if (id == FIGHTER_HAWK2) return &model_figc;
  else if (id == FIGHTER_BUZZARD) return &model_figd;
  else if (id == FIGHTER_CROW) return &model_fige;
  else if (id == FIGHTER_PHOENIX) return &model_figf;
  else if (id == FIGHTER_REDARROW) return &model_figg;
  else if (id == FIGHTER_BLACKBIRD) return &model_figh;
  else if (id == FIGHTER_STORM) return &model_figi;
  else if (id == FIGHTER_TRANSPORT) return &model_figt;
  else if (id == FIGHTER_TRANSPORT2) return &model_figu;
  else if (id == MISSILE_AIR1) return &model_missile1;
  else if (id == MISSILE_AIR2) return &model_missile2;
  else if (id == MISSILE_AIR3) return &model_missile3;
  else if (id == MISSILE_GROUND1) return &model_missile4;
  else if (id == MISSILE_GROUND2) return &model_missile5;
  else if (id == MISSILE_DF1) return &model_missile6;
  else if (id == MISSILE_FF1) return &model_missile7;
  else if (id == MISSILE_FF2) return &model_missile8;
  else if (id == TANK_AIR1) return &model_tank1;
  else if (id == TANK_GROUND1) return &model_tank2;
  else if (id == TANK_TRSAM1) return &model_trsam;
  else if (id == TANK_PICKUP1) return &model_pickup1;
  else if (id == TANK_TRUCK1) return &model_truck1;
  else if (id == TANK_TRUCK2) return &model_truck2;
  else if (id == SHIP_CRUISER) return &model_ship1;
  else if (id == SHIP_DESTROYER1) return &model_ship2;
  else if (id == FLAK_AIR1) return &model_flak1;
  else if (id == FLARAK_AIR1) return &model_flarak1;
  else if (id == STATIC_TENT1) return &model_tent1;
  else if (id == STATIC_TENT4) return &model_tent4;
  else if (id == STATIC_CONTAINER1) return &model_container1;
  else if (id == STATIC_HALL1) return &model_hall1;
  else if (id == STATIC_HALL2) return &model_hall2;
  else if (id == STATIC_OILRIG1) return &model_oilrig;
  else if (id == STATIC_COMPLEX1) return &model_egg;
  else if (id == STATIC_RADAR1) return &model_radar;
  else if (id == STATIC_BASE1) return &model_base1;
  else if (id == STATIC_DEPOT1) return &model_depot1;
  else if (id == STATIC_BARRIER1) return &model_barrier1;
  else if (id == ASTEROID) return &model_aster1;
  return &model_fig;
}

int game_levelInit ()
{
  int i, i2;

  // init all objects
  for (i = 0; i < maxfighter; i ++)
  {
    fighter [i]->dinit ();
    fighter [i]->aiinit ();
//    fighter [i]->fighterinit ();
    fighter [i]->explode = 0;
    fighter [i]->tl->x = 0;
    fighter [i]->tl->z = -i * 10;
    fighter [i]->party = 0;
    fighter [i]->zoom = 0.35;
    fighter [i]->deactivate ();
    fighter [i]->recheight = 15;
    fighter [i]->recthrust = 0.2;
    fighter [i]->rectheta = 0;
    fighter [i]->o = &model_fig;
  }
  for (i = 0; i < maxgroundobj; i ++)
  {
    groundobj [i]->dinit ();
    groundobj [i]->zoom = 1;
    groundobj [i]->id = STATIC_PASSIVE;
    groundobj [i]->deactivate ();
  }

//  if (l != NULL) delete l;
  if (!multiplayer || isserver || !isserver) // clients do not need the mission
  {
    Mission *missionold = mission;
    mission = missionnew;
    missionnew = NULL;
    mission->difficulty = difficulty;
    mission->start ();
    if (mission->id >= MISSION_CUSTOM1 && mission->id <= MISSION_CUSTOM2)
    {
      if (((MissionCustom *) mission)->reterror)
      {
        display ("Could not startup mission", LOG_ERROR);
        delete mission;
        mission = missionold;
        return 0;
      }
    }
    if (missionold != NULL)
    { delete missionold; missionold = NULL; }
  }

  initing = true;
  flash = 0;

  if (clouds == 0) highclouds->setTexture (NULL);
  else if (clouds == 1) highclouds->setTexture (texclouds1);
  else if (clouds == 2) highclouds->setTexture (texclouds2);
  else if (clouds == 3) highclouds->setTexture (texclouds3);

  if (clouds == 0) highclouds2->setTexture (NULL);
  else if (clouds == 1) highclouds2->setTexture (texclouds2);
  else if (clouds == 2) highclouds2->setTexture (NULL);
  else if (clouds == 3) highclouds2->setTexture (NULL);

  // place missiles to racks
  for (i = 0; i < maxfighter; i ++)
  {
    if (fighter [i]->id >= FIGHTER1 && fighter [i]->id <= FIGHTER2)
      for (i2 = 0; i2 < 4; i2 ++)
      {
        int type = fighter [i]->missilerack [i2];
        CModel *rm = getModel (MISSILE1 + type);
        fighter [i]->refmodel [i2 * 3] = rm;
        fighter [i]->refmodel [i2 * 3 + 1] = rm;
        fighter [i]->refmodel [i2 * 3 + 2] = rm;
        fighter [i]->reftl [i2 * 3 + 1].z = fighter [i]->reftl [i2 * 3].z - 0.04;
        fighter [i]->reftl [i2 * 3 + 2].z = fighter [i]->reftl [i2 * 3].z + 0.04;
        fighter [i]->reftl [i2 * 3].y = fighter [i]->reftl [i2 * 3 + 1].y - 0.04;
        int tmp = fighter [i]->missilerackn [i2];
        fighter [i]->refscale [i2 * 3] = 0.25;
        fighter [i]->refscale [i2 * 3 + 1] = 0.25;
        fighter [i]->refscale [i2 * 3 + 2] = 0.25;
        if (tmp < 3) fighter [i]->refscale [i2 * 3] = 0;
        if (tmp < 2) fighter [i]->refscale [i2 * 3 + 1] = 0;
      }
  }

  // initialize object's height over the surface
  for (i = 0; i < maxfighter; i ++)
  {
    if (fighter [i]->id >= FLAK1 && fighter [i]->id <= FLAK2)
    {
      fighter [i]->tl->y = l->getExactHeight (fighter [i]->tl->x, fighter [i]->tl->z) + fighter [i]->zoom;
    }
    else if (fighter [i]->id == SHIP_DESTROYER1)
    {
      fighter [i]->tl->y = l->getExactHeight (fighter [i]->tl->x, fighter [i]->tl->z) + fighter [i]->zoom / 4;
    }
    else if (fighter [i]->id == SHIP_CRUISER)
    {
      fighter [i]->tl->y = l->getExactHeight (fighter [i]->tl->x, fighter [i]->tl->z) + fighter [i]->zoom / 4;
    }
    else if (fighter [i]->id >= TANK1 && fighter [i]->id <= TANK2)
    {
      fighter [i]->tl->y = l->getExactHeight (fighter [i]->tl->x, fighter [i]->tl->z) + fighter [i]->zoom * 0.55;
    }
    else if (fighter [i]->id == STATIC_TENT1)
    {
      fighter [i]->tl->y = l->getExactHeight (fighter [i]->tl->x, fighter [i]->tl->z);
    }
    else if (fighter [i]->id == STATIC_CONTAINER1)
    {
      fighter [i]->tl->y = l->getExactHeight (fighter [i]->tl->x, fighter [i]->tl->z) + fighter [i]->zoom / 2;
    }
    else if (fighter [i]->id == STATIC_HALL1)
    {
      fighter [i]->tl->y = l->getExactHeight (fighter [i]->tl->x, fighter [i]->tl->z) + fighter [i]->zoom / 3;
    }
    else if (fighter [i]->id == STATIC_HALL2)
    {
      fighter [i]->tl->y = l->getExactHeight (fighter [i]->tl->x, fighter [i]->tl->z) + fighter [i]->zoom / 3;
    }
    else if (fighter [i]->id == STATIC_TENT4)
    {
      fighter [i]->tl->y = l->getExactHeight (fighter [i]->tl->x, fighter [i]->tl->z) + fighter [i]->zoom / 3;
    }
    else if (fighter [i]->id == STATIC_OILRIG1)
    {
      fighter [i]->tl->y = l->getExactHeight (fighter [i]->tl->x, fighter [i]->tl->z) + fighter [i]->zoom / 5;
    }
    else if (fighter [i]->id == STATIC_COMPLEX1)
    {
      fighter [i]->tl->y = l->getExactHeight (fighter [i]->tl->x, fighter [i]->tl->z) + 0.55 * fighter [i]->zoom;
    }
    else if (fighter [i]->id == STATIC_RADAR1)
    {
      fighter [i]->tl->y = l->getExactHeight (fighter [i]->tl->x, fighter [i]->tl->z) + 0.7 * fighter [i]->zoom;
    }
    else if (fighter [i]->id == STATIC_BASE1)
    {
      fighter [i]->tl->y = l->getExactHeight (fighter [i]->tl->x, fighter [i]->tl->z) + 0.5 * fighter [i]->zoom;
    }
    else if (fighter [i]->id == STATIC_DEPOT1)
    {
      fighter [i]->tl->y = l->getExactHeight (fighter [i]->tl->x, fighter [i]->tl->z) + 0.5 * fighter [i]->zoom;
    }
    else if (fighter [i]->id == STATIC_BARRIER1)
    {
      fighter [i]->tl->y = l->getExactHeight (fighter [i]->tl->x, fighter [i]->tl->z) + 0.3 * fighter [i]->zoom;
    }
    else if (fighter [i]->id == MISSILE_MINE1)
    {
      fighter [i]->tl->y = l->getHeight (fighter [i]->tl->x, fighter [i]->tl->z) + 5 + myrandom (20);
    }
    else if (fighter [i]->id == ASTEROID)
    {
      ;
    }
    else
    {
      fighter [i]->tl->y = l->getHeight (fighter [i]->tl->x, fighter [i]->tl->z) + 20;
    }
  }

  for (i = 0; i < maxlaser; i ++)
  {
    laser [i]->deactivate ();
    if (day)
    {
      laser [i]->o = &model_cannon1;
      laser [i]->zoom = 0.08;
      laser [i]->drawlight = false;
    }
    else
    {
      laser [i]->o = &model_cannon2;
      laser [i]->zoom = 0.1;
    }
  }

  for (i = 0; i < maxmissile; i ++)
  {
    missile [i]->aiinit ();
    missile [i]->deactivate ();
  }

  for (i = 0; i < maxflare; i ++)
  {
    flare [i]->dinit ();
    flare [i]->deactivate ();
  }

  for (i = 0; i < maxchaff; i ++)
  {
    chaff [i]->dinit ();
    chaff [i]->deactivate ();
  }

  for (i = 0; i < maxstar; i ++)
  {
    star [i]->phi = myrandom (360);
    star [i]->gamma = myrandom (85);
    star [i]->size = 0.6 + 0.15 * myrandom (8);
  }

  CColor skycolor;

  if (day && weather == WEATHER_SUNNY)
  {
    if (sungamma < 35)
    {
      skycolor.setColor ((unsigned short) (127 + 70 - 2 * sungamma), 127, 127);
      objsphere->setColor (&skycolor);
    }
    else
    {
      skycolor.setColor (127, 127, 127 + 5);
      objsphere->setColor (&skycolor);
    }
    if (sungamma < 35)
    {
      gl->fogcolor [0] = (float) (127 + 70 - 2 * sungamma) / 256.0;
      gl->fogcolor [1] = 0.5;
      gl->fogcolor [2] = 0.5;
    }
    else
    {
      gl->fogcolor [0] = 0.5;
      gl->fogcolor [1] = 0.5;
      gl->fogcolor [2] = 0.52;
    }
    skycolor.setColor (50, 200, 255);
    objsphere->setNorthPoleColor (&skycolor, 1.8);
    if (sungamma < 35)
    {
      skycolor.setColor ((unsigned short) (180 + 70 - 2 * sungamma), 180, 180);
      objsphere->setPoleColor (270, (int) (90 - sungamma), &skycolor, 0.3);
    }
    else
    {
      skycolor.setColor (200, 200, 200);
      objsphere->setPoleColor (270, (int) (90 - sungamma), &skycolor, 0.3);
    }
  }
  else if (!day && weather == WEATHER_SUNNY)
  {
    skycolor.setColor (64, 64, 64);
    objsphere->setColor (&skycolor);
    gl->fogcolor [0] = 0.25;
    gl->fogcolor [1] = 0.25;
    gl->fogcolor [2] = 0.25;
    if (l->type != LAND_MOON)
    {
      skycolor.setColor (0, 0, 170);
      objsphere->setNorthPoleColor (&skycolor, 1.8);
      skycolor.setColor (64, 64, 64);
      objsphere->setPoleColor (270, (int) (90 - sungamma), &skycolor, 0.3);
    }
    else
    {
      skycolor.setColor (0, 0, 0);
      objsphere->setNorthPoleColor (&skycolor, 1.8);
    }
  }
  else if (day && weather == WEATHER_THUNDERSTORM)
  {
    skycolor.setColor (102, 102, 102);
    objsphere->setColor (&skycolor);
    gl->fogcolor [0] = 0.4;
    gl->fogcolor [1] = 0.4;
    gl->fogcolor [2] = 0.4;
    skycolor.setColor (102, 102, 102);
    objsphere->setNorthPoleColor (&skycolor, 1.8);
  }
  else if (!day && weather == WEATHER_THUNDERSTORM)
  {
    skycolor.setColor (40, 40, 40);
    objsphere->setColor (&skycolor);
    gl->fogcolor [0] = 0.16;
    gl->fogcolor [1] = 0.16;
    gl->fogcolor [2] = 0.16;
    skycolor.setColor (40, 40, 40);
    objsphere->setNorthPoleColor (&skycolor, 1.8);
  }
  else if (weather == WEATHER_CLOUDY)
  {
    skycolor.setColor (20, 20, 20);
    objsphere->setColor (&skycolor);
    gl->fogcolor [0] = 0.08;
    gl->fogcolor [1] = 0.08;
    gl->fogcolor [2] = 0.08;
  }
  glDeleteLists (objsphere->list1, 1);
  glDeleteLists (objsphere->list2, 1);
  glDeleteLists (objsphere->list3, 1);
  objsphere->list1 = -1;
  objsphere->list2 = -1;
  objsphere->list3 = -1;

//  glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient );
/*  float light_specular [3] = {0.5, 0.5, 0.5};
  glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular );*/
  setLightSource ((int) sungamma);
  glLightModeli (GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
//  glLightModeli (GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
  glEnable (GL_LIGHT0);                // Turn on a light with defaults set
  float light_ambient [4] = {0.2, 0.2, 0.2, 1.0};
  float light_diffuse [4] = {1.0, 1.0, 1.0, 1.0};
  float light_specular [4] = {1.0, 1.0, 1.0, 1.0};
  glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv (GL_LIGHT0, GL_SPECULAR, light_specular);
  glEnable (GL_LIGHTING);                // Turn on lighting
  glEnable (GL_COLOR_MATERIAL);            // Allow color

  glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
//  glDisable (GL_DITHER);

  fplayer->missiletype = fplayer->firstMissile ();
  initing = false;
  lastshield = (int) fplayer->shield;

  fps = 30;

  redout = 0;
  blackout = 0;
  
  if (!fplayer->ai)
    fplayer->realism = physics;

/*#ifdef HAVE_SDL_NET
  if (multiplayer)
  {
    if (threadnet != NULL)
      SDL_KillThread (threadnet);
    threadnet = SDL_CreateThread (net_thread_main, NULL);
  }
#endif*/

  return 1;
}

void game_reshape ()
{
  glViewport(0, 0, (GLint) width, (GLint) height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  float v = getView ();
  if (camera == 50) v = 100000.0;
  gluPerspective (visibleangle, (float) width / height, nearclippingplane * GLOBALSCALE, v * GLOBALSCALE);
  glPolygonMode (GL_FRONT_AND_BACK, polygonMode);

#ifndef USE_GLUT
/*  if (controls == 1)
    SDL_ShowCursor (1);
  else*/
    SDL_ShowCursor (0);
#endif
}

void menu_reshape ()
{
  glViewport(0, 0, (GLint) width, (GLint) height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  float v = getView ();
  if (camera == 50) v = 100000.0;
  gluPerspective (visibleangle, 1.0, nearclippingplane * GLOBALSCALE, v * GLOBALSCALE);
  glPolygonMode (GL_FRONT_AND_BACK, polygonMode);

#ifndef USE_GLUT
  SDL_ShowCursor (0);
#endif
}

void credits_reshape ()
{
  glViewport(0, 0, (GLint) width, (GLint) height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  float v = getView ();
  if (camera == 50) v = 100000.0;
  gluPerspective (visibleangle, 1.0, nearclippingplane * GLOBALSCALE, v * GLOBALSCALE);
  glPolygonMode (GL_FRONT_AND_BACK, polygonMode);

#ifndef USE_GLUT
  SDL_ShowCursor (0);
#endif
}

void stats_reshape ()
{
  glViewport(0, 0, (GLint) width, (GLint) height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  float v = getView ();
  if (camera == 50) v = 100000.0;
  gluPerspective (visibleangle, 1.0, nearclippingplane * GLOBALSCALE, v * GLOBALSCALE);
  glPolygonMode (GL_FRONT_AND_BACK, polygonMode);

#ifndef USE_GLUT
  SDL_ShowCursor (0);
#endif
}

void playRandomMusic ()
{
  int r = myrandom (6);
  if (r == 0)
    sound->loadMusic (MUSIC_DARK1);
  else if (r == 1)
    sound->loadMusic (MUSIC_STANDBY1);
  else if (r == 2)
    sound->loadMusic (MUSIC_ELECTRO1);
  else if (r == 3)
    sound->loadMusic (MUSIC_STARS1);
  else if (r == 4)
    sound->loadMusic (MUSIC_SOFTTEC1);
  else
    sound->loadMusic (MUSIC_AMBIENT1);
  sound->playMusic (1);
#ifdef HAVE_SDL_MIXER
  Mix_HookMusicFinished (playRandomMusic);
#endif
}

void switch_menu ()
{
  lasttime = 0;
  setLightSource ((int) sungamma);
  game = GAME_MENU;
  menu_reshape ();
  sound->stop (SOUND_PLANE1);
  if (!sound->musicplaying)
    playRandomMusic ();
  allmenus.visible = true;
  allmenus.components [0]->setVisible (true);
  if (currentsubmenu)
  {
    currentsubmenu->setVisible (true);
  }
  if (missionactive)
    mainbutton [6]->setVisible (true);
  else
    mainbutton [6]->setVisible (false);
#ifdef HAVE_SDL
  SDL_WM_GrabInput (SDL_GRAB_OFF);
#endif
}

bool ispromoted;
int statsitemselected = 0;

void switch_stats ()
{
  lasttime = 0;
  setLightSource ((int) sungamma);
  game = GAME_STATS;
  statsitemselected = 0;
  int lastrank = pilots->pilot [pilots->aktpilot]->ranking;
  (void) pilots->pilot [pilots->aktpilot]->getRank ();
  ispromoted = false;
  if (lastrank < pilots->pilot [pilots->aktpilot]->ranking)
    ispromoted = true;
  stats_reshape ();
  sound->stop (SOUND_PLANE1);
  if (!sound->musicplaying)
  {
    int missionstate = mission->processtimer (1);
    if (missionstate == 1)
      sound->loadMusic (MUSIC_WINNER1);
    else
      sound->loadMusic (MUSIC_LOSER1);
    sound->playMusic ();
  }
}

void createMission (int missionid)
{
  if (missionid >= MISSION_MULTIPLAYER)
  {
    multiplayer = true;
  }
  else
  {
    multiplayer = false;
    isserver = false;
  }
  if (missionnew != NULL) delete missionnew;
  if (missionid == MISSION_DEMO) missionnew = new MissionDemo1 ();
  else if (missionid == MISSION_TEST1) missionnew = new MissionTest1 ();
  else if (missionid == MISSION_TEST2) missionnew = new MissionTest2 ();
  else if (missionid == MISSION_TRANSPORT) missionnew = new MissionTransport ();
  else if (missionid == MISSION_CONVOY) missionnew = new MissionConvoy ();
  else if (missionid == MISSION_DOGFIGHT2) missionnew = new MissionDogfight2 ();
  else if (missionid == MISSION_AIRBATTLE) missionnew = new MissionAirBattle ();
  else if (missionid == MISSION_SADEFENSE) missionnew = new MissionGround1 ();
  else if (missionid == MISSION_SCOUT) missionnew = new MissionScout ();
  else if (missionid == MISSION_BASE) missionnew = new MissionBase ();
  else if (missionid == MISSION_DEPOT) missionnew = new MissionDepot ();
  else if (missionid == MISSION_DEFEND1) missionnew = new MissionDefend1 ();
  else if (missionid == MISSION_DOGFIGHT3) missionnew = new MissionDogfight3 ();
  else if (missionid == MISSION_TANK1) missionnew = new MissionTank1 ();
  else if (missionid == MISSION_CONVOY2) missionnew = new MissionConvoy2 ();
  else if (missionid == MISSION_SHIP1) missionnew = new MissionShip1 ();
  else if (missionid == MISSION_SHIP2) missionnew = new MissionShip2 ();
  else if (missionid == MISSION_SHIP3) missionnew = new MissionShip3 ();
  else if (missionid == MISSION_CANYON1) missionnew = new MissionCanyon1 ();
  else if (missionid == MISSION_CANYON2) missionnew = new MissionCanyon2 ();
  else if (missionid == MISSION_CANYON3) missionnew = new MissionCanyon3 ();
  else if (missionid == MISSION_TUNNEL1) missionnew = new MissionTunnel1 ();
  else if (missionid == MISSION_MOON1) missionnew = new MissionMoonDefense1 ();
  else if (missionid == MISSION_MOONBATTLE) missionnew = new MissionMoonBattle ();
  else if (missionid == MISSION_MOON2) missionnew = new MissionMoonDogfight1 ();
  else if (missionid == MISSION_MOON3) missionnew = new MissionMoonBase1 ();
  else if (missionid == MISSION_TUTORIAL) missionnew = new MissionTutorial1 ();
  else if (missionid == MISSION_TUTORIAL2) missionnew = new MissionTutorial2 ();
  else if (missionid == MISSION_DOGFIGHT) missionnew = new MissionDogfight1 ();
  else if (missionid == MISSION_TUTORIAL3) missionnew = new MissionTutorial3 ();
  else if (missionid == MISSION_FREEFLIGHT1) missionnew = new MissionFreeFlight1 ();
  else if (missionid == MISSION_DEATHMATCH1) missionnew = new MissionDeathmatch1 ();
  else if (missionid == MISSION_DEATHMATCH2) missionnew = new MissionDeathmatch2 ();
  else if (missionid == MISSION_DEATHMATCH3) missionnew = new MissionDeathmatch3 ();
  else if (missionid == MISSION_TEAMBASE1) missionnew = new MissionTeamBase1 ();
  else if (missionid == MISSION_WAVES1) missionnew = new MissionWaves1 ();
  else if (missionid == MISSION_MULTIPLAYER_DOGFIGHT) missionnew = new MissionMultiDogfight1 ();
  else if (missionid >= MISSION_CUSTOM1 && missionid <= MISSION_CUSTOM2)
  {
    missionnew = new MissionCustom ();
    missionnew->id = missionid;
    ((MissionCustom *) missionnew)->init ();
  }
  if (mission != NULL)
  {
    if (mission->id == missionnew->id)
    {
      missionnew->wantfighter = mission->wantfighter;
      missionnew->wantweapon = mission->wantweapon;
    }
  }
}

void switch_mission (int missionid)
{
  lasttime = 0;
  setLightSource (60);
  game = GAME_MISSION;
  createMission (missionid);
  if (missionid >= MISSION_CUSTOM1 && missionid <= MISSION_CUSTOM2)
  {
    if (((MissionCustom *) missionnew)->reterror)
    {
      display ("Could not init mission", LOG_ERROR);
      // play error sound
      switch_menu ();
      return;
    }
  }
  menu_reshape ();
  sound->stop (SOUND_PLANE1);
  if (!sound->musicplaying)
    playRandomMusic ();
}

void switch_fame ()
{
  lasttime = 0;
  setLightSource (60);
  game = GAME_FAME;
  menu_reshape ();
  sound->stop (SOUND_PLANE1);
  if (!sound->musicplaying)
    playRandomMusic ();
}

void switch_fighter ()
{
  lasttime = 0;
  setLightSource (60);
  game = GAME_FIGHTER;
  menu_reshape ();
  sound->stop (SOUND_PLANE1);
  if (!sound->musicplaying)
    playRandomMusic ();
}

void switch_create ()
{
  lasttime = 0;
  setLightSource (60);
  game = GAME_CREATE;
  isserver = true;
  if (server != NULL) delete server;
  server = new Server ();
  menu_reshape ();
  sound->stop (SOUND_PLANE1);
  if (!sound->musicplaying)
    playRandomMusic ();
}

void switch_join ()
{
  lasttime = 0;
  setLightSource (60);
  game = GAME_JOIN;
  isserver = false;
  if (client != NULL) delete client;
  client = new Client ();
  menu_reshape ();
  sound->stop (SOUND_PLANE1);
  if (!sound->musicplaying)
    playRandomMusic ();
}

int creditstimer, finishtimer;

void switch_credits ()
{
  lasttime = 0;
  game = GAME_CREDITS;
  creditstimer = 50 * timestep;
  credits_reshape ();
  sound->stop (SOUND_PLANE1);
  sound->stop (SOUND_CANNON1);
}

void switch_finish ()
{
  lasttime = 0;
  game = GAME_FINISH;
  creditstimer = 50 * timestep;
  credits_reshape ();
  if (sound->musicplaying)
    sound->haltMusic ();
  sound->loadMusic (MUSIC_ELECTRO1);
  sound->playMusic ();
  sound->stop (SOUND_PLANE1);
  sound->stop (SOUND_CANNON1);
}

void switch_quit ()
{
  lasttime = 0;
  game = GAME_QUIT;
  save_saveconfig (); // this configuration seems to work => save it
}

void switch_game ()
{
  lasttime = 0;
  setLightSource ((int) sungamma);
  game = GAME_PLAY;
  game_reshape ();
  if (sound->musicplaying)
    sound->haltMusic ();
  sound->playLoop (SOUND_PLANE1);
  setPlaneVolume ();
#ifdef HAVE_SDL
  SDL_WM_GrabInput (SDL_GRAB_ON);
#endif
}

bool startcannon = false;

void event_setAntialiasing ()
{
  if (antialiasing)
  {
    gl->enableLinearTexture (font1->texture->textureID);
    gl->enableLinearTexture (font2->texture->textureID);
    if (texmoon != NULL)
    {
      gl->enableLinearTexture (texmoon->textureID);
      gl->enableLinearTexture (texsun->textureID);
      gl->enableLinearTexture (texearth->textureID);
    }
  }
  else
  {
    gl->disableLinearTexture (font1->texture->textureID);
    gl->disableLinearTexture (font2->texture->textureID);
    if (texmoon != NULL)
    {
      gl->disableLinearTexture (texmoon->textureID);
      gl->disableLinearTexture (texsun->textureID);
      gl->disableLinearTexture (texearth->textureID);
    }
  }
}

void event_fireCannon ()
{
  if (!fplayer->active) return;
  if (fplayer->firecannonttl > 0) return;
  if (fplayer->ammo == 0) return;
#ifdef USE_GLUT
  fplayer->fireCannon (laser);
#else
  fplayer->autofire = true;
#endif
}

void event_stopCannon ()
{
  fplayer->autofire = false;
}

void event_fireMissile ()
{
  if (!fplayer->active) return;
  if (fplayer->firemissilettl > 0) return;
  if (fplayer->fireMissile (fplayer->missiletype + MISSILE1, missile))
    sound->play (SOUND_MISSILE1, false);
}

void event_fireChaff ()
{
  if (!fplayer->active) return;
  if (fplayer->firechaffttl > 0) return;
  if (fplayer->fireChaff (chaff, missile))
    sound->play (SOUND_CHAFF1, false);
}

void event_fireFlare ()
{
  if (!fplayer->active) return;
  if (fplayer->fireflarettl > 0) return;
  if (fplayer->fireFlare (flare, missile))
    sound->play (SOUND_CHAFF1, false);
}

void event_selectMissile ()
{
  if (!fplayer->active) return;
  fplayer->missiletype = fplayer->nextMissile (fplayer->missiletype);
}

void event_targetNearest ()
{
  if (!fplayer->active) return;
  fplayer->targetNearestEnemy ((AIObj **) fighter);
  sound->play (SOUND_CLICK1, false);
}

void event_targetNext ()
{
  if (!fplayer->active) return;
  fplayer->targetNext ((AIObj **) fighter);
  sound->play (SOUND_CLICK1, false);
}

void event_targetPrevious ()
{
  if (!fplayer->active) return;
  fplayer->targetPrevious ((AIObj **) fighter);
  sound->play (SOUND_CLICK1, false);
}

void event_targetLocking ()
{
  if (!fplayer->active) return;
  fplayer->targetLockingEnemy ((AIObj **) fighter);
  sound->play (SOUND_CLICK1, false);
}

void event_thrustUp ()
{
  if (!fplayer->active) return;
  fplayer->thrustUp ();
  fplayer->thrustUp ();
}

void event_thrustDown ()
{
  if (!fplayer->active) return;
  fplayer->thrustDown ();
  fplayer->thrustDown ();
}

void game_key (int key, int x, int y)
{
  unsigned int hikey = key;
  unsigned int lokey = key;

  if (camera == 50 && game == GAME_PAUSE)
  {
    camera = 0;
    game = GAME_PLAY;
    return;
  }
  else if (key == 27)
  {
    switch_menu ();
  }
  else if (hikey == key_firemissile || lokey == key_firemissile)
  {
    event_fireMissile ();
  }
  else if (hikey == key_dropflare || lokey == key_dropflare)
  {
    event_fireFlare ();
  }
  else if (hikey == key_dropchaff || lokey == key_dropchaff)
  {
    event_fireChaff ();
  }
  else if (hikey == key_firecannon || lokey == key_firecannon)
  {
    event_fireCannon ();
  }
  else if (hikey == key_selectmissile || lokey == key_selectmissile)
  {
    event_selectMissile ();
  }
  else if (hikey == key_thrustup || lokey == key_thrustup)
  {
    event_thrustUp ();
  }
  else if (hikey == key_thrustdown || lokey == key_thrustdown)
  {
    event_thrustDown ();
  }
  else if (key >= '1' && key <= '9')
  {
    fplayer->recthrust = fplayer->maxthrust * (1.0 / 18.0 * (key - '0') + 0.5);
    sound->play (SOUND_CLICK1, false);
  }
  else if (hikey == key_targetnearest || lokey == key_targetnearest)
  {
    event_targetNearest ();
  }
  else if (hikey == key_targetnext || lokey == key_targetnext)
  {
    event_targetNext ();
  }
  else if (hikey == key_targetprevious || lokey == key_targetprevious)
  {
    event_targetPrevious ();
  }
  else if (hikey == key_targetlocking || lokey == key_targetlocking)
  {
    event_targetLocking ();
  }
/*  else if (key == 'K')
  {
    if (fplayer->target != NULL)
      fplayer->target->shield = -1;
  }
  else if (key == 'V')
  {
    visibleangle += 10.0;
    if (visibleangle > 120.0) visibleangle = 50.0;
  }*/
  else if (hikey == key_firecannon || lokey == key_firecannon)
  {
    fplayer->autofire = false;
  }

  switch (key - 256) // standard special keys
  {
    case KEY_LSHIFT:
      keyb_lshift = 1;
      break;
    case KEY_UP:
      keyb_elev = -1;
      break;
    case KEY_DOWN:
      keyb_elev = 1;
      break;
    case KEY_LEFT:
      if (keyb_lshift) keyb_rudder = -1;
      else keyb_roll = 1;
      break;
    case KEY_RIGHT:
      if (keyb_lshift) keyb_rudder = 1;
      else keyb_roll = -1;
      break;
    case KEY_F1:
      camera = 0;
      game_reshape ();
      break;
    case KEY_F2:
      camera = 1;
      game_reshape ();
      break;
    case KEY_F3:
      camera = 2;
      game_reshape ();
      break;
    case KEY_F4:
      camera = 6;
      game_reshape ();
      break;
    case KEY_F5:
      camera = 7;
      game_reshape ();
      break;
    case KEY_F6:
      camera = 9;
      game_reshape ();
      break;
    case KEY_F7:
      camera = 8;
      game_reshape ();
      break;
    case KEY_F8:
      camera = 5;
      game_reshape ();
      break;
    default:
      return;
  }

#ifdef USE_GLUT
  glutPostRedisplay();
#else
  sdldisplay = true;
#endif
}

void game_keyup (int key, int x, int y)
{
  unsigned int hikey = (unsigned int) key;
  unsigned int lokey = (unsigned int) key;

  if (hikey == key_firecannon || lokey == key_firecannon)
  {
    event_stopCannon ();
  }

  switch (key - 256) // standard special keys
  {
    case KEY_LSHIFT:
      keyb_lshift = 0;
      break;
    case KEY_UP:
      keyb_elev = 0;
      break;
    case KEY_DOWN:
      keyb_elev = 0;
      break;
    case KEY_LEFT:
      keyb_roll = 0;
      keyb_rudder = 0;
      break;
    case KEY_RIGHT:
      keyb_roll = 0;
      keyb_rudder = 0;
      break;
    case KEY_LALT:
      keyb_rudder = 0;
      break;
    case KEY_LCTRL:
      keyb_rudder = 0;
      break;
  }

#ifdef USE_GLUT
    glutPostRedisplay();
#else
    sdldisplay = true;
#endif
}

int aktcam = 0;

void game_mouse (int button, int state, int x, int y)
{
  if (state == MOUSE_DOWN && fplayer->active)
  {
    if (button == mouse_firecannon)
    {
      fplayer->autofire = !fplayer->autofire;
    }
    else if (button == mouse_firemissile)
    {
      event_fireMissile ();
    }
    else if (button == mouse_selectmissile)
    {
      event_selectMissile ();
    }
  }
}

float dtheta = 0, dgamma = 0;

int lastmousex = 0;

int lastmx = 0, lastmy = 0;

void game_mouserelmotion (int xrel, int yrel)
{
  if (controls != CONTROLS_MOUSE || !mouse_relative) return;
  float xr = (float) xrel / width, yr = (float) yrel / height;
  
  float roll = (float) -xr * 20;

  if (roll > 1.0F) roll = 1.0F;
  else if (roll < -1.0F) roll = -1.0F;
  fplayer->rolleffect = roll;
  if (roll < 1E-3)
    fplayer->ruddereffect = (float) xr * 200;
  else 
    fplayer->ruddereffect = 0.0f; 
  if (fplayer->ruddereffect > 1.0) fplayer->ruddereffect = 1.0;
  else if (fplayer->ruddereffect < -1.0) fplayer->ruddereffect = -1.0;

  fplayer->elevatoreffect = (float) yr * fabs (yr) * 20000;
  if (!mouse_reverse) fplayer->elevatoreffect *= -1;
  if (fplayer->elevatoreffect > 1.0f) fplayer->elevatoreffect = 1.0f; 
  else if (fplayer->elevatoreffect < -0.5f) fplayer->elevatoreffect = -0.5f; 
}

void game_mousemotion (int x, int y)
{
  if (controls != CONTROLS_MOUSE || mouse_relative) return;

  float mx = width / 2, my = height / 2;
  float dx = x - mx, dy = my - y;
  dx *= mouse_sensitivity / 70.0F;
  dy *= mouse_sensitivity / 70.0F;
  if (mouse_reverse)
    dy *= -1;

// mouse interface code added by Lourens Veen
  float nx = dx / width; // normalised x-coordinate, -1 at lefthand 
                         // side of the screen, 1 at righthand side 
  if (mouse_autorudder)
  {
    if (fabs(nx) < (1.0f/3.0f)) 
    if (nx < 0.0f) 
      fplayer->ruddereffect = 729.0f*nx*(nx+1.0f/3.0f)*(nx+1.0f/3.0f)/4.0f; 
    else 
      fplayer->ruddereffect = 729.0f*nx*(nx-1.0f/3.0f)*(nx-1.0f/3.0f)/4.0f; 
    else 
      fplayer->ruddereffect = 0.0f; 
  }
  else
  {
    fplayer->ruddereffect = 0;
  }

  const float roll_deadarea = (float) mouse_autorudder / 1000.0F; //(1.0f/30.0f);

  if (fabs(nx) > roll_deadarea) 
  { 
    if (nx > 0.0f) nx -= roll_deadarea; 
    else nx += roll_deadarea; 

    if (nx > 0) 
      fplayer->rolleffect = -(exp(log(nx) * 1.3f)) * 3.0f; 
    else 
      fplayer->rolleffect = (exp(log(-nx) * 1.3f)) * 3.0f;

    if (fplayer->rolleffect < -1.0F) fplayer->rolleffect = -1.0F;
    if (fplayer->rolleffect > 1.0F) fplayer->rolleffect = 1.0F;
  } 
  else 
    fplayer->rolleffect = 0.0f; 

  fplayer->elevatoreffect = dy / height * 2.5; 
  if (fplayer->elevatoreffect > 1.0f) fplayer->elevatoreffect = 1.0f; 
  else if (fplayer->elevatoreffect < -0.5f) fplayer->elevatoreffect = -0.5f; 

#ifdef USE_GLUT
  glutPostRedisplay();
#else
  sdldisplay = true;
#endif
}

int jaxis [maxjaxis * maxjoysticks];

int getJoystickAxisIndex (int n)
{
  int idx = (n / 1000) * 10 + (n % 1000);
  if (idx < 0) idx = 0;
  if (idx >= maxjaxis) idx = 0;
  return idx;
}

void game_joystickaxis (int x, int y, int rudder, int throttle)
{
  if (fplayer->ai) return;
  float xx = x / 32768.0;
  xx *= fabs (xx);
  fplayer->rolleffect = (float) -xx;
  fplayer->elevatoreffect = (float) y / 30000;
  if (fplayer->elevatoreffect > 1.0) fplayer->elevatoreffect = 1.0;
  else if (fplayer->elevatoreffect < -1.0) fplayer->elevatoreffect = -1.0;
  if (fplayer->elevatoreffect < -0.5) fplayer->elevatoreffect = -0.5;
  fplayer->ruddereffect = (float) rudder / 30000;
  if (fplayer->ruddereffect > 1.0) fplayer->ruddereffect = 1.0;
  else if (fplayer->ruddereffect < -1.0) fplayer->ruddereffect = -1.0;
  fplayer->recthrust = fplayer->maxthrust * 0.75 - fplayer->maxthrust / 4 * throttle / 32638;
}

void game_joystickbutton (int button)
{
  if (!fplayer->active) return;
  if (button == joystick_firecannon)
  {
    fplayer->fireCannon (laser);
#ifdef HAVE_SDL_MIXER
    if (!startcannon)
    {
      sound->playLoop (SOUND_CANNON1);
      startcannon = true;
    }
#else
    sound->play (SOUND_CANNON1, 1);
#endif
  }
  if (button == joystick_firemissile)
  {
    event_fireMissile ();
  }
  if (button == joystick_dropflare)
  {
    event_fireFlare ();
  }
  if (button == joystick_dropchaff)
  {
    event_fireChaff ();
  }
  if (button == joystick_selectmissile)
  {
    event_selectMissile ();
  }
  if (button == joystick_targetnearest)
  {
    event_targetNearest ();
  }
  if (button == joystick_targetlocking)
  {
    event_targetLocking ();
  }
  if (button == joystick_targetnext)
  {
    event_targetNext ();
  }
  if (button == joystick_targetprevious)
  {
    event_targetPrevious ();
  }
  if (button == joystick_thrustup)
  {
    event_thrustUp ();
  }
  if (button == joystick_thrustdown)
  {
    event_thrustDown ();
  }
}

void game_joystickhat (int normhat)
{
#ifndef USE_GLUT
  game_joystickbutton (normhat);
#endif
}

int frames = 0, time2 = 0, time1 = 0;

void frame ()
{
  frames ++;
#ifndef USE_GLUT
  time2 = SDL_GetTicks ();
#else
  time2 = glutGet (GLUT_ELAPSED_TIME);
#endif
  if (time2 - time1 > 1000)
  {
    if (time2 - time1 < 1500)
      fps = (float) frames * 1000.0 / (time2 - time1);
    time1 = time2;
    frames = 0;
  }
}

void game_view ()
{
  frame ();
#ifdef USE_GLUT
  glutSwapBuffers();
#else
  SDL_GL_SwapBuffers ();
#endif
}

int missionending = 0;
int missionstate = 0;

int menuitem = 0, menutimer = 0, menuitemselected = -1, missionmenutimer;
int missionmenuitemselected = 0, missionmenufighterselected = -1, missionmenuweaponselected = -1;

void drawCircles (CColor *colorstd)
{
  int i;
  float zf = -3, yf, xf;
  int t = 360 - (missionmenutimer * 10 / timestep) % 360;
  for (i = 0; i < 20; i ++)
  {
    yf = -3 + 0.3 * i; // - (float) (missionmenutimer & 63) / 64.0;
    glBegin (GL_QUAD_STRIP);
    for (int i2 = 0; i2 < 20; i2 ++)
    {
      xf = -3 + 0.3 * i2;
      yf = -3 + 0.3 * i;
      float cola = sine [(int) (sqrt (xf * xf + yf * yf) * 200 + t) % 360] / 10 + 0.2;
      if (colorstd == &colorblue) glColor3f (0, 0, cola);
      else glColor3f (cola, 0, 0);
      glVertex3f (xf, yf, zf);
      yf = -3 + 0.3 * (i + 1);
      cola = sine [(int) (sqrt (xf * xf + yf * yf) * 200 + t) % 360] / 10 + 0.2;
      if (colorstd == &colorblue) glColor3f (0, 0, cola);
      else glColor3f (cola, 0, 0);
      glVertex3f (xf, yf, zf);
    }
    glEnd ();
  }
}

void drawQuads (CColor *colorstd)
{
  int i;
  float zf = -3, yf;
  for (i = 0; i < 14; i ++)
  {
    yf = -3 + 0.5 * i - (float) (missionmenutimer / timestep & 63) / 64.0;
    glBegin (GL_QUAD_STRIP);
    for (int i2 = 0; i2 < 14; i2 ++)
    {
      float cola = sine [(i * 100+missionmenutimer*4 / timestep) % 360] / 10 + sine [(i2 * 100) % 360] / 10 + 0.2;
      if (colorstd == &colorblue) glColor3f (0, 0, cola);
      else glColor3f (cola, 0, 0);
      glVertex3f (-3 + 0.5 * i2, -3 + 0.5 * i, zf + sine [(i * 100) % 360] / 2);
      cola = sine [((i+1) * 100+missionmenutimer*4 / timestep) % 360] / 10 + sine [(i2 * 100) % 360] / 10 + 0.2;
      if (colorstd == &colorblue) glColor3f (0, 0, cola);
      else glColor3f (cola, 0, 0);
      glVertex3f (-3 + 0.5 * i2, -2.5 + 0.5 * i, zf + sine [((i+1) * 100) % 360] / 2);
    }
    glEnd ();
  }
}

void drawPlasma (CColor *colorstd)
{
  int i;
  float yf;
  float zf1 = -2.98;
  float yind = 200.0 * sin (0.003 * missionmenutimer / timestep) + 200.0;
  float xind = 200.0 * cos (0.003 * missionmenutimer / timestep) + 200.0;
  float yind2 = -200.0 * sin (0.003 * missionmenutimer / timestep) + 200.0;
  float xind2 = -200.0 * cos (0.003 * missionmenutimer / timestep) + 200.0;
  for (i = 0; i < 21; i ++)
  {
    yf = -2.5 + 0.25 * i;
    float yif = yind + i * 5;
    int yi = (int) yif;
    float yr = yif - yi;
    float yif2 = yind2 + i * 5;
    int yi2 = (int) yif2;
    float yr2 = yif2 - yi2;
    glBegin (GL_QUAD_STRIP);
    for (int i2 = 0; i2 < 21; i2 ++)
    {
      float xf = -2.5 + 0.25 * i2;
      float xif = xind + i2 * 5;
      int xi = (int) xif;
      float xr = xif - xi;
      float xif2 = xind2 + i2 * 5;
      int xi2 = (int) xif2;
      float xr2 = xif2 - xi2;
      float h1 = xr * yr * l->h [yi + 1] [xi + 1] + (1.0 - xr) * (1.0 - yr) * l->h [yi] [xi] +
                 xr * (1.0 - yr) * l->h [yi] [xi + 1] + (1.0 - xr) * yr * l->h [yi + 1] [xi];
      float h2 = xr * yr * l->h [yi + 1 + 5] [xi + 1] + (1.0 - xr) * (1.0 - yr) * l->h [yi + 5] [xi] +
                 xr * (1.0 - yr) * l->h [yi + 5] [xi + 1] + (1.0 - xr) * yr * l->h [yi + 1 + 5] [xi];
      float h3 = xr2 * yr2 * l->h [yi2 + 1] [xi2 + 1] + (1.0 - xr2) * (1.0 - yr2) * l->h [yi2] [xi2] +
                 xr2 * (1.0 - yr2) * l->h [yi2] [xi2 + 1] + (1.0 - xr2) * yr2 * l->h [yi2 + 1] [xi2];
      float h4 = xr2 * yr2 * l->h [yi2 + 1 + 5] [xi2 + 1] + (1.0 - xr2) * (1.0 - yr2) * l->h [yi2 + 5] [xi2] +
                 xr2 * (1.0 - yr2) * l->h [yi2 + 5] [xi2 + 1] + (1.0 - xr2) * yr2 * l->h [yi2 + 1 + 5] [xi2];
      float intens = sin (0.15 * (h1 / 256 + 0.5 * missionmenutimer / timestep)) * 0.14 + 0.14;
      float intens2 = sin (0.15 * (h3 / 256 + 0.5 * missionmenutimer / timestep)) * 0.06 + 0.06;
      if (colorstd == &colorblue) glColor3f (intens2, 0, intens);
      else glColor3f (intens, 0, intens2);
      glVertex3f (xf, yf, zf1);
      intens = sin (0.15 * (h2 / 256 + 0.5 * missionmenutimer / timestep)) * 0.16 + 0.16;
      intens2 = sin (0.15 * (h4 / 256 + 0.5 * missionmenutimer / timestep)) * 0.08 + 0.08;
      if (colorstd == &colorblue) glColor3f (intens2, 0, intens);
      else glColor3f (intens, 0, intens2);
      glVertex3f (xf, yf + 0.25, zf1);
    }
    glEnd ();
  }
}

void menu_key (unsigned char key, int x, int y)
{
  if (key == 27)
  {
    if (missionactive)
    {
      switch_game ();
    }
    else
    {
      switch_quit ();
    }
  }
}

void pleaseWait ()
{
  float xf = 1.0, yf = 0.2, zf = -2.5;
  int c1 = 100, c2 = 10, c3 = 180;
  glBegin (GL_QUADS);
  glColor4ub (c2, c2, c2, 255);
  glVertex3f (-xf, -yf, zf);
  glColor4ub (c1, c1, c1, 255);
  glVertex3f (xf, -yf, zf);
  glColor4ub (c2, c2, c2, 255);
  glVertex3f (xf, yf, zf);
  glColor4ub (c1, c1, c1, 255);
  glVertex3f (-xf, yf, zf);
  glEnd ();
  glLineWidth (LINEWIDTH(1));
  glBegin (GL_LINE_STRIP);
  glColor4ub (c3, c3, c3, 255);
  glVertex3f (-xf, -yf, zf);
  glVertex3f (xf, -yf, zf);
  glVertex3f (xf, yf, zf);
  glVertex3f (-xf, yf, zf);
  glVertex3f (-xf, -yf, zf);
  glEnd ();

  font1->drawTextCentered (0, -0.5, -1.5, "PLEASE WAIT...", &colorwhite);
  game_view ();
}

void stats_key (unsigned char key, int x, int y)
{
  if (key == 27)
  {
    pleaseWait ();
    display ("Initing new mission", LOG_MOST);
    missionactive = false;
    createMission (MISSION_DEMO);
    game_levelInit ();
    sound->haltMusic ();
    switch_menu ();
  }
}

void credits_key (unsigned char key, int x, int y)
{
  if (key)
  {
//    sound->haltMusic ();
    switch_menu ();
  }
}

void finish_key (unsigned char key, int x, int y)
{
  if (key)
  {
//    sound->haltMusic ();
    switch_menu ();
  }
}

void mission_key (unsigned char key, int x, int y)
{
  if (key == 27)
  {
    switch_menu ();
  }
  else if (key == 13)
  {
    pleaseWait ();
    if (!game_levelInit ())
    {
      switch_menu ();
      return;
    }
    switch_game ();
    missionactive = true;
  }
}

void fame_key (unsigned char key, int x, int y)
{
  if (key == 27)
  {
    switch_menu ();
  }
}

void create_key (unsigned char key, int x, int y)
{
  if (key == 's')
  {
//    server->sendMessage (0, "s", 1);
    if (server->checkStart()) 
    {
      printf ("Starting..");
      createMission (MISSION_MULTIPLAYER_DOGFIGHT);
      game_levelInit ();
      switch_game ();
      missionactive = true;
    }
  }
  if (key == 27)
  {
    switch_menu ();
  }
}

void join_key (unsigned char key, int x, int y)
{
  if (key == 's')
  {
    createMission (MISSION_MULTIPLAYER_DOGFIGHT);
    game_levelInit ();
    switch_game ();
    missionactive = true;
  }
  if (key == 27)
  {
    switch_menu ();
  }
}

void fighter_key (unsigned char key, int x, int y)
{
  if (key == 27)
  {
    switch_menu ();
  }
}

char *getModelText (int id)
{
  if (id == FIGHTER_FALCON)
    return "THE FALCON IS A NIMBLE AND\nFAST FIGHTER PROVIDING\nBOTH AIR-AIR AND\nAIR-GROUND-MISSILES.";
  else if (id == FIGHTER_HAWK)
    return "THE HAWK IS A TYPICAL\nALLROUNDER AND DESIGNED\nFOR GROUND ATTACKS.";
  else if (id == FIGHTER_HAWK2)
    return "THE HAWK II IS A ...";
  return "NO INFO";
}

char *getModelName (int id)
{
  if (id == FIGHTER_FALCON)
    return "GL-16 FALCON";
  else if (id == FIGHTER_HAWK)
    return "GL-22 HAWK";
  else if (id == FIGHTER_HAWK2)
    return "GL-23 HAWK II";
  else if (id == FIGHTER_REDARROW)
    return "GL-15 REDARROW";
  else if (id == FIGHTER_PHOENIX)
    return "GL-117 PHOENIX";
  else if (id == FIGHTER_STORM)
    return "GL-50 STORM";
  else if (id == FIGHTER_CROW)
    return "CROW";
  else if (id == FIGHTER_BUZZARD)
    return "BUZZARD";
  else if (id == FIGHTER_SWALLOW)
    return "SWALLOW";
  else if (id == FIGHTER_BLACKBIRD)
    return "BLACKBIRD";
  else if (id == MISSILE_AIR1)
    return "AIR-AIR MISSILE PACK";
  else if (id == MISSILE_GROUND1)
    return "AIR-GROUND MISSILE PACK";
  else if (id == MISSILE_DF1)
    return "DUMB FIRE MISSILE PACK";
  return "NO INFO";
}

void mission_mouse (int button, int state, int x, int y)
{
  float rx = (float) x / width;
  float ry = (float) y / height;
  missionmenuitemselected = -1;

  missionmenufighterselected = -1;
  if (ry >= 0.53 && ry <= 0.65)
  {
    if (rx >= 0.18 && rx < 0.28)
    {
      missionmenufighterselected = 0;
      if (state == MOUSE_DOWN)
        missionnew->wantfighter = 0;
    }
    if (rx >= 0.28 && rx < 0.38 && missionnew->selfighters >= 2)
    {
      missionmenufighterselected = 1;
      if (state == MOUSE_DOWN)
        missionnew->wantfighter = 1;
    }
    if (rx >= 0.38 && rx < 0.48 && missionnew->selfighters >= 3)
    {
      missionmenufighterselected = 2;
      if (state == MOUSE_DOWN)
        missionnew->wantfighter = 2;
    }
  }

  missionmenuweaponselected = -1;
  if (ry >= 0.53 && ry <= 0.65)
  {
    if (rx >= 0.54 && rx < 0.64)
    {
      missionmenuweaponselected = 0;
      if (state == MOUSE_DOWN)
        missionnew->wantweapon = 0;
    }
    if (rx >= 0.64 && rx < 0.74 && missionnew->selweapons >= 2)
    {
      missionmenuweaponselected = 1;
      if (state == MOUSE_DOWN)
        missionnew->wantweapon = 1;
    }
    if (rx >= 0.74 && rx < 0.84 && missionnew->selweapons >= 3)
    {
      missionmenuweaponselected = 2;
      if (state == MOUSE_DOWN)
        missionnew->wantweapon = 2;
    }
  }
}

void drawArrow (float x, float y, float w, float h)
{
  float zf = -3;
  gl->enableTextures (texarrow->textureID);
  gl->enableAlphaBlending ();
  glBegin (GL_QUADS);
  glColor3ub (180, 180, 180);

  if (w > h)
  {
    glTexCoord2f (0, 1);
    glVertex3f (x, y, zf);
    glTexCoord2f (0, 0);
    glVertex3f (x + w, y, zf);
    glTexCoord2f (1, 0);
    glVertex3f (x + w, y + h, zf);
    glTexCoord2f (1, 1);
    glVertex3f (x, y + h, zf);
  }
  else
  {
    glTexCoord2f (1, 0);
    glVertex3f (x, y - h, zf);
    glTexCoord2f (1, 1);
    glVertex3f (x, y, zf);
    glTexCoord2f (0, 1);
    glVertex3f (x + w, y, zf);
    glTexCoord2f (0, 0);
    glVertex3f (x + w, y - h, zf);
  }

  glEnd ();
  glDisable (GL_BLEND);
  glDisable (GL_TEXTURE_2D);
}

void mission_display ()
{
  allmenus.setVisible (false);
  missionmenu.setVisible (true);
  missionmenu.draw ();

  char buf [256];
  int i;
  float piloty = 5;
  Pilot *p = pilots->pilot [pilots->aktpilot];
  CColor *colorstd = &colorred;
  if (p->mission_state [missionnew->id] == 1)
    colorstd = &colorblue;
  CColor *col = &menu_colwhite;
  Font *font2 = font1;

  CVector3 vec;
  CVector3 tl (-4.5, 5.5, -8.0);
  CRotation rot;
  rot.a = 270;
  rot.b = (270 - missionmenutimer * 4 / timestep) % 360;
  rot.c = 90;
  
  // Draw dummy missile
  glEnable (GL_LIGHTING);
  model_missile1.draw (&vec, &tl, &rot, 0.05, 1.0, 0);
  glDisable (GL_LIGHTING);
  
  glEnable (GL_LIGHTING);
  glEnable (GL_DEPTH_TEST);
  
  tl.x = -0.24; tl.y = -0.075; tl.z = -0.5;
  rot.a = 300;
  rot.b = 0;
  for (i = 0; i < missionnew->selfighters; i ++)
  {
    tl.x = -0.24 + (float) i * 0.085;
    if (missionnew->wantfighter == i)
      rot.c = (5 + missionmenutimer * 4 / timestep) % 360;
    else
      rot.c = 5;
    getModel (missionnew->selfighter [i])->draw (&vec, &tl, &rot, 0.04, 0.1, 0);
  }

  tl.x = 0; tl.y = -0.075; tl.z = -0.5;
  rot.a = 300;
  rot.b = 0;
  for (i = 0; i < missionnew->selweapons; i ++)
  {
    tl.x = 0.08 + (float) i * 0.07;
    if (missionnew->wantweapon == i)
      rot.c = (5 + missionmenutimer * 4 / timestep) % 360;
    else
      rot.c = 5;
    getModel (missionnew->selweapon [i])->draw (&vec, &tl, &rot, 0.04, 0.1, 0);
  }
  glDisable (GL_DEPTH_TEST);
  glDisable (GL_LIGHTING);


  float textx = -12, texty = 9.8;
  font1->zoom = 0.105;
  font2->zoom = 0.105;
  float fontscale = 1.05;
  font1->drawTextCentered (0, texty / fontscale, -2, missionnew->name, col);
  texty -= 1.1;
  font1->zoom = 0.05;
  font2->zoom = 0.05;
  fontscale = 0.5;
  font1->drawText (textx / fontscale, texty / fontscale, -2, "BRIEFING:", col);
  texty -= 1;
  font1->drawText (textx / fontscale, texty / fontscale, -2, missionnew->briefing, col);

  float xstats = 0.8, ystats = 5;
  float xstatstab = 5;
  font1->drawText (xstats / fontscale, ystats / fontscale, -2, "STATUS:", col);
  if (p->mission_state [missionnew->id] == 1)
    font1->drawText (xstatstab / fontscale, ystats / fontscale, -2, "SUCCESS", colorstd);
  else if (p->mission_state [missionnew->id] == 2)
    font1->drawText (xstatstab / fontscale, ystats / fontscale, -2, "FAILED", colorstd);
  else
    font1->drawText (xstatstab / fontscale, ystats / fontscale, -2, "EMPTY", colorstd);
  if (missionnew->id >= MISSION_CAMPAIGN1 && missionnew->id <= MISSION_CAMPAIGN2)
  {
    texty = ystats - 0.7;
    font1->drawText (xstats / fontscale, texty / fontscale, -2, "SCORE:", col);
    int score = p->mission_score [missionnew->id];
    if (score < -10000 || score > 100000) score = 0;
    sprintf (buf, "%d", score);
    font1->drawText (xstatstab / fontscale, texty / fontscale, -2, buf, colorstd);
    texty -= 0.7;
    font1->drawText (xstats / fontscale, texty / fontscale, -2, "KILLS:", col);
    sprintf (buf, "%d AIRCRAFTS", p->mission_fighterkills [missionnew->id]);
    font1->drawText (xstatstab / fontscale, texty / fontscale, -2, buf, colorstd);
//    drawMedal (xstatstab + 2, ystats - 3, -2, getMedal (p->mission_score [missionnew->id]), 2, mission->id);
  }
  else
  {
    texty = ystats - 0.7;
    font1->drawText (xstats / fontscale, texty / fontscale, -2, "SCORE:", col);
    font1->drawText (xstatstab / fontscale, texty / fontscale, -2, "TRAINING", col);
  }
  
  font1->drawText (textx / fontscale, piloty / fontscale, -2, "PILOTS:", col);
  strcpy (buf, pilots->pilot [pilots->aktpilot]->getShortRank ());
  strcat (buf, " ");
  strcat (buf, pilots->pilot [pilots->aktpilot]->name);
  font2->drawText ((textx + 1.5) / fontscale, (piloty - 0.8) / fontscale, -2, buf, col);
  drawRank (textx, piloty - 0.8, -2, pilots->pilot [pilots->aktpilot]->ranking, 0.5);
  for (i = 1; i < missionnew->alliedfighters; i ++)
    if (missionnew->alliedpilot [i - 1] >= 0 && missionnew->alliedpilot [i - 1] < 100)
    {
      drawRank (textx, piloty - 1 - 0.6 * i, -2, pilots->pilot [pilots->aktpilot]->tp [missionnew->alliedpilot [i - 1]]->ranking, 0.5);
      font2->drawText ((textx + 1.5) / fontscale, (piloty - 1 - 0.6 * i) / fontscale, -2, pilots->pilot [pilots->aktpilot]->tp [missionnew->alliedpilot [i - 1]]->getShortName (), col);
    }

  font1->drawText (textx / fontscale, -1 / fontscale, -2, "CHOOSE FIGHTER:", col);
  font1->drawText (xstats / fontscale, -1 / fontscale, -2, "CHOOSE WEAPON PACK:", col);
  font2->drawText (textx / fontscale, -6 / fontscale, -2, getModelName (missionnew->selfighter [missionnew->wantfighter]), col);
  font2->drawText (xstats / fontscale, -6 / fontscale, -2, getModelName (missionnew->selweapon [missionnew->wantweapon]), col);
  font1->zoom = 0.1;
  font2->zoom = 0.1;

  drawMouseCursor ();
}

void fame_mouse (int button, int state, int x, int y)
{
  missionmenuitemselected = -1;
}

void create_mouse (int button, int state, int x, int y)
{
  float rx = (float) x / width;
  float ry = (float) y / height;
  missionmenuitemselected = -1;

  if (ry >= 0.8 && ry <= 0.9)
  {
    if (rx >= 0.4 && rx <= 0.6)
    {
      missionmenuitemselected = 0;
      if (state == MOUSE_DOWN)
      {
        create_key (27, 0, 0);
      }
    }
  }
}

void create_display ()
{
  CColor *colorstd = &colorblue;

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  glPushMatrix ();

  drawQuads (colorstd);

#ifdef HAVE_SDL_NET
  float my = 0;
  int i;
  for (i = 0; i < server->num_clients; i ++)
  {
    font1->drawTextCentered (0, my, -2.5, server->clients [i].name);
    my -= 2;
  }
#endif

  font1->drawTextCentered (0, 9, -1.5, "CREATE GAME");

  if (missionmenuitemselected == 0)
    font1->drawTextScaled (-2, -12, -2, "BACK", colorstd, -missionmenutimer * 5);
  else
    font1->drawText (-2, -12, -2, "BACK");

  glPopMatrix ();

  adjustBrightness ();

  drawMouseCursor ();
}

void join_mouse (int button, int state, int x, int y)
{
  float rx = (float) x / width;
  float ry = (float) y / height;
  missionmenuitemselected = -1;

  if (ry >= 0.8 && ry <= 0.9)
  {
    if (rx >= 0.4 && rx <= 0.6)
    {
      missionmenuitemselected = 0;
      if (state == MOUSE_DOWN)
      {
        create_key (27, 0, 0);
      }
    }
  }
}

void join_display ()
{
  CColor *colorstd = &colorblue;

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  glPushMatrix ();

  drawQuads (colorstd);

  font1->drawTextCentered (0, 9, -1.5, "JOIN GAME");

  if (missionmenuitemselected == 0)
    font1->drawTextScaled (-2, -12, -2, "BACK", colorstd, -missionmenutimer * 5);
  else
    font1->drawText (-2, -12, -2, "BACK");

  glPopMatrix ();

  adjustBrightness ();

  drawMouseCursor ();
}

int aktfighter = 0;

void fighter_mouse (int button, int state, int x, int y)
{
}

void fighter_display ()
{
  allmenus.setVisible (false);
  fightermenu.setVisible (true);
  fightermenu.draw ();
  AIObj ffighter;

  char buf [256];
  int i;

  CVector3 vec;
  CVector3 tl;
  tl.y = -0.04;
  tl.z = -0.4;
  CRotation rot;
  rot.a = 300;
  rot.b = 0;
  rot.c = (5 + missionmenutimer * 4 / timestep) % 360;
  CModel *model = NULL;
  int id = 0;
  if (aktfighter == 0) { model = &model_fig; id = FIGHTER_FALCON; }
  else if (aktfighter == 1) { model = &model_fige; id = FIGHTER_CROW; }
  else if (aktfighter == 2) { model = &model_figb; id = FIGHTER_HAWK; }
  else if (aktfighter == 3) { model = &model_figi; id = FIGHTER_STORM; }
  else if (aktfighter == 4) { model = &model_figa; id = FIGHTER_SWALLOW; }
  else if (aktfighter == 5) { model = &model_figd; id = FIGHTER_BUZZARD; }
  else if (aktfighter == 6) { model = &model_figc; id = FIGHTER_HAWK2; }
  else if (aktfighter == 7) { model = &model_figg; id = FIGHTER_REDARROW; }
  else if (aktfighter == 8) { model = &model_figf; id = FIGHTER_PHOENIX; }
  else if (aktfighter == 9) { model = &model_figh; id = FIGHTER_BLACKBIRD; }

  glEnable (GL_DEPTH_TEST);
  glEnable (GL_LIGHTING);
  model->draw (&vec, &tl, &rot, 0.11, 0.5, 0);
  glDisable (GL_LIGHTING);
  glDisable (GL_DEPTH_TEST);

  ffighter.o = model;
  ffighter.newinit (id, 1, 0);

  float fontzoom = 0.7;
  float textx = -9.5;
  font1->zoom = 0.07;
  font1->drawText (textx / fontzoom, 9.7 / fontzoom, -2, getModelName (id), &menu_colwhite);
  float yf = 9.6 - 1.35;
  strcpy (buf, "TYPE: ");
  if (ffighter.id == FIGHTER_FALCON || ffighter.id == FIGHTER_CROW || ffighter.id == FIGHTER_BUZZARD || ffighter.id == FIGHTER_REDARROW || ffighter.id == FIGHTER_BLACKBIRD)
    strcat (buf, "FIGHTER");
  else if (ffighter.id == FIGHTER_HAWK || ffighter.id == FIGHTER_HAWK2)
    strcat (buf, "FIGHTER-BOMBER");
  else
    strcat (buf, "BOMBER");
  font1->drawText (textx / fontzoom, yf / fontzoom, -2, buf, &menu_colwhite);
  yf -= 1;
  strcpy (buf, "SPEED: ");
  int stars = (int) ((ffighter.maxthrust - 0.2) * 40);
  font1->drawText (textx / fontzoom, yf / fontzoom, -2, buf, &menu_colwhite);
  for (i = 0; i < stars; i ++)
    drawMedal (4 + i * 1.1, yf + 0.7, -2, 0, 1, MISSION_CAMPAIGN1);
  yf -= 1;
  strcpy (buf, "NIMBILITY: ");
  stars = (int) ((ffighter.manoeverability - 0.3) * 20 + 1);
  font1->drawText (textx / fontzoom, yf / fontzoom, -2, buf, &menu_colwhite);
  for (i = 0; i < stars; i ++)
    drawMedal (4 + i * 1.1, yf + 0.7, -2, 0, 1, MISSION_CAMPAIGN1);
  yf -= 1;
  strcpy (buf, "SHIELD: ");
  stars = (int) ((ffighter.maxshield - 30) / 30);
  font1->drawText (textx / fontzoom, yf / fontzoom, -2, buf, &menu_colwhite);
  for (i = 0; i < stars; i ++)
    drawMedal (4 + i * 1.1, yf + 0.7, -2, 0, 1, MISSION_CAMPAIGN1);
  yf -= 1;
  strcpy (buf, "FIREPOWER: ");
  stars = ffighter.statfirepower;
  font1->drawText (textx / fontzoom, yf / fontzoom, -2, buf, &menu_colwhite);
  for (i = 0; i < stars; i ++)
    drawMedal (4 + i * 1.1, yf + 0.7, -2, 0, 1, MISSION_CAMPAIGN1);
  font1->zoom = 0.1;

  drawMouseCursor ();
}

void fame_display ()
{
  allmenus.setVisible (false);
  famemenu.setVisible (true);
  famemenu.draw ();

  char buf [256];
  int i, i2;
  Pilot *p = pilots->pilot [pilots->aktpilot];

  float textx = -10;
  int sum = 0;
  for (i = MISSION_CAMPAIGN1; i < MISSION_CAMPAIGN2; i ++)
    sum += p->mission_fighterkills [i];
  p->tp [10]->fighterkills = sum;
  strcpy (p->tp [10]->name, p->name);
  p->tp [10]->ranking = p->ranking;
  int index [11];
  for (i = 0; i < 11; i ++)
    index [i] = i;
  for (i = 0; i < 11; i ++)
    for (i2 = 0; i2 < i; i2 ++)
    {
      if (p->tp [index [i]]->fighterkills < p->tp [index [i2]]->fighterkills)
      {
        int temp = index [i];
        index [i] = index [i2];
        index [i2] = temp;
      }
    }

  for (i = 0; i < 11; i ++)
  {
    font1->zoom = 0.07;
    drawRank (textx + 0.2, i - 3.7, -2, p->tp [index [i]]->ranking, 0.7);
    sprintf (buf, "%s %s", p->tp [index [i]]->getRank (), p->tp [index [i]]->name);
    font1->drawText ((textx + 2) / 0.7, (i - 3.7) / 0.7, -2, buf, &menu_colwhite);
    sprintf (buf, "%d", p->tp [index [i]]->fighterkills);
    font1->drawText ((textx + 18) / 0.7, (i - 3.7) / 0.7, -2, buf, &menu_colwhite);
    font1->zoom = 0.1;
  }

  drawMouseCursor ();
}

CModel *explsphere;
CSphere *mysphere;

void game_quit ()
{
  int i;
  volumesound = sound->volumesound;
  volumemusic = sound->volumemusic;
  save_config ();
  save_configInterface ();
  pilots->save (dirs->getSaves ("pilots"));
  display ("Pilots saved", LOG_MOST);
  for (i = 0; i < maxlaser; i ++)
    delete (laser [i]);
  for (i = 0; i < maxmissile; i ++)
    delete (missile [i]);
  for (i = 0; i < maxflare; i ++)
    delete (flare [i]);
  for (i = 0; i < maxchaff; i ++)
    delete (chaff [i]);
  for (i = 0; i < maxexplosion; i ++)
    delete (explosion [i]);
  for (i = 0; i < maxstar; i ++)
    delete (star [i]);
  for (i = 0; i < maxgroundobj; i ++)
    delete (groundobj [i]);
  for (i = 0; i < maxblacksmoke; i ++)
    delete (blacksmoke [i]);
  delete pilots;
  delete tlinf;
  delete tlminf;
  delete tlnull;
  delete explsphere;
  delete objsphere;
  delete sphere;
  delete flash1;
  delete cockpit;
  delete font1;
  delete font2;
  delete space;
  delete dirs;
  delete gl;
#ifndef USE_GLUT
//  SDL_CloseAudio();
//  SDL_FreeWAV(wave.sound);
//  SDL_Quit (); // done atexit()
  delete sound;
#endif
  exit (EXIT_NORMAL);
}

void quit_key (unsigned char key, int x, int y)
{
  if (key == 'y' || key == 'Y')
    game_quit ();
  else
    switch_menu ();
}

void quit_mouse (int button, int state, int x, int y)
{
  float rx = (float) x / width;
  float ry = (float) y / height;
  missionmenuitemselected = -1;

  if (ry >= 0.5 && ry <= 0.55)
  {
    if (rx >= 0.35 && rx <= 0.45)
    {
      missionmenuitemselected = 0;
      if (state == MOUSE_DOWN)
      {
        quit_key ('y', 0, 0);
      }
    }
    if (rx >= 0.55 && rx <= 0.65)
    {
      missionmenuitemselected = 1;
      if (state == MOUSE_DOWN)
      {
        quit_key (27, 0, 0);
      }
    }
  }
}

void menu_mouse (int button, int state, int x, int y)
{
}

void stats_mouse (int button, int state, int x, int y)
{
}



/****************************************************************************
  DISPLAY EVENTS
****************************************************************************/

void drawMissionElement (float x, float y, float z, int thismissionid, int missionid, int selected, char *string)
{
  Pilot *p = pilots->pilot [pilots->aktpilot];
  drawMedal (x - 0.8, y + 0.6, z, getMedal (p->mission_score [thismissionid]), 1.0, thismissionid);
}

void stats_display ()
{
  allmenus.setVisible (false);
  statsmenu.setVisible (true);
  statsmenu.draw ();

  float yf = 1.4, zf = -2.5;

  drawMedal (-5.5, 5.8, -1.5, getMedal (fplayer->score), 1.6, mission->id);
  drawMedal (5.5, 5.8, -1.5, getMedal (fplayer->score), 1.6, mission->id);

  char buf [100];
  float xf1 = -12, xf2 = 0, xf3 = 8;
  float linedist = 1.3, linedist2 = 1.1;
  yf = 6.5; zf = -2.5;
  CColor *color;
  Pilot *p = pilots->pilot [pilots->aktpilot];

  color = &menu_colwhite;
  if (missionstate == 1)
  {
    font1->drawTextCentered (0, 7, -2, "SUCCESS", &colorblue);
  }
  else
  {
    font1->drawTextCentered (0, 7, -2, "FAILED", &colorred);
  }

  font1->drawText (xf1, yf, zf, "SCORE:", color);
  sprintf (buf, "%d", fplayer->score);
  font1->drawText (xf3, yf, zf, buf, color);
  yf -= linedist * 1.2;

  font1->drawText (xf1, yf, zf, "DIFFICULTY:", color);
  if (mission->difficulty == 0) font1->drawText (xf2, yf, zf, "EASY", color);
  else if (mission->difficulty == 1) font1->drawText (xf2, yf, zf, "NORMAL", color);
  else if (mission->difficulty == 2) font1->drawText (xf2, yf, zf, "HARD", color);
  if (mission->difficulty == 0) font1->drawText (xf3, yf, zf, "-25", color);
  else if (mission->difficulty == 1) font1->drawText (xf3, yf, zf, "+25", color);
  else if (mission->difficulty == 2) font1->drawText (xf3, yf, zf, "+75", color);
  yf -= linedist;

  font1->drawText (xf1, yf, zf, "FLIGHT MODEL:", color);
  if (!fplayer->realism) font1->drawText (xf2, yf, zf, "ACTION", color);
  else font1->drawText (xf2, yf, zf, "SIM", color);
  if (!fplayer->realism) font1->drawText (xf3, yf, zf, "-25", color);
  else font1->drawText (xf3, yf, zf, "+25", color);

  int timebonus = 0;
  if (mission->timer < mission->maxtime)
    timebonus = (mission->maxtime - mission->timer) * 100 / mission->maxtime;
  yf -= linedist;
  font1->drawText (xf1, yf, zf, "TIME BONUS:", color);
  sprintf (buf, "%d%%", timebonus);
  font1->drawText (xf2, yf, zf, buf, color);
  sprintf (buf, "+%d", timebonus);
  font1->drawText (xf3, yf, zf, buf, color);
  int shieldbonus = (int) (fplayer->shield * 100 / fplayer->maxshield);
  yf -= linedist;
  font1->drawText (xf1, yf, zf, "SHIELD BONUS:", color);
  sprintf (buf, "%d%%", shieldbonus);
  font1->drawText (xf2, yf, zf, buf, color);
  sprintf (buf, "+%d", shieldbonus);
  font1->drawText (xf3, yf, zf, buf, color);
  yf -= linedist;
  font1->drawText (xf1, yf, zf, "KILLS:", color);
  if (fplayer->fighterkills > 0)
  {
    sprintf (buf, "%d AIR", fplayer->fighterkills);
    font1->drawText (xf2, yf, zf, buf, color);
    sprintf (buf, "+%d", fplayer->fighterkills * 20);
    font1->drawText (xf3, yf, zf, buf, color);
    yf -= linedist2;
  }
  if (fplayer->tankkills > 0)
  {
    sprintf (buf, "%d TANKS", fplayer->tankkills);
    font1->drawText (xf2, yf, zf, buf, color);
    sprintf (buf, "+%d", fplayer->tankkills * 10);
    font1->drawText (xf3, yf, zf, buf, color);
    yf -= linedist2;
  }
  if (fplayer->shipkills > 0)
  {
    sprintf (buf, "%d SHIPS", fplayer->shipkills);
    font1->drawText (xf2, yf, zf, buf, color);
    sprintf (buf, "+%d", fplayer->shipkills * 12);
    font1->drawText (xf3, yf, zf, buf, color);
    yf -= linedist2;
  }
  if (fplayer->otherkills > 0)
  {
    sprintf (buf, "%d OTHERS", fplayer->otherkills);
    font1->drawText (xf2, yf, zf, buf, color);
    sprintf (buf, "+%d", fplayer->otherkills * 5);
    font1->drawText (xf3, yf, zf, buf, color);
    yf -= linedist2;
  }
  yf -= (linedist - linedist2);

  if (ispromoted)
  {
    yf = -6;
    font1->drawTextCentered (0, yf, zf, "PROMOTED TO", color);
    yf -= 1.5;
    sprintf (buf, "%s", p->getRank ());
    font1->drawTextCentered (0, yf, zf, buf, color);
    drawRank (-11, yf + 0.2, zf, pilots->pilot [pilots->aktpilot]->ranking, 2);
    drawRank (7, yf + 0.2, zf, pilots->pilot [pilots->aktpilot]->ranking, 2);
  }
  yf = -9;

  drawMouseCursor ();
}

void test ()
{
  view ++;
  if (view > VIEW_MAX) view = VIEW_MIN;
}

void menu_display ()
{
  int i;

  glInitNames ();
  glPushName (0);

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();

  int menutimernorm = menutimer * 5 / timestep;
  if (menutimernorm != 0) menutimernorm %= 360;
  if (menutimernorm < 0) menutimernorm *= -1;
  CColor color2 (255, 255, (int) (255.0 * cosi [menutimernorm]), 255);

  Pilot *p = pilots->pilot [pilots->aktpilot];

  if (allmenus.components [3]->isVisible ())
  {
    ((Container *) allmenus.components [3])->components [0]->setVisible (true);
    for (i = 1; i < 25; i ++)
    {
      if (p->mission_state [getCampaignIdFromValue (i - 1)] == 1)
        ((Container *) allmenus.components [3])->components [i]->setVisible (true);
      else
        ((Container *) allmenus.components [3])->components [i]->setVisible (false);
    }
  }

  allmenus.draw ();

  if (allmenus.components [1]->isVisible ())
  {
    float textx2 = -0.7, yf = 10.15, zf = -2, ydiff = 1;
    drawRank (textx2, yf, zf, pilots->pilot [pilots->aktpilot]->ranking, 0.8);
    yf -= 2;
    for (i = 0; i < pilots->aktpilots; i ++)
    {
      drawRank (textx2, yf, zf, pilots->pilot [i]->ranking, 0.8);
      yf -= ydiff;
    }
  }

  if (allmenus.components [3]->isVisible ())
  {
    float textx2 = 11, yf = 9.05, zf = -2, ydiff = 0.8;
    for (i = 0; i < 24; i ++)
    {
      drawMedal (textx2 - 0.8, yf + 0.6, zf, getMedal (p->mission_score [getCampaignIdFromValue (i)]), 0.8, getCampaignIdFromValue (i));
      yf -= ydiff;
    }
  }

  drawMouseCursor ();
}

int selectMouse (int x, int y, int motionx, int motiony, int mode, bool shift)
{
//  glutSwapBuffers ();
	GLuint selectBuff [20 * 4];
	GLint hits, viewport [4];

	// Select buffer parameters
	glSelectBuffer (20 * 4, selectBuff);
	glGetIntegerv (GL_VIEWPORT, viewport);
	
	// Enter to selection mode
	glEnable (GL_DEPTH_TEST);
	glDisable (GL_LIGHTING);
	glMatrixMode (GL_PROJECTION);
	glPushMatrix ();
	glLoadIdentity ();

	// Set-up pick matrix
  gluPickMatrix (x, viewport [3] - y, 5, 5, viewport);

	// Set our perpective transformation matrix
  gluPerspective (visibleangle, 1.0, nearclippingplane * GLOBALSCALE, view * GLOBALSCALE);
//  gluPerspective (60.0, 1.0, 0.2, 200.0);
	
	glMatrixMode (GL_MODELVIEW);

	// Render all scene and fill selection buffer
	glRenderMode (GL_SELECT);
  menu_display ();

	// Get hits and go back to normal rendering
	hits = glRenderMode (GL_RENDER);

	// Get first model in selection buffer stack
  GLuint *ptr = selectBuff;
  int mypicks = 0;
  int pickz2 = -1;
  int pick [100];
	if (hits > 0)
  {
    int i;
    GLuint z = 0xFFFFFFFF;
    for (i = 0; i < hits; i ++)
    {
      int names = *ptr;
      ptr ++;
      GLuint z1 = *ptr;
      ptr ++;
      ptr ++;
      if (motionx < 0 && motiony < 0)
      {
        if (z1 < z)
        {
          z = z1; pickz2 = *ptr; mypicks = 1;
        }
      }
      else
      {
        pick [mypicks] = *ptr; mypicks ++;
      }
      ptr += names;
      if (mypicks >= 20) break;
    }
  }

	glMatrixMode (GL_PROJECTION);

	glPopMatrix ();
	glMatrixMode (GL_MODELVIEW);

  char buf [STDSIZE];
  sprintf (buf, "selectMouse: picks=%d, pickz=%d, shift=%d", mypicks, pickz2, shift);
  display (buf, LOG_ALL);
  return pickz2;
}

void pause_display ()
{
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  glPushMatrix ();

  font1->drawText (-3, -1, -1, "PAUSED", &colorwhite);

  glPopMatrix ();
}

void credits_mouse (int button, int state, int x, int y)
{
  switch_menu ();
}

void credits_display ()
{
#ifdef SHOW_SOFTWARE_PATENTS
  float xt = -22, yt = 12, zf = -2.4, ydist = 0.7;
  glPushMatrix ();
//  glTranslatef (0, -3.5 + 0.014 * (float) creditstimer / timestep, 0);
  glTranslatef (0, -3.4 + 0.004 * (float) creditstimer / timestep, 0);
  CColor *col = &colorwhite;
  CColor *col2 = &coloryellow;
  float fontzoom = 1.0;
  font1->zoom = 0.1;
  font2->zoom = 0.1;
  font1->drawTextCentered (0, (yt -= 2 * ydist) / fontzoom, zf, "MONEY VERSUS DEMOCRACY", col2);
  font1->drawTextCentered (0, (yt -= 3 * ydist) / fontzoom, zf, "PATENTS ON SOFTWARE", col2);
  fontzoom = 0.8;
  font1->zoom = 0.08;
  font2->zoom = 0.08;

  font1->drawText (xt, (yt -= 5 * ydist) / fontzoom, zf, "WHAT ARE PATENTS?", col2);
  font2->drawText (xt, (yt -= 2 * ydist) / fontzoom, zf, "PATENTS IN EUROPE COST ABOUT 30000-50000 EURO (= US$).", col);
  font2->drawText (xt, (yt -= 1.5 * ydist) / fontzoom, zf, "THEY GRANT PROPERTY FOR SOMETHING, THAT NOONE CAN PUBLISH", col);
  font2->drawText (xt, (yt -= 1.5 * ydist) / fontzoom, zf, "EXCEPT THE PATENT OWNER.", col);

  font1->drawText (xt, (yt -= 4 * ydist) / fontzoom, zf, "WHO CAN AFFORD PATENTS?", col2);
  font2->drawText (xt, (yt -= 2 * ydist) / fontzoom, zf, "ONLY HUGE COMPANIES.", col);

  font1->drawText (xt, (yt -= 4 * ydist) / fontzoom, zf, "WHAT CAN BE PATENTED?", col2);
  font2->drawText (xt, (yt -= 2 * ydist) / fontzoom, zf, "CURRENTLY EVERYTHING. AMAZON HAS A PATENT TO SELL PRODUCTS", col);
  font2->drawText (xt, (yt -= 1.5 * ydist) / fontzoom, zf, "WITH ONE MOUSE CLICK. THERE ARE EUROPEAN PATENTS ON THE", col);
  font2->drawText (xt, (yt -= 1.5 * ydist) / fontzoom, zf, "PROGRESS BAR, ON THE UNDO FUNCTION, ETC.", col);
  font2->drawText (xt, (yt -= 1.5 * ydist) / fontzoom, zf, "CURRENTLY EURO SOFTWARE PATENTS ARE STOPPED UNTIL THE END", col);
  font2->drawText (xt, (yt -= 1.5 * ydist) / fontzoom, zf, "OF SUMMER 2004.", col);

  font1->drawText (xt, (yt -= 4 * ydist) / fontzoom, zf, "AN EXAMPLE FOR PATENTS ON COURT?", col2);
  font2->drawText (xt, (yt -= 2 * ydist) / fontzoom, zf, "IN 2001 MICROSOFT HAD TO PAY 500000000 US$, BECAUSE ANOTHER", col);
  font2->drawText (xt, (yt -= 1.5 * ydist) / fontzoom, zf, "COMPANY HAD A PATENT ON BROWSER PLUGINS (US).", col);

  font1->drawText (xt, (yt -= 4 * ydist) / fontzoom, zf, "WHAT IS SOFTWARE?", col2);
  font2->drawText (xt, (yt -= 2 * ydist) / fontzoom, zf, "SOFTWARE IS BASED ON ABSTRACT DESCRIPTIONS, WHICH EVERYONE", col);
  font2->drawText (xt, (yt -= 1.5 * ydist) / fontzoom, zf, "CAN FORMULATE IN PROSE LANGUAGE. CODE IS JUST A MIRROR", col);
  font2->drawText (xt, (yt -= 1.5 * ydist) / fontzoom, zf, "IMAGE OF THE DESCRIPTION.", col);

  font1->drawText (xt, (yt -= 4 * ydist) / fontzoom, zf, "WHY DOES MICROSOFT WANT PATENTS?", col2);
  font2->drawText (xt, (yt -= 2 * ydist) / fontzoom, zf, "TO PATENT CONCEPTS, AND DICTATE WHICH SYSTEMS MAY USE THEM.", col);
  font2->drawText (xt, (yt -= 1.5 * ydist) / fontzoom, zf, "EVENTUALLY TO ELIMINATE LINUX AND OPEN SOURCE.", col);

  font1->drawText (xt, (yt -= 4 * ydist) / fontzoom, zf, "WHY CAN EVERYTHING BE PATENTED?", col2);
  font2->drawText (xt, (yt -= 2 * ydist) / fontzoom, zf, "HUGE COMPANIES ARE PUSHING FOR EXECUTIVE POWERS.", col);
  font2->drawText (xt, (yt -= 1.5 * ydist) / fontzoom, zf, "PATENT LAWYERS ARE TAKING THE MONEY.", col);

  font1->drawText (xt, (yt -= 4 * ydist) / fontzoom, zf, "WHAT IS DEMOCRACY?", col2);
  font2->drawText (xt, (yt -= 2 * ydist) / fontzoom, zf, "YOUR POWER TO STOP THIS ABUSE.", col);
  font2->drawText (xt, (yt -= 1.5 * ydist) / fontzoom, zf, "VOTE AGAINST SOFTWARE PATENTS ON HTTP://PETITION.EUROLINUX.ORG.", col);
  font2->drawText (xt, (yt -= 1.5 * ydist) / fontzoom, zf, "TELL YOUR FRIENDS AND SEARCH THE INTERNET FOR MORE INSANITY.", col);
#else
  float yt = 12, zf = -2.4, ydist = 0.7;
  glPushMatrix ();
  glTranslatef (0, -3.5 + 0.014 * (float) creditstimer / timestep, 0);
  CColor *col = &colorwhite;
  CColor *col2 = &coloryellow;
  float fontzoom = 1;
  font2->drawTextCentered (0, (yt -= 2 * ydist) / fontzoom, zf, "GAME PROGRAMMING,", col);
  font2->drawTextCentered (0, (yt -= 2 * ydist) / fontzoom, zf, "GRAPHICS, MODELS, SOUND & MUSIC", col);
  font1->drawTextCentered (0, (yt -= 2 * ydist) / fontzoom, zf, "THOMAS A. DREXL", col2);
  font1->zoom = 0.08;
  font2->zoom = 0.08;
  fontzoom = 0.8;
  font2->drawTextCentered (0, (yt -= 4 * ydist) / fontzoom, zf, "SPECIAL THANKS TO...", col);
  font2->drawTextCentered (0, (yt -= 4 * ydist) / fontzoom, zf, "LENS FLARES & FURTHER DEBUGGING", col);
  font1->drawTextCentered (0, (yt -= 2 * ydist) / fontzoom, zf, "PIOTR PAWLOW", col2);
  font2->drawTextCentered (0, (yt -= 4 * ydist) / fontzoom, zf, "MOUSE INTERFACE & LANDSCAPE IMPROVEMENTS", col);
  font1->drawTextCentered (0, (yt -= 2 * ydist) / fontzoom, zf, "LOURENS VEEN", col2);
  font2->drawTextCentered (0, (yt -= 4 * ydist) / fontzoom, zf, "PUBLISHING & FURTHER GAME IDEAS", col);
  font1->drawTextCentered (0, (yt -= 2 * ydist) / fontzoom, zf, "BERNHARD KAINDL", col2);
  font2->drawTextCentered (0, (yt -= 4 * ydist) / fontzoom, zf, "MOON TERRAIN", col);
  font1->drawTextCentered (0, (yt -= 2 * ydist) / fontzoom, zf, "NORBERT DREXL", col2);
  font2->drawTextCentered (0, (yt -= 4 * ydist) / fontzoom, zf, "PHYSICAL MODEL (ACTION) & COCKPIT IMPROVEMENTS", col);
  font1->drawTextCentered (0, (yt -= 2 * ydist) / fontzoom, zf, "ARNE REINERS", col2);
  font2->drawTextCentered (0, (yt -= 4 * ydist) / fontzoom, zf, "...AND THANKS TO ALL PEOPLE GIVING FEEDBACK AND ADVICE", col);
  float xf = -10;
  font1->zoom = 0.1;
  font2->zoom = 0.1;
  fontzoom = 1;
  font1->drawTextCentered (0, (yt -= 10 * ydist) / fontzoom, zf, "***********************", col);
  font1->drawTextCentered (0, (yt -= 10 * ydist) / fontzoom, zf, "DO YOU WANT TO CONTRIBUTE?", col);
  font1->zoom = 0.08;
  font2->zoom = 0.08;
  fontzoom = 0.8;
  font2->drawText (xf / fontzoom, (yt -= 4 * ydist) / fontzoom, zf, "LOTS OF THINGS ARE STILL NEEDED:", col);
  font2->drawText (xf / fontzoom, (yt -= 2 * ydist) / fontzoom, zf, "- DRAW TEXTURES OF TREES, BUILDINGS, ETC", col);
  font2->drawText (xf / fontzoom, (yt -= 2 * ydist) / fontzoom, zf, "- DESIGN 3D MODELS", col);
  font2->drawText (xf / fontzoom, (yt -= 2 * ydist) / fontzoom, zf, "- CREATE SOUNDTRACKS", col);
  font2->drawText (xf / fontzoom, (yt -= 2 * ydist) / fontzoom, zf, "- MAKE MISSION SUGGESTIONS", col);
  font2->drawText (xf / fontzoom, (yt -= 2 * ydist) / fontzoom, zf, "- GIVE FEEDBACK AND ADVICE", col);
  font1->zoom = 0.1;
  font2->zoom = 0.1;
  glPopMatrix ();
#endif
}

void finish_display ()
{
  glTranslatef (0, -3.5 + 0.01 * (float) finishtimer / timestep, 0);
  CColor *col = &colorwhite;
  font1->drawTextCentered (0, 12, -3, "CONGRATULATIONS!", col);
  font1->drawTextCentered (0, 10, -3, "THE WORLD HAS BEEN SAVED YET AGAIN.", col);
  font1->drawTextCentered (0, 6, -3, "HOPE YOU HAD FUN PLAYING GL-117!", col);
  font1->drawTextCentered (0, 4, -3, "THIS GAME HAS ORIGINALLY BEEN DEVELOPED AS PART", col);
  font1->drawTextCentered (0, 2, -3, "OF THE COURSE \"APPLICATIONS OF COMPUTER GRAPHICS\"", col);
  font1->drawTextCentered (0, 0, -3, "AT THE TECHNICAL UNIVERSITY OF MUNICH, GERMANY.", col);
  font1->drawTextCentered (0, -2, -3, "IN FEBRUARY 2002 THE WORK WAS DONE AND I", col);
  font1->drawTextCentered (0, -4, -3, "PRESENTED THE PROTOTYPE OF A FLIGHT SIM,", col);
  font1->drawTextCentered (0, -6, -3, "YET WITHOUT TEXTURES, JOYSTICK, SOUNDS, MUSIC,", col);
  font1->drawTextCentered (0, -8, -3, "NO CAMPAIGN!", col);
  font1->drawTextCentered (0, -10, -3, "TWO MONTHS LATER THE FIRST RELEASE OF GL-117 WAS READY.", col);
  font1->drawTextCentered (0, -12, -3, "FURTHER RELEASES FOLLOWED, FEEDBACK ARRIVED, AND", col);
  font1->drawTextCentered (0, -14, -3, "PROGRAMMERS JOINED (LISTED IN THE CREDITS SECTION).", col);
  font1->drawTextCentered (0, -16, -3, "SPECIAL THANKS TO JOSEF DREXL FOR THE MODELING IDEAS,", col);
  font1->drawTextCentered (0, -18, -3, "AND THE UNIX AWARD GOES TO WOLFGANG HOMMEL ;-)", col);
}

void quit_display ()
{
  allmenus.setVisible (false);
  quitmenu.setVisible (true);
  quitmenu.draw ();

  font1->drawTextCentered (0, 0, -2, "REALLY QUIT?", &menu_colwhite);

  drawMouseCursor ();
}



/****************************************************************************
  IN-GAME DISPLAY EVENT
****************************************************************************/

int vibration = 0;
int starttime;

void game_display ()
{
  int i;
  double sunx = 0, suny = 0, sunz;

  if (dithering) glEnable (GL_DITHER);
  else glDisable (GL_DITHER);

  bool sunvisible = false;
  float pseudoview = getView ();

  float mycamtheta = camtheta, mycamphi = camphi + view_x , mycamgamma = camgamma + view_y;

  if (vibration > 0)
  {
    if (camera == 0)
    {
      float sinvib = sin ((float) vibration / timestep);
      mycamphi += 0.2 * vibration * sinvib / timestep;
      mycamgamma += 0.2 * vibration * sinvib / timestep;
    }
  }


  // calculate light factor
  if (camera == 0 && sunblinding && day && weather == WEATHER_SUNNY)
  {
    float np = fplayer->phi - 180;
    if (np >= 180) np -= 360;
    float sunfactor = fabs (np) + fabs (fplayer->gamma - 180 - sungamma);
    if (sunfactor < 50)
    {
      sunfactor = (50 - sunfactor) / 10;
      if (sunfactor < 1.0) sunfactor = 1.0;
    }
    else sunfactor = 1.0;
    sunlight_dest = sunfactor;
    if (game == GAME_PLAY)
      pseudoview /= sunfactor;
  }
  else
  {
    if (day) sunlight_dest = 1.0;
    else sunlight_dest = 0.75;
    if (flash > 0)
    {
      sunlight_dest = (float) flash / timestep;
    }
  }

  // show a short flash when an object explodes
  if (dynamiclighting)
  {
    for (i = 0; i < maxfighter; i ++)
    {
      if (fighter [i]->draw)
        if (fighter [i]->explode > 0)
          if (fighter [i] != fplayer)
          {
            float dgamma = fabs (fplayer->getAngleH (fighter [i]));
            float dphi = fabs (fplayer->getAngle (fighter [i]));
            if (dphi < 45 && dgamma < 45)
            {
              float ddist = fplayer->distance (fighter [i]);
              if (ddist < 40)
              {
                ddist /= 15;
                ddist ++;
                dphi /= 25;
                dphi ++;
                dgamma /= 25;
                dgamma ++;
                if (fighter [i]->explode < 8 * timestep)
                  sunlight_dest = (float) fighter [i]->explode / timestep * 4 / ddist / dphi / dgamma;
                else if (fighter [i]->explode < 16 * timestep)
                  sunlight_dest = (16.0 - fighter [i]->explode / timestep) * 4 / ddist / dphi / dgamma;
              }
            }
          }
    }
  }

  // sunlight for glittering does not look good
/* if (camera == 0 && l->glittering > 1.0F)
  {
    sunlight += l->glittering * 1.0F - 1.0F;
  }*/
  l->glittering = 0;

  // turn down global sunlight when menu is showing
  if (game != GAME_PLAY && sunlight > 0.9F) sunlight = 0.9F;

  // start rendering

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();

  glScalef (GLOBALSCALE, GLOBALSCALE, GLOBALSCALE);

  glShadeModel (GL_SMOOTH);

  glPushMatrix ();
  
  if (camera != 1 && camera != 5)
    glRotatef (-mycamtheta, 0.0, 0.0, 1.0);
  glRotatef (mycamgamma, 1.0, 0.0, 0.0);
  glRotatef (-mycamphi, 0.0, 1.0, 0.0);

  // draw sky

  glDisable (GL_FOG);
  glDisable (GL_DEPTH_TEST);

  float mylight = sunlight;
  if (!day) mylight /= 0.75;
  if (mylight > 1.0 && day)
    mylight = mylight / 5.0 + 0.8;
  else if (mylight > 1.0 && !day)
    mylight = mylight / 5.0 + 0.8;
  gl->foglum = mylight;
  sphere->drawGL (tlminf, tlinf, tlnull, space->alpha, mylight, true, false);

  if (weather == WEATHER_SUNNY || weather == WEATHER_CLOUDY)
  {
    if (!day)
    {
      glPointSize (LINEWIDTH(1.0F));
      int stars = maxstar;
      if (weather != WEATHER_CLOUDY) stars = maxstar / 2;
      for (i = 0; i < stars; i ++)
      {
        glPushMatrix ();
        glRotatef (star [i]->phi, 0.0, 1.0, 0.0);
        glRotatef (star [i]->gamma, 1.0, 0.0, 0.0);
        glTranslatef (0, 0, -10);
        star [i]->draw ();
        glPopMatrix ();
      }
    }
  }

  glEnable (GL_DEPTH_TEST);
  glEnable (GL_FOG);

  if (quality >= 1 && clouds > 0)
  {
    float cloudfog = pseudoview;
    if (cloudfog > 110) cloudfog = 110;
    gl->enableFog (cloudfog);

    highclouds->zoom = 400;
    float ch2 = -382 - fplayer->tl->y / 10.0;
    CVector3 tlsphere2 (0, ch2, 0);
    highclouds->drawGL (&tlsphere2, fplayer->tl);

    glDisable (GL_FOG);
  }

// draw sun or moon (or earth)
  float fac = view, zfac = view * 0.2;
  if (weather == WEATHER_SUNNY || weather == WEATHER_CLOUDY)
  {
    glRotatef (180, 0.0, 1.0, 0.0);
    if (camera == 0)
      glRotatef (sungamma, 1.0, 0.0, 0.0);
    else
      glRotatef (mycamgamma + sungamma, 1.0, 0.0, 0.0);
    float zf = -11;
    if (day)
      zf = -10;
    if (l->type == LAND_MOON && !day)
      zf = -8; // diplay bigger earth
    glTranslatef (0, 0, zf);
    gl->extractFrustum ();
    if (gl->isPointInFrustum (-1, 1, 0) || gl->isPointInFrustum (-1, -1, 0) ||
        gl->isPointInFrustum (1, -1, 0) || gl->isPointInFrustum (1, 1, 0))
    {
      glDisable (GL_DEPTH_TEST);
      if (day) gl->enableTextures (texsun->textureID);
      else if (l->type != LAND_MOON) gl->enableTextures (texmoon->textureID);
      else gl->enableTextures (texearth->textureID);
      if (day && l->type != 1)
        glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      gl->enableAlphaBlending ();
      glEnable (GL_ALPHA_TEST);
      float alphamax = 0.1;
      if (day) alphamax = 0.1;
      glAlphaFunc (GL_GEQUAL, alphamax);
      glBegin (GL_QUADS);
      fac = view; zfac = view * 3.5;
      if (day && l->type != LAND_MOON)
      {
        float gm = (40.0 - sungamma) / 80.0;
        if (gm < 0) gm = 0;
        if (gm > 0.5) gm = 0.5;
        glColor4f (1.0, 1.0 - gm, 0.8 - gm, 1.0);
      }
      else
        glColor4f (1.0, 1.0, 1.0, 1.0);
      glTexCoord2d (0, 1);
      glVertex3f (-1, 1, 0);
      glTexCoord2d (1, 1);
      glVertex3f (1, 1, 0);
      glTexCoord2d (1, 0);
      glVertex3f (1, -1, 0);
      glTexCoord2d (0, 0);
      glVertex3f (-1, -1, 0);
      glEnd ();

      // Where is the sun?
      double proj[16];
      double modl[16];
      int vp[4];
      glGetDoublev( GL_PROJECTION_MATRIX, proj );
      glGetDoublev( GL_MODELVIEW_MATRIX, modl );
      glGetIntegerv( GL_VIEWPORT, vp );
      gluProject (0, 0, 0, modl, proj, vp, &sunx, &suny, &sunz);
      if ((sunx>=vp[0])&&(suny>=vp[1])&&(sunx<(vp[0]+vp[2]))&&(suny<(vp[1]+vp[3]))) sunvisible = true;

      glDisable (GL_ALPHA_TEST);
      glEnable (GL_DEPTH_TEST);
      glDisable (GL_TEXTURE_2D);
      gl->disableAlphaBlending ();
    }
  }

  glPopMatrix ();
    
  glPushMatrix ();

  if (camera != 1 && camera != 5)
    glRotatef (-mycamtheta, 0.0, 0.0, 1.0);

  glRotatef (mycamgamma, 1.0, 0.0, 0.0);

  glDisable (GL_FOG);

  glRotatef (-mycamphi, 0.0, 1.0, 0.0);
  glScalef (1, 1, 1);

  glTranslatef (-camx, -camy, -camz);

  if (camera != 50)
  {
    gl->enableFog (pseudoview);
  }

  // draw terrain
  l->calcDynamicLight (explosion, laser, (DynamicObj **) missile, flare);
  glEnable (GL_CULL_FACE);
  glCullFace (GL_FRONT);
  l->draw ((int) mycamphi, (int) (-mycamgamma + 180.0));
  glDisable (GL_CULL_FACE);

  // draw objects
  gl->extractFrustum ();
  if (camera != 50)
  {
    space->lum = sunlight;
    float dayfac = 1.0;
    if (!day) dayfac = 0.5;
    if (weather == WEATHER_SUNNY || weather == WEATHER_CLOUDY)
    {
      for (i = 0; i < space->no; i ++)
      {
        if (space->o [i]->tl->y < l->getExactRayHeight (space->o [i]->tl->x, space->o [i]->tl->z))
          space->o [i]->lum = 0.5 * dayfac;
        else
          space->o [i]->lum = 1.0 * dayfac;
      }
    }
    else
    {
      for (i = 0; i < space->no; i ++)
        space->o [i]->lum = dayfac;
    }
//    printf ("%2.1f*%2.1f ", fplayer->lum, sunlight);
    if (flash > 7 * timestep)
    {
      if (antialiasing)
        flash1->draw ();
      else
        flash1->drawHQ ();
    }
    if (lighting)
    {
      glEnable( GL_LIGHTING);
      glEnable( GL_LIGHT0 );
      glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      space->drawGL (); // draw all objects
      glDisable (GL_LIGHTING);
      glDepthMask (GL_FALSE);
      for (i = 0; i < space->no; i ++)
      {
        AIObj *dobj = (AIObj *) space->o [i];
        if (dobj->id >= MISSILE1)
          if (dobj->draw && dobj->drawlight && dobj->active)
          {
            if (dobj->smoke)
              if ((dobj->id >= MISSILE1 && dobj->id <= MISSILE2) || (dobj->id >= FIGHTER1 && dobj->id <= FIGHTER2))
                if (!(dobj->ttl == 0 && dobj->id >= MISSILE1 && dobj->id <= MISSILE2))
                {
                  dobj->smoke->draw ();
                }
          }
      }
      glDepthMask (GL_TRUE);
    }
    else
    {
      space->drawGL ();
    }
  }
  glDisable (GL_TEXTURE_2D);

  glDisable (GL_LIGHTING);

  if (camera == 0)
  {
    glDisable (GL_DEPTH_TEST);
    if (antialiasing)
      glEnable (GL_LINE_SMOOTH);
    cockpit->drawTargeter ();
    if (antialiasing)
      glDisable (GL_LINE_SMOOTH);
    glEnable (GL_DEPTH_TEST);
  }

  glPopMatrix ();

// draw flares
  if (specialeffects)
    if (sunvisible && /*camera == 0 &&*/ sunblinding && day)
    {
      CTexture* fl_texture[]= {texflare1,texflare3,texflare2,texflare4,texflare2,texflare4,texflare3,0};
      double fl_position[]=   {0.2,      1.6,      3.2,      8.1,      -1.4,     -2.2,     -3.5,     0};
      double fl_size[]=       {0.8,      0.6,      0.5,      0.4,      0.8,      0.4,      0.5,      0};
      double proj[16];
      double modl[16];
      double dr[2];
      int vp[4];
      double objx,objy,objz;

      glGetDoublev( GL_PROJECTION_MATRIX, proj );
      glGetDoublev( GL_MODELVIEW_MATRIX, modl );
      glGetIntegerv( GL_VIEWPORT, vp );
      glGetDoublev( GL_DEPTH_RANGE, dr );

      double cx=vp[2]/2+vp[0];
      double cy=vp[3]/2+vp[1];

      glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
      int i=0;
      double flarez_eye=4;
      double flarez_win=(-proj[10]+proj[14]/flarez_eye)*(dr[1]-dr[0])/2+(dr[1]+dr[0])/2;
      while (CTexture *tex=fl_texture[i])
      {
        double position=fl_position[i];
        double flarex,flarey,size=fl_size[i]/GLOBALSCALE;

        i++;

        flarex=(cx-sunx)*position+sunx;
        flarey=(cy-suny)*position+suny;

        if (gluUnProject (flarex, flarey, flarez_win, modl, proj, vp, &objx, &objy, &objz)==GL_TRUE)
        {
          gl->enableTextures (tex->textureID);
          gl->enableAlphaBlending ();
          glDisable (GL_ALPHA_TEST);
          glDisable (GL_DEPTH_TEST);
          glDisable (GL_FOG);

          glBegin (GL_QUADS);
          glColor4f (1.0, 1.0, 1.0, 1.0);
          glTexCoord2d (0, 1);
          glVertex3f (-size+objx, size+objy, objz);
          glTexCoord2d (1, 1);
          glVertex3f (size+objx, size+objy, objz);
          glTexCoord2d (1, 0);
          glVertex3f (size+objx, -size+objy, objz);
          glTexCoord2d (0, 0);
          glVertex3f (-size+objx, -size+objy, objz);
          glEnd ();
        }
      }
      glEnable (GL_DEPTH_TEST);
      glDisable (GL_TEXTURE_2D);
      gl->disableAlphaBlending ();
      glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }

// sunblinding test

  if (sunvisible)
  {
    GLfloat zbuf[1];
    glReadPixels((int)sunx,(int)suny,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,zbuf);
    sunblinding=(zbuf[0]<1)?false:true;
  }
  else
  {
    sunblinding=false;
  }

  // draw cockpit
  if (camera == 0)
  {
    if (antialiasing)
      glEnable (GL_LINE_SMOOTH);
    glPushMatrix ();
    glRotatef (view_y, 1, 0, 0);
    glRotatef (-view_x, 0, 1, 0);
    cockpit->drawCross ();
    cockpit->drawHeading ();
    glPopMatrix ();
    if (antialiasing)
      glDisable (GL_LINE_SMOOTH);
    cockpit->drawRadar ();
    cockpit->drawTargetedElement ();
    cockpit->drawWeapon ();
    cockpit->drawCounter ();
    cockpit->drawThrustBar ();
    cockpit->drawRelativeHeightBar ();
   }

  // draw blackout/redout (blending)
  if (camera == 0)
  {
    int black = (int) blackout;
    if (black > 255) black = 255;
    int red = (int) redout;
    if (red > 255) red = 255;
    float xf = 2.0, yf = 1.5, zf = 1.0;
    if (black > 0)
    {
      glColor4ub (0, 0, 0, black);
    }
    else if (red > 0)
    {
      glColor4ub (255, 0, 0, red);
    }
    if (black > 0 || red > 0)
    {
      glDisable (GL_DEPTH_TEST);
      glEnable (GL_BLEND);
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glBegin (GL_QUADS);
      glVertex3f (-xf, -yf, -zf);
      glVertex3f (-xf, yf, -zf);
      glVertex3f (xf, yf, -zf);
      glVertex3f (xf, -yf, -zf);
      glEnd ();
      gl->disableAlphaBlending ();
      glDisable (GL_BLEND);
      glEnable (GL_DEPTH_TEST);
    }
  }

  if (brightness)
  {
    adjustBrightness ();
    glEnable (GL_DEPTH_TEST);
  }

  // draw mission dependant informations
  mission->draw ();

  glPushMatrix ();
  glDisable (GL_DEPTH_TEST);

  glPopMatrix ();
  char buf [25];
  sprintf (buf, "FPS: %d", (int) fps);
  font1->drawText (-25, 25, -3.5, buf, &colorwhite);

  bool write = false;
  if (firststart)
  {
    int akttime;
#ifndef USE_GLUT
    akttime = SDL_GetTicks ();
#else
    akttime = glutGet (GLUT_ELAPSED_TIME);
#endif
    if (akttime - starttime < 20000)
      if ((akttime - starttime) / 300 % 3)
      {
        font1->drawTextCentered (0, 0, -1.8, "PLEASE WAIT WHILE", &colorred);
        font1->drawTextCentered (0, -1, -1.8, "ADJUSTING QUALITY", &colorred);
        write = true;
      }
  }
  if (fps >= 5 && fps <= 20 && !write)
  {
    font1->drawTextCentered (0, -8, -2, "FPS TOO LOW", &colorred);
    font1->drawTextCentered (0, -9, -2, "TURN DOWN VIEW OR QUALITY", &colorred);
  }

  if (fps < 5 && !write)
  {
    font1->drawTextCentered (0, -8, -2, "FPS FAR TOO LOW", &colorred);
    font1->drawTextCentered (0, -9, -2, "SEE \"README\" OR \"FAQ\" FILES", &colorred);
    font1->drawTextCentered (0, -10, -2, "HIT \"ESC\" AND 'Y' TO EXIT THE GAME", &colorred);
  }

  if (controls == CONTROLS_MOUSE && !mouse_relative)
    drawMouseCursor ();
}



/****************************************************************************
  TIMER EVENTS
****************************************************************************/

float lastthrust;
int gametimer;

void game_timer (int dt)
{
//  if (multiplayer) return;
  int i, i2;

  sunlight += (sunlight_dest - sunlight) / 10 * dt / timestep;

  gametimer += dt;
  cockpit->dt = dt;
  l->lsticker += dt;
  if (l->lsticker >= 36000000)
    l->lsticker = 0;

  if (vibration > 0)
  {
    vibration -= dt;
  }

  if (fplayer->autofire && fplayer->active)
  {
    if (fplayer->ammo != 0)
    {
      fplayer->fireCannon (laser);
#ifdef HAVE_SDL_MIXER
      if (!startcannon)
      {
        sound->playLoop (SOUND_CANNON1);
        startcannon = true;
      }
#else
      sound->play (SOUND_CANNON1, 1);
#endif
    }
  }
#ifdef HAVE_SDL_MIXER
  else
  {
    if (startcannon)
    {
      startcannon = false;
      sound->stop (SOUND_CANNON1);
    }
  }
#endif

  if (lastshield > fplayer->shield && !fplayer->ai)
  {
    sound->play (SOUND_HIT1, false);
    lastshield = (int) fplayer->shield;
    vibration = 25 * timestep;
  }

  // create flash during thunderstorm
  if (weather == WEATHER_THUNDERSTORM && flash <= 0 && !myrandom (2000 / dt))
  {
    flash = 18 * timestep;
    int fphi = (int) camphi + myrandom (50) - 25;
    if (fphi < 0) fphi += 360;
    else if (fphi >= 360) fphi -= 360;
    float pseudoview = getView ();
    float fdist = myrandom ((int) pseudoview - 20) + 10;
    float fx = fplayer->tl->x - sine [fphi] * fdist;
    float fz = fplayer->tl->z - cosi [fphi] * fdist;
    flash1->set (fx, l->getHeight (fx, fz), fz, (int) camphi);
    int lev = (int) (128.0 - 80.0 * fdist / (pseudoview - 10));
    sound->setVolume (SOUND_THUNDER1, lev);
    sound->play (SOUND_THUNDER1, false);
  }

  if (initing) return;

  if (flash > 0)
    flash -= dt;
  if (flash <= 7 * timestep && flash > 0)
    flash -= dt;

  if (!fplayer->ai)
    if (sound->engine != (int) ((fplayer->thrust / fplayer->maxthrust * 20) - 10))
    {
      setPlaneVolume ();
      lastthrust = fplayer->thrust;
    }

  // collision tests
  for (i = 0; i < maxfighter; i ++)
  {
    for (i2 = 0; i2 < maxlaser; i2 ++)
      if (laser [i2]->active)
        fighter [i]->collide (laser [i2], dt);
    for (i2 = 0; i2 < maxmissile; i2 ++)
      if (missile [i2]->active)
        fighter [i]->collide (missile [i2], dt);
    for (i2 = 0; i2 < i; i2 ++)
      if (fighter [i2]->active)
        if (i != i2)
          fighter [i]->collide (fighter [i2], dt);
  }

  for (i = 0; i < maxflare; i ++)
  {
    for (i2 = 0; i2 < maxmissile; i2 ++)
      if (missile [i2]->active)
        flare [i]->collide (missile [i2], dt);
  }

  for (i = 0; i < maxchaff; i ++)
  {
    for (i2 = 0; i2 < maxmissile; i2 ++)
      if (missile [i2]->active)
        chaff [i]->collide (missile [i2], dt);
  }

  // move objects
  for (i = 0; i < maxfighter; i ++)
  {
    fighter [i]->aiAction (dt, (AIObj **) fighter, missile, laser, flare, chaff);
    float lev;
    if (fighter [i]->explode == 1 && (lev = fplayer->distance (fighter [i])) < 32)
    {
      sound->setVolume (SOUND_EXPLOSION1, 128);
      float angle = fplayer->getAngle (fighter [i]);
      angle = 180 - angle;
      sound->setPosition (SOUND_EXPLOSION1, (int) angle, (int) (lev * 4.0));
      sound->play (SOUND_EXPLOSION1, false);
    }
  }
  for (i = 0; i < maxlaser; i ++)
  {
    laser [i]->move (dt);
  }
  for (i = 0; i < maxmissile; i ++)
  {
    missile [i]->aiAction (dt, (AIObj **) fighter, missile, laser, flare, chaff);
  }
  for (i = 0; i < maxflare; i ++)
  {
    flare [i]->move (dt);
  }
  for (i = 0; i < maxchaff; i ++)
  {
    chaff [i]->move (dt);
  }

  for (i = 0; i < maxexplosion; i ++)
    explosion [i]->move (dt);
  for (i = 0; i < maxblacksmoke; i ++)
    blacksmoke [i]->move (dt);

  // show blackout/redout
  if (blackout > 0) blackout -= 3.0F * dt / timestep;
  if (blackout < 0) blackout = 0;
  if (redout > 0) redout -= 3.0F * dt / timestep;
  if (redout < 0) redout = 0;
  if (blackout > 400) blackout = 400;
  if (redout > 400) redout = 400;
  float testout;
  if (redout < 1)
  {
    testout = (fplayer->realspeed * fplayer->elevatoreffect - 0.13F) * 40.0F;
    if (testout > 0) blackout += testout * dt / timestep;
  }
  if (blackout < 1)
  {
    testout = (fplayer->realspeed * fplayer->elevatoreffect + 0.065F) * 80.0F;
    if (testout < 0) redout -= testout * dt / timestep;
  }

  // show own explosion from chase cam
  if (fplayer->shield <= 0)
    camera = 1;

  float cf = -fplayer->zoom / 2;
  camtheta = fplayer->theta;
  if (camera == 0)  // cockpit
  {
    float cgamma = fplayer->gamma + 25.0F * COS(fplayer->theta);
    float cphi = fplayer->phi + 25.0F * SIN(fplayer->theta);
    float fac = fplayer->zoom / 2;
    camx = fplayer->tl->x + COS(cgamma) * SIN(cphi) * fac;
    camy = fplayer->tl->y - SIN(cgamma) * fac;
    camz = fplayer->tl->z + COS(cgamma) * COS(cphi) * fac;
    camphi = fplayer->phi;
    camgamma = -fplayer->gamma + 180;
    fplayer->draw = 0;
  }
  if (camera == 1) // chase
  {
    cf = fplayer->zoom * 3;
    camx = fplayer->tl->x + cf * SIN(fplayer->phi);
    camy = fplayer->tl->y + fplayer->zoom;
    camz = fplayer->tl->z + cf * COS(fplayer->phi);
    camphi = fplayer->phi;
    fplayer->draw = 1;
    camgamma = 20;
  }
  else if (camera == 2) // backwards
  {
    cf = -fplayer->zoom * 3;
    camx = fplayer->tl->x + cf * SIN(fplayer->phi);
    camy = fplayer->tl->y + fplayer->zoom;
    camz = fplayer->tl->z + cf * COS(fplayer->phi);
    camphi = fplayer->phi + 180.0;
    fplayer->draw = 1;
    camgamma = 20;
  }
  else if (camera == 3) // other players
  {
    cf = fighter [aktcam]->zoom * 3;
    camx = fighter [aktcam]->tl->x + cf * SIN(fighter [aktcam]->phi);
    camy = fighter [aktcam]->tl->y + fighter [aktcam]->zoom;
    camz = fighter [aktcam]->tl->z + cf * COS(fighter [aktcam]->phi);
    camphi = fighter [aktcam]->phi;
    camgamma = 20;
    camtheta = fighter [aktcam]->theta;
    fplayer->draw = 1;
  }
  else if (camera == 4) // missile
  {
    cf = missile [0]->zoom * 10;
    camx = missile [0]->tl->x + cf * SIN(missile [0]->phi);
    camy = missile [0]->tl->y + fplayer->zoom * 2;
    camz = missile [0]->tl->z + cf * COS(missile [0]->phi);
    camphi = missile [0]->phi;
    fplayer->draw = 1;
  }
  else if (camera == 5) // top
  {
    cf = fplayer->zoom * 15;
    camx = fplayer->tl->x + cf * SIN(fplayer->phi);
    camy = fplayer->tl->y + 5.5;
    camz = fplayer->tl->z + cf * COS(fplayer->phi);
    camphi = fplayer->phi;
    fplayer->draw = 1;
    camgamma = 50;
  }
  else if (camera == 6) // left
  {
    cf = fplayer->zoom * 2;
    camphi = fplayer->phi + 90.0;
    if (camphi >= 360) camphi -= 360;
    else if (camphi < 0) camphi += 360;
    camx = fplayer->tl->x + cf * SIN(camphi);
    camy = fplayer->tl->y + fplayer->zoom;
    camz = fplayer->tl->z + cf * COS(camphi);
    fplayer->draw = 1;
    camgamma = 20;
  }
  else if (camera == 7) // right
  {
    cf = fplayer->zoom * 2;
    camphi = fplayer->phi + 270.0;
    if (camphi >= 360) camphi -= 360;
    else if (camphi < 0) camphi += 360;
    camx = fplayer->tl->x + cf * SIN(camphi);
    camy = fplayer->tl->y + fplayer->zoom;
    camz = fplayer->tl->z + cf * COS(camphi);
    fplayer->draw = 1;
    camgamma = 20;
  }
  else if (camera == 8) // top near
  {
    cf = fplayer->zoom * 5;
    camx = fplayer->tl->x + cf * SIN(fplayer->phi);
    camy = fplayer->tl->y + 2.5;
    camz = fplayer->tl->z + cf * COS(fplayer->phi);
    camphi = fplayer->phi;
    fplayer->draw = 1;
    camgamma = 50;
  }
  else if (camera == 9) // top very near
  {
    cf = fplayer->zoom * 2;
    camx = fplayer->tl->x + cf * SIN(fplayer->phi);
    camy = fplayer->tl->y + 1.0;
    camz = fplayer->tl->z + cf * COS(fplayer->phi);
    camphi = fplayer->phi;
    fplayer->draw = 1;
    camgamma = 50;
  }
  else if (camera == 50)
  {
    camx = 20;
    camz = 80;
    camy = 250;
    camphi = 20;
    camgamma = 75;
    game = GAME_PAUSE;
  }
  if (camphi >= 360.0) camphi -= 360.0;
#ifdef USE_GLUT
  glutPostRedisplay();
#else
  sdldisplay = true;
#endif

  fplayer->rectheta -= dtheta;
  fplayer->recgamma += dgamma;

  missionstate = mission->processtimer (dt);

  if (missionstate == 2)
  {
    missionending ++;
    if (missionending >= 25)
    {
      missionending = 0;
      fplayer->score = mission->getScore (missionstate);
      if (!mission->id == MISSION_DEMO)
      {
        switch_stats ();
      }
      else
      {
        pleaseWait ();
        createMission (MISSION_DEMO);
        game_levelInit ();
      }
    }
  }

  if (missionstate == 1)
  {
    missionending ++;
    if (missionending >= 25)
    {
      missionending = 0;
      fplayer->score = mission->getScore (missionstate);
      if (mission->id != MISSION_DEMO)
      {
        if (mission->id != MISSION_MOON3)
          switch_stats ();
        else
          switch_finish ();
      }
      else
      {
        pleaseWait ();
        createMission (MISSION_DEMO);
        game_levelInit ();
      }
    }
  }

  if (controls == CONTROLS_KEYBOARD)
  {
    float fac = 0.005;
    if (keyb_elev == 0) fplayer->elevatoreffect = 0;
    else fplayer->elevatoreffect += (float) dt * keyb_elev * fac;
    if (keyb_roll == 0) fplayer->rolleffect = 0;
    else fplayer->rolleffect += (float) dt * keyb_roll * fac;
    if (keyb_rudder == 0) fplayer->ruddereffect = 0;
    else fplayer->ruddereffect += (float) dt * keyb_rudder * fac;
    if (fplayer->elevatoreffect > 1.0) fplayer->elevatoreffect = 1.0;
    if (fplayer->elevatoreffect < -0.5) fplayer->elevatoreffect = -0.5;
    if (fplayer->rolleffect > 1.0) fplayer->rolleffect = 1.0;
    if (fplayer->rolleffect < -1.0) fplayer->rolleffect = -1.0;
    if (fplayer->ruddereffect > 1.0) fplayer->ruddereffect = 1.0;
    if (fplayer->ruddereffect < -1.0) fplayer->ruddereffect = -1.0;
  }
}

float lastfps = -1;
int newcamera = 0;

void menu_timer (Uint32 dt)
{
  char buf [256];
  menutimer += dt;
  int cycle = (menutimer / timestep) % 256;
  if (cycle == 0)
  {
    newcamera = 0;
  }
  if (cycle == 200 && !newcamera)
  {
    newcamera = 1;
    if (camera == 5)
    {
      camera = 1;
      if (missile [0]->active)
        camera = 4;
    }
    else if (camera == 1 || camera == 4) camera = 2;
    else if (camera == 2) camera = 5;
  }

  int akttime;
#ifndef USE_GLUT
  akttime = SDL_GetTicks ();
#else
  akttime = glutGet (GLUT_ELAPSED_TIME);
#endif
  if (firststart)
    if (akttime - starttime < 20000)
    {
      if (lastfps != fps)
      {
        lastfps = fps;
        if (fps > 40)
        {
          if (view < quality * 20 + 60 && view < 100)
            view += 20;
          else if (quality < 5)
          {
            quality ++;
            view = quality * 20 + 30;
            if (quality >= 1)
            { antialiasing = 1; specialeffects = 1; dithering = 1; dynamiclighting = 0; }
          }
          sprintf (buf, "%d", (int) quality);
          ((Label *) optmenu [0]->components [1])->setText (buf);
          sprintf (buf, "%d", (int) view);
          ((Label *) optmenu [0]->components [3])->setText (buf);
        }
        else if (fps < 30)
        {
          if (view > quality * 20 + 30 && view > 20)
            view -= 20;
          else if (quality > 0)
          {
            quality --;
            view = quality * 20 + 60;
            if (quality < 1)
            { antialiasing = 0; specialeffects = 0; dithering = 0; dynamiclighting = 0; }
          }
          sprintf (buf, "%d", (int) quality);
          ((Label *) optmenu [0]->components [1])->setText (buf);
          sprintf (buf, "%d", (int) view);
          ((Label *) optmenu [0]->components [3])->setText (buf);
        }
        menu_reshape ();
      }
    }
#ifdef USE_GLUT
  glutPostRedisplay();
#else
  sdldisplay = true;
#endif
}

void stats_timer (Uint32 dt)
{
  menutimer += dt;
#ifdef USE_GLUT
  glutPostRedisplay();
#else
  sdldisplay = true;
#endif
}

void mission_timer (Uint32 dt)
{
  missionmenutimer += dt;
#ifdef USE_GLUT
  glutPostRedisplay();
#else
  sdldisplay = true;
#endif
}

void credits_timer (Uint32 dt)
{
  creditstimer += dt;
#ifdef SHOW_SOFTWARE_PATENTS
  if (creditstimer > 2500 * timestep)
    creditstimer = 0;
#else
  if (creditstimer > 700 * timestep)
    creditstimer = 0;
#endif

#ifdef USE_GLUT
  glutPostRedisplay();
#else
  sdldisplay = true;
#endif
}

void finish_timer (Uint32 dt)
{
  finishtimer += dt;
  if (finishtimer > 800 * timestep)
    finishtimer = 0;
#ifdef USE_GLUT
  glutPostRedisplay();
#else
  sdldisplay = true;
#endif
}

void create_timer (Uint32 dt)
{
#ifndef USE_GLUT
//  server->createSocketSet ();
  server->getClient ();
  SDL_Delay (100);
  mission_timer (dt);
#endif
}

void join_timer (Uint32 dt)
{
#ifdef HAVE_SDL_NET
#ifndef USE_GLUT
  char buf [STDSIZE];
  if (client->sock == NULL) client->getServer ("127.0.0.1", "client1");
  else 
  {
    SDL_Delay (100);
    mission_timer (dt);
    client->getMessage (buf);
  
    if (buf [0] == 's')
    {
      createMission (MISSION_MULTIPLAYER_DOGFIGHT);
      game_levelInit ();
      switch_game ();
      missionactive = true;
    }
  }
#endif
#endif
}



/****************************************************************************
  GAME DATA INITIALIZATION
****************************************************************************/

// calculate missile rack positions for a fighter
void setMissiles (CModel *model)
{
  int i;
  CVector3 tlmissile (0, 0.3, 0.3);
  for (i = 0; i < model->numObjects; i ++)
  {
    if (model->object [i]->numVertices == 4)
    {
      CObject *o = model->object [i];
      float sumx = 0, sumz = 0;
      float maxy = 2;
      int i2;
      for (i2 = 0; i2 < o->numVertices; i2 ++)
      {
        sumx += o->vertex [i2].vector.x;
        if (o->vertex [i2].vector.y < maxy)
          maxy = o->vertex [i2].vector.y;
        sumz += o->vertex [i2].vector.z;
      }
      tlmissile.x = sumx / 4.0F;
      tlmissile.y = maxy;
      tlmissile.z = sumz / 4.0F;

      tlmissile.y = maxy;
      model->addRefPoint (&tlmissile);
    }
  }
}

// initialize game data
void myInit ()
{
  int i, i2;

  // useful global variables/constants
  tlinf = new CVector3 (1E10, 1E10, 1E10);
  tlminf = new CVector3 (-1E10, -1E10, -1E10);
  tlnull = new CVector3 (0, 0, 0);
  rotnull = new CRotation ();
  rotmissile = new CRotation ();
  rotmissile->a = 90;
  rotmissile->c = 270;

  // initialize all global variables

  for (i = 0; i < maxgroundobj; i ++)
  {
    groundobj [i] = new DynamicObj (space, &model_tent1, 3);
  }

  explsphere = new CSphere ();
  ((CSphere *) explsphere)->init (1, 9);
  CColor explcolor (255, 255, 1);
  explsphere->setColor (&explcolor);
  explsphere->alpha = true;
  for (i = 0; i < explsphere->object [0]->numVertices; i ++)
  {
    explsphere->object [0]->vertex [i].color.setColor (myrandom (100) + 155, myrandom (100) + 100, 0, myrandom (3) / 2 * 255);
  }
  for (i = 0; i < maxexplosion; i ++)
  {
    explosion [i] = new CExplosion (space, explsphere);
  }

  for (i = 0; i < maxblacksmoke; i ++)
  {
    blacksmoke [i] = new CBlackSmoke (space);
  }

  for (i = 0; i < maxfighter; i ++)
  {
    fighter [i] = new AIObj (space, &model_fig, 0.4);
    for (i2 = 0; i2 < 12; i2 ++)
      fighter [i]->addRefModel (&model_missile1, tlnull, rotmissile, 0.2);
  }

  highclouds = new HighClouds (25);
  highclouds->setTexture (texclouds3);

  highclouds2 = new HighClouds (25);
  highclouds2->setTexture (texclouds3);

  objsphere = new CSphere (1, 9, 1, 1, 1);
  sphere = new CSpaceObj (objsphere, 10.0);
  sphere->rot->a = 90;
  sphere->rot->b = 90;
  sphere->rot->c = 270;
  sphere->draw = 2;
  sphere->drawlight = false;

  flash1 = new Flash ();

  for (i = 0; i < maxlaser; i ++)
  {
    laser [i] = new DynamicObj (space, &model_cannon1, 0.07);
  }

  for (i = 0; i < maxmissile; i ++)
  {
    missile [i] = new AIObj (space, &model_missile1, 0.1);
  }

  for (i = 0; i < maxflare; i ++)
  {
    flare [i] = new DynamicObj (space, &model_flare1, 0.1);
  }

  for (i = 0; i < maxchaff; i ++)
  {
    chaff [i] = new DynamicObj (space, &model_chaff1, 0.1);
  }

  for (i = 0; i < maxstar; i ++)
  {
    star [i] = new Star (myrandom (360), myrandom (85), 0.4 + 0.1 * myrandom (8));
  }

  cockpit = new Cockpit ();

  pleaseWait ();
  createMission (MISSION_DEMO);
  game_levelInit ();

  menu_reshape ();
}



/****************************************************************************
  INTRO
****************************************************************************/

CRotation rot;
CRotation rot2;
CVector3 tl;
CVector3 tl2;
int initexplode = 0;
int initexplode1 = 0;
int i;
int inittimer = 0;
int inittimer_gl117 = 0;

void init_reshape ()
{
  // use whole window
  glViewport (0, 0, (GLint) width, (GLint) height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  
  // angle, aspectratio, nearclip, farclip
  gluPerspective (visibleangle, 1.0, nearclippingplane, 80);
  glPolygonMode (GL_FRONT_AND_BACK, polygonMode);
}

// load game data (this method does not really belong to the intro itself)
void myFirstInit ()
{
  display ("Creating calculation tables", LOG_ALL);
  mathtab_init ();

  display ("Creating advanced OpenGL methods", LOG_ALL);
  gl = new GL ();

  // create textures (OpenGL)
  display ("Loading textures", LOG_ALL);
  texgrass = gl->genTextureTGA (dirs->getTextures ("grass1.tga"), 0, 0, 1, false);
  texrocks = gl->genTextureTGA (dirs->getTextures ("rocks1.tga"), 0, 0, 1, false);
  texwater = gl->genTextureTGA (dirs->getTextures ("water1.tga"), 0, 0, 1, false);
  texsand = gl->genTextureTGA (dirs->getTextures ("sand1.tga"), 0, 0, 1, false);
  texredsand = gl->genTextureTGA (dirs->getTextures ("redsand1.tga"), 0, 0, 1, false);
  texredstone = gl->genTextureTGA (dirs->getTextures ("redstone2.tga"), 0, 0, 1, false);
  texgravel1 = gl->genTextureTGA (dirs->getTextures ("gravel1.tga"), 0, 0, 1, false);
  texglitter1 = gl->genTextureTGA (dirs->getTextures ("glitter.tga"), 0, -1, 0, true);
  textree = gl->genTextureTGA (dirs->getTextures ("tree1.tga"), 0, -1, 1, true);
  textreeu = gl->genTextureTGA (dirs->getTextures ("treeu1.tga"), 0, -1, 1, true);
  textree2 = gl->genTextureTGA (dirs->getTextures ("tree2.tga"), 0, -1, 1, true);
  textreeu2 = gl->genTextureTGA (dirs->getTextures ("treeu2.tga"), 0, -1, 1, true);
  textree3 = gl->genTextureTGA (dirs->getTextures ("tree3.tga"), 0, 3, 1, true);
  textreeu3 = gl->genTextureTGA (dirs->getTextures ("treeu3.tga"), 0, 3, 1, true);
  textree4 = gl->genTextureTGA (dirs->getTextures ("tree4.tga"), 0, 3, 1, true);
  textreeu4 = gl->genTextureTGA (dirs->getTextures ("treeu4.tga"), 0, 3, 1, true);
  textree5 = gl->genTextureTGA (dirs->getTextures ("tree5.tga"), 0, -1, 1, true);
  textreeu5 = gl->genTextureTGA (dirs->getTextures ("treeu5.tga"), 0, -1, 1, true);
  texcactus1 = gl->genTextureTGA (dirs->getTextures ("cactus1.tga"), 0, 3, 1, true);
  texcactusu1 = gl->genTextureTGA (dirs->getTextures ("cactusu1.tga"), 0, 3, 1, true);
  texsmoke = gl->genTextureTGA (dirs->getTextures ("smoke1.tga"), 0, -1, 1, true);
  texsmoke2 = gl->genTextureTGA (dirs->getTextures ("smoke2.tga"), 0, -1, 1, true);
  texsmoke3 = gl->genTextureTGA (dirs->getTextures ("smoke3.tga"), 0, 5, 1, true);
  texsun = gl->genTextureTGA (dirs->getTextures ("sun2.tga"), 1, -1, 0, true);
  texmoon = gl->genTextureTGA (dirs->getTextures ("moon1.tga"), 1, 2, 0, true);
  texearth = gl->genTextureTGA (dirs->getTextures ("earth.tga"), 1, 0, 0, true);
  texflare1 = gl->genTextureTGA (dirs->getTextures ("flare1.tga"), 1, -1, 0, true);
  texflare2 = gl->genTextureTGA (dirs->getTextures ("flare2.tga"), 1, -1, 0, true);
  texflare3 = gl->genTextureTGA (dirs->getTextures ("flare3.tga"), 1, -1, 0, true);
  texflare4 = gl->genTextureTGA (dirs->getTextures ("flare4.tga"), 1, -1, 0, true);
  texcross = gl->genTextureTGA (dirs->getTextures ("cross.tga"), 0, -1, 1, true);
  texcross2 = gl->genTextureTGA (dirs->getTextures ("cross2.tga"), 0, -1, 1, true);
  texranks = gl->genTextureTGA (dirs->getTextures ("ranks.tga"), 0, 0, 0, true);
  texmedals = gl->genTextureTGA (dirs->getTextures ("medals.tga"), 0, 0, 0, true);
  texclouds1 = gl->genTextureTGA (dirs->getTextures ("clouds1.tga"), 0, -1, 1, true);
  texclouds2 = gl->genTextureTGA (dirs->getTextures ("clouds2.tga"), 0, 4, 1, true);
  texclouds3 = gl->genTextureTGA (dirs->getTextures ("clouds3.tga"), 0, 6, 1, true);
  texradar1 = gl->genTextureTGA (dirs->getTextures ("radar2.tga"), 0, -1, 0, true);
  texradar2 = gl->genTextureTGA (dirs->getTextures ("radar1.tga"), 0, -1, 0, true);
  texarrow = gl->genTextureTGA (dirs->getTextures ("arrow.tga"), 0, -1, 0, true);

  display ("Loading Fonts", LOG_ALL);
  font1 = new Font (dirs->getTextures ("font1.tga"), 32, '!', 64);
//  font1 = new Font (dirs->getTextures ("font3.tga"), 37, '!', 100);
  font2 = new Font (dirs->getTextures ("font2.tga"), 32, '!', 64);

  display ("Loading 3ds models:", LOG_ALL);
  display (" * gl-16.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_fig, dirs->getModels ("gl-16.3ds"));
  model_fig.setName ("FALCON");
  display (" * gl-15.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_figa, dirs->getModels ("gl-15.3ds"));
  model_figa.setName ("SWALLOW");
  display (" * gl-14c.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_figb, dirs->getModels ("gl-14c.3ds"));
  model_figb.setName ("HAWK");
  display (" * gl-14d.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_figc, dirs->getModels ("gl-14d.3ds"));
  model_figc.setName ("HAWK II");
  display (" * gl-21b.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_figd, dirs->getModels ("gl-21b.3ds"));
  model_figd.setName ("BUZZARD");
  display (" * gl-21.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_fige, dirs->getModels ("gl-21.3ds"));
  model_fige.setName ("CROW");
  display (" * gl-14b.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_figf, dirs->getModels ("gl-14b.3ds"));
  model_figf.setName ("PHOENIX");
  display (" * gl-14.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_figg, dirs->getModels ("gl-14.3ds"));
  model_figg.setName ("RED ARROW");
  display (" * gl-29.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_figh, dirs->getModels ("gl-29.3ds"));
  model_figh.setName ("BLACKBIRD");
  model_figh.scaleTexture (0.3, 0.3);
  display (" * gl-50.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_figi, dirs->getModels ("gl-50.3ds"));
  model_figi.setName ("STORM");
  display (" * transp2.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_figt, dirs->getModels ("transp2.3ds"));
  model_figt.setName ("TRANSPORT");
  display (" * transp4.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_figu, dirs->getModels ("transp4.3ds"));
  model_figu.setName ("TRANSPORT");

  // cannon at daylight
  float cannoncube = 0.025;
  display (" * cannon1.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_cannon1, dirs->getModels ("cannon1.3ds"));
  model_cannon1.cubex = cannoncube; model_cannon1.cubey = cannoncube; model_cannon1.cubez = cannoncube;
  display (" * cannon1b.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_cannon1b, dirs->getModels ("cannon1b.3ds"));
  model_cannon1b.cubex = cannoncube; model_cannon1b.cubey = cannoncube; model_cannon1b.cubez = cannoncube;

  // cannon at night
  display (" * cannon2.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_cannon2, dirs->getModels ("cannon2.3ds"));
  model_cannon2.nolight = true;
  model_cannon2.alpha = true;
  for (i = 0; i < 4; i ++)
  {
    model_cannon2.object [0]->vertex [i].color.c [0] = 255;
    model_cannon2.object [0]->vertex [i].color.c [1] = 255;
    model_cannon2.object [0]->vertex [i].color.c [2] = 0;
    model_cannon2.object [0]->vertex [i].color.c [3] = 255;
  }
  model_cannon2.object [0]->vertex [1].color.c [3] = 50;
  model_cannon2.object [0]->vertex [2].color.c [3] = 50;
  model_cannon2.cubex = cannoncube; model_cannon2.cubey = cannoncube; model_cannon2.cubez = cannoncube;

  display (" * cannon2b.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_cannon2b, dirs->getModels ("cannon2b.3ds"));
  model_cannon2b.nolight = true;
  model_cannon2b.alpha = true;
  for (int i2 = 0; i2 < 2; i2 ++)
  {
    for (i = 0; i < 4; i ++)
    {
      model_cannon2b.object [i2]->vertex [i].color.c [0] = 255;
      model_cannon2b.object [i2]->vertex [i].color.c [1] = 255;
      model_cannon2b.object [i2]->vertex [i].color.c [2] = 0;
      model_cannon2b.object [i2]->vertex [i].color.c [3] = 255;
    }
    model_cannon2b.object [i2]->vertex [1].color.c [3] = 50;
    model_cannon2b.object [i2]->vertex [2].color.c [3] = 50;
  }
  model_cannon2b.cubex = cannoncube; model_cannon2b.cubey = cannoncube; model_cannon2b.cubez = cannoncube;

  display (" * flare1.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_flare1, dirs->getModels ("flare1.3ds"));
  model_flare1.setName ("FLARE");
  model_flare1.alpha = true;
  model_flare1.nolight = true;
  display (" * chaff1.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_chaff1, dirs->getModels ("chaff1.3ds"));
  model_chaff1.setName ("CHAFF");
  model_chaff1.alpha = true;
  model_chaff1.nolight = true;
  display (" * missile1.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_missile1, dirs->getModels ("missile1.3ds"));
  model_missile1.setName ("AAM HS MK1");
  display (" * missile2.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_missile2, dirs->getModels ("missile2.3ds"));
  model_missile2.setName ("AAM HS MK2");
  display (" * missile3.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_missile3, dirs->getModels ("missile3.3ds"));
  model_missile3.setName ("AAM HS MK3");
  display (" * missile4.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_missile4, dirs->getModels ("missile4.3ds"));
  model_missile4.setName ("AGM MK1");
  display (" * missile5.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_missile5, dirs->getModels ("missile5.3ds"));
  model_missile5.setName ("AGM MK2");
  display (" * missile6.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_missile6, dirs->getModels ("missile6.3ds"));
  model_missile6.setName ("DFM");
  display (" * missile7.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_missile7, dirs->getModels ("missile7.3ds"));
  model_missile7.setName ("AAM FF MK1");
  display (" * missile8.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_missile8, dirs->getModels ("missile8.3ds"));
  model_missile8.setName ("AAM FF MK2");
  display (" * flak2.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_flak1, dirs->getModels ("flak2.3ds"));
  model_flak1.setName ("SA CANNON");
  display (" * flarak1.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_flarak1, dirs->getModels ("flarak1.3ds"));
  model_flarak1.setName ("SAM");
  display (" * ship1.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_ship1, dirs->getModels ("ship1.3ds"));
  model_ship1.setName ("CRUISER");
  display (" * tent1.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_tent1, dirs->getModels ("tent1.3ds"));
  model_tent1.setName ("TENT");
  display (" * gl-117.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_gl117, dirs->getModels ("gl-117.3ds"));
  model_gl117.displaylist = false;
  display (" * tank1.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_tank1, dirs->getModels ("tank1.3ds"));
  model_tank1.setName ("WIESEL");
  model_tank1.scaleTexture (0.5, 0.5);
  display (" * container1.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_container1, dirs->getModels ("container1.3ds"));
  model_container1.setName ("CONTAINER");
  display (" * ship2.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_ship2, dirs->getModels ("ship2.3ds"));
  model_ship2.setName ("LIGHT DESTROYER");
  display (" * truck1.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_truck1, dirs->getModels ("truck1.3ds"));
  model_truck1.setName ("TRUCK");
  display (" * truck2.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_truck2, dirs->getModels ("truck2.3ds"));
  model_truck2.setName ("TRUCK");
  display (" * trsam.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_trsam, dirs->getModels ("trsam.3ds"));
  model_trsam.setName ("MOBILE SAM");
  display (" * pickup1.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_pickup1, dirs->getModels ("pickup1.3ds"));
  model_pickup1.setName ("PICKUP");
  display (" * pickup2.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_pickup2, dirs->getModels ("pickup2.3ds"));
  model_pickup2.setName ("PICKUP");
  display (" * tank2.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_tank2, dirs->getModels ("tank2.3ds"));
  model_tank2.setName ("PANTHER");
  model_tank2.scaleTexture (0.5, 0.5);
  display (" * tent4.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_tent4, dirs->getModels ("tent4.3ds"));
  model_tent4.setName ("BIG TENT");
  display (" * hall1.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_hall1, dirs->getModels ("hall1.3ds"));
  model_hall1.setName ("HALL");
  display (" * hall2.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_hall2, dirs->getModels ("hall2.3ds"));
  model_hall2.setName ("HALL");
  display (" * oilrig.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_oilrig, dirs->getModels ("oilrig.3ds"));
  model_oilrig.setName ("OILRIG");
  model_oilrig.alpha = true;
  display (" * egg.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_egg, dirs->getModels ("egg.3ds"));
  model_egg.scaleTexture (0.08, 0.08);
  model_egg.setName ("COMPLEX");
  display (" * radar.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_radar, dirs->getModels ("radar.3ds"));
  model_radar.setName ("RADAR");
  display (" * mine1.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_mine1, dirs->getModels ("mine1.3ds"));
  model_mine1.setName ("MINE");
  display (" * aster1.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_aster1, dirs->getModels ("aster1.3ds"));
  model_aster1.setName ("ASTEROID");
  display (" * base1.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_base1, dirs->getModels ("base1.3ds"));
  model_base1.setName ("MOON BASE");
  display (" * barrier.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_barrier1, dirs->getModels ("barrier.3ds"));
  model_barrier1.setName ("MOON BASE");
  model_barrier1.scaleTexture (10, 10);
  model_barrier1.alpha = true;
  display (" * rubble.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_rubble1, dirs->getModels ("rubble.3ds"));
  model_base1.setName ("RUBBLE");
  display (" * depot1.3ds", LOG_ALL);
  g_Load3ds.Import3DS (&model_depot1, dirs->getModels ("depot1.3ds"));
  model_depot1.setName ("DEPOT");
  model_depot1.scaleTexture (2, 2);
  g_Load3ds.Import3DS (&model_house1, dirs->getModels ("house1.3ds"));
  model_house1.setName ("HOUSE");

  setMissiles (&model_fig);
  setMissiles (&model_figa);
  setMissiles (&model_figb);
  setMissiles (&model_figc);
  setMissiles (&model_figd);
  setMissiles (&model_fige);
  setMissiles (&model_figf);
  setMissiles (&model_figg);
  setMissiles (&model_figh);
  setMissiles (&model_figi);

  // enable Z-Buffer
  glEnable (GL_DEPTH_TEST);

  // fill polygons (GL_LINE for wireframe models)
  glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

  display ("Setting up world geometry", LOG_ALL);
  space = new Space ();
  space->drawlight = true;
  clip1 = space->z1;
  clip2 = space->z2;
  clip1->x = -ZOOM;
  clip1->y = -ZOOM;
  clip1->z = -ZOOM;
  clip2->x = ZOOM;
  clip2->y = ZOOM;
  clip2->z = ZOOM;

  // prepare intro
  init_reshape ();

  tl.x = 0.0;
  tl.y = 0.0;
  tl.z = -5.0;
  tl2.x = 0.0;
  tl2.y = 0.0;
  tl2.z = -5.0;
  rot.a = 270;
  rot2.c = 90;
  rot2.a = 270;
  rot2.b = 270;
  initexplode = 0;
  initexplode1 = 0;

  textitle = new CTexture ();
  textitle = gl->genTextureTGA (dirs->getTextures ("patents.tga"), 0, 0, 0, true);

  sungamma = 60;
  setLightSource (60);

  event_setAntialiasing ();

  glLightModeli (GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
//  glLightModeli (GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
  glEnable (GL_LIGHT0);
  glEnable (GL_LIGHTING);
  glEnable (GL_COLOR_MATERIAL);

  glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
}

void init_key (int key, int x, int y)
{
  gl->clearScreen (); // exit intro
  myInit ();
  switch_menu ();
  fplayer->ai = true;
  camera = 5;
#ifndef USE_GLUT
  starttime = SDL_GetTicks ();
#else
  starttime = glutGet (GLUT_ELAPSED_TIME);
#endif
}

void init_mouse (int button, int state, int x, int y)
{
  init_key (32, x, y);
}

const int maxfx = 256;
const int maxfy = 64;

int heat [maxfy] [maxfx];
int heat2 [maxfy] [maxfx];

void init_display ()
{
  CVector3 vec;
  CColor color (200, 200, 200, 255);

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();

  glPushMatrix ();

  glEnable (GL_DEPTH_TEST);
  glEnable (GL_LIGHTING);

  // draw fighter
  glPushMatrix ();
  glTranslatef (0, 0, -5);
  model_fig.draw (&vec, &tl, &rot, 1.0, 2.0, initexplode1);
  glPopMatrix ();

  glDisable (GL_DEPTH_TEST);
  glEnable (GL_BLEND);

  if (inittimer_gl117 > 2000)
  {
    float xf = 1.3F, yf = 0.65F, zf = 2.0F;
    int col = (inittimer_gl117 - 2000) / 2;
    if (col < 0 || col > 255) col = 255;
    glColor3ub (col, col, col);
    glPushMatrix ();
    glTranslatef (0, 0.5F, 0);
    gl->enableTextures (textitle->textureID);
    if (antialiasing)
      gl->enableLinearTexture (textitle->textureID);
    else
      gl->disableLinearTexture (textitle->textureID);
    glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBegin (GL_QUADS);
    glTexCoord2d (0, 1);
    glVertex3f (-xf, yf, -zf);
    glTexCoord2d (1, 1);
    glVertex3f (xf, yf, -zf);
    glTexCoord2d (1, 0);
    glVertex3f (xf, -yf, -zf);
    glTexCoord2d (0, 0);
    glVertex3f (-xf, -yf, -zf);
    glEnd ();
    glPopMatrix ();
    glDisable (GL_BLEND);
  }
  
  glDisable (GL_LIGHTING);
  glPopMatrix ();

  // draw fire (heat array)
  glDisable (GL_DEPTH_TEST);
  glEnable (GL_BLEND);

  float xf = 1.75F, yf = 1.78F, zf = 2.0F;
  glPushMatrix ();
  gl->enableTextures (5000);
  glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glBegin (GL_QUADS);
  glTexCoord2d (0, 1);
  glVertex3f (-xf, -yf, -zf);
  glTexCoord2d (1, 1);
  glVertex3f (xf, -yf, -zf);
  glTexCoord2d (1, 0);
  glVertex3f (xf, yf, -zf);
  glTexCoord2d (0, 0);
  glVertex3f (-xf, yf, -zf);
  glEnd ();
  glPopMatrix ();
  glDisable (GL_BLEND);

  font2->drawText (20, -20, -3, VERSIONSTRING, &color);
}

void genFireLine ()
{
  int i, i2;
  for (i = 0; i < maxfx; i ++)
  {
    heat [maxfy - 1] [i] = myrandom (400);
  }
  for (i = 0; i < 5; i ++)
  {
    int r = myrandom (maxfx - 7) + 3;
    for (i2 = -3; i2 <= 3; i2 ++)
    {
      heat [maxfy - 1] [r + i2] = 1200; // insert hot spots at the bottom line
    }
  }
}

unsigned char firetex [maxfx * maxfy * 4];

void proceedFire ()
{
  int i, i2;
  for (i = maxfy - 2; i >= 1; i --)
    for (i2 = 2; i2 < maxfx - 2; i2 ++)
    {
      heat2 [i] [i2] = 0 * heat [i + 1] [i2 - 2] + 4 * heat [i + 1] [i2 - 1] + 16 * heat [i + 1] [i2] + 4 * heat [i + 1] [i2 + 1] + 0 * heat [i + 1] [i2 + 2]; // heat diffusion
      heat2 [i] [i2] += 0 * heat [i] [i2 - 2] + 4 * heat [i] [i2 - 1] + 16 * heat [i] [i2] + 4 * heat [i] [i2 + 1] + 0 * heat [i] [i2 + 2]; // heat diffusion
	  heat2 [i] [i2] /= 48;
      heat2 [i] [i2] -= (int) (300.0F / maxfy); // heat sink
      if (heat2 [i] [i2] < 0) heat2 [i] [i2] = 0;
    }
  memcpy (heat, heat2, maxfx * maxfy * sizeof (int)); // copy back buffer to heat array

  
  for (i = 0; i < maxfy; i ++)
  {
    for (i2 = 0; i2 < maxfx + 1; i2 ++)
    {
      // rotate through fire colors (white-yellow-red-black-blue-black)
      // col in [0...512]
      int yind = i;
	    int h = heat [yind] [i2];
	    int b = h * 5;
	    if (h > 30) b = (60 - h) * 5;
	    if (h >= 60) b = 0;
	    h -= 50;
      int r = h * 2; // blend out late for red->black
      if (r > 255) r = 255;
	    else if (r < 0) r = 0;
	    h -= 127;
      int g = h * 2; // blend out for yellow->red
      if (g > 255) g = 255;
      else if (g < 0) g = 0;
	    h -= 127;
      if (h > 0)
      {
        b = h - 256; // blend out early to get white->yellow
        if (b > 255) b = 255;
      }
      else if (b < 0) b = 0;
      int a = r >= b ? r : b; // alpha value: transparent after yellow-red phase
      glColor4ub (r, g, b, a);
	    firetex [(i * maxfx + i2) * 4] = r;
	    firetex [(i * maxfx + i2) * 4 + 1] = g;
	    firetex [(i * maxfx + i2) * 4 + 2] = b;
	    firetex [(i * maxfx + i2) * 4 + 3] = a;
    }
    glEnd ();
  }
  glPopMatrix ();
  glBindTexture (GL_TEXTURE_2D, 5000);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D (GL_TEXTURE_2D, 0, 4, maxfx, maxfy, 0, GL_RGBA, GL_UNSIGNED_BYTE, firetex);
}

int initsynchrotimer = 0;

void init_timer (Uint32 dt)
{
  inittimer_gl117 += dt;
  initsynchrotimer += dt;
  if (initsynchrotimer > 20)
  {
    initsynchrotimer -= 20;
    dt = 20;
  }
  else return;

  int r = myrandom (100);
  if (r == 50) r = myrandom (100); // do not optimize this: random number generator initialization

  tl.x = 6.0 * pow (1.5, -(5 + tl.z));
  tl.y = (tl.z + 3) * (tl.z + 3) * 0.02 - 0.8; //0.9 * tl.x;
  tl.z += 0.14;

  tl2.y = 0.3F;
  if (initexplode1 == -1 && tl2.z < 3)
  {
    tl2.z += 0.25;
  }
  if (tl2.z < 3)
  {
    rot2.b -= 20;
    if (rot2.b < 0) rot2.b += 360;
  }
  if (tl.z > -4.0)
  {
    rot.b ++;
  }

  if (inittimer == 0)
  {
    memset (heat, 0, maxfx * maxfy * sizeof (int));
    memset (heat2, 0, maxfx * maxfy * sizeof (int));
  }

  genFireLine ();
  proceedFire ();

  if (inittimer >= 75)
  {
    initexplode1 = -1;
  }

  if (inittimer >= 350)
  {
    initexplode += dt;
  }

  if (inittimer >= 400)
  {
    init_key (27, 0, 0); // switch to menu
  }

  inittimer ++;

#ifdef USE_GLUT
  glutPostRedisplay();
#else
  sdldisplay = true;
#endif
}



/****************************************************************************
  ABSTRACT EVENT BRANCHES
****************************************************************************/

static void myKeyboardFunc (unsigned char uckey, int x, int y)
{
  int key = (int) uckey;
  if (key >= 'a' && key <= 'z') key = toupper (key);
  if (game == GAME_PLAY || game == GAME_PAUSE)
    game_key (key, x, y);
  else if (game == GAME_INIT)
    init_key (key, x, y);
  else if (game == GAME_MENU)
  {
    allmenus.eventKey (key);
    menu_key (key, x, y);
  }
  else if (game == GAME_CREDITS)
    credits_key (key, x, y);
  else if (game == GAME_CREATE)
    create_key (key, x, y);
  else if (game == GAME_JOIN)
    join_key (key, x, y);
  else if (game == GAME_MISSION)
    mission_key (key, x, y);
  else if (game == GAME_STATS)
    stats_key (key, x, y);
  else if (game == GAME_FAME)
    fame_key (key, x, y);
  else if (game == GAME_FIGHTER)
    fighter_key (key, x, y);
  else if (game == GAME_FINISH)
    finish_key (key, x, y);
  else if (game == GAME_QUIT)
    quit_key (key, x, y);
}

static void myKeyboardFuncUp (unsigned char key, int x, int y)
{
  if (game == GAME_PLAY || game == GAME_PAUSE)
    game_keyup ((int) key, x, y);
}

static void mySpecialFunc (int key, int x, int y)
{
  if (game == GAME_PLAY || game == GAME_PAUSE)
    game_key (key + 256, x, y);
  else if (game == GAME_MENU)
  {
    allmenus.eventSpecial (key);
  }
}

static void mySpecialFuncUp (int key, int x, int y)
{
  if (game == GAME_PLAY || game == GAME_PAUSE)
    game_keyup (key + 256, x, y);
}

static void myPassiveRelMotionFunc (int xrel, int yrel)
{
  if (game == GAME_PLAY)
    if (controls == CONTROLS_MOUSE)
      game_mouserelmotion (xrel, yrel);
}

static void myPassiveMotionFunc (int x, int y)
{
  mousex = x; mousey = y;
  if (game == GAME_PLAY)
    game_mousemotion (x, y);
  else if (game == GAME_MENU)
  {
    allmenus.eventMouse (x, y, 0);
    menu_mouse (-1, -1, x, y);
  }
  else if (game == GAME_STATS)
  {
    statsmenu.eventMouse (x, y, 0);
    stats_mouse (-1, -1, x, y);
  }
  else if (game == GAME_MISSION)
  {
    missionmenu.eventMouse (x, y, 0);
    mission_mouse (-1, -1, x, y);
  }
  else if (game == GAME_FAME)
  {
    famemenu.eventMouse (x, y, 0);
    fame_mouse (-1, -1, x, y);
  }
  else if (game == GAME_FIGHTER)
  {
    fightermenu.eventMouse (x, y, 0);
    fighter_mouse (-1, -1, x, y);
  }
  else if (game == GAME_CREATE)
    create_mouse (-1, -1, x, y);
  else if (game == GAME_JOIN)
    join_mouse (-1, -1, x, y);
  else if (game == GAME_QUIT)
  {
    quitmenu.eventMouse (x, y, 0);
    quit_mouse (-1, -1, x, y);
  }
}

static void myMouseFunc (int button, int state, int x, int y)
{
#ifdef USE_GLUT
  button ++;
#endif
  if (game == GAME_PLAY)
  {
    if (controls == CONTROLS_MOUSE)
      game_mouse (button, state, x, y);
  }
  else if (game == GAME_INIT)
  {
    init_mouse (button, state, x, y);
  }
  else if (game == GAME_MENU)
  {
#ifdef USE_GLUT
    if (state == 0)
#endif
    allmenus.eventMouse (x, y, button);
    menu_mouse (button, state, x, y);
  }
  else if (game == GAME_MISSION)
  {
#ifdef USE_GLUT
    if (state == 0)
#endif
    missionmenu.eventMouse (x, y, button);
    mission_mouse (button, state, x, y);
  }
  else if (game == GAME_STATS)
  {
#ifdef USE_GLUT
    if (state == 0)
#endif
    statsmenu.eventMouse (x, y, button);
    stats_mouse (button, state, x, y);
  }
  else if (game == GAME_FAME)
  {
#ifdef USE_GLUT
    if (state == 0)
#endif
    famemenu.eventMouse (x, y, button);
    fame_mouse (button, state, x, y);
  }
  else if (game == GAME_FIGHTER)
  {
#ifdef USE_GLUT
    if (state == 0)
#endif
    fightermenu.eventMouse (x, y, button);
    fighter_mouse (button, state, x, y);
  }
  else if (game == GAME_CREATE)
  {
    create_mouse (button, state, x, y);
  }
  else if (game == GAME_JOIN)
  {
    join_mouse (button, state, x, y);
  }
  else if (game == GAME_QUIT)
  {
#ifdef USE_GLUT
    if (state == 0)
#endif
    quitmenu.eventMouse (x, y, button);
    quit_mouse (button, state, x, y);
  }
  else if (game == GAME_CREDITS)
  {
#ifdef USE_GLUT
    if (state == 0)
#endif
    credits_mouse (button, state, x, y);
  }
}

static void myReshapeFunc (int width, int height)
{
  ::width = width;
  ::height = height;
  ::wantwidth = width;
  ::wantheight = height;
  if (game == GAME_PLAY || game == GAME_PAUSE)
    game_reshape ();
  else if (game == GAME_MENU || game == GAME_MISSION || game == GAME_QUIT)
    menu_reshape ();
  else if (game == GAME_INIT)
    init_reshape ();
  else if (game == GAME_CREDITS || game == GAME_FINISH)
    credits_reshape ();
  else if (game == GAME_STATS)
    stats_reshape ();
}

static void myDisplayFunc ()
{
  if (game == GAME_INIT)
  {
    init_display ();
    game_view ();
  }
  else if (game == GAME_PLAY)
  {
    game_display ();
    game_view ();
  }
  else if (game == GAME_PAUSE)
  {
    game_display ();
    pause_display ();
    game_view ();
  }
  else if (game == GAME_MENU)
  {
    if (camera == 50) camera = 0;
    game_display ();
    menu_display ();
    game_view ();
  }
  else if (game == GAME_CREDITS)
  {
    if (camera == 50) camera = 0;
    game_display ();
    credits_display ();
    game_view ();
  }
  else if (game == GAME_MISSION)
  {
    if (camera == 50) camera = 0;
    game_display ();
    mission_display ();
    game_view ();
  }
  else if (game == GAME_STATS)
  {
    if (camera == 50) camera = 0;
    game_display ();
    stats_display ();
    game_view ();
  }
  else if (game == GAME_FAME)
  {
    game_display ();
    fame_display ();
    game_view ();
  }
  else if (game == GAME_FIGHTER)
  {
    game_display ();
    fighter_display ();
    game_view ();
  }
  else if (game == GAME_CREATE)
  {
    create_display ();
    game_view ();
  }
  else if (game == GAME_JOIN)
  {
    join_display ();
    game_view ();
  }
  else if (game == GAME_FINISH)
  {
    if (camera == 50) camera = 0;
    game_display ();
    finish_display ();
    game_view ();
  }
  else if (game == GAME_QUIT)
  {
    game_display ();
    quit_display ();
    game_view ();
  }
}

static void myIdleFunc ()
{
#ifdef USE_GLUT
  glutPostRedisplay ();
#endif
}

static void myJoystickAxisFunc (int x, int y, int t, int r)
{
  if (game == GAME_PLAY)
  {
    if (controls == CONTROLS_JOYSTICK)
      game_joystickaxis (x, y, t, r);
  }
}

static void myJoystickButtonFunc (int button)
{
  if (game == GAME_PLAY)
  {
    if (controls == CONTROLS_JOYSTICK)
      game_joystickbutton (button);
  }
  else if (game == GAME_MENU)
  {
    allmenus.eventJoystick (button);
  }
}

static void myJoystickHatFunc (int hat)
{
  int normhat = hat;
#ifndef USE_GLUT
  if (hat % 1000 == SDL_HAT_RIGHT) normhat = 100;
  if (hat % 1000 == SDL_HAT_UP) normhat = 101;
  if (hat % 1000 == SDL_HAT_LEFT) normhat = 102;
  if (hat % 1000 == SDL_HAT_DOWN) normhat = 103;
  normhat += (hat / 1000) * 1000;
#endif
  if (game == GAME_PLAY)
  {
    if (controls == CONTROLS_JOYSTICK)
      game_joystickhat (normhat);
  }
  else if (game == GAME_MENU)
  {
    allmenus.eventJoystick (normhat);
  }
}

static void myTimerFunc (int value)
{
  Uint32 akttime, dt;
#ifndef USE_GLUT
    akttime = SDL_GetTicks ();
#else
    akttime = glutGet (GLUT_ELAPSED_TIME);
#endif
  if (lasttime == 0) dt = 1;
  else dt = akttime - lasttime;
  lasttime = akttime;

  if (dt > 1000)
  {
    dt = 1;
    if (game == GAME_PLAY && multiplayer)
    {
      display ("Out of sync", LOG_ERROR);
      switch_menu ();
    }
  }

  if (game == GAME_PLAY)
    game_timer (dt);
  else if (game == GAME_INIT)
    init_timer (dt);
  else if (game == GAME_MENU)
  {
    if (!missionactive)
      game_timer (dt);
    menu_timer (dt);
  }
  else if (game == GAME_CREDITS)
  {
    if (!missionactive)
      game_timer (dt);
    credits_timer (dt);
  }
  else if (game == GAME_CREATE)
    create_timer (dt);
  else if (game == GAME_JOIN)
    join_timer (dt);
  else if (game == GAME_MISSION)
  {
    if (!missionactive)
      game_timer (dt);
    mission_timer (dt);
  }
  else if (game == GAME_STATS)
    stats_timer (dt);
  else if (game == GAME_FAME)
  {
    if (!missionactive)
      game_timer (dt);
    mission_timer (dt);
  }
  else if (game == GAME_FIGHTER)
  {
    if (!missionactive)
      game_timer (dt);
    mission_timer (dt);
  }
  else if (game == GAME_FINISH)
    finish_timer (dt);
  else if (game == GAME_QUIT)
  {
    if (!missionactive)
      game_timer (dt);
    mission_timer (dt);
  }

#ifdef USE_GLUT
  glutTimerFunc (1, myTimerFunc, 0); // do as many timer calls as possible
#endif
}

#ifndef USE_GLUT

Uint32 nexttime = 0;

int joystickbutton = -1;
bool joystickfirebutton = false;

// This loop emulates the glutMainLoop() of GLUT using SDL!!!
void sdlMainLoop ()
{
  int sym = 0;
  SDL_Event event;
  
  while (true)
  {
    while (SDL_PollEvent (&event)) // process events
    {
      switch (event.type)
      {
        case SDL_MOUSEMOTION:
          myPassiveMotionFunc (event.motion.x, event.motion.y);
          if (game == GAME_PLAY && controls == CONTROLS_MOUSE && mouse_relative)
          {
            fplayer->rolleffect = 0;
            fplayer->elevatoreffect = 0;
            fplayer->ruddereffect = 0;
          }
          myPassiveRelMotionFunc (event.motion.xrel, event.motion.yrel);
          break;

        case SDL_MOUSEBUTTONDOWN:
          myMouseFunc (event.button.button, event.button.state, event.button.x, event.button.y);
          break;

        case SDL_KEYDOWN:
          if (!event.key.keysym.unicode)
            mySpecialFunc (event.key.keysym.sym, 0, 0);
          else
            myKeyboardFunc (event.key.keysym.sym, 0, 0);
          break;

        case SDL_KEYUP:
          sym = event.key.keysym.sym;
          if (sym == 8 || sym == 9 || sym == 13 || (sym >= 32 && sym <= 'z'))
            myKeyboardFuncUp (event.key.keysym.sym, 0, 0);
          else
            mySpecialFuncUp (event.key.keysym.sym, 0, 0);
          break;

        case SDL_JOYAXISMOTION:
          if (abs (event.jaxis.value) < 2000)
          {
            jaxis [event.jaxis.axis + event.jaxis.which * 10] = 0;
          }
          else
          {
            if (event.jaxis.value < 0)
              event.jaxis.value += 2500;
            else
              event.jaxis.value -= 2500;
            jaxis [event.jaxis.axis + event.jaxis.which * 10] = (int) event.jaxis.value * 32768 / 30268;
          }
          break;

        case SDL_JOYBUTTONDOWN:
          joystickbutton = event.jbutton.button + event.jbutton.which * 1000;
          if (joystickbutton == (int) joystick_firecannon)
            joystickfirebutton = true;
          myJoystickButtonFunc (joystickbutton);
          joystickbutton = -1;
          break;

        case SDL_JOYBUTTONUP:
          joystickbutton = event.jbutton.button + event.jbutton.which * 1000;
          if (joystickbutton == (int) joystick_firecannon)
            joystickfirebutton = false;
          joystickbutton = -1;
          break;

        case SDL_JOYHATMOTION:
          myJoystickHatFunc (event.jhat.value + event.jhat.which * 1000);
          break;

        case SDL_ACTIVEEVENT:
          sdlreshape = true;
          sdldisplay = true;
          break;
      }
    }
    
    if (controls == CONTROLS_JOYSTICK)
    {
      int x = jaxis [getJoystickAxisIndex (joystick_aileron)];
      int y = jaxis [getJoystickAxisIndex (joystick_elevator)];
      int rudder = jaxis [getJoystickAxisIndex (joystick_rudder)];
      int throttle = jaxis [getJoystickAxisIndex (joystick_throttle)];
      myJoystickAxisFunc (x, y, rudder, throttle);
      view_x = (float) jaxis [getJoystickAxisIndex (joystick_view_x)]/(-328.0);
      view_y = (float) jaxis [getJoystickAxisIndex (joystick_view_y)]/(-328.0);
      if (joystickfirebutton)
        myJoystickButtonFunc (joystick_firecannon);
    }
    
    if (sdldisplay) myDisplayFunc ();
    sdldisplay = false;
    if (sdlreshape) myReshapeFunc (width, height);
    sdlreshape = false;
    myTimerFunc (1); // dummy value
  }
}
#endif



/****************************************************************************
  STARTUP METHODS
****************************************************************************/

// common GLUT screen init code, return 0 on error
int setGlutScreen (int w, int h, int b, int f)
{
  char gamestr [256];
  sprintf (gamestr, "%dx%d:%d", w, h, b);
  glutGameModeString (gamestr);
  if (f)
  {
    if (glutGameModeGet (GLUT_GAME_MODE_POSSIBLE))
    {
      glutEnterGameMode ();
    }
    else
    {
      return 0;
    }
  }
  else
  {
    glutInitWindowPosition (0, 0);
    glutInitWindowSize (w, h);
    glutwindow = glutCreateWindow ("GL-117");
    if (glutwindow == GL_FALSE)
      return 0;
  }
  return 1;
}

// set screen to (width, height, bpp, fullscreen), return 0 on error
int setScreen (int w, int h, int b, int f)
{

#ifdef USE_GLUT

  if (!setGlutScreen (w, h, b, f))
  {
    b = 16;
    if (!setGlutScreen (w, h, b, f))
    {
      b = 8;
      if (!setGlutScreen (w, h, b, f))
      {
        b = 2;
        if (!setGlutScreen (w, h, b, f))
        {
          return 0;
        }
      }
    }
  }

#else

  Uint32 video_flags;
  if (f)
  {
    video_flags = SDL_OPENGL | SDL_FULLSCREEN;
  }
  else
  {
    video_flags = SDL_OPENGL;
  }
  int rgb_size [3];
  switch (b)
  {
    case 8:
      rgb_size [0] = 2;
      rgb_size [1] = 3;
      rgb_size [2] = 3;
      break;
    case 15:
    case 16:
      rgb_size [0] = 5;
      rgb_size [1] = 5;
      rgb_size [2] = 5;
      break;
    default:
      rgb_size [0] = 8;
      rgb_size [1] = 8;
      rgb_size [2] = 8;
      break;
  }
  SDL_GL_SetAttribute (SDL_GL_RED_SIZE, rgb_size [0]);
  SDL_GL_SetAttribute (SDL_GL_GREEN_SIZE, rgb_size [1]);
  SDL_GL_SetAttribute (SDL_GL_BLUE_SIZE, rgb_size [2]);
  SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
  if (SDL_SetVideoMode (w, h, b, video_flags) == NULL)
  {
    if ((b = SDL_VideoModeOK (w, h, b, video_flags)) != 0)
    {
      b = 16;
      SDL_GL_SetAttribute (SDL_GL_RED_SIZE, 5);
      SDL_GL_SetAttribute (SDL_GL_GREEN_SIZE, 5);
      SDL_GL_SetAttribute (SDL_GL_BLUE_SIZE, 5);
      if (SDL_SetVideoMode (w, h, b, video_flags) == NULL)
      {
        b = 8;
        SDL_GL_SetAttribute (SDL_GL_RED_SIZE, 2);
        SDL_GL_SetAttribute (SDL_GL_GREEN_SIZE, 3);
        SDL_GL_SetAttribute (SDL_GL_BLUE_SIZE, 3);
        if (SDL_SetVideoMode (w, h, b, video_flags) == NULL)
        {
          return 0;
        }
      }
    }
  }

#endif

  glViewport (0, 0, (GLint) w, (GLint) h);

  // take over results in global variables
  width = w;
  height = h;
  bpp = b;
  fullscreen = f;
  wantwidth = w; // requested values for next restart
  wantheight = h;
  wantfullscreen = f;
  return 1;
}

// get approximate speed by drawing polygons (obsolete)
int speedTest ()
{
  int frames = 0, time2 = 0, time1 = 0;

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective (visibleangle, 1.0, nearclippingplane, 20.0); // should be sqrt(2) or 1.5
  glPolygonMode (GL_FRONT_AND_BACK, polygonMode);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();

  #ifndef USE_GLUT
    time1 = SDL_GetTicks ();
  #else
    time1 = glutGet (GLUT_ELAPSED_TIME);
  #endif

  while (time2 - time1 < 1000)
  {
    frames ++;
  #ifndef USE_GLUT
    time2 = SDL_GetTicks ();
  #else
    time2 = glutGet (GLUT_ELAPSED_TIME);
  #endif

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glRotatef (1, 0, 0, 1);
    for (float zf = -13; zf <= -3; zf += 0.05)
    {
      glBegin (GL_QUADS);
      glColor4d (255, 0, 0, 255);
      glVertex3f (-1, -1, zf);
      glVertex3f (-1, 1, zf);
      glVertex3f (1, 1, zf);
      glVertex3f (1, -1, zf);
      glEnd ();
    }

#ifdef USE_GLUT
  glutSwapBuffers();
#else
  SDL_GL_SwapBuffers ();
#endif

  }
  return frames;
}

bool configinit = false; // has GLUT/SDL already been inited?

// test screen settings automatically
void config_test (int argc, char **argv)
{
  display ("No configuration file found. Testing...", LOG_MOST);
  int bppi [4];

#ifdef USE_GLUT // GLUT ONLY
  display ("Using GLUT only", LOG_MOST);
  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);
  configinit = true;
#else // SDL
  char buf [STDSIZE];
  display ("Using SDL and GLUT", LOG_MOST);
  if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0)
  {
    sprintf (buf, "Couldn't initialize SDL: %s", SDL_GetError ());
    display (buf, LOG_FATAL);
    exit (EXIT_INIT);
  }
  configinit = true;
#endif

  int valids = -1; // valid screen mode? (-1 = no mode)
  int n = 0;
  while (n < 4)
  {
    if (setScreen (resolution [n] [0], resolution [n] [1], resolution [n] [2], resolution [n] [3]))
    {
      bppi [n] = bpp; // store bpp setting
      valids = n;
      break; // first mode found => exit loop
    }
    else
    {
      bppi [n] = -1;
    }
    n ++;
  }

  if (valids == -1)
  {
    display ("No working display modes found! Try editing the file conf yourself. You may not be able to play this game.", LOG_FATAL);
    exit (EXIT_INIT);
  }

  // start with lowest quality/view settings
  quality = 0;
  view = 20;
  width = resolution [valids] [0];
  height = resolution [valids] [1];
  bpp = bppi [valids];
  fullscreen = resolution [valids] [3];
  wantwidth = width; // requested values for next restart
  wantheight = height;
  wantfullscreen = fullscreen;
}

// get startup help screen
void viewParameters ()
{
  display (" ", LOG_NONE);
  display ("Usage: gl-117 [-h -v -dLEVEL]", LOG_NONE);
  display (" ", LOG_NONE);
  display ("-h: Display this help screen and quit", LOG_NONE);
  display ("-v: Display version string and quit", LOG_NONE);
  display ("-dLEVEL: Set debug LEVEL to 0=silent...5=log all", LOG_NONE);
  display (" ", LOG_NONE);
}

void checkargs (int argc, char **argv)
{
  char buf [STDSIZE]; // temp buffer
  int i; // temp counter

  for (i = 1; i < argc; i ++) // for each arg (argument/parameter)
  {
    if (argv [i] [1] == 'd') // change log/debug level
    {
      char *ptr = &argv [i] [2];
      debuglevel = atoi (ptr);
      if (debuglevel < LOG_NONE || debuglevel > LOG_ALL) // look at common.h for the constants
      {
        display ("Invalid debug level", LOG_FATAL);
        viewParameters ();
        exit (EXIT_COMMAND);
      }
      else
      {
        sprintf (buf, "Entering debug level %d", debuglevel);
        display (buf, LOG_MOST);
      }
    }
    else if (argv [i] [1] == 'v') // display version string
    {
      display (VERSIONSTRING, LOG_NONE);
      exit (EXIT_NORMAL);
    }
    else if (argv [i] [1] == 'h') // display startup help screen
    {
      viewParameters ();
      exit (EXIT_NORMAL);
    }
    else
    {
      display ("Invalid command line parameter", LOG_FATAL);
      viewParameters ();
      exit (EXIT_COMMAND);
    }
  }
}

void textMouseButton (char *buf, int button)
{
  if (button == 0) strcpy (buf, "NONE");
  else sprintf (buf, "BUTTON%d", button);
}

int campaignstartid;
int trainingstartid;

void callbackReturn (Component *comp, int key)
{
  switch_game ();
}

void callbackStats (Component *comp, int key)
{
  stats_key (27, 0, 0);
}

void callbackQuitNow (Component *comp, int key)
{
  game_quit ();
}

void callbackSwitchMainMenu (Component *comp, int key)
{
  switch_menu ();
}

void callbackQuit (Component *comp, int key)
{
  switch_quit ();
}

void callbackCredits (Component *comp, int key)
{
  switch_credits ();
}

void callbackPilots (Component *comp, int key)
{
  switch_fame ();
}

int currentaxis = 0;

#ifndef USE_GLUT
void callbackJoystickAxis (Component *comp, int key)
{
  int i;
  int buttonnum = -1;

  // get clicked button's number
  for (i = 0; i < 12; i ++)
  {
    if (comp == controlsmenu [2]->components [i])
    {
      buttonnum = i;
    }
  }
  if (buttonnum == -1) return;

  // read text from label of button
  Label *la = (Label *) controlsmenu [2]->components [buttonnum + 1];
  
  int joynum = 0;
  int joyaxis = 0;
  if (la->text [1] == '/') joynum = -1;
  else
  {
    joynum = la->text [0] - 'A';
    joyaxis = la->text [2] - '0';
  }

  if (key == MOUSE_BUTTON_LEFT)
  {
    if (joynum == -1)
    {
      joynum = 0;
      joyaxis = 0;
    }
    else if (joynum >= joysticks) joynum = -1;
    else
    {
      joyaxis ++;
      if (joyaxis >= sdljoystickaxes [joynum])
      {
        joyaxis = 0;
        joynum ++;
        if (joynum >= joysticks) joynum = -1;
      }
    }
  }
  else
  {
    if (joynum == -1)
    {
      joynum = joysticks - 1;
      joyaxis = sdljoystickaxes [joynum] - 1;
    }
    else if (joynum >= joysticks) joynum = -1;
    else
    {
      joyaxis --;
      if (joyaxis < 0)
      {
        joynum --;
        if (joynum < 0)
        {
          joynum = -1;
          joyaxis = 0;
        }
        else
          joyaxis = sdljoystickaxes [joynum] - 1;
      }
    }
  }

  int *joysetting = NULL;
  if (buttonnum == 0) joysetting = &joystick_aileron;
  else if (buttonnum == 2) joysetting = &joystick_elevator;
  else if (buttonnum == 4) joysetting = &joystick_throttle;
  else if (buttonnum == 6) joysetting = &joystick_rudder;
  else if (buttonnum == 8) joysetting = &joystick_view_x;
  else if (buttonnum == 10) joysetting = &joystick_view_y;

  *joysetting = joynum * 1000 + joyaxis;
  char latext [4];
  if (joynum >= 0)
  {
    latext [0] = 'A' + joynum;
    latext [1] = 32;
    latext [2] = '0' + joyaxis;
    latext [3] = 0;
    la->setText (latext);
  }
  else
  {
    la->setText ("N/A");
  }
}
#endif

void callbackSwitchStartMission (Component *comp, int key)
{
  pleaseWait ();
  if (!game_levelInit ())
  {
    switch_menu ();
    return;
  }
  switch_game ();
  missionactive = true;
}

void callbackFighterPrev (Component *comp, int key)
{
  int maxfighter = 6;
  Pilot *p = pilots->pilot [pilots->aktpilot];
  if (p->mission_state [MISSION_DEPOT] == 1) maxfighter ++;
  if (p->mission_state [MISSION_SHIP1] == 1) maxfighter ++;
  if (p->mission_state [MISSION_CANYON3] == 1) maxfighter ++;
  if (p->mission_state [MISSION_MOON1] == 1) maxfighter ++;
  if (key == MOUSE_BUTTON_LEFT)
  {
    aktfighter --;
    if (aktfighter < 0) aktfighter = maxfighter - 1;
  }
}

void callbackFighterNext (Component *comp, int key)
{
  int maxfighter = 6;
  Pilot *p = pilots->pilot [pilots->aktpilot];
  if (p->mission_state [MISSION_DEPOT] == 1) maxfighter ++;
  if (p->mission_state [MISSION_SHIP1] == 1) maxfighter ++;
  if (p->mission_state [MISSION_CANYON3] == 1) maxfighter ++;
  if (p->mission_state [MISSION_MOON1] == 1) maxfighter ++;
  if (key == MOUSE_BUTTON_LEFT)
  {
    aktfighter ++;
    if (aktfighter >= maxfighter) aktfighter = 0;
  }
}

void callbackFighter (Component *comp, int key)
{
  switch_fighter ();
}

void setpilotstext ()
{
  char buf [64];
  int i;
  sprintf (buf, "     %s %s", pilots->pilot [pilots->aktpilot]->getShortRank (), pilots->pilot [pilots->aktpilot]->name);
  ((Label *) submenu [0]->components [1])->setText (buf);
  for (i = 0; i < pilots->aktpilots; i ++)
  {
    sprintf (buf, "     %s %s", pilots->pilot [i]->getShortRank (), pilots->pilot [i]->name);
    ((Label *) submenu [0]->components [i + 2])->setText (buf);
  }
  for (; i < 5; i ++)
  {
    ((Label *) submenu [0]->components [i + 2])->setText ("N/A");
  }
}

void callbackPilotsAdd (Component *comp, int key)
{
  char buf [64];
  if (key == MOUSE_BUTTON_LEFT)
  {
    strcpy (buf, ((EditField *) submenu [0]->components [9])->text);
    if (strlen (buf) > 0 && pilots->aktpilots < 5)
    {
      pilots->add (buf);
      ((EditField *) submenu [0]->components [9])->setText ("");
      setpilotstext ();
    }
  }
}

void callbackPilotsDelete (Component *comp, int key)
{
  if (key == MOUSE_BUTTON_RIGHT)
  {
    pilots->rm ();
    setpilotstext ();
  }
}

void callbackPilotsList (Component *comp, int key)
{
  int i;
  for (i = 0; i < 5; i ++)
  {
    if (comp == submenu [0]->components [i + 2])
    {
      if (i < pilots->aktpilots)
      {
        pilots->aktpilot = i;
        setpilotstext ();
      }
    }
  }
}

void callbackJoystick (Component *comp, int key)
{
  int i;
  for (i = 12; i < 23; i ++)
    if (comp == controlsmenu [0]->components [i])
    {
      int z = 12;
      if (i == z ++) joystick_firecannon = key;
      else if (i == z ++) joystick_firemissile = key;
      else if (i == z ++) joystick_dropflare = key;
      else if (i == z ++) joystick_dropchaff = key;
      else if (i == z ++) joystick_selectmissile = key;
      else if (i == z ++) joystick_targetnearest = key;
      else if (i == z ++) joystick_targetlocking = key;
      else if (i == z ++) joystick_targetnext = key;
      else if (i == z ++) joystick_targetprevious = key;
      else if (i == z ++) joystick_thrustup = key;
      else if (i == z ++) joystick_thrustdown = key;
    }
}

void callbackMouseDefaults (Component *comp, int key)
{
  char buf [256];
  mouse_sensitivity = 100;
  mouse_reverse = false;
  mouse_relative = false;
  mouse_autorudder = 30;

  sprintf (buf, "%d%%", mouse_autorudder);
  ((Label *) controlsmenu [1]->components [7])->setText (buf);
  if (mouse_relative) sprintf (buf, "ON");
  else sprintf (buf, "OFF");
  ((Label *) controlsmenu [1]->components [5])->setText (buf);
  if (mouse_reverse) sprintf (buf, "ON");
  else sprintf (buf, "OFF");
  ((Label *) controlsmenu [1]->components [3])->setText (buf);
  sprintf (buf, "%d%%", mouse_sensitivity);
  ((Label *) controlsmenu [1]->components [1])->setText (buf);
}

void callbackMouseAutorudder (Component *comp, int key)
{
  char buf [256];
  if (key == MOUSE_BUTTON_LEFT)
  {
    mouse_autorudder += 10;
    if (mouse_autorudder > 100) mouse_autorudder = 0;
  }
  else
  {
    mouse_autorudder -= 10;
    if (mouse_autorudder < 0) mouse_autorudder = 100;
  }
  sprintf (buf, "%d%%", mouse_autorudder);
  ((Label *) controlsmenu [1]->components [7])->setText (buf);
}

void callbackMouseRelative (Component *comp, int key)
{
  char buf [256];
  mouse_relative = !mouse_relative;
  if (mouse_relative) sprintf (buf, "ON");
  else sprintf (buf, "OFF");
  ((Label *) controlsmenu [1]->components [5])->setText (buf);
}

void callbackMouseReverse (Component *comp, int key)
{
  char buf [256];
  mouse_reverse = !mouse_reverse;
  if (mouse_reverse) sprintf (buf, "ON");
  else sprintf (buf, "OFF");
  ((Label *) controlsmenu [1]->components [3])->setText (buf);
}

void callbackMouseSensitivity (Component *comp, int key)
{
  char buf [256];
  if (key == MOUSE_BUTTON_LEFT)
  {
    mouse_sensitivity += 10;
    if (mouse_sensitivity > 200) mouse_sensitivity = 70;
  }
  else
  {
    mouse_sensitivity -= 10;
    if (mouse_sensitivity < 70) mouse_sensitivity = 200;
  }
  sprintf (buf, "%d%%", mouse_sensitivity);
  ((Label *) controlsmenu [1]->components [1])->setText (buf);
}

void callbackDefaultsJoystick (Component *comp, int key)
{
  joystick_aileron = 0;
  joystick_elevator = 1;
  joystick_throttle = 2;
  joystick_rudder = 3;
  joystick_view_x = 4;
  joystick_view_y = 5;

  int z = 1;
  ((Label *) controlsmenu [2]->components [z])->setText ("A 0");
  z += 2;
  ((Label *) controlsmenu [2]->components [z])->setText ("A 1");
  z += 2;
  ((Label *) controlsmenu [2]->components [z])->setText ("A 2");
  z += 2;
  ((Label *) controlsmenu [2]->components [z])->setText ("A 3");
  z += 2;
  ((Label *) controlsmenu [2]->components [z])->setText ("A 4");
  z += 2;
  ((Label *) controlsmenu [2]->components [z])->setText ("A 5");

  joystick_firecannon = 0;
  joystick_firemissile = 2;
  joystick_dropflare = 3;
  joystick_dropchaff = 3;
  joystick_selectmissile = 1;
  joystick_targetnearest = 101;
  joystick_targetlocking = 103;
  joystick_targetnext = 100;
  joystick_targetprevious = 102;
  joystick_thrustup = 4;
  joystick_thrustdown = 5;

  z = 12;
  ((EditJoystick *) controlsmenu [2]->components [z ++])->setButton (joystick_firecannon);
  ((EditJoystick *) controlsmenu [2]->components [z ++])->setButton (joystick_firemissile);
  ((EditJoystick *) controlsmenu [2]->components [z ++])->setButton (joystick_dropflare);
  ((EditJoystick *) controlsmenu [2]->components [z ++])->setButton (joystick_dropchaff);
  ((EditJoystick *) controlsmenu [2]->components [z ++])->setButton (joystick_selectmissile);
  ((EditJoystick *) controlsmenu [2]->components [z ++])->setButton (joystick_targetnearest);
  ((EditJoystick *) controlsmenu [2]->components [z ++])->setButton (joystick_targetlocking);
  ((EditJoystick *) controlsmenu [2]->components [z ++])->setButton (joystick_targetnext);
  ((EditJoystick *) controlsmenu [2]->components [z ++])->setButton (joystick_targetprevious);
  ((EditJoystick *) controlsmenu [2]->components [z ++])->setButton (joystick_thrustup);
  ((EditJoystick *) controlsmenu [2]->components [z ++])->setButton (joystick_thrustdown);
}

void callbackDefaults (Component *comp, int key)
{
  key_firecannon = 32;
  key_firemissile = 13;
  key_dropflare = 'F';
  key_dropchaff = 'C';
  key_selectmissile = 'M';
  key_targetnearest = 'E';
  key_targetlocking = 'L';
  key_targetnext = 'N';
  key_targetprevious = 'P';
  key_thrustup = 'S';
  key_thrustdown = 'X';

  int z = 5;
  ((EditKey *) controlsmenu [0]->components [z ++])->setKey (key_firecannon);
  ((EditKey *) controlsmenu [0]->components [z ++])->setKey (key_firemissile);
  ((EditKey *) controlsmenu [0]->components [z ++])->setKey (key_dropflare);
  ((EditKey *) controlsmenu [0]->components [z ++])->setKey (key_dropchaff);
  ((EditKey *) controlsmenu [0]->components [z ++])->setKey (key_selectmissile);
  ((EditKey *) controlsmenu [0]->components [z ++])->setKey (key_targetnearest);
  ((EditKey *) controlsmenu [0]->components [z ++])->setKey (key_targetlocking);
  ((EditKey *) controlsmenu [0]->components [z ++])->setKey (key_targetnext);
  ((EditKey *) controlsmenu [0]->components [z ++])->setKey (key_targetprevious);
  ((EditKey *) controlsmenu [0]->components [z ++])->setKey (key_thrustup);
  ((EditKey *) controlsmenu [0]->components [z ++])->setKey (key_thrustdown);
}

void callbackKeyboard (Component *comp, int key)
{
  int i;
  for (i = 5; i < 16; i ++)
    if (comp == controlsmenu [0]->components [i])
    {
      int z = 5;
      if (i == z ++) key_firecannon = key;
      else if (i == z ++) key_firemissile = key;
      else if (i == z ++) key_dropflare = key;
      else if (i == z ++) key_dropchaff = key;
      else if (i == z ++) key_selectmissile = key;
      else if (i == z ++) key_targetnearest = key;
      else if (i == z ++) key_targetlocking = key;
      else if (i == z ++) key_targetnext = key;
      else if (i == z ++) key_targetprevious = key;
      else if (i == z ++) key_thrustup = key;
      else if (i == z ++) key_thrustdown = key;
    }
}

void textControls (char *buf)
{
  if (controls == CONTROLS_KEYBOARD) sprintf (buf, "%s", "KEYBOARD");
  else if (controls == CONTROLS_MOUSE) sprintf (buf, "%s", "MOUSE");
  else if (controls == CONTROLS_JOYSTICK) sprintf (buf, "%s", "JOYSTICK");
}

void callbackControls (Component *comp, int key)
{
  char buf [256];
  if (key == MOUSE_BUTTON_LEFT)
  {
    fplayer->rolleffect = 0;
    fplayer->ruddereffect = 0;
    fplayer->elevatoreffect = 0;
    keyb_elev = 0;
    keyb_roll = 0;
    keyb_rudder = 0;
    controls ++;
    if (controls > 2) controls = 0;
    if (controls == CONTROLS_JOYSTICK && !joysticks) controls = CONTROLS_KEYBOARD;
#ifdef USE_GLUT
    if (controls == CONTROLS_KEYBOARD) controls = CONTROLS_MOUSE;
#endif
  }
  textControls (buf);
  ((Label *) optmenu [2]->components [5])->setText (buf);
  allmenus.components [11]->setVisible (false);
  allmenus.components [12]->setVisible (false);
  allmenus.components [13]->setVisible (false);
  if (controls == CONTROLS_KEYBOARD) allmenus.components [11]->setVisible (true);
  else if (controls == CONTROLS_JOYSTICK) allmenus.components [13]->setVisible (true);
  else allmenus.components [12]->setVisible (true);
}

void callbackPhysics (Component *comp, int key)
{
  char buf [256];
  physics = !physics;
  if (!physics) sprintf (buf, "%s", "ACTION");
  else sprintf (buf, "%s", "SIMULATION");
  ((Label *) optmenu [2]->components [3])->setText (buf);
}

void callbackDifficulty (Component *comp, int key)
{
  char buf [256];
  if (key == MOUSE_BUTTON_LEFT)
  {
    difficulty ++;
    if (difficulty > 2) difficulty = 0;
  }
  else
  {
    difficulty --;
    if (difficulty < 0) difficulty = 2;
  }
  if (difficulty == 0) sprintf (buf, "%s", "EASY");
  else if (difficulty == 1) sprintf (buf, "%s", "NORMAL");
  else if (difficulty == 2) sprintf (buf, "%s", "HARD");
  ((Label *) optmenu [2]->components [1])->setText (buf);
}

void callbackSound (Component *comp, int key)
{
  char buf [256];
  if (sound->audio)
  {
    if (key == MOUSE_BUTTON_LEFT)
    {
      sound->volumesound += 10;
      if (sound->volumesound > 100)
        sound->volumesound = 0;
      volumesound = sound->volumesound;
      sound->setVolume ();
      setPlaneVolume ();
      sound->play (SOUND_CLICK1, false);
      menu_reshape ();
    }
    else
    {
      sound->volumesound -= 10;
      if (sound->volumesound < 0)
        sound->volumesound = 100;
      volumesound = sound->volumesound;
      sound->setVolume ();
      setPlaneVolume ();
      sound->play (SOUND_CLICK1, false);
      menu_reshape ();
    }
    sprintf (buf, "%d%%", volumesound);
    ((Label *) optmenu [1]->components [1])->setText (buf);
  }
}

void callbackMusic (Component *comp, int key)
{
  char buf [256];
  if (sound->audio)
  {
    if (key == MOUSE_BUTTON_LEFT)
    {
      sound->volumemusic += 10;
      if (sound->volumemusic > 100)
      {
        sound->volumemusic = 0;
        sound->haltMusic ();
      }
      volumemusic = sound->volumemusic;
      sound->setVolumeMusic ();
      menu_reshape ();
    }
    else
    {
      sound->volumemusic -= 10;
      if (sound->volumemusic < 0)
        sound->volumemusic = 100;
      volumemusic = sound->volumemusic;
      sound->setVolumeMusic ();
      menu_reshape ();
    }
    if (sound->volumemusic != 0 && !sound->musicplaying)
      playRandomMusic ();
    sprintf (buf, "%d%%", volumemusic);
    ((Label *) optmenu [1]->components [3])->setText (buf);
  }
}

void callbackBrightness (Component *comp, int key)
{
  char buf [256];
  if (key == MOUSE_BUTTON_LEFT)
  {
    brightness += 10;
    if (brightness > 50) brightness = -50;
  }
  else
  {
    brightness -= 10;
    if (brightness < -50) brightness = 50;
  }
  sprintf (buf, "%d%%", brightness);
  ((Label *) optmenu [0]->components [13])->setText (buf);
}

void callbackResolution (Component *comp, int key)
{
  const int numres = 4;
  int resx [numres] = { 640, 800, 1024, 1280 };
  int resy [numres] = { 480, 600, 800, 1024 };
  int found = 0;
  char buf [256];

  if (key == MOUSE_BUTTON_LEFT)
  {
    for (int i = 0; i < numres; i ++)
      if (wantwidth == resx [i])
      {
        found = i + 1;
      }
  }
  else
  {
    for (int i = 0; i < numres; i ++)
      if (wantwidth == resx [i])
      {
        found = i - 1;
      }
  }
  
  if (found < 0) found = numres - 1;
  else if (found >= numres) found = 0;
  
  wantwidth = resx [found];
  wantheight = resy [found];
  
  sprintf (buf, "%d*%d", wantwidth, wantheight);
  ((Label *) optmenu [0]->components [16])->setText (buf);
}

void callbackFullscreen (Component *comp, int key)
{
  if (wantfullscreen) wantfullscreen = 0;
  else wantfullscreen = 1;

  if (wantfullscreen) ((Label *) optmenu [0]->components [18])->setText ("YES");
  else ((Label *) optmenu [0]->components [18])->setText ("NO");
}

void callbackDynamicLighting (Component *comp, int key)
{
  char buf [256];
  dynamiclighting = (dynamiclighting == 0 ? 1 : 0);
  if (dynamiclighting) strcpy (buf, "ON");
  else strcpy (buf, "OFF");
  ((Label *) optmenu [0]->components [11])->setText (buf);
}

void callbackSpecialEffects (Component *comp, int key)
{
  char buf [256];
  specialeffects = (specialeffects == 0 ? 1 : 0);
  if (specialeffects) strcpy (buf, "ON");
  else strcpy (buf, "OFF");
  ((Label *) optmenu [0]->components [9])->setText (buf);
}

void callbackAntialiasing (Component *comp, int key)
{
  char buf [256];
  antialiasing = (antialiasing == 0 ? 1 : 0);
  if (antialiasing) strcpy (buf, "ON");
  else strcpy (buf, "OFF");
  ((Label *) optmenu [0]->components [7])->setText (buf);
  event_setAntialiasing ();
}

void callbackDithering (Component *comp, int key)
{
  char buf [256];
  dithering = (dithering == 0 ? 1 : 0);
  if (dithering) strcpy (buf, "ON");
  else strcpy (buf, "OFF");
  ((Label *) optmenu [0]->components [5])->setText (buf);
}

void callbackView (Component *comp, int key)
{
  char buf [256];
  if (key == MOUSE_BUTTON_LEFT)
  {
    view += 10;
    if (view > VIEW_MAX) view = VIEW_MIN;
    menu_reshape ();
  }
  else
  {
    view -= 10;
    if (view < VIEW_MIN) view = VIEW_MAX;
    menu_reshape ();
  }
  sprintf (buf, "%d", (int) view);
  ((Label *) optmenu [0]->components [3])->setText (buf); // this is also set by autoconfig in menu_timer
}

void callbackQuality (Component *comp, int key)
{
  char buf [256];
  if (key == MOUSE_BUTTON_LEFT)
  {
    quality ++;
    if (quality > 5) quality = 0;
  }
  else
  {
    quality --;
    if (quality < 0) quality = 5;
  }
  sprintf (buf, "%d", quality);
  ((Label *) optmenu [0]->components [1])->setText (buf); // this is also set by autoconfig in menu_timer
}

void callbackTraining (Component *comp, int key)
{
  allmenus.setVisible (false);

  switch_mission (getTrainingIdFromValue (comp->id - trainingstartid));
}

void callbackCampaign (Component *comp, int key)
{
  allmenus.setVisible (false);
  switch_mission (getCampaignIdFromValue (comp->id - campaignstartid));
}

void callbackMainMenu (Component *comp, int key)
{
  for (i = 0; i < 6; i ++)
    if (comp == mainbutton [i])
    {
      allmenus.setVisible (false);
      allmenus.visible = true;
      allmenus.components [0]->setVisible (true);
      allmenus.components [i + 1]->setVisible (true);
      currentsubmenu = allmenus.components [i + 1];
      if (missionactive)
        mainbutton [6]->setVisible (true);
      else
        mainbutton [6]->setVisible (false);
      if (i == 3)
      {
        if (currentoptmenu) currentoptmenu->setVisible (true);
        if (currentoptmenu == allmenus.components [10])
        {
          if (controls == CONTROLS_KEYBOARD) allmenus.components [11]->setVisible (true);
          else if (controls == CONTROLS_JOYSTICK) allmenus.components [13]->setVisible (true);
          else allmenus.components [12]->setVisible (true);
        }
      }
      return;
    }
  for (i = 0; i < 3; i ++)
    if (comp == optbutton [i])
    {
      allmenus.components [8]->setVisible (false);
      allmenus.components [9]->setVisible (false);
      allmenus.components [10]->setVisible (false);
      allmenus.components [i + 1 + 7]->setVisible (true);
      if (i + 1 + 7 == 10)
      {
        if (controls == CONTROLS_KEYBOARD) allmenus.components [11]->setVisible (true);
        else if (controls == CONTROLS_JOYSTICK) allmenus.components [13]->setVisible (true);
        else allmenus.components [12]->setVisible (true);
      }
      else
      {
        allmenus.components [11]->setVisible (false);
        allmenus.components [12]->setVisible (false);
        allmenus.components [13]->setVisible (false);
      }
      currentoptmenu = allmenus.components [i + 1 + 7];
      return;
    }
}

void setJoystickAxisString (int joysetting, char *joystr)
{
  joystr [3] = 0;
  if (joysetting < 0)
  {
    strcpy (joystr, "N/A");
  }
  else
  {
    joystr [0] = 'A' + (joysetting / 1000);
    joystr [1] = 32;
    joystr [2] = '0' + (joysetting % 1000);
  }
}

char mainmenunames [10] [25];
char submenu1names [20] [25];
char submenu2names [30] [25];
char optmenunames [5] [25];
char controlsmenu0names [15] [25];

void createMenu ()
{
  char buf [256];
  char buf2 [256];
  int i;
  int z = 0;
  float xf = -2;
  float xfstep = 4;
  float yf = 12;
  float yfstep = 1.0;
  float xsubmenu = -1;
  float ysubmenu = 12;
  Button *button;
  Label *label;
  TextField *textfield;
  EditKey *editkey;
  EditField *editfield;
#ifndef USE_GLUT
  EditJoystick *editjoystick;
#endif
  
/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Register main menu
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

  strcpy (mainmenunames [0], "PILOTS");
  strcpy (mainmenunames [1], "TRAINING");
  strcpy (mainmenunames [2], "CAMPAIGN");
  strcpy (mainmenunames [3], "OPTIONS");
#ifdef SHOW_SOFTWARE_PATENTS
  strcpy (mainmenunames [4], "SOFTWARE PATENTS");
#else
  strcpy (mainmenunames [4], "CREDITS");
#endif
  strcpy (mainmenunames [5], "QUIT");
  strcpy (mainmenunames [6], "RETURN");

  mainmenu = new Container ();
//  mainmenu->setBounds (-14.3, 12.2, 10.6, 7.4);
  allmenus.add (mainmenu);

  for (i = 0; i < 7; i ++)
  {
    submenu [i] = new Container ();
    allmenus.add (submenu [i]);
  }

/*  submenu [0]->setBounds (-1.3, 12.2, 14.1, 12.4);
  submenu [1]->setBounds (-1.3, 12.2, 12.6, 7.4);
  submenu [2]->setBounds (-1.3, 12.2, 12.6, 1.4);
  submenu [3]->setBounds (-1.3, 12.2, 14.1, 1.4);*/

  for (i = 0; i < 7; i ++)
  {
    mainbutton [i] = new Button (mainmenunames [i]);
    mainbutton [i]->setBounds (-14, yf, 10, yfstep - 0.1);
    yf -= yfstep;
    mainbutton [i]->setFunction (callbackMainMenu);
    mainmenu->add (mainbutton [i]);
  }

  currentsubmenu = submenu [0];
  mainbutton [4]->setFunction (callbackCredits);
  mainbutton [5]->setFunction (callbackQuit);
  mainbutton [6]->setVisible (false);
  mainbutton [6]->setFunction (callbackReturn);

/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Register pilots submenu
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

  xf = xsubmenu; yf = ysubmenu; xfstep = 13.5; yfstep = 1;
  sprintf (buf, "ACTIVE:");
  label = new Label (buf);
  label->setBounds (xf, yf, xfstep, yfstep - 0.1);
  submenu [0]->add (label);
  yf -= yfstep;

  sprintf (buf, "     %s %s", pilots->pilot [pilots->aktpilot]->getShortRank (), pilots->pilot [pilots->aktpilot]->name);
  label = new Label (buf);
  label->setBounds (xf, yf, xfstep, yfstep - 0.1);
  submenu [0]->add (label);

  yf -= 2;
  for (i = 0; i < 5; i ++)
  {
    if (i < pilots->aktpilots)
      sprintf (buf, "     %s %s", pilots->pilot [i]->getShortRank (), pilots->pilot [i]->name);
    else
      sprintf (buf, "N/A");
    button = new Button (buf);
    button->setFunction (callbackPilotsList);
    button->setBounds (xf, yf, xfstep, yfstep - 0.1);
    submenu [0]->add (button);
    yf -= yfstep;
  }

  yf -= 0.5;
  button = new Button ("DELETE (RIGHT MB)");
  button->setFunction (callbackPilotsDelete);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  submenu [0]->add (button);
  yf -= yfstep;

  yf -= 0.5;
  button = new Button ("ADD");
  button->setFunction (callbackPilotsAdd);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  submenu [0]->add (button);
  yf -= yfstep;

  editfield = new EditField (15);
  editfield->setBounds (xf, yf, xfstep, yfstep - 0.1);
  submenu [0]->add (editfield);
  yf -= yfstep;

/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Register training submenu
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

  z = 0;
  strcpy (submenu1names [z ++], "TUTORIAL: PILOTING");
  strcpy (submenu1names [z ++], "TUTORIAL: BOMBER");
  strcpy (submenu1names [z ++], "TUTORIAL: FIGHTER");
  strcpy (submenu1names [z ++], "TUTORIAL: AERODYNAMICS");
  strcpy (submenu1names [z ++], "FREE FLIGHT");
  strcpy (submenu1names [z ++], "DEATHMATCH");
  strcpy (submenu1names [z ++], "TEAM DEATHMATCH");
  strcpy (submenu1names [z ++], "CANNON DEATHMATCH");
  strcpy (submenu1names [z ++], "TEAM BASE");
  strcpy (submenu1names [z ++], "WAVES");

  xf = xsubmenu; yf = ysubmenu - 2; xfstep = 12; yfstep = 0.8;
  for (i = 0; i < 10; i ++)
  {
    button = new Button (submenu1names [i]);
    button->setBounds (xf, yf, xfstep, yfstep - 0.1);
    button->setFunction (callbackTraining);
    yf -= yfstep;
    if (i == 3) yf -= yfstep;
    submenu [1]->add (button);
  }

  trainingstartid = submenu [1]->components [0]->id;

  yf -= yfstep;
  for (i = 0; i < maploader->getMapNumber (); i ++)
  {
    char str [256];
    strcpy (str, maploader->getNextMap ());
    maploader->toUpper (str);
    button = new Button (str);
    button->setBounds (xf, yf, xfstep, yfstep - 0.1);
    button->setFunction (callbackTraining);
    yf -= yfstep;
    submenu [1]->add (button);
  }

  xf = xsubmenu; yf = ysubmenu; yfstep = 1;
  button = new Button ("FIGHTER INFO");
  button->setFunction (callbackFighter);
  button->setBounds (xf, yf, xfstep - 0.2, yfstep - 0.1);
  submenu [1]->add (button);
  xf += xfstep;

/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Register campaign submenu
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/
 
  z = 0;
  strcpy (submenu2names [z ++], "FIRST TEST");
  strcpy (submenu2names [z ++], "SECOND TEST");
  strcpy (submenu2names [z ++], "TRANSPORT");
  strcpy (submenu2names [z ++], "CONVOY");
  strcpy (submenu2names [z ++], "DOGFIGHT");
  strcpy (submenu2names [z ++], "AIR BATTLE");
  strcpy (submenu2names [z ++], "SURFACE-AIR DEFENSE");
  strcpy (submenu2names [z ++], "VETERAN DOGFIGHT");
  strcpy (submenu2names [z ++], "BASE ATTACK");
  strcpy (submenu2names [z ++], "DEPOTS");
  strcpy (submenu2names [z ++], "DEFEND SAM");
  strcpy (submenu2names [z ++], "DESERT DOGFIGHT");
  strcpy (submenu2names [z ++], "TANK ASSAUT");
  strcpy (submenu2names [z ++], "SAM CONVOY");
  strcpy (submenu2names [z ++], "DESTROYERS");
  strcpy (submenu2names [z ++], "OILRIG");
  strcpy (submenu2names [z ++], "CRUISER");
  strcpy (submenu2names [z ++], "RADAR BASE");
  strcpy (submenu2names [z ++], "CANYON BATTLE");
  strcpy (submenu2names [z ++], "TUNNEL");
  strcpy (submenu2names [z ++], "MAIN BASE");
  strcpy (submenu2names [z ++], "TURRETS");
  strcpy (submenu2names [z ++], "MOON BATTLE");
  strcpy (submenu2names [z ++], "ELITE DOGFIGHT");
  strcpy (submenu2names [z ++], "SNEAKING");

  xf = xsubmenu; yf = ysubmenu - 2; xfstep = 12; yfstep = 0.8;
  for (i = 0; i < z; i ++)
  {
    button = new Button (submenu2names [i]);
    button->setFunction (callbackCampaign);
    button->setBounds (xf, yf, xfstep, yfstep - 0.1);
    yf -= yfstep;
    submenu [2]->add (button);
  }

  campaignstartid = submenu [2]->components [0]->id;

  xf = xsubmenu; yf = ysubmenu; yfstep = 1;
  button = new Button ("PILOTS RANKING");
  button->setFunction (callbackPilots);
  button->setBounds (xf, yf, xfstep - 0.2, yfstep - 0.1);
  submenu [2]->add (button);
  xf += xfstep;

/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Register options submenu
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

  xf = xsubmenu; yf = 12; xfstep = 4.6;
  strcpy (optmenunames [0], "VIDEO");
  strcpy (optmenunames [1], "AUDIO");
  strcpy (optmenunames [2], "GAME");

  for (i = 0; i < 3; i ++)
  {
    optmenu [i] = new Container ();
    allmenus.add (optmenu [i]);
  }

  for (i = 0; i < 3; i ++)
  {
    optbutton [i] = new Button (optmenunames [i]);
    optbutton [i]->setBounds (xf, yf, xfstep - 0.2, 1);
    xf += xfstep;
    optbutton [i]->setFunction (callbackMainMenu);
    submenu [3]->add (optbutton [i]);
  }

  currentoptmenu = optmenu [0];

/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Register video options submenu
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

  float xftab = 3;
  xf = xsubmenu; yf = 10; xfstep = 13.6; yfstep = 1;
  button = new Button ("QUALITY");
  button->setFunction (callbackQuality);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  optmenu [0]->add (button);

  sprintf (buf, "%d", quality);
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab, yf, 2, yfstep - 0.1);
  optmenu [0]->add (label);
  yf -= yfstep;

  button = new Button ("VIEW");
  button->setFunction (callbackView);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  optmenu [0]->add (button);

  sprintf (buf, "%d", (int) view);
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab, yf, 2, yfstep - 0.1);
  optmenu [0]->add (label);
  yf -= yfstep;

  button = new Button ("DITHERING");
  button->setFunction (callbackDithering);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  optmenu [0]->add (button);

  if (dithering) strcpy (buf, "ON");
  else strcpy (buf, "OFF");
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab, yf, 2, yfstep - 0.1);
  optmenu [0]->add (label);
  yf -= yfstep;

  button = new Button ("ANTIALIASING");
  button->setFunction (callbackAntialiasing);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  optmenu [0]->add (button);

  if (antialiasing) strcpy (buf, "ON");
  else strcpy (buf, "OFF");
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab, yf, 2, yfstep - 0.1);
  optmenu [0]->add (label);
  yf -= yfstep;

  button = new Button ("SPECIAL EFFECTS");
  button->setFunction (callbackSpecialEffects);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  optmenu [0]->add (button);

  if (specialeffects) strcpy (buf, "ON");
  else strcpy (buf, "OFF");
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab, yf, 2, yfstep - 0.1);
  optmenu [0]->add (label);
  yf -= yfstep;

  button = new Button ("DYN LIGHTING");
  button->setFunction (callbackDynamicLighting);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  optmenu [0]->add (button);

  if (dynamiclighting) strcpy (buf, "ON");
  else strcpy (buf, "OFF");
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab, yf, 2, yfstep - 0.1);
  optmenu [0]->add (label);
  yf -= yfstep;

  button = new Button ("BRIGHTNESS");
  button->setFunction (callbackBrightness);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  optmenu [0]->add (button);

  sprintf (buf, "%d%%", brightness);
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab, yf, 2, yfstep - 0.1);
  optmenu [0]->add (label);
  yf -= yfstep;

  yf -= yfstep / 2;
  sprintf (buf, "NEED RESTART:");
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf, yf, 2, yfstep - 0.3);
  optmenu [0]->add (label);
  yf -= yfstep;

  button = new Button ("RESOLUTION");
  button->setFunction (callbackResolution);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  optmenu [0]->add (button);

  sprintf (buf, "%d*%d", wantwidth, wantheight);
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab * 2, yf, 2, yfstep - 0.1);
  optmenu [0]->add (label);
  yf -= yfstep;

  button = new Button ("FULLSCREEN");
  button->setFunction (callbackFullscreen);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  optmenu [0]->add (button);

  if (wantfullscreen) sprintf (buf, "YES");
  else sprintf (buf, "NO");
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab, yf, 2, yfstep - 0.1);
  optmenu [0]->add (label);
  yf -= yfstep;

/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Register sound options submenu
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

  yf = 10; xfstep = 13.6; yfstep = 1;
#ifdef USE_GLUT
  textfield = new TextField ("PLEASE INSTALL SDL AND\nSDLMIXER AND RECOMIPLE\nGL-117 TO ENABLE SOUND\nAND MUSIC", 0.6);
  textfield->setBounds (xf, yf, xfstep, 0.6 * 4);
  optmenu [1]->add (textfield);
  yf -= 0.6 * 4;
#else
  button = new Button ("SOUND VOLUME");
  button->setFunction (callbackSound);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  optmenu [1]->add (button);

  if (sound->audio) sprintf (buf, "%d%%", volumesound);
  else sprintf (buf, "N/A");
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab, yf, 2, yfstep - 0.1);
  optmenu [1]->add (label);
  yf -= yfstep;

  button = new Button ("MUSIC VOLUME");
  button->setFunction (callbackMusic);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  optmenu [1]->add (button);

  if (sound->audio) sprintf (buf, "%d%%", volumemusic);
  else sprintf (buf, "N/A");
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab, yf, 2, yfstep - 0.1);
  optmenu [1]->add (label);
  yf -= yfstep;

  if (!sound->audio)
  {
    optmenu [1]->components [0]->setActive (false);
    optmenu [1]->components [1]->setActive (false);
    optmenu [1]->components [2]->setActive (false);
    optmenu [1]->components [3]->setActive (false);
  }
#endif

/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Register controls options submenu
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

  xftab = 6.8;
  xf = xsubmenu; yf = 10; xfstep = 13.5; yfstep = 1;
  button = new Button ("DIFFICULTY");
  button->setFunction (callbackDifficulty);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  optmenu [2]->add (button);

  if (difficulty == 0) sprintf (buf, "%s", "EASY");
  else if (difficulty == 1) sprintf (buf, "%s", "NORMAL");
  else if (difficulty == 2) sprintf (buf, "%s", "HARD");
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab, yf, 2, yfstep - 0.1);
  optmenu [2]->add (label);
  yf -= yfstep;

  button = new Button ("PHYSICS");
  button->setFunction (callbackPhysics);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  optmenu [2]->add (button);

  if (!physics) sprintf (buf, "%s", "ACTION");
  else sprintf (buf, "%s", "SIMULATION");
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab, yf, 2, yfstep - 0.1);
  optmenu [2]->add (label);
  yf -= yfstep;

  button = new Button ("CONTROLS");
  button->setFunction (callbackControls);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  optmenu [2]->add (button);

  textControls (buf);
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab, yf, 2, yfstep - 0.1);
  optmenu [2]->add (label);
  yf -= yfstep;

/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Register keyboard control submenu
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

  xf = xsubmenu + 1; yf = ysubmenu - 6; xfstep = 12.5; yfstep = 0.8;
  for (i = 0; i < 3; i ++)
  {
    controlsmenu [i] = new Container ();
    allmenus.add (controlsmenu [i]);
  }

  label = new Label ("ROLL:       \t   LEFT, RIGHT");
  label->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [0]->add (label);
  yf -= yfstep;

  label = new Label ("ELEVATOR: \t   UP, DOWN");
  label->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [0]->add (label);
  yf -= yfstep;

  label = new Label ("RUDDER:  \t   LSHIFT+ROLL");
  label->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [0]->add (label);
  yf -= yfstep;

  label = new Label ("THROTTLE: \t   1...9");
  label->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [0]->add (label);
  yf -= yfstep;

  label = new Label ("CAMERA:  \t   F1...F8");
  label->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [0]->add (label);
  yf -= yfstep;

  z = 0;
  strcpy (controlsmenu0names [z ++], "FIRE CANNON");
  strcpy (controlsmenu0names [z ++], "FIRE MISSILE");
  strcpy (controlsmenu0names [z ++], "DROP FLARE");
  strcpy (controlsmenu0names [z ++], "DROP CHAFF");
  strcpy (controlsmenu0names [z ++], "SELECT MISSILE");
  strcpy (controlsmenu0names [z ++], "TARGET NEAREST");
  strcpy (controlsmenu0names [z ++], "TARGET LOCKING");
  strcpy (controlsmenu0names [z ++], "TARGET NEXT");
  strcpy (controlsmenu0names [z ++], "TARGET PREVIOUS");
  strcpy (controlsmenu0names [z ++], "INC THRUST");
  strcpy (controlsmenu0names [z ++], "DEC THRUST");

  int keys [11];
  keys [0] = key_firecannon;
  keys [1] = key_firemissile;
  keys [2] = key_dropflare;
  keys [3] = key_dropchaff;
  keys [4] = key_selectmissile;
  keys [5] = key_targetnearest;
  keys [6] = key_targetlocking;
  keys [7] = key_targetnext;
  keys [8] = key_targetprevious;
  keys [9] = key_thrustup;
  keys [10] = key_thrustdown;

  yf -= 0.5;
  for (i = 0; i < 11; i ++)
  {
    editkey = new EditKey (controlsmenu0names [i]);
    editkey->setKey (keys [i]);
    editkey->setFunction (callbackKeyboard);
    editkey->setBounds (xf, yf, xfstep, yfstep - 0.1);
    controlsmenu [0]->add (editkey);
    yf -= yfstep;
  }

  yf -= 1; yfstep = 1.0;
  button = new Button ("LOAD DEFAULTS");
  button->setFunction (callbackDefaults);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [0]->add (button);

/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Register mouse control submenu
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

  xftab = 3.5;
  xf = xsubmenu + 1; yf = ysubmenu - 6; xfstep = 12.5; yfstep = 0.8;
  button = new Button ("SENSITIVITY");
  button->setFunction (callbackMouseSensitivity);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [1]->add (button);

  sprintf (buf, "%d%%", (int) mouse_sensitivity);
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab, yf, 2, yfstep - 0.1);
  controlsmenu [1]->add (label);
  yf -= yfstep;

  button = new Button ("REVERSE");
  button->setFunction (callbackMouseReverse);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [1]->add (button);

  if (mouse_reverse) sprintf (buf, "ON");
  else sprintf (buf, "OFF");
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab, yf, 2, yfstep - 0.1);
  controlsmenu [1]->add (label);
  yf -= yfstep;

  button = new Button ("RELATIVE");
  button->setFunction (callbackMouseRelative);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [1]->add (button);

  if (mouse_relative) sprintf (buf, "ON");
  else sprintf (buf, "OFF");
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab, yf, 2, yfstep - 0.1);
  controlsmenu [1]->add (label);
  yf -= yfstep;

  button = new Button ("AUTORUDDER");
  button->setFunction (callbackMouseAutorudder);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [1]->add (button);

  sprintf (buf, "%d%%", mouse_autorudder);
  label = new Label (buf);
  label->setTransparent (true);
  label->setBounds (xf + xfstep - xftab, yf, 2, yfstep - 0.1);
  controlsmenu [1]->add (label);
  yf -= yfstep;

  yf -= 0.5;
  textMouseButton (buf2, mouse_firecannon);
  sprintf (buf, "FIRE CANNON:            \t %s", buf2);
  label = new Label (buf);
  label->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [1]->add (label);
  yf -= yfstep;

  textMouseButton (buf2, mouse_firemissile);
  sprintf (buf, "FIRE MISSILE:            \t %s", buf2);
  label = new Label (buf);
  label->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [1]->add (label);
  yf -= yfstep;

  textMouseButton (buf2, mouse_selectmissile);
  sprintf (buf, "SELECT MISSILE:      \t %s", buf2);
  label = new Label (buf);
  label->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [1]->add (label);
  yf -= yfstep;

  yf -= 1; yfstep = 1.0;
  button = new Button ("LOAD DEFAULTS");
  button->setFunction (callbackMouseDefaults);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [1]->add (button);

/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Register joystick control submenu
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

#ifndef USE_GLUT
  char joystr [4];

  xf = xsubmenu + 1; yf = ysubmenu - 6; xfstep = 12.5; yfstep = 0.8;
  button = new Button ("AILERON");
  button->setFunction (callbackJoystickAxis);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [2]->add (button);
  setJoystickAxisString (joystick_aileron, joystr);
  label = new Label (joystr);
  label->setBounds (xf + xfstep - xftab, yf, xftab, yfstep - 0.1);
  label->setTransparent (true);
  controlsmenu [2]->add (label);
  yf -= yfstep;

  button = new Button ("ELEVATOR");
  button->setFunction (callbackJoystickAxis);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [2]->add (button);
  setJoystickAxisString (joystick_elevator, joystr);
  label = new Label (joystr);
  label->setBounds (xf + xfstep - xftab, yf, xftab, yfstep - 0.1);
  label->setTransparent (true);
  controlsmenu [2]->add (label);
  yf -= yfstep;

  button = new Button ("THROTTLE");
  button->setFunction (callbackJoystickAxis);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [2]->add (button);
  setJoystickAxisString (joystick_throttle, joystr);
  label = new Label (joystr);
  label->setBounds (xf + xfstep - xftab, yf, xftab, yfstep - 0.1);
  label->setTransparent (true);
  controlsmenu [2]->add (label);
  yf -= yfstep;

  button = new Button ("RUDDER");
  button->setFunction (callbackJoystickAxis);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [2]->add (button);
  setJoystickAxisString (joystick_rudder, joystr);
  label = new Label (joystr);
  label->setBounds (xf + xfstep - xftab, yf, xftab, yfstep - 0.1);
  label->setTransparent (true);
  controlsmenu [2]->add (label);
  yf -= yfstep;

  button = new Button ("VIEW X");
  button->setFunction (callbackJoystickAxis);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [2]->add (button);
  setJoystickAxisString (joystick_view_x, joystr);
  label = new Label (joystr);
  label->setBounds (xf + xfstep - xftab, yf, xftab, yfstep - 0.1);
  label->setTransparent (true);
  controlsmenu [2]->add (label);
  yf -= yfstep;

  button = new Button ("VIEW Y");
  button->setFunction (callbackJoystickAxis);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [2]->add (button);
  setJoystickAxisString (joystick_view_y, joystr);
  label = new Label (joystr);
  label->setBounds (xf + xfstep - xftab, yf, xftab, yfstep - 0.1);
  label->setTransparent (true);
  controlsmenu [2]->add (label);
  yf -= yfstep;

  int buttons [11];
  buttons [0] = joystick_firecannon;
  buttons [1] = joystick_firemissile;
  buttons [2] = joystick_dropflare;
  buttons [3] = joystick_dropchaff;
  buttons [4] = joystick_selectmissile;
  buttons [5] = joystick_targetnearest;
  buttons [6] = joystick_targetlocking;
  buttons [7] = joystick_targetnext;
  buttons [8] = joystick_targetprevious;
  buttons [9] = joystick_thrustup;
  buttons [10] = joystick_thrustdown;

  yf -= 0.5;
  for (i = 0; i < 11; i ++)
  {
    editjoystick = new EditJoystick (controlsmenu0names [i]);
    editjoystick->setButton (buttons [i]);
    editjoystick->setFunction (callbackJoystick);
    editjoystick->setBounds (xf, yf, xfstep, yfstep - 0.1);
    controlsmenu [2]->add (editjoystick);
    yf -= yfstep;
  }

  yf -= 1; yfstep = 1.0;
  button = new Button ("LOAD DEFAULTS");
  button->setFunction (callbackDefaultsJoystick);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  controlsmenu [2]->add (button);
#endif

/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Register top pilots menu
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

  xf = -10; yf = 9; xfstep = 20; yfstep = 1.2;
  sprintf (buf, "         PILOTS RANKING");
  label = new Label (buf);
  label->setBounds (xf, yf, xfstep, yfstep - 0.1);
  famemenu.add (label);
  yf -= yfstep + 0.25;

  yfstep = 1.0;
  textfield = new TextField ("", yfstep - 0.1);
  textfield->setBounds (xf, yf, xfstep, (yfstep - 0.1) * 13);
  famemenu.add (textfield);
  yf -= yfstep * 13;

  yfstep = 1.1;
  sprintf (buf, "         BACK TO MAIN MENU");
  button = new Button (buf);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  button->setFunction (callbackSwitchMainMenu);
  famemenu.add (button);
  yf -= yfstep + 0.25;

/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Register fighter menu
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

  xf = -10; yf = 12; xfstep = 20; yfstep = 1.2;
  sprintf (buf, "          FIGHTER INFO");
  label = new Label (buf);
  label->setBounds (xf, yf, xfstep, yfstep - 0.1);
  fightermenu.add (label);
  yf -= yfstep + 0.25;

  yfstep = 1.0;
  buf [0] = 0;
  label = new Label (buf);
  label->setBounds (xf, yf, xfstep, yfstep - 0.1);
  fightermenu.add (label);
  yf -= yfstep + 0.25;

  yfstep = 1.1;
  textfield = new TextField ("", yfstep - 0.1);
  textfield->setBounds (xf, yf, xfstep, (yfstep - 0.1) * 5 + 0.15);
  fightermenu.add (textfield);
  yf -= yfstep * 5 + 0.05;

  button = new Button ("<");
  button->setBounds (xf, yf, 1.5, 1.5);
  button->setFunction (callbackFighterPrev);
  fightermenu.add (button);
  button = new Button (">");
  button->setBounds (-xf -1.5, yf, 1.5, 1.5);
  button->setFunction (callbackFighterNext);
  fightermenu.add (button);
  textfield = new TextField ("", yfstep - 0.1);
  textfield->setBounds (xf + 1.5, yf, 17, 12.5);
  fightermenu.add (textfield);
  yf -= 13;
  
  sprintf (buf, "         BACK TO MAIN MENU");
  button = new Button (buf);
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  button->setFunction (callbackSwitchMainMenu);
  fightermenu.add (button);
  yf -= yfstep + 0.25;

/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Register mission menu
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

  xf = -12.5; yf = 11; xfstep = 25; yfstep = 1.3;
  buf [0] = 0;
  label = new Label (buf);
  label->setBounds (xf, yf, xfstep, yfstep - 0.1);
  missionmenu.add (label);
  yf -= yfstep + 0.15;

  yfstep = 1.0;
  buf [0] = 0;
  label = new Label (buf);
  label->setBounds (xf, yf, xfstep, 3.5);
  missionmenu.add (label);
  yf -= 3.75;

  buf [0] = 0;
  label = new Label (buf);
  label->setBounds (xf, yf, xfstep / 2 - 0.1, 5.7);
  missionmenu.add (label);

  buf [0] = 0;
  label = new Label (buf);
  label->setBounds (xf + xfstep / 2 + 0.1, yf, xfstep / 2 - 0.1, 5.7);
  missionmenu.add (label);
  yf -= 5.95;

  buf [0] = 0;
  label = new Label (buf);
  label->setBounds (xf, yf, xfstep / 2 - 0.1, 6.8);
  missionmenu.add (label);

  buf [0] = 0;
  label = new Label (buf);
  label->setBounds (xf + xfstep / 2 + 0.1, yf, xfstep / 2 - 0.1, 6.8);
  missionmenu.add (label);
  yf -= 7.15;

  yfstep = 1.0;
  button = new Button (" BACK TO MAIN MENU");
  button->setBounds (xf, yf, xfstep / 2 - 0.1, yfstep - 0.1);
  button->setFunction (callbackSwitchMainMenu);
  missionmenu.add (button);

  button = new Button ("    START MISSION");
  button->setBounds (xf + xfstep / 2 + 0.1, yf, xfstep / 2 - 0.1, yfstep - 0.1);
  button->setFunction (callbackSwitchStartMission);
  missionmenu.add (button);

/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Register quit dialog
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

  xf = -6; yf = 2; xfstep = 12; yfstep = 1.2;
  buf [0] = 0;
  label = new Label (buf);
  label->setBounds (xf, yf, xfstep, 3);
  quitmenu.add (label);
  yf -= 3.25;

  yfstep = 1.0; xfstep = 12;
  button = new Button ("    YES");
  button->setBounds (xf, yf, xfstep / 2 - 0.1, yfstep - 0.1);
  button->setFunction (callbackQuitNow);
  quitmenu.add (button);

  button = new Button ("     NO");
  button->setBounds (xf + xfstep / 2 + 0.1, yf, xfstep / 2 - 0.1, yfstep - 0.1);
  button->setFunction (callbackSwitchMainMenu);
  quitmenu.add (button);

/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Register stats dialog
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

  xf = -10; yf = 9; xfstep = 20; yfstep = 1.2;
  buf [0] = 0;
  textfield = new TextField (buf, 1);
  textfield->setBounds (xf, yf, xfstep, 15.8);
  statsmenu.add (textfield);
  yf -= 16;

  yfstep = 1.0; xfstep = 20;
  button = new Button ("                  CONTINUE");
  button->setBounds (xf, yf, xfstep, yfstep - 0.1);
  button->setFunction (callbackStats);
  statsmenu.add (button);



  famemenu.setVisible (false);
  fightermenu.setVisible (false);
  missionmenu.setVisible (false);
  quitmenu.setVisible (false);
  statsmenu.setVisible (false);
  allmenus.setVisible (false);
  allmenus.visible = true;
  allmenus.components [0]->setVisible (true);
}



/****************************************************************************
  GL-117 ENTRY POINT
****************************************************************************/

int main (int argc, char **argv)
{
  char buf [STDSIZE]; // temp buffer

  checkargs (argc, argv); // process command line parameters

  dirs = new Dirs (argv [0]); // get data directory (DATADIR, defined via autoconf)


  sprintf (buf, "Startup %s, %s ... ", argv [0], VERSIONSTRING);
  display (buf, LOG_MOST);

#ifdef _MSC_VER
  display ("Windows detected ", LOG_MOST);
#endif

  display ("Getting directory locations", LOG_ALL);
  
  if (!load_config ()) // try to load conf file (conf.cpp) and validate settings
  {
    // no conf file found => create new one
    display ("Creating new configuration", LOG_ALL);
    config_test (argc, argv); // do screen test
    firststart = true; // enable adjusting quality/view/graphics automatically by the game
  }

  save_config (); // save conf file (validated)

  load_configInterface (); // load interface settings from conf.interface and validate
  save_configInterface (); // save interface settings

  maploader = new MapLoader ();

// here srand should be called to initialize the random number generator
// this is currently done by grabbing random numbers via the init methods (not very elegant)

  server = NULL;
  client = NULL;

  display ("Creating/Loading pilots list", LOG_ALL);
  pilots = new PilotList (dirs->getSaves ("pilots")); // look at pilots.h

// NO SDL FOUND => USE GLUT ONLY
#ifdef USE_GLUT

  display ("Using GLUT only", LOG_MOST);
  if (!configinit)
  {
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);
    if (!setScreen (width, height, bpp, fullscreen))
    {
      load_saveconfig ();
      if (!setScreen (width, height, bpp, fullscreen))
      {
        sprintf (buf, "No working display mode %dx%d found", width, height);
        display (buf, LOG_FATAL);
        exit (EXIT_INIT);
      }
    }
  }

  display ("Calling main initialization method", LOG_ALL);
  myFirstInit ();

  display ("Creating dummy sound system, install SDL to enable sound", LOG_ALL);
  sound = new SoundSystem ();

  createMenu ();

  display ("Registering GLUT callbacks", LOG_ALL);
  glutReshapeFunc (myReshapeFunc);
  glutDisplayFunc (myDisplayFunc);
  glutKeyboardFunc (myKeyboardFunc);
  glutSpecialFunc (mySpecialFunc);
  glutPassiveMotionFunc (myPassiveMotionFunc);
  glutMouseFunc (myMouseFunc);
  glutIdleFunc (myIdleFunc);
  glutTimerFunc (20, myTimerFunc, 0);

  // parameters: visible angle, aspectracio, z-nearclip, z-farclip
  gluPerspective (visibleangle, (float) width / height, nearclippingplane * GLOBALSCALE, 50.0 * GLOBALSCALE);
  
  // no keyboard available with GLUT, as there are no KEY_DOWN/UP events
  if (controls <= 0)
    controls = CONTROLS_MOUSE;

  display ("Entering GLUT main loop", LOG_ALL);
  glutMainLoop(); // give controls to GLUT

// SDL FOUND
#else

  display ("Using SDL and GLUT", LOG_MOST);
  if (!configinit)
    if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0)
    {
      sprintf (buf, "Couldn't initialize SDL: %s", SDL_GetError ());
      display (buf, LOG_FATAL);
      exit (EXIT_INIT);
    }
  atexit (SDL_Quit);

// SDL_NET INSTALLED?
#ifdef HAVE_SDL_NET
  if (SDLNet_Init () == -1) // initialize SDL_net
  {
    sprintf (buf, "SDLNet_Init: %s", SDLNet_GetError ());
    display (buf, LOG_FATAL);
    exit (EXIT_INIT);
  }
  display ("Using SDL_net", LOG_MOST);
#endif

  if (!configinit)
  {
    if (!setScreen (width, height, bpp, fullscreen))
    {
      load_saveconfig ();
      if (!setScreen (width, height, bpp, fullscreen))
      {
        sprintf (buf, "No working display mode %dx%d found.", width, height);
        display (buf, LOG_FATAL);
        exit (EXIT_INIT);
      }
    }
  }

  display ("Setting SDL caption", LOG_ALL);
  SDL_WM_SetCaption ("GL-117", "GL-117"); // window name

  SDL_ShowCursor (0);

  display ("Creating sound system", LOG_ALL);
  sound = new SoundSystem (); // look at audio.cpp
  sound->volumesound = volumesound;
  sound->volumemusic = volumemusic;
  sound->setVolume (); // set all sound volumes
  sound->setVolumeMusic (); // set all music volumes

  display ("Playing startup music", LOG_ALL);
  sound->playMusic (1);
#ifdef HAVE_SDL_MIXER
  Mix_HookMusicFinished (playRandomMusic);
#endif

  display ("Calling main initialization method", LOG_ALL);
  myFirstInit ();
  myReshapeFunc (width, height);

  display ("Querying joystick", LOG_ALL);
  joysticks = SDL_NumJoysticks ();
  memset (jaxis, 0, maxjaxis * maxjoysticks * sizeof (int));
  if (joysticks > 0)
  {
    for (i = 0; i < joysticks; i ++)
    {
      SDL_JoystickEventState (SDL_ENABLE);
      sdljoystick [i] = SDL_JoystickOpen (i);
      sdljoystickaxes [i] = SDL_JoystickNumAxes (sdljoystick [i]);
      sprintf (buf, "Joystick \"%s\" detected", SDL_JoystickName (i));
      display (buf, LOG_MOST);
    }
  }
  else
  {
    display ("No joystick found", LOG_MOST);
//    sdljoystick [0] = NULL;
    if (controls == CONTROLS_JOYSTICK) // no joystick available, so switch to mouse controls
      controls = CONTROLS_MOUSE;
  }

// disable joystick manually
//  joysticks = 0;

/*  joysticks = 2;
  sdljoystickaxes [0] = 4;
  sdljoystickaxes [1] = 2;*/

  SDL_EnableUNICODE (1);
  SDL_EnableKeyRepeat (SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

// Restrict mouse to SDL window
//  SDL_WM_GrabInput (SDL_GRAB_ON);

  createMenu ();

  display ("Entering SDL main loop (GLUT emulation)", LOG_ALL);
  sdlMainLoop (); // simulate GLUT's main loop (above)

#endif
  
  return 0; // exit without signaling errors
}

#endif
