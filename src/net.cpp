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

#include "net.h"
#include "main.h"

int port = 4567;
char *message;

#ifdef HAVE_SDL_NET
int getMsg (TCPsocket sock, char *buf)
{
  Uint32 len, result;

  // receive the length of the string message
  result = SDLNet_TCP_Recv (sock, &len, sizeof (len));
  if (result < sizeof (len))
  {
    if (SDLNet_GetError () && strlen (SDLNet_GetError ())) // sometimes blank!
      printf ("SDLNet_TCP_Recv: %s\n", SDLNet_GetError ());
    else 
      printf ("unknown error"); fflush (stdout);
    return 0;
  }
  
  // swap byte order to our local order
  len = SDL_SwapBE32 (len);
  
  // check if anything is strange, like a zero length buffer
  if (!len)
    return 0;

  // get the string buffer over the socket
  result = SDLNet_TCP_Recv (sock, buf, len);
  if (result<len)
  {
    if (SDLNet_GetError () && strlen (SDLNet_GetError ())) // sometimes blank!
      printf ("SDLNet_TCP_Recv: %s\n", SDLNet_GetError ());
    return 0;
  }

  return 1;
}
#endif

// send a string buffer over a TCP socket with error checking
// returns 0 on any errors, length sent on success
#ifdef HAVE_SDL_NET
int putMsg (TCPsocket sock, char *buf, int len)
{
  Uint32 result;

  // change endianness to network order
  len = SDL_SwapBE32 (len);

  // send the length of the string
  result = SDLNet_TCP_Send (sock, &len, sizeof(len));
  if (result < sizeof (len)) {
    if (SDLNet_GetError () && strlen (SDLNet_GetError ())) // sometimes blank!
      printf ("SDLNet_TCP_Send: %s\n", SDLNet_GetError ());
    return(0);
  }
  
  // revert to our local byte order
  len = SDL_SwapBE32 (len);
  
  // send the buffer, with the NULL as well
  result = SDLNet_TCP_Send (sock, buf, len);
  if (result < len)
  {
    if (SDLNet_GetError () && strlen (SDLNet_GetError ())) // sometimes blank!
      printf ("SDLNet_TCP_Send: %s\n", SDLNet_GetError ());
    return 0;
  }
  
  // return the length sent
  return result;
}
#endif

Server::Server()
{
#ifdef HAVE_SDL_NET
  num_clients = 0;
  id = 0;
  clients = NULL;
  set=SDLNet_AllocSocketSet(1);
  if(!set)
  {
    printf("SDLNet_AllocSocketSet: %s\n", SDLNet_GetError());
    SDLNet_Quit();
    SDL_Quit();
    exit(4); //most of the time this is a major error, but do what you want.
  }

  if(SDLNet_ResolveHost(&ip,NULL,port)==-1)
  {
    printf("SDLNet_ResolveHost: %s\n",SDLNet_GetError());
    SDLNet_Quit();
    SDL_Quit();
    exit(5);
  }

  server=SDLNet_TCP_Open(&ip);
  if(!server)
  {
    printf("SDLNet_TCP_Open: %s\n",SDLNet_GetError());
    SDLNet_Quit();
    SDL_Quit();
    exit(6);
  }

  if(SDLNet_TCP_AddSocket(set,server)==-1)
  {
    printf("SDLNet_TCP_AddSocket: %s\n",SDLNet_GetError());
    SDLNet_Quit();
    SDL_Quit();
    exit(7);
  }
#endif
}

int Server::getIP()
{
#ifdef HAVE_SDL_NET
  return ip.host;
#else 
  return 0;
#endif
}

void Server::removeClient(int id_client)
{
#ifdef HAVE_SDL_NET
/*  int i = -1;
  for (int a = 0; a < num_clients; a++)
    if (name == clients[i].name) i = a;
  
  if (i == -1) 
  {
    printf ("Not able to remove client: %d",ip);
    return;
  }*/
  int i = getClientFromID(id_client);

  if(i<0 && i>=num_clients)
    return;
  
  // close the old socket, even if it's dead...
  SDLNet_TCP_Close(clients[i].sock);
  
  num_clients--;
  if(num_clients>i)
    memmove(&clients[i], &clients[i+1], (num_clients-i)*sizeof(MyClient));
  clients=(MyClient*)realloc(clients, num_clients*sizeof(MyClient));
  // server side info
  printf("<-- REMOVE %s\n",name);
  if(name)
    free(name);
#endif
}

void Server::createSocketSet ()
{
#ifdef HAVE_SDL_NET
//   static SDLNet_SocketSet set=NULL;

  if(set)
    SDLNet_FreeSocketSet(set);
    set=SDLNet_AllocSocketSet(num_clients+1);
  if(!set) {
    printf("SDLNet_AllocSocketSet: %s\n", SDLNet_GetError());
    exit(1); //most of the time this is a major error, but do what you want.
  }
  SDLNet_TCP_AddSocket(set,server);
  for(int i=0;i<num_clients;i++)
    SDLNet_TCP_AddSocket(set,clients[i].sock);
#endif
}


void Server::getClient()
{
#ifdef HAVE_SDL_NET
  char buf [STDSIZE];
  TCPsocket newclient;
  IPaddress *remoteip;
  int active = SDLNet_CheckSockets (set, 0);
  if(SDLNet_SocketReady(server))
  {
    newclient=SDLNet_TCP_Accept(server);
    if(newclient)
    {
      clients=(MyClient*)realloc(clients, (num_clients+1)*sizeof(MyClient));
/*      if (!getMsg(newclient,&clients[num_clients].name)) printf ("name not received");
      else printf ("Name: %s", clients[num_clients].name);*/
      clients[num_clients].sock = newclient;
      id ++;
      clients[num_clients].id = id;
      // get the clients IP and port number
      remoteip = SDLNet_TCP_GetPeerAddress(clients[num_clients].sock);
      if(!remoteip)
      {
        printf("SDLNet_TCP_GetPeerAddress: %s\n",SDLNet_GetError());
      }
      sprintf (buf, "CLIENT ID=%d IP=%d PORT=%d", id + 1, remoteip->host, remoteip->port);
      strcpy (clients[num_clients].name, buf);
      num_clients++;
      // print out the clients IP and port number
/*      int ipaddr=SDL_SwapBE32(remoteip->host);
      printf("Accepted a connection from %d.%d.%d.%d port %hu\n",
        ipaddr>>24,
        (ipaddr>>16)&0xff,
        (ipaddr>>8)&0xff,
        ipaddr&0xff,
        remoteip->port);*/
      printf ("\n#Clients: %d ", num_clients); fflush (stdout);
      createSocketSet ();
      }
      else
        SDLNet_TCP_Close(newclient);
  }
#endif
}

int Server::getClientFromID(int id_client)
{
#ifdef HAVE_SDL_NET
  for (int i = 0; i < num_clients; i++)
  {
    if (clients[i].id == id_client) return i;
  }
#endif

  return 0;
}

int Server::getMessage(int id_client, char *buf)
{
#ifdef HAVE_SDL_NET
  
  int numready=SDLNet_CheckSockets(set, 0);
  if(numready==-1)
  {
    printf("SDLNet_CheckSockets: %s\n",SDLNet_GetError());
    return 0;
  }
  int n = getClientFromID(id_client);
  if(numready)
  {
    if (SDLNet_SocketReady(clients[n].sock))
    {
       if (getMsg (clients[n].sock, buf))
      {
        printf ("clientnummer: %d",n);
//        printf("Received: %.*s\n",len,message);
        return 1;
      }
      else 
      {
        removeClient(clients[n].id);
        return 0;
      }
    }    
    else 
    {
      return 0;
    }
  }
return 0;
#else
return 0;
#endif
}

int Server::checkStart()
{
#ifdef HAVE_SDL_NET

  int i;
  for (i = 0; i < num_clients; i++)
  {
    sendMessage (clients[i].id, "ready", 5);
  }
 printf ("Clients ready ??");

 int waittime = 0;
 char *buf;
 bool *ok = new bool [num_clients];
 for (i = 0; i < num_clients; i++) ok[i] = false;
 while (waittime < 600000)
 {
   waittime += SDL_GetTicks ();
   for (int i = 0; i < num_clients; i++)
   {
     if (getMessage (clients[i].id, buf))
     {
       if (strcmp (buf,"ready")) ok[i] = true;
     }
   }
   for (i = 0; i < num_clients; i++)
   {
     if (ok[i] == false) break;
     if (i == num_clients && ok[i] == true)
     {
       printf ("Clients are ready ");
       return 1;
     }
   }
 }
 printf ("Clients not ready");

#endif
 return 0;
}

void Server::sendMessage(int id_client, char *buf, int len)
{
#ifdef HAVE_SDL_NET

 int i = getClientFromID(id_client);
 if (!putMsg(clients[i].sock, buf, len))
 {
   removeClient (id_client);
 }

#endif
}



Client::Client ()
{
#ifdef HAVE_SDL_NET
  sock = NULL;
  set=SDLNet_AllocSocketSet(1);
  if(!set)
  {
    printf("SDLNet_AllocSocketSet: %s\n", SDLNet_GetError());
    SDLNet_Quit();
    SDL_Quit();
    exit(4); //most of the time this is a major error, but do what you want.
  }
#endif
}

bool Client::getServer(char *hostname, char *name)
{
#ifdef HAVE_SDL_NET

  printf("try join,");

  if (sock != NULL) return true;
  IPaddress ip;
  //if(SDLNet_ResolveHost(&ip,hostname,port)==-1)
  if(SDLNet_ResolveHost(&ip,hostname,port)==-1)
  {
    printf("SDLNet_ResolveHost: %s\n",SDLNet_GetError());
    SDLNet_Quit();
    SDL_Quit();
    exit(5);
  }

  // open the server socket
  sock=SDLNet_TCP_Open(&ip);
  if(!sock)
  {
    printf("SDLNet_TCP_Open: %s\n",SDLNet_GetError());
    return false;
//    SDLNet_Quit();
//    SDL_Quit();
//    exit(6);
  }
  printf("Connecting to %s port %d\n",hostname,port);

  if(SDLNet_TCP_AddSocket(set,sock)==-1)
  {
    printf("SDLNet_TCP_AddSocket: %s\n",SDLNet_GetError());
    SDLNet_Quit();
    SDL_Quit();
    exit(7);
  }
  
  // login with a name
/*  if(!putMsg(sock,name))
  {
    SDLNet_TCP_Close(sock);
    SDLNet_Quit();
    SDL_Quit();
    exit(8);
  }*/
return true;
#else
return false;
#endif

}

int Client::getMessage(char *buf)
{
#ifdef HAVE_SDL_NET
  
  int numready=SDLNet_CheckSockets(set, 0);
  if(numready==-1)
  {
    printf("SDLNet_CheckSockets: %s\n",SDLNet_GetError());
    return 0;
  }
  if(numready)
  {
    if (SDLNet_SocketReady(sock))
    {
      if (getMsg (sock, buf))
      {
//        printf ("clientnummer: %d",n);
//        printf("Received: %.*s\n",len,message);
        if (strcmp (buf,"ready"))
        {
          sendMessage("ready", 5);
        }
        return 1;
      }
      else 
      {
        printf ("Server lost, closing connection...");  
        SDLNet_TCP_Close(sock);
        sock = NULL;
        return 0;
      }
    }    
    else 
    {
      printf ("Socket not ready");  
      return 0;
    }
  }
// getch();
// printf ("numready = 0");  
return 0;
#else
return 0;
#endif
}

void Client::sendMessage(char *buf, int len)
{
#ifdef HAVE_SDL_NET
  if (!putMsg(sock, buf, len)) printf ("FEHLER");
#endif
}



#endif
