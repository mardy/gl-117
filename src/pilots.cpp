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

/* This file handles all pilots data. */

#ifndef IS_PILOTS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dirs.h"
#include "pilots.h"
#include "mission.h"

TeamPilot::TeamPilot (int ranking, char *name, int intelligence, int precision, int aggressivity, int fighterkills)
{
  this->ranking = ranking;
  strcpy (this->name, name);
  this->intelligence = intelligence;
  this->precision = precision;
  this->aggressivity = aggressivity;
  this->fighterkills = fighterkills;
}

void TeamPilot::flyMission (int averagekills)
{
  float f = (float) (intelligence + precision + aggressivity);
  f /= 1000;
  f = 1.0F - f;
  f *= (float) averagekills;
  if (f < 0) f = 0;
  fighterkills += (int) f;
}

char *TeamPilot::getRank ()
{
  if (ranking == 0) return "AIRMAN BASIC";
  else if (ranking == 1) return "AIRMAN";
  else if (ranking == 2) return "AIRMAN 1ST CLASS";
  else if (ranking == 3) return "2ND LIEUTNANT";
  else if (ranking == 4) return "1ST LIEUTNANT";
  else if (ranking == 5) return "CAPTAIN";
  else if (ranking == 6) return "MAJOR";
  else if (ranking == 7) return "COLONEL";
  else return "GENERAL";
}

char *TeamPilot::getShortRank ()
{
  if (ranking == 0) return "AB";
  else if (ranking == 1) return "ARM";
  else if (ranking == 2) return "A1C";
  else if (ranking == 3) return "2ND LT";
  else if (ranking == 4) return "1ST LT";
  else if (ranking == 5) return "CAPT";
  else if (ranking == 6) return "MAJ";
  else if (ranking == 7) return "COL";
  else return "GEN";
}

char *TeamPilot::getName ()
{
  strcpy (fullname, getRank ());
  strcat (fullname, " ");
  strcat (fullname, name);
  return fullname;
}

char *TeamPilot::getShortName ()
{
  strcpy (fullname, getShortRank ());
  strcat (fullname, " ");
  strcat (fullname, name);
  return fullname;
}

void TeamPilot::load (FILE *in)
{
  fread (&ranking, sizeof (int), 1, in);
  int len;
  fread (&len, sizeof (int), 1, in);
  if (len > 50) return;
  fread (name, sizeof (char), len, in);
  fread (&intelligence, sizeof (int), 1, in);
  fread (&precision, sizeof (int), 1, in);
  fread (&aggressivity, sizeof (int), 1, in);
  fread (&fighterkills, sizeof (int), 1, in);
}

void TeamPilot::save (FILE *out)
{
  fwrite (&ranking, sizeof (int), 1, out);
  int len = strlen (name);
  fwrite (&len, sizeof (int), 1, out);
  fwrite (name, sizeof (char), len, out);
  fwrite (&intelligence, sizeof (int), 1, out);
  fwrite (&precision, sizeof (int), 1, out);
  fwrite (&aggressivity, sizeof (int), 1, out);
  fwrite (&fighterkills, sizeof (int), 1, out);
}



void Pilot::load ()
{
  int i;
  for (i = 0; i < maxpilotdata; i ++)
  {
    mission_state [i] = 0;
    mission_time [i] = 0;
    mission_fighterkills [i] = 0;
    mission_shipkills [i] = 0;
    mission_tankkills [i] = 0;
    mission_otherkills [i] = 0;
    mission_shield [i] = 0;
    mission_points [i] = 0;
    mission_score [i] = 0;
  }
  if (tp == NULL)
  {
    tp = new TeamPilot *[11];
    tp [0] = new TeamPilot (5, "PRIMETIME", 200, 100, 150, 20);
    tp [1] = new TeamPilot (1, "HEPTARGON", 80, 220, 300, 1);
    tp [2] = new TeamPilot (2, "KARA", 180, 160, 50, 4);
    tp [3] = new TeamPilot (6, "SHARK", 70, 90, 120, 22);
    tp [4] = new TeamPilot (3, "BOSS", 250, 180, 80, 10);
    tp [5] = new TeamPilot (2, "DR DOOM", 320, 210, 20, 2);
    tp [6] = new TeamPilot (4, "SHADOW", 130, 200, 320, 15);
    tp [7] = new TeamPilot (7, "MATRIX", 40, 80, 180, 25);
    tp [8] = new TeamPilot (1, "FIREBIRD", 250, 140, 100, 1);
    tp [9] = new TeamPilot (4, "THUNDER", 150, 170, 60, 18);
    tp [10] = new TeamPilot (0, "PLAYER", 0, 0, 0, 0);
  }

  char buf [4096];
  strcpy (buf, dirs->getSaves (name));
  FILE *in = fopen (buf, "rb");
  if (in == NULL)
  {
    display ("Could not load pilot", LOG_WARN);
    return;
  }
  char saveversion [20];
  fread (saveversion, sizeof (char), strlen (SAVEVERSION), in);
  fread (mission_state, sizeof (int), maxpilotdata, in);
  fread (mission_time, sizeof (int), maxpilotdata, in);
  fread (mission_fighterkills, sizeof (int), maxpilotdata, in);
  fread (mission_shipkills, sizeof (int), maxpilotdata, in);
  fread (mission_tankkills, sizeof (int), maxpilotdata, in);
  fread (mission_otherkills, sizeof (int), maxpilotdata, in);
  fread (mission_shield, sizeof (int), maxpilotdata, in);
  fread (mission_points, sizeof (int), maxpilotdata, in);
  fread (mission_score, sizeof (int), maxpilotdata, in);
  for (i = 0; i < 10; i ++)
    tp [i]->load (in);
  fclose (in);
}

void Pilot::save ()
{
  int i;
  char buf [4096];
  strcpy (buf, dirs->getSaves (name));
  FILE *out = fopen (buf, "wb");
  if (out == NULL)
  {
    display ("Could not write pilot", LOG_WARN);
    return;
  }
  fwrite (SAVEVERSION, sizeof (char), strlen (SAVEVERSION), out);
  fwrite (mission_state, sizeof (int), maxpilotdata, out);
  fwrite (mission_time, sizeof (int), maxpilotdata, out);
  fwrite (mission_fighterkills, sizeof (int), maxpilotdata, out);
  fwrite (mission_shipkills, sizeof (int), maxpilotdata, out);
  fwrite (mission_tankkills, sizeof (int), maxpilotdata, out);
  fwrite (mission_otherkills, sizeof (int), maxpilotdata, out);
  fwrite (mission_shield, sizeof (int), maxpilotdata, out);
  fwrite (mission_points, sizeof (int), maxpilotdata, out);
  fwrite (mission_score, sizeof (int), maxpilotdata, out);
  for (i = 0; i < 10; i ++)
    tp [i]->save (out);
  fclose (out);
}

char *Pilot::getRank ()
{
  int i, sum = 0;
  for (i = MISSION_CAMPAIGN1; i < MISSION_CAMPAIGN2; i ++)
  {
    if (mission_score [i] > 0 && mission_score [i] < 2000)
      sum += mission_score [i];
  }
  if (sum < 500)
  { ranking = 0; return "AIRMAN BASIC"; }
  else if (sum < 1000)
  { ranking = 1; return "AIRMAN"; }
  else if (sum < 1500)
  { ranking = 2; return "AIRMAN 1ST CLASS"; }
  else if (sum < 2500)
  { ranking = 3; return "2ND LIEUTNANT"; }
  else if (sum < 3500)
  { ranking = 4; return "1ST LIEUTNANT"; }
  else if (sum < 5000)
  { ranking = 5; return "CAPTAIN"; }
  else if (sum < 7000)
  { ranking = 6; return "MAJOR"; }
  else if (sum < 9000)
  { ranking = 7; return "COLONEL"; }
  else
  { ranking = 8; return "GENERAL"; }
}

/*
US RANKS:
Second Lieutenant, 2nd Lt
First Lieutenant, 1st Lt
Captain, Capt
Major, Maj
Lieutenant Colonel, Lt Col
Colonel, Col
Brigadier General, Brig Gen
Major General, Maj Gen
Lieutenant General, Lt Gen
General, Gen
*/

char *Pilot::getShortRank ()
{
  int i, sum = 0;
  for (i = MISSION_CAMPAIGN1; i < MISSION_CAMPAIGN2; i ++)
  {
    if (mission_score [i] > 0 && mission_score [i] < 2000)
      sum += mission_score [i];
  }
  if (sum < 500)
  { ranking = 0; return "AB"; }
  else if (sum < 1000)
  { ranking = 1; return "AMN"; }
  else if (sum < 1500)
  { ranking = 2; return "A1C"; }
  else if (sum < 2500)
  { ranking = 3; return "2ND LT"; }
  else if (sum < 3500)
  { ranking = 4; return "1ST LT"; }
  else if (sum < 5000)
  { ranking = 5; return "CAPT"; }
  else if (sum < 7000)
  { ranking = 6; return "MAJ"; }
  else if (sum < 9000)
  { ranking = 7; return "COL"; }
  else
  { ranking = 8; return "GEN"; }
}

Pilot::Pilot (char *name)
{
  tp = NULL;
  strcpy (this->name, name);
  load ();
}

Pilot::~Pilot ()
{
  int i;
  for (i = 0; i < 11; i ++)
    delete (tp [i]);
}



void PilotList::load (char *fname)
{
  char buf [16];
  int i;
  FILE *in = fopen (fname, "rb");
  if (in == NULL)
  {
    display ("Could not load saves/pilots", LOG_WARN);
    aktpilots = 0;
    add ("PLAYER");
    return;
  }
  fgets (buf, 16, in);
  aktpilots = atoi (buf);
  fgets (buf, 16, in);
  aktpilot = atoi (buf);
  int z = 0;
  while (fgets (buf, 16, in) && z < maxpilots)
  {
    for (i = 0; i < (int) strlen (buf); i ++)
      if (buf [i] == '\n')
        buf [i] = '\0';
    pilot [z] = new Pilot (buf);
    z ++;
  }
  fclose (in);
}

void PilotList::save (char *fname)
{
  char buf [256];
  int i;
  
  FILE *out = fopen (fname, "wb");
  if (out == NULL)
  {
    display ("Could not write saves/pilots", LOG_WARN);
    return;
  }
  sprintf (buf, "%d\n%d\n", aktpilots, aktpilot);
  fwrite (buf, 1, strlen (buf), out);
  for (i = 0; i < aktpilots; i ++)
  {
    sprintf (buf, "%s\n", pilot [i]->name);
    fwrite (buf, 1, strlen (buf), out);
  }
  fclose (out);
  
  for (i = 0; i < aktpilots; i ++)
  {
    pilot [i]->save ();
  }
}

PilotList::PilotList (char *fname)
{
  int i;
  for (i = 0; i < maxpilots; i ++)
    pilot [i] = NULL;
  load (fname);
}

PilotList::~PilotList ()
{
  int i;
  for (i = 0; i < aktpilots; i ++)
  {
    if (pilot [i] != NULL)
      delete pilot [i];
  }
}

void PilotList::rm ()
{
  int i, z = 0;
  for (i = 0; i < maxpilots; i ++)
  {
    if (pilot [i] != NULL) z ++;
  }
  if (z <= 1) return;
  if (pilot [aktpilot] != NULL)
  {
    char buf [4096];
    strcpy (buf, dirs->getSaves (pilot [aktpilot]->name));
    remove (buf);
    delete pilot [aktpilot];
    pilot [aktpilot] = NULL;
  }
  for (i = aktpilot; i < aktpilots; i ++)
    pilot [i] = pilot [i + 1];
  pilot [aktpilots] = NULL;
  aktpilots --;
  if (aktpilots < 0) aktpilots = 0;
  aktpilot = 0;
}

void PilotList::add (char *name)
{
  if (aktpilots >= maxpilots) return;
  aktpilot = aktpilots;
  pilot [aktpilot] = new Pilot (name);
  aktpilots ++;
}

#endif
