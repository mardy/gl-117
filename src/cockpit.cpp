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

/* This file includes the cockpit rendering methods. */

#ifndef IS_COCKPIT_H

#include <stdio.h>
#include <stdlib.h>
#include "cockpit.h"
#include "main.h"
#include "pilots.h"
#include "mathtab.h"
#include "mission.h"
#include "gl.h"
#include "glland.h"

#define TRIANGLE_BLIP 1
#define SQUARE_BLIP   2

Cockpit::Cockpit ()
{
  flarewarning = 0;
  chaffwarning = 0;
}

// Mitchell Perilstein:
void Cockpit::drawBlip(int shape, float x, float y, float z, unsigned char r, unsigned char g, unsigned char b)
{
  float blipsize = 0.02;
  int alpha = 200;
  CColor color;

//  setColor (&color, alpha); // no need here
  glColor4ub (r, g, b, alpha);
  glBegin (GL_QUADS);
 
  switch (shape) {
  case SQUARE_BLIP:
    glVertex3f (x + blipsize, y + blipsize, z);	  
    glVertex3f (x + blipsize, y - blipsize, z);	  
    glVertex3f (x - blipsize, y - blipsize, z);
    glVertex3f (x - blipsize, y + blipsize, z);	  
    glVertex3f (x + blipsize, y + blipsize, z);	  
    break;
  case TRIANGLE_BLIP:
    glVertex3f (x, y + blipsize, z);
    glVertex3f (x + blipsize, y - blipsize, z);
    glVertex3f (x - blipsize, y - blipsize, z);
    glVertex3f (x, y + blipsize, z);
    break;
  default:
    abort();
  }
  glEnd ();
}

void Cockpit::cockpitvertex (float phi, float gamma) // cylindrical headup projection
{
  float fpt = -fplayer->theta + 180;
  float sinfpt = SIN(fpt), cosfpt = COS(fpt);
  float x = cosfpt * phi + sinfpt * gamma + 180;
  float y = sinfpt * phi - cosfpt * gamma + 180;
  glVertex3f (10*SIN(x),10*SIN(y),-10*COS(x)*COS(y));
}

void Cockpit::setColor (int alpha)
{
  if (fplayer->o == &model_fig) glColor4ub (255, 255, 0, alpha);
  else if (fplayer->o == &model_figb) glColor4ub (255, 150, 100, alpha);
  else if (fplayer->o == &model_figc) glColor4ub (200, 200, 100, alpha);
  else if (fplayer->o == &model_figg) glColor4ub (255, 0, 0, alpha);
  else if (fplayer->o == &model_figi) glColor4ub (200, 200, 100, alpha);
  else glColor4ub (100, 100, 255, alpha);
}

void Cockpit::setColor (CColor *color, int alpha)
{
  if (fplayer->o == &model_fig) color->setColor (255, 255, 0, alpha);
  else if (fplayer->o == &model_figb) color->setColor (255, 150, 100, alpha);
  else if (fplayer->o == &model_figc) color->setColor (200, 200, 100, alpha);
  else if (fplayer->o == &model_figg) color->setColor (255, 0, 0, alpha);
  else if (fplayer->o == &model_figi) color->setColor (200, 200, 100, alpha);
  else color->setColor (100, 100, 255, alpha);
}

void Cockpit::drawCounter ()
{
  int i;
  char buf [STDSIZE];
  float xf = 2.8F, yf = -1.0F, zf = -4.0F, yf2 = -1.3F, xfl = 0.4F, yfl = 0.1F;
//    glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  bool flarewarn = false, chaffwarn = false;
  for (i = 0; i < maxmissile; i ++)
    if (missile [i]->active)
      if (missile [i]->target == fplayer)
      {
        if (missile [i]->id <= MISSILE_AIR1)
          flarewarn = true;
        else
          chaffwarn = true;
      }
  if (flarewarn && game == GAME_PLAY)
  {
    if (flarewarning <= 0)
    {
      flarewarning = 10 * timestep;
      sound->play (SOUND_BEEP2, false);
    }
    flarewarning -= dt;
    if (flarewarning <= 6 * timestep)
    {
      glColor3ub (200, 0, 0);
      glBegin (GL_QUADS);
      glVertex3f (xf - xfl, yf2 - yfl, zf);
      glVertex3f (xf + xfl, yf2 - yfl, zf);
      glVertex3f (xf + xfl, yf2 + yfl, zf);
      glVertex3f (xf - xfl, yf2 + yfl, zf);
      glEnd ();
    }
  }
  if (chaffwarn && game == GAME_PLAY)
  {
    if (chaffwarning <= 0)
    {
      chaffwarning = 10 * timestep;
      sound->play (SOUND_BEEP1, false);
    }
    chaffwarning -= dt;
    if (chaffwarning <= 6 * timestep)
    {
      glColor3ub (0, 0, 200);
      glBegin (GL_QUADS);
      glVertex3f (xf - xfl, yf - yfl, zf);
      glVertex3f (xf + xfl, yf - yfl, zf);
      glVertex3f (xf + xfl, yf + yfl, zf);
      glVertex3f (xf - xfl, yf + yfl, zf);
      glEnd ();
    }
  }
  glLineWidth (LINEWIDTH(1.0F));
  glColor3ub (255, 0, 0);
  glBegin (GL_LINE_STRIP);
  glVertex3f (xf - xfl, yf2 - yfl, zf);
  glVertex3f (xf + xfl, yf2 - yfl, zf);
  glVertex3f (xf + xfl, yf2 + yfl, zf);
  glVertex3f (xf - xfl, yf2 + yfl, zf);
  glVertex3f (xf - xfl, yf2 - yfl, zf);
  glEnd ();
  glColor3ub (0, 100, 255);
  glBegin (GL_LINE_STRIP);
  glVertex3f (xf - xfl, yf - yfl, zf);
  glVertex3f (xf + xfl, yf - yfl, zf);
  glVertex3f (xf + xfl, yf + yfl, zf);
  glVertex3f (xf - xfl, yf + yfl, zf);
  glVertex3f (xf - xfl, yf - yfl, zf);
  glEnd ();
  CColor blue (0, 100, 255);
  CColor red (255, 0, 0);
  sprintf (buf, "CHAFF: %d", fplayer->chaffs);
  font1->drawTextCentered (xf*10, (yf-0.05)*10, zf, buf, &blue);
  sprintf (buf, "FLARE: %d", fplayer->flares);
  font1->drawTextCentered (xf*10, (yf2-0.05)*10, zf, buf, &red);

  if (mission->id == MISSION_DEATHMATCH1 || mission->id == MISSION_DEATHMATCH3)
  {
    sprintf (buf, "%s: %d", pilots->pilot [pilots->aktpilot]->name, fplayer->fighterkills);
    font1->drawText (-30.0F, 15.0F, -3.0F, buf, &blue);
    for (i = 1; i < 8; i ++)
    {
      sprintf (buf, "PILOT%d: %d", i, fighter [i]->fighterkills);
      font1->drawText (-30.0F, 15.0F - i, -3.0F, buf, &red);
    }
  }
  if (mission->id == MISSION_DEATHMATCH2)
  {
    for (i = 0; i < 4; i ++)
    {
      sprintf (buf, "TEAM%d: %d", i, fighter [i * 2]->fighterkills + fighter [i * 2 + 1]->fighterkills);
      font1->drawText (-30.0F, 15.0F - i, -3.0F, buf, &red);
    }
  }
}

/*void Cockpit::drawTargeter ()
{
  if (! fplayer->target)
    return;

  glMatrixMode (GL_MODELVIEW);
  glPushMatrix();
  if (fplayer->target->active)
  {
    DynamicObj* target = fplayer->target;

    float hud_width  = 800.0f; // so x will be from -400.0f to 400.0f
    float hud_height = 600.0f; //        and y from -300.0f to 300.0f
    float view_angle = 80.0f / 180.0f * PI; // in radians!

    float view_matrix [16];
    glGetFloatv (GL_MODELVIEW_MATRIX, view_matrix);

    glLoadIdentity ();
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluOrtho2D (-hud_width * 0.5f,  hud_width * 0.5f,
                -hud_height * 0.5f, hud_height * 0.5f);

    CVector3 pos = *target->tl;
    CVector3 delta = *fplayer->tl;
    delta.sub (&pos);

    // multiply Vector by Matrix
    float* m = view_matrix;
    float x = m[0] * pos.x + m[4] * pos.y + m[8] * pos.z + m[12];
    float y = m[1] * pos.x + m[5] * pos.y + m[9] * pos.z + m[13];
    float z = m[2] * pos.x + m[6] * pos.y + m[10] * pos.z + m[14];
    pos.x = x; pos.y = y; pos.z = z;
    
    float dist = delta.length ();

    // sorry, but some variables names as good as my english :(
    float a = sqrt (pos.x * pos.x + pos.y * pos.y);
    float b = sqrt (dist * dist - a * a);
    float descale = hud_height * 0.5f / tan (view_angle * 0.5f) / b;

    gl->enableAlphaBlending ();
    glTranslatef (pos.x * descale, pos.y * descale, 0.0f);
    glPushMatrix ();

      // FIXME: don't sure that this is right:
      float object_radius = target->zoom;

      float second_descale = hud_height * 0.5f / tan (view_angle * 0.5f) / (b - object_radius);
      float radius = object_radius * second_descale + 4.0f;

      bool full = false;
      if (static_cast <AIObj*> (fplayer->target)->party != fplayer->party)
      {
        if (fplayer->ttf < 0 && fplayer->missiletype != MISSILE_DF1 - MISSILE1)
        {
          glColor4ub (255, 255, 0, 255); full = true;
        }
        else
        {
          glColor4ub (255, (50 * timestep - fplayer->ttf) * 255 / 60 / timestep, 0, 255);
        }
      }
      else
      {
        glColor4ub (0, 0, 255, 255);
      }
      if (! full)
      {
        const float corner_size = 0.5f; // in half's of rect side
        glBegin (GL_LINE_STRIP);
        glVertex2f (- radius, radius * (1.0f - corner_size));
        glVertex2f (- radius, radius);
        glVertex2f (- radius * (1.0f - corner_size), radius);
        glEnd ();

        glBegin (GL_LINE_STRIP);
        glVertex2f (radius * (1.0f - corner_size), radius);
        glVertex2f (radius, radius);
        glVertex2f (radius, radius * (1.0f - corner_size));
        glEnd ();

        glBegin (GL_LINE_STRIP);
        glVertex2f (radius * (1.0f - corner_size), - radius);
        glVertex2f (radius, - radius);
        glVertex2f (radius, - radius * (1.0f - corner_size));
        glEnd ();

        glBegin (GL_LINE_STRIP);
        glVertex2f (- radius, - radius * (1.0f - corner_size));
        glVertex2f (- radius, - radius);
        glVertex2f (- radius * (1.0f - corner_size), - radius);
        glEnd ();
      }
      else
      {
        glBegin (GL_LINE_STRIP);
        glVertex2f (- radius, - radius);
        glVertex2f (radius, - radius);
        glVertex2f (radius, radius);
        glVertex2f (- radius, radius);
        glVertex2f (- radius, - radius);
        glEnd ();
      }
    glPopMatrix ();
    gl->disableAlphaBlending ();
  }
  glMatrixMode (GL_MODELVIEW);
  glPopMatrix();

  glViewport(0, 0, (GLint) width, (GLint) height);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  float v = getView ();
  if (camera == 50) v = 100000.0;
  gluPerspective (80.0, (float) width / height, 0.25 * GLOBALSCALE, v * GLOBALSCALE);
  glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
}*/

void Cockpit::drawTargeter ()
{
  if (fplayer->target)
  if (fplayer->target->active)
  {
    DynamicObj *o = fplayer->target;
    float myphi = fplayer->phi;
    if (myphi < 0) myphi += 360;
    float ex1 = cosi [(int) myphi] * o->zoom, ey1 = -sine [(int) myphi] * o->zoom;
    float ex2 = -ex1, ey2 = -ey1;
    float ez = o->zoom;
    gl->enableAlphaBlending ();
    bool full = false;
    if (((AIObj *) fplayer->target)->party != fplayer->party)
    {
      if (fplayer->ttf <= 0 && fplayer->missiletype != MISSILE_DF1 - MISSILE1)
      {
        glColor4ub (255, 255, 0, 255); full = true;
      }
      else
      {
        glColor4ub (255, (50 * timestep - fplayer->ttf) * 255 / 60 / timestep, 0, 255);
      }
    }
    else
    {
      glColor4ub (0, 0, 255, 255);
    }
    if (!full)
    {
      float dx = (ex2 - ex1) / 4;
      float dy = (ey2 - ey1) / 4;
      float dz = ez / 2;
      glBegin (GL_LINE_STRIP);
      glVertex3f (o->tl->x + ex1, o->tl->y + ez - dz, o->tl->z + ey1);
      glVertex3f (o->tl->x + ex1, o->tl->y + ez, o->tl->z + ey1);
      glVertex3f (o->tl->x + ex1 + dx, o->tl->y + ez, o->tl->z + ey1 + dy);
      glEnd ();
      glBegin (GL_LINE_STRIP);
      glVertex3f (o->tl->x + ex2 - dx, o->tl->y + ez, o->tl->z + ey2 - dy);
      glVertex3f (o->tl->x + ex2, o->tl->y + ez, o->tl->z + ey2);
      glVertex3f (o->tl->x + ex2, o->tl->y + ez - dz, o->tl->z + ey2);
      glEnd ();
      glBegin (GL_LINE_STRIP);
      glVertex3f (o->tl->x + ex2, o->tl->y - ez + dz, o->tl->z + ey2);
      glVertex3f (o->tl->x + ex2, o->tl->y - ez, o->tl->z + ey2);
      glVertex3f (o->tl->x + ex2 - dx, o->tl->y - ez, o->tl->z + ey2 - dy);
      glEnd ();
      glBegin (GL_LINE_STRIP);
      glVertex3f (o->tl->x + ex1 + dx, o->tl->y - ez, o->tl->z + ey1 + dy);
      glVertex3f (o->tl->x + ex1, o->tl->y - ez, o->tl->z + ey1);
      glVertex3f (o->tl->x + ex1, o->tl->y - ez + dz, o->tl->z + ey1);
      glEnd ();
    }
    else
    {
      glBegin (GL_LINE_STRIP);
      glVertex3f (o->tl->x + ex1, o->tl->y + ez, o->tl->z + ey1);
      glVertex3f (o->tl->x + ex2, o->tl->y + ez, o->tl->z + ey2);
      glVertex3f (o->tl->x + ex2, o->tl->y - ez, o->tl->z + ey2);
      glVertex3f (o->tl->x + ex1, o->tl->y - ez, o->tl->z + ey1);
      glVertex3f (o->tl->x + ex1, o->tl->y + ez, o->tl->z + ey1);
      glEnd ();
    }
    gl->disableAlphaBlending ();
  }
}

void Cockpit::drawCross ()
{
  glDisable (GL_LIGHTING);

  gl->enableAlphaBlending ();
  glEnable (GL_ALPHA_TEST);
  glAlphaFunc (GL_GEQUAL, 0.1);
  //  glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  if (fplayer->o == &model_fig || fplayer->o == &model_figg)
  {
    gl->enableTextures (texcross->textureID);
  }
  else
  {
    gl->enableTextures (texcross2->textureID);
  }

  float xf = 0.1, yf = 0.1, zf = 1.0;
  setColor (150);
  glBegin (GL_QUADS);
  glTexCoord2d (0, 0);
  glVertex3f (-xf, -yf, -zf);
  glTexCoord2d (0, 1);
  glVertex3f (-xf, yf, -zf);
  glTexCoord2d (1, 1);
  glVertex3f (xf, yf, -zf);
  glTexCoord2d (1, 0);
  glVertex3f (xf, -yf, -zf);
  glEnd ();
  glDisable (GL_ALPHA_TEST);
  glDisable (GL_TEXTURE_2D);
  gl->disableAlphaBlending ();
  //  glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

void Cockpit::drawHeading ()
{
  char str [STDSIZE];
  CColor color;
  int i = 0;
  int alpha = 175;
  setColor (&color, alpha);
  glDisable (GL_LIGHTING);
  glLineWidth (LINEWIDTH(2.0F));
  glDisable (GL_DEPTH_TEST);
  float xf;
  float yf = 17.5;
  float zf = -2.5;
  float g = 0.5;
  for (i = 0; i < 360; i += 5)
  {
    float p = (float) i - fplayer->phi;
    if (p < -180) p += 360;
    if (p > 180) p -= 360;
    if (p >= -40 && p <= 40)
    {
      if ((i - mission->heading) % 90 == 0)
        g = 0.6;
      else if (!(i % 10))
        g = 0.3;
      else
        g = 0.1;
      xf = p / 6.0;
      gl->enableAlphaBlending ();
      setColor (alpha);
      glBegin (GL_LINES);
      glVertex3f (xf * 0.1, (yf - g) * 0.1, zf);
      glVertex3f (xf * 0.1, yf * 0.1, zf);
      glEnd ();
      if (i == mission->heading)
        font1->drawText (xf - 0.5, yf - 2.0, zf, "S", &color);
      else if (i == mission->heading + 90 || i == mission->heading - 270)
        font1->drawText (xf - 0.5, yf - 2.0, zf, "E", &color);
      else if (i == mission->heading + 180 || i == mission->heading - 180)
        font1->drawText (xf - 0.5, yf - 2.0, zf, "N", &color);
      else if (i == mission->heading + 270 || i == mission->heading - 90)
        font1->drawText (xf - 0.5, yf - 2.0, zf, "W", &color);
    }
  }

  if (fplayer->target != NULL)
    if (fplayer->target->active)
    {
      float dgamma = fplayer->getAngleH (fplayer->target);
      float dphi = fplayer->getAngle (fplayer->target);
      if (fabs (dphi) > 50 || fabs (dgamma) > 50)
      {
        if (dphi < -90) dphi = -90;
        else if (dphi > 90) dphi = 90;
        float delta = atan (dgamma / dphi) * 180 / PI;
        if (dphi > 0) delta -= 180;
        delta += fplayer->theta;
        float xs = COS(delta) * 3;
        float ys = -SIN(delta) * 3;
        float delta1 = delta + 5;
        float xs1 = COS(delta1) * 2.8;
        float ys1 = -SIN(delta1) * 2.8;
        float delta2 = delta - 5;
        float xs2 = COS(delta2) * 2.8;
        float ys2 = -SIN(delta2) * 2.8;
        zf = -4;
        glBegin (GL_LINE_STRIP);
        glColor4ub (255, 0, 0, alpha);
        glVertex3f (xs2, ys2, zf);
        glVertex3f (xs, ys, zf);
        glVertex3f (xs1, ys1, zf);
        glEnd ();
      }
    }
  
  sprintf (str, "SPEED %d", (int) (fplayer->realspeed / timestep * 80000.0F));
  font1->drawTextCentered (-8.0, -8.5, -4.0, str, &color);

  sprintf (str, "AMMO %d", fplayer->ammo);
  font1->drawTextCentered (8.0, -8.5, -4.0, str, &color);

  gl->enableAlphaBlending ();

  float dgamma = fplayer->gamma - (int) fplayer->gamma;
  float innerx = 5, outerx = 10, widthx = 1;
  int step = 15;

  glLineWidth (LINEWIDTH(1.2F));

  glBegin (GL_LINES);
  float tmp = fplayer->gamma - 180;
  cockpitvertex (-innerx,tmp);
  cockpitvertex (-15,tmp);
  cockpitvertex (innerx,tmp);
  cockpitvertex (15,tmp);

  for (i = -180 + step; i < 0; i += step)
  {
    float tmp=int(-i+fplayer->gamma+540)%360-180;
    tmp += dgamma;
    cockpitvertex (-innerx,tmp+1);
    cockpitvertex (-innerx,tmp);
    cockpitvertex (innerx,tmp+1);
    cockpitvertex (innerx,tmp);
    cockpitvertex (-outerx,tmp);
    cockpitvertex (-innerx,tmp);
    cockpitvertex (innerx,tmp);
    cockpitvertex (outerx,tmp);
  }

  for (i = step; i < 180; i += step)
  {
    float tmp=int(-i+fplayer->gamma+540)%360-180;
    tmp += dgamma;
    cockpitvertex (-innerx,tmp+1);
    cockpitvertex (-innerx,tmp);
    cockpitvertex (innerx,tmp+1);
    cockpitvertex (innerx,tmp);
    cockpitvertex (-outerx,tmp);
    cockpitvertex (-outerx+widthx,tmp);
    cockpitvertex (innerx,tmp);
    cockpitvertex (innerx+widthx,tmp);
    cockpitvertex (-innerx-widthx,tmp);
    cockpitvertex (-innerx,tmp);
    cockpitvertex (outerx-widthx,tmp);
    cockpitvertex (outerx,tmp);
  }
  glEnd ();
  gl->disableAlphaBlending ();

  glLineWidth (LINEWIDTH(2.0F));
}

void Cockpit::drawTargetedElement ()
{
  char str [20];
  CColor color;
  CVector3 tl, n;
  CRotation r;
  color.setColor (255, 0, 0);
  color.c [3] = 255;
  tl.x = -0.35; tl.y = -0.3; tl.z = -0.5;
  gl->disableAlphaBlending ();
  glEnable (GL_DEPTH_TEST);
  if (fplayer->target != NULL)
    if (fplayer->target->active)
    {
      glEnable (GL_LIGHTING);
      fplayer->target->o->draw (&n, &tl, fplayer->target->rot, 0.05, 0.3, 0);
      glDisable (GL_LIGHTING);
      if (((AIObj *) fplayer->target)->party == fplayer->party)
        color.setColor (0, 0, 255);
      font1->drawText (-24.0, -23.0, -4.0, fplayer->target->o->name, &color);
      sprintf (str, "%d", (int) (15.0 * fplayer->distance (fplayer->target)));
      font1->drawText (-24.0, -25.0, -4.0, str, &color);
    }
  glDisable (GL_DEPTH_TEST);
}

void Cockpit::drawWeapon ()
{
  char str [20];
  CColor color;
  CVector3 tl, n;
  CRotation r;
  if (fplayer->missiles [fplayer->missiletype] <= 0)
    return;
  color.setColor (255, 0, 0);
  color.c [3] = 255;
  tl.x = 0.35; tl.y = -0.3; tl.z = -0.5;
  gl->disableAlphaBlending ();
  glEnable (GL_DEPTH_TEST);
  CModel *missile = NULL;
  if (fplayer->missiletype == 0) missile = &model_missile1;
  else if (fplayer->missiletype == 1) missile = &model_missile2;
  else if (fplayer->missiletype == 2) missile = &model_missile3;
  else if (fplayer->missiletype == 3) missile = &model_missile4;
  else if (fplayer->missiletype == 4) missile = &model_missile5;
  else if (fplayer->missiletype == 5) missile = &model_missile6;
  else if (fplayer->missiletype == 6) missile = &model_missile7;
  else if (fplayer->missiletype == 7) missile = &model_missile8;
  glEnable (GL_LIGHTING);
  missile->draw (&n, &tl, fplayer->rot, 0.05, 1.0, 0);
  glDisable (GL_LIGHTING);
  glDisable (GL_DEPTH_TEST);
  font1->drawText (16.0, -22.0, -4.0, missile->name, &color);
  sprintf (str, "N %d", fplayer->missiles [fplayer->missiletype]);
  font1->drawText (16.0, -24.0, -4.0, str, &color);
}

void Cockpit::drawRadar ()
{
  int i;
  float yf = -4.2, zf = -7.0;

  gl->enableAlphaBlending ();
  glEnable (GL_ALPHA_TEST);
  glAlphaFunc (GL_GEQUAL, 0.1);
  glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  setColor (150);
  float xl, yl;
  int type;
  if (fplayer->o == &model_fig || fplayer->o == &model_figg)
  {
    gl->enableTextures (texradar2->textureID);
    xl = 1.4; yl = 1.3;
    type = 0;
  }
  else
  {
    gl->enableTextures (texradar1->textureID);
    xl = 1.3; yl = 1.2;
    type = 1;
  }
  glBegin (GL_QUADS);
  glTexCoord2d (0, 0);
  glVertex3f (-xl, yf - yl, zf);
  glTexCoord2d (0, 1);
  glVertex3f (-xl, yf + yl, zf);
  glTexCoord2d (1, 1);
  glVertex3f (xl, yf + yl, zf);
  glTexCoord2d (1, 0);
  glVertex3f (xl, yf - yl, zf);
  glEnd ();
  glDisable (GL_ALPHA_TEST);
  glDisable (GL_TEXTURE_2D);
  glLineWidth (LINEWIDTH(1.0F));
  glBegin (GL_LINES);
  glVertex3f (0, yf - yl * 0.9, zf);
  glVertex3f (0, yf + yl * 0.9, zf);
  glVertex3f (-xl * 0.9, yf, zf);
  glVertex3f (xl * 0.9, yf, zf);
  glEnd ();
  int radarzoom = 1;
  if (fplayer->disttarget < 40) radarzoom = 2;
  for (i = 0; i < maxfighter; i ++)
    if (fighter [i] != fplayer && fighter [i]->active)
    {
      int aw = fplayer->getAngle (fighter [i]);
      if (aw < 0) aw += 360;
      float d = fplayer->distanceXZ (fighter [i]) / 100.0 * radarzoom;
      float px = -d * sine [aw];
      float py = yf + d * cosi [aw];
      if ((type == 0 && d < 1.2) || (type == 1 && px >= -1.2 && px <= 1.2 && py >= yf - 1.1 && py <= yf + 1.1))
      {
        if (fighter [i] == fplayer->target)
        {
          drawBlip(SQUARE_BLIP, px, py, zf, 255, 200, 0);
        }
        else if (fighter [i]->party != fplayer->party)
        {
      	  drawBlip(SQUARE_BLIP, px, py, zf, 255, 0, 0);
        }
        else
        {
      	  drawBlip(SQUARE_BLIP, px, py, zf, 0, 0, 255);
        }
      }
    }
  for (i = 0; i < maxmissile; i ++)
  {
    if (missile [i]->target == fplayer && missile [i]->active)
    {
      int aw = fplayer->getAngle (missile [i]);
      if (aw < 0) aw += 360;
      float d = fplayer->distance (missile [i]) / 100.0 * radarzoom;
      float px = -d * sine [aw];
      float py = yf + d * cosi [aw];
      if ((type == 0 && d < 1.2) || (type == 1 && px >= -1.2 && px <= 1.2 && py >= yf - 1.1 && py <= yf + 1.1))
      {
        drawBlip(TRIANGLE_BLIP, px, py, zf, 255, 255, 255);
      }
    }
  }
  gl->disableAlphaBlending ();
}

void Cockpit::drawRelativeHeightBar()
{
  char str [STDSIZE];
  CColor color;
  int alpha = 175;
  setColor (&color, alpha);
  setColor (alpha);
  glDisable (GL_LIGHTING);
  glLineWidth (LINEWIDTH(1.0F));
  glDisable (GL_DEPTH_TEST);
  gl->enableAlphaBlending ();
  
  float xf = 1.5F, xfl = 0.06F, yf=0.0F, yfl = 0.7F, zf=-4.0F;
  float px = fplayer->tl->x, py = fplayer->tl->y, pz = fplayer->tl->z;
  float lh = l->getExactHeight(px, pz);
  setColor(80); // low alpha for better visibility
  // add 100.0 to each player_y and land_h to avoid values <= 0.0
  float rel_height = ((100.0F+py)/(100.0F+lh) * 100.0F) - 100.0F;	

  // turn 0 -> 50.0 range to -0.7 -> 0.7 for drawing, 1.4/50=0.0208
  float yfv = (rel_height * 0.0208F)-0.7F;
  // cap max-min values  
  if(yfv >  0.7F) yfv =  0.7F;
  if(yfv < -0.7F) yfv = -0.7F;
  
  glBegin (GL_QUADS);
  glVertex3f (xf - xfl, yf - yfl, zf);  // fixed
  glVertex3f (xf + xfl, yf - yfl, zf);  // fixed
  glVertex3f (xf + xfl, yf + yfv, zf);  // var
  glVertex3f (xf - xfl, yf + yfv, zf);  // var
  glEnd ();
  setColor(alpha);
  
  sprintf (str, "RHEIGHT");
  font1->drawTextCentered (xf*11.0F, (yf-0.85F)*10.0F, zf, str, &color);
  glEnd();
  gl->disableAlphaBlending ();
}

void Cockpit::drawThrustBar()
{
  char str [STDSIZE];
  CColor color;
  int alpha = 175;
  setColor (&color, alpha);
  setColor (alpha);
  glDisable (GL_LIGHTING);
  glLineWidth (LINEWIDTH(1.0F));
  glDisable (GL_DEPTH_TEST);
  gl->enableAlphaBlending ();

  setColor(80); // don't block visibility too much
  float xf = -1.5F, xfl = 0.06F, yf=0.0F, yfl = 0.7F, zf=-4.0F;
  
  // turn 0.5 -> 1.0 range to -0.7 -> 0.7 for drawing
  float yfv = fplayer->thrust / fplayer->maxthrust * 2.8F - 2.1F;
  // cap max-min values
  if(yfv >  0.7F) yfv =  0.7F;
  if(yfv < -0.7F) yfv = -0.7F;
  glBegin (GL_QUADS);
  glVertex3f (xf - xfl, yf - yfl, zf);  // fixed
  glVertex3f (xf + xfl, yf - yfl, zf);  // fixed
  glVertex3f (xf + xfl, yf + yfv, zf);  // var
  glVertex3f (xf - xfl, yf + yfv, zf);  // var
  glEnd ();
  setColor(alpha);
  sprintf (str, "THRUST");
  font1->drawTextCentered (xf*11.0F, (yf-0.85F)*10.0F, zf, str, &color);

  glEnd();
  gl->disableAlphaBlending ();
}

#endif
//vim:sw=2 cino=c5,:.4s,=.2s,(0,u0,g0,h2,+2
