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

/* This file contains a map loader. */

#ifndef IS_MAPLOADER_H
#define IS_MAPLOADER_H

#include "aiobject.h" // ok
#include "mission.h" // ok

#define MAXMAPS 100
#define TOKENLEN 1024

class MapLoader
{
  public:
  int mapcount;
  int mapptr;
  char mapfile [MAXMAPS] [TOKENLEN];

  void readMapFile ();
  MapLoader ();

  void toUpper (char *str);

  int getMapNumber ();
  char *getNextMap ();

  Mission *getMission ();
};

extern MapLoader *maploader;

const int maxtext = 30;

class MissionCustom : public Mission
{
  public:
  int reterror;
  char token [TOKENLEN];
  int map_type;
  int save_map_type;
  int weather, clouds;
  AIObj obj [maxfighter];
  int numobjects;
  int relx, rely;
  char text [maxtext] [TOKENLEN];
  int textstart [maxtext], textdelay [maxtext];
  int numtext;
  char mapfile [TOKENLEN];
  float scaleheight;
  int tree [3];
  int minh;

  MissionCustom ();
  void error (char *msg);
  int readAttribute (char *attribute, char *value, char *casevalue);
  void toUpper (char *str);
  int isInteger (char *str);
  int isDouble (char *str);
  double toDouble (char *str);
  int isGlobal (char *token);
  int synchronize ();
  int parseMapData ();
  int parseObjectData ();
  int parseTextData ();
  int parseRelCoords ();
  int parseInfoData ();
  void getLakeMin (short x, short y);
  void setLakeMin (short x, short y);
  void init ();
  void start ();
  int processtimer (Uint32 dt);
  void draw ();
};

#endif
