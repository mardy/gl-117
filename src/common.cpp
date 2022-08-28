/* This file includes all global definitions. */

#ifndef IS_COMMON_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "common.h"
#include "dirs.h"

void display_stream (FILE *stream, char *str, int level)
{
  if (level == LOG_NONE) fprintf (stream, "%s\n", str);
  else if (level == LOG_FATAL) fprintf (stream, "Fatal: %s\n", str);
  else if (level == LOG_ERROR) fprintf (stream, "Error: %s\n", str);
  else if (level == LOG_WARN) fprintf (stream, "Warning: %s\n", str);
  else if (level == LOG_MOST) fprintf (stream, "Info: %s\n", str);
  else fprintf (stream, "Debug: %s\n", str);
  fflush (stream);
}

FILE *display_out = NULL;

void display_exit ()
{
  if (display_out) fclose (display_out);
  display_out = NULL;
}

// display log/debug message
void display (char *str, int level)
{
  int len = strlen (str);
  if (!len) return;
  if (level <= debuglevel)
  {
    FILE *stream = stdout;
    if (level == LOG_FATAL || level == LOG_ERROR || level == LOG_WARN)
      stream = stderr;
    display_stream (stream, str, level);

    if (!display_out)
    {
      if (dirs)
        if ((display_out = fopen (dirs->getSaves ("logfile.txt"), "wt")) != NULL)
        {
          display_stream (display_out, str, level);
        }
    }
    else
    {
      display_stream (display_out, str, level);
    }
  }
}

// display "out of memory" error and exit
void error_outofmemory ()
{
  display ("Out of memory", LOG_FATAL);
  exit (EXIT_ALLOC);
}

void key2string (int key, char *buf)
{
  buf [0] = '\0';

  // try one byte keys
  if (key > 32 && key <= 'z') sprintf (buf, "%c", key);
  else if (key == KEY_SPACE) strcpy (buf, "SPACE");
  else if (key == KEY_TAB) strcpy (buf, "TAB");
  else if (key == KEY_ENTER) strcpy (buf, "ENTER");
  else if (key == KEY_BACKSPACE) strcpy (buf, "BSPACE");
  if (key < 256) return;

  // try special keys
  key -= 256;
  if (key == KEY_F1) strcpy (buf, "F1");
  else if (key == KEY_F2) strcpy (buf, "F2");
  else if (key == KEY_F3) strcpy (buf, "F3");
  else if (key == KEY_F4) strcpy (buf, "F4");
  else if (key == KEY_F5) strcpy (buf, "F5");
  else if (key == KEY_F6) strcpy (buf, "F6");
  else if (key == KEY_F7) strcpy (buf, "F7");
  else if (key == KEY_F8) strcpy (buf, "F8");
  else if (key == KEY_F9) strcpy (buf, "F9");
  else if (key == KEY_F10) strcpy (buf, "F10");
  else if (key == KEY_F10) strcpy (buf, "F11");
  else if (key == KEY_F10) strcpy (buf, "F12");
  else if (key == KEY_UP) strcpy (buf, "UP");
  else if (key == KEY_DOWN) strcpy (buf, "DOWN");
  else if (key == KEY_LEFT) strcpy (buf, "LEFT");
  else if (key == KEY_RIGHT) strcpy (buf, "RIGHT");
  else if (key == KEY_PGUP) strcpy (buf, "PGUP");
  else if (key == KEY_PGDOWN) strcpy (buf, "PGDOWN");
  else if (key == KEY_LALT) strcpy (buf, "LALT");
  else if (key == KEY_RALT) strcpy (buf, "RALT");
  else if (key == KEY_LCTRL) strcpy (buf, "LCTRL");
  else if (key == KEY_RCTRL) strcpy (buf, "RCTRL");
  else if (key == KEY_LSHIFT) strcpy (buf, "LSHIFT");
  else if (key == KEY_RSHIFT) strcpy (buf, "RSHIFT");
  else if (key == KEY_CAPSLOCK) strcpy (buf, "CAPS");
  else if (key == KEY_DELETE) strcpy (buf, "DEL");
  else if (key == KEY_HOME) strcpy (buf, "HOME");
  else if (key == KEY_INSERT) strcpy (buf, "INS");
  else if (key == KEY_END) strcpy (buf, "END");
}

void joystick2string (int button, char *buf)
{
  buf [0] = '\0';
  char joystick = 'A' + button / 1000;
  int b = button % 1000;

  // try one byte keys
  if (b >= 0 && b < 100) sprintf (buf, "%c BTN%d", joystick, b);
  else if (b >= 100) sprintf (buf, "%c HAT%d", joystick, b - 100);
}

#endif
