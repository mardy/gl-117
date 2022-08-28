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

/* This file contains all mission data and definitions (really trivial). */

#ifndef IS_MISSION_H
#define IS_MISSION_H

#include "model.h" // ok
#include "aiobject.h" // ok

#define MISSION_DEMO 0
#define MISSION_CAMPAIGN1 1
#define MISSION_TEST1 1
#define MISSION_TEST2 2
#define MISSION_TRANSPORT 10
#define MISSION_CONVOY 11
#define MISSION_DOGFIGHT2 12
#define MISSION_AIRBATTLE 13
#define MISSION_SADEFENSE 14
#define MISSION_SCOUT 15
#define MISSION_BASE 16
#define MISSION_DEPOT 17
#define MISSION_DEFEND1 20
#define MISSION_DOGFIGHT3 21
#define MISSION_TANK1 22
#define MISSION_CONVOY2 23
#define MISSION_SHIP1 25
#define MISSION_SHIP2 26
#define MISSION_SHIP3 27
#define MISSION_CANYON1 30
#define MISSION_CANYON2 31
#define MISSION_TUNNEL1 36
#define MISSION_CANYON3 32
#define MISSION_MOON1 33
#define MISSION_MOONBATTLE 37
#define MISSION_MOON2 34
#define MISSION_MOON3 35
#define MISSION_CAMPAIGN2 40
#define MISSION_TUTORIAL 80
#define MISSION_DOGFIGHT 81
#define MISSION_TUTORIAL2 82
#define MISSION_TUTORIAL3 83
#define MISSION_FREEFLIGHT1 84
#define MISSION_DEATHMATCH1 85
#define MISSION_DEATHMATCH2 86
#define MISSION_DEATHMATCH3 87
#define MISSION_TEAMBASE1 88
#define MISSION_WAVES1 89
#define MISSION_CUSTOM1 800
#define MISSION_CUSTOM2 999
#define MISSION_MULTIPLAYER 1000
#define MISSION_MULTIPLAYER_DOGFIGHT 1000

class Mission
{
  protected:
  void autoLFBriefing ();

  public:
  int timer; // mission timer
  int maxtime; // maximum time to get a time bonus
  int alliedfighters; // number of allied fighters
  char briefing [1024]; // briefing text
  char name [1024]; // mission title
  int id; // mission id
  int selfighter [3]; // fighter to choose
  int selfighters, wantfighter; // selected fighter
  int selweapon [3]; // weapon pack to choose
  int selweapons, wantweapon; // selected weapon pack
  int alliedpilot [10]; // id of allied pilots
  CColor textcolor; // standard text color to blend in during flight
  int difficulty; // difficulty level for THIS mission
  int heading; // define northern direction (the sun is at 0 degree)
  int state; // different mission states (individual meaning)
  int physics; // action=0 or simulation=1

  Mission ();
  virtual ~Mission () {}
  void playerInit ();
  void alliedInit (int fighterid, int pilotid, AIObj *aiobj);
  void init ();
  virtual void start (); // custom definitions for a mission
  virtual int processtimer (Uint32 dt); // custom definitions controlled by the timer, mission success/failure
  virtual void draw (); // custom definitions that have to be drawn
  void invertZ (); // invert Z coordinate of all fighters and objects
  void checkScore (int missionstate, int timebonus, int fighterkills, int shipkills, int tankkills, int otherkills, int shieldbonus, int points);
  int getScore (int missionstate, int timebonus, int fighterkills, int shipkills, int tankkills, int otherkills, int shieldbonus, int points);
  int getScore (int missionstate);
};

class MissionDemo1 : public Mission
{
  public:
  MissionDemo1 ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionTutorial1 : public Mission
{
  public:
  MissionTutorial1 ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionTutorial2 : public Mission
{
  public:
  MissionTutorial2 ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionTutorial3 : public Mission
{
  public:
  MissionTutorial3 ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionDogfight1 : public Mission
{
  public:
  int laststate, texttimer;
  MissionDogfight1 ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionFreeFlight1 : public Mission
{
  public:
  int laststate, texttimer;
  MissionFreeFlight1 ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionDeathmatch1 : public Mission
{
  public:
  int laststate, texttimer;
  MissionDeathmatch1 ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionDeathmatch2 : public Mission
{
  public:
  int laststate, texttimer;
  MissionDeathmatch2 ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionDeathmatch3 : public Mission
{
  public:
  int laststate, texttimer;
  MissionDeathmatch3 ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionTeamBase1 : public Mission
{
  public:
  int laststate, texttimer;
  int team1x, team1y, team2x, team2y;
  MissionTeamBase1 ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionWaves1 : public Mission
{
  public:
  int laststate, texttimer;
  MissionWaves1 ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionTest1 : public Mission
{
  public:
  MissionTest1 ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionTest2 : public Mission
{
  public:
  MissionTest2 ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionTransport : public Mission
{
  public:
  MissionTransport ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionConvoy : public Mission
{
  public:
  MissionConvoy ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionDogfight2 : public Mission
{
  public:
  MissionDogfight2 ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionAirBattle : public Mission
{
  public:
  MissionAirBattle ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionGround1 : public Mission
{
  public:
  MissionGround1 ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionScout : public Mission
{
  public:
  MissionScout ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionBase : public Mission
{
  public:
  MissionBase ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionDepot : public Mission
{
  public:
  MissionDepot ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionDefend1 : public Mission
{
  public:
  MissionDefend1 ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionDogfight3 : public Mission
{
  public:
  MissionDogfight3 ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionTank1 : public Mission
{
  public:
  MissionTank1 ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionConvoy2 : public Mission
{
  public:
  MissionConvoy2 ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionShip1 : public Mission
{
  public:
  MissionShip1 ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionShip2 : public Mission
{
  public:
  MissionShip2 ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionShip3 : public Mission
{
  public:
  MissionShip3 ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionCanyon1 : public Mission
{
  public:
  MissionCanyon1 ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionCanyon2 : public Mission
{
  public:
  MissionCanyon2 ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionCanyon3 : public Mission
{
  public:
  MissionCanyon3 ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionMoonDefense1 : public Mission
{
  public:
  MissionMoonDefense1 ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionMoonBattle : public Mission
{
  public:
  MissionMoonBattle ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionMoonDogfight1 : public Mission
{
  public:
  MissionMoonDogfight1 ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionTunnel1 : public Mission
{
  public:
  MissionTunnel1 ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionMoonBase1 : public Mission
{
  public:
  MissionMoonBase1 ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionMoon1 : public Mission
{
  public:
  MissionMoon1 ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

class MissionMultiDogfight1 : public Mission
{
  public:
  MissionMultiDogfight1 ();
  virtual void start ();
  virtual int processtimer (Uint32 dt);
  virtual void draw ();
};

extern Mission *mission;
extern Mission *missionnew;

#endif
