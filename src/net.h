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

/* This file includes multiplayer support. Copyright Norbert Drexl. */

#ifndef IS_NET_H
#define IS_NET_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h" // ok

#ifdef HAVE_SDL_NET
extern int getMsg(TCPsocket sock, char *buf);
extern int putMsg(TCPsocket sock, char *buf);
#endif

extern int port;
extern char *message;

class Client
{
  public:
  bool isready;
  Client();
  bool getServer(char *hostname, char *name);
  char *name;
  void sendMessage (char *buf, int len);
  int getMessage(char *buf);
  void sendReady();
//  private:
#ifdef HAVE_SDL_NET
  IPaddress ip;
  TCPsocket sock;
  SDLNet_SocketSet set;
#endif
};

class Server
{
  public:
  bool isready;
#ifdef HAVE_SDL_NET
  typedef struct
  {
    char name [100];
    TCPsocket sock;
    int id;
  } MyClient;
#endif
  Server();
  char *name;
  int getMessage(int ip_client, char *buf);
  int getClientFromID(int id_client);
  int getIP();
  void removeClient(int id_client);
  void getClient();
  void createSocketSet();
  void sendMessage (int ip_client, char *message, int len);

  int checkStart(); // 1 all Clients ready, 0 Clients not ready

  int id;

//  private:
  int num_clients;
#ifdef HAVE_SDL_NET
  MyClient *clients;
  TCPsocket server;
  IPaddress ip;
  SDLNet_SocketSet set;
#endif
};

extern Server *server;
extern Client *client;

#endif
