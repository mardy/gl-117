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

/* This file handles als input/output directories. */

#ifndef IS_DIRS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef _MSC_VER // no microsoft?
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

#include "dirs.h"
#include "common.h"

char *mystrtok (char *str, int len, char *tok)
{
  char *str0 = str;
  int z = 0;
  while (*str != tok [0])
  {
    str ++;
    z ++;
    if (z >= len)
      return NULL;
  }
  *str = 0;
  return str0;
}

void Dirs::append (char *target, char *str)
{
#ifdef _MSC_VER
  if (target [strlen (target) - 1] != '\\')
    strcat (target, "\\");
  strcat (target, str);
  strcat (target, "\\");
#else
  if (target [strlen (target) - 1] != '/')
    strcat (target, "/");
  strcat (target, str);
  strcat (target, "/");
#endif
}

Dirs::Dirs (char *arg)
{
#ifdef _MSC_VER
  char path [4096];
  int bscount = 0;
  strcpy (path, arg);
  char *p;
  for (p = path; p < path + strlen (path); p ++)
    if (*p == '\\')
      bscount ++;
  if (bscount >= 2)
  {
    p = path + strlen (path);
    while (*p != '\\') p --;
    p --;
    while (*p != '\\') p --;
    if (p [1] == 'D' && p [2] == 'e' && p [3] == 'b')
    {
      p --;
      while (*p != '\\') p --;
    }
    *p = 0;
  }
  else
  {
    display ("Binary file has no context to the data files", LOG_FATAL);
    display ("Do not execute from console, just doubleclick", LOG_MOST);
    exit (EXIT_CONTEXT);
  }
  strcpy (textures, path);
  strcpy (music, path);
  strcpy (sound, path);
  strcpy (models, path);
  strcpy (saves, path);
  strcpy (maps, path);
  append (saves, "saves");
#else
  char buf [STDSIZE];
  char *home = getenv ("HOME");
  char *env = getenv ("GL117");
  char *path = getenv ("PATH");
  char myfile [4096];
#ifdef __APPLE__
  strcpy(myfile, DATADIR);
#else
  sprintf (myfile, "%s/gl-117", DATADIR);
#endif
  bool founddir = false;
  struct stat mystat;

  if (!stat (myfile, &mystat))
  {
    if (S_ISDIR (mystat.st_mode))
    {
      founddir = true;
    }
  }
  if (!founddir)
  {

//    FILE *in;
    char cwd [4096];
    getcwd (cwd, 4096); // get current working directory
    char mypath [8092];
    strcpy (mypath, cwd);
    strcat (mypath, ":");
    strcat (mypath, path);
    char *p = mypath;
    int pathlen = strlen (mypath);

    if (arg [0] != '/')
    {
      p = mystrtok (p, (int) (path + pathlen - p), ":");
      while (p + strlen (p) - 1 < mypath + pathlen)
      {
        strcpy (myfile, p);
        if (myfile [strlen (myfile) - 1] != '/')
          strcat (myfile, "/");
        if (*arg == '.' && *(arg+1) == '/')
          strcat (myfile, arg + 2);
        else
          strcat (myfile, arg);
        if (!stat (myfile, &mystat))
        {
	        if (S_ISREG (mystat.st_mode))
	        {
            goto found;
          }
        }
        p = mystrtok (p + strlen (p) + 1, (int) (path + pathlen - p), ":");
      }
    }
    display ("Binary file has no context to the data files.", LOG_FATAL);
    exit (EXIT_CONTEXT);

  found:;
    int bscount = 0;
    for (p = myfile; p < myfile + strlen (myfile); p ++)
      if (*p == '/')
        bscount ++;
    if (bscount >= 2)
    {
      p = myfile + strlen (myfile);
      while (*p != '/') p --;
      p --;
      while (*p != '/') p --;
      if (p [1] == '.' && p [2] == '/')
      {
        p --;
        while (*p != '/') p --;
      }
      p ++;
      *p = 0;
    }
    else
    {
      display ("Binary file has no context to the data files.", LOG_FATAL);
      exit (EXIT_CONTEXT);
    }

  } // if (!founddir)

  if (home != NULL)
  {
    strcpy (saves, home);
    append (saves, ".gl-117");
    if (stat (saves, &mystat))
    {
      mkdir (saves, S_IRWXU);
    }
  }
  else
  {
    if (env != NULL)
    {
      strcpy (saves, env);
    }
    else
    {
      strcpy (saves, "..");
    }
  }
  if (env != NULL)
  {
    strcpy (textures, env);
    strcpy (music, env);
    strcpy (sound, env);
    strcpy (models, env);
    strcpy (maps, env);
  }
  else
  {
    sprintf (buf, "Found gl-117 data directory %s ", myfile);
    display (buf, LOG_MOST);
    strcpy (textures, myfile);
    strcpy (music, myfile);
    strcpy (sound, myfile);
    strcpy (models, myfile);
    strcpy (maps, myfile);
  }
#endif
  append (textures, "textures");
  append (music, "music");
  append (sound, "sounds");
  append (models, "models");
  append (maps, "maps");
}

Dirs::~Dirs () {}

char *Dirs::getSaves (char *name)
{
  strcpy (dir, saves);
  strcat (dir, name);
  return dir;
}

char *Dirs::getTextures (char *name)
{
  strcpy (dir, textures);
  strcat (dir, name);
  return dir;
}

char *Dirs::getMusic (char *name)
{
  strcpy (dir, music);
  strcat (dir, name);
  return dir;
}

char *Dirs::getSounds (char *name)
{
  strcpy (dir, sound);
  strcat (dir, name);
  return dir;
}

char *Dirs::getModels (char *name)
{
  strcpy (dir, models);
  strcat (dir, name);
  return dir;
}

char *Dirs::getMaps (char *name)
{
  strcpy (dir, maps);
  strcat (dir, name);
  return dir;
}

#endif
