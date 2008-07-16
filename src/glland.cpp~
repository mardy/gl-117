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

/* This file includes a random landscape generator. */

#ifndef IS_GLLAND_H

#include "glland.h"
#include "main.h"
#include "mathtab.h"
#include "gl.h"
#include "common.h"

#include <stdlib.h>

const float zoomz = 1.0/(100.0*MAXX);
const float hh = (float) 1 / (float) MAXX;
const float zoomz2 = 32768.0 * zoomz;
const float hh2 = 2.0*hh;
CTexture *texgrass, *texrocks, *texwater, *textree, *textree2, *textree3, *texcactus1, *texredstone;
CTexture *textreeu, *textreeu2, *textreeu3, *textreeu4, *textreeu5, *texcactusu1;
CTexture *textree4, *textree5, *texearth, *texsand, *texredsand, *texgravel1;
CTexture *texglitter1;

VertexArray *va;
VertexArray vertexarrayquadstrip;
VertexArray vertexarrayquad [20];
VertexArray vertexarraytriangle [20];
VertexArray vertexarrayglitter [2];

class IndexCounter
{
  public:
  int index [100];
  int n;

  IndexCounter ()
  {
    init ();
  }

  ~IndexCounter ()
  {
  }

  void init ()
  {
    n = 0;
    memset (index, 0, 100 * sizeof (int));
  }

  int get (int index1)
  {
    int i;
    for (i = 0; i < n; i ++)
    {
      if (index [i] == index1)
        return i;
    }
    index [n] = index1;
    n ++;
    return n - 1;
  }
};

IndexCounter ic;

void GLLandscape::norm (float *c)
{
  float n;
  n = 1.0/sqrt(c[0]*c[0]+c[1]*c[1]+c[2]*c[2]);
  c[0] *= n;
  c[1] *= n;
  c[2] *= n;
}

void GLLandscape::normalcrossproduct( float* a, float* b, float*c )
{
  c[0] = a[1]*b[2] - a[2]*b[1];
  c[1] = a[2]*b[0] - a[0]*b[2];
  c[2] = a[0]*b[1] - a[1]*b[0];
  norm (c);
}

int GLLandscape::selectColor (int x, int y)
{
  return f [x] [y];
/*  if (f [x] [y] == GRASS) return 0;
  else if (f [x] [y] >= CONIFEROUSWOODS1 && f [x] [y] <= MIXEDWOODS3) return 1;
  else if (f [x] [y] == ROCKS) return 2;
  else if (f [x] [y] == GLACIER) return 3;
  else if (f [x] [y] >= DWARFPINES1 && f [x] [y] <= BUSHES3) return 1;
  else if (f [x] [y] == WATER) return 5;
  else if (f [x] [y] == SHALLOWWATER) return 4;
  else if (f [x] [y] == DEEPWATER) return 6;
  else if (f [x] [y] == ROCKS2) return 7;
  else if (f [x] [y] == XSHALLOWWATER) return 8;
  else if (f [x] [y] == XDEEPWATER) return 9;
  else if (f [x] [y] == MOONSAND) return 10;
  else if (f [x] [y] == REDSTONE) return 11;
  else if (f [x] [y] == REDSAND || f [x] [y] == REDTREE0) return 12;
  else if (f [x] [y] == DESERTSAND || f [x] [y] == CACTUS0) return 13;
  else if (f [x] [y] == GREYSAND) return 14;
  else if (f [x] [y] == GRAVEL) return 15;
  else if (f [x] [y] == TOWN) return 16;
  else return 0;*/
}

void GLLandscape::precalculate ()
{
  int i, i2, i3, i4, x, z;
  float no[3];
  float nw[3];
  float so[3];
  float sw[3];
  float a[3];
  float c[3];

  // initialize dynamic light mask
  memset (dl, 0, (MAXX + 1) * (MAXX + 1));

  norm (lv); // normalize light vector

  lightfac = (0.002 * contrast + 1.0) * 0.001 / 256.0;

  // generate greyish material values for night if necessary
  float matgrey [MAXMATERIAL] [4];
  if (!day)
  {
    float greyfactor = 0.75; // 0 = grey, 1 = no modification
    for (i = 0; i < MAXMATERIAL; i ++)
    {
      float mid = (mat [i] [0] + mat [i] [1] + mat [i] [2]) / 3.0F;
      matgrey [i] [0] = mid + (mat [i] [0] - mid) * greyfactor;
      matgrey [i] [1] = mid + (mat [i] [1] - mid) * greyfactor;
      matgrey [i] [2] = mid + (mat [i] [2] - mid) * greyfactor;
    }
  }

  // create coarse height map
  for (x = 0; x <= MAXX - 4; x += 4)
    for (z = 0; z <= MAXX - 4; z += 4)
    {
      unsigned short max = 0, min = 65535;
      for (i = 0; i <= 3; i ++)
        for (i2 = 0; i2 <= 3; i2 ++)
        {
          if (hw [x + i] [z + i2] > max) max = hw [x + i] [z + i2];
          if (hw [x + i] [z + i2] < min) min = hw [x + i] [z + i2];
        }
      hcmax [x / 4] [z / 4] = max;
      hcmin [x / 4] [z / 4] = min;
    }

  // set the colors of the landscape
  hastowns = false;
  float mzoom = zoomz;
  for (x=0; x<=MAXX; x++)
    for (z=0; z<=MAXX; z++)
    {
      int a;
      a = f [x] [z];
      if (type == 2 && a == 4)
      {
        a = 11;
      }
      else if (type == 4 && a == 4)
      {
        a = 2;
      }
      else if (a == 16)
      {
        hastowns = true;
      }
      if (day)
      {
        r [x] [z] = (unsigned char) (mat [a] [0] * 255.9);
        g [x] [z] = (unsigned char) (mat [a] [1] * 255.9);
        b [x] [z] = (unsigned char) (mat [a] [2] * 255.9);
      }
      else
      {
        r [x] [z] = (unsigned char) (matgrey [a] [0] * 255.9);
        g [x] [z] = (unsigned char) (matgrey [a] [1] * 255.9);
        b [x] [z] = (unsigned char) (matgrey [a] [2] * 255.9);
      }
    }

  long sum;
  // smooth the colors (obsolete)
/*  long g3[3][3]={{0,1,0},
        {1,4,1},
        {0,1,0}};
  for (i = 1; i < MAXX; i ++)
    for (i2 = 1; i2 < MAXX; i2 ++)
    {
      sum = 0;
      for (i3 = 0; i3 < 3; i3 ++)
        for (i4 = 0; i4 < 3; i4 ++)
        {
          sum += g3[i3][i4] * r[i+i3-1][i2+i4-1];
        }
      sum /= 8;
      lg[i][i2]=sum;
    }
  for (i = 1; i <= MAXX - 1; i ++)
    for (i2 = 1; i2 <= MAXX - 1; i2 ++)
    {
      r [i] [i2] = (unsigned char) lg [i] [i2];
    }
  for (i = 1; i < MAXX; i ++)
    for (i2 = 1; i2 < MAXX; i2 ++)
    {
      sum = 0;
      for (i3 = 0; i3 < 3; i3 ++)
        for (i4 = 0; i4 < 3; i4 ++)
        {
          sum += g3[i3][i4] * g[i+i3-1][i2+i4-1];
        }
      sum /= 8;
      lg[i][i2]=sum;
    }
  for (i = 1; i <= MAXX - 1; i ++)
    for (i2 = 1; i2 <= MAXX - 1; i2 ++)
    {
      g [i] [i2] = (unsigned char) lg [i] [i2];
    }
  for (i = 1; i < MAXX; i ++)
    for (i2 = 1; i2 < MAXX; i2 ++)
    {
      float sum = 0;
      for (i3 = 0; i3 < 3; i3 ++)
        for (i4 = 0; i4 < 3; i4 ++)
        {
          sum += g3[i3][i4] * b[i+i3-1][i2+i4-1];
        }
      sum /= 8;
      lg[i][i2]=sum;
    }
  for (i = 1; i <= MAXX - 1; i ++)
    for (i2 = 1; i2 <= MAXX - 1; i2 ++)
    {
      b [i] [i2] = (unsigned char) lg [i] [i2];
    }*/

  // Set the height mask for the lowest sunrays touching the landscape's surface
  // This is just an approximation presuming the sun is a vertical line
  float m1 = mzoom / hh;
  float ih = tan ((sungamma + 5) * PI / 180) / m1; // 0 degree vertical sun radius
  for (x = 0; x <= MAXX; x ++)
  {
    float rayheight = hw [x] [MAXX];
    for (z = MAXX; z >= 0; z --)
    {
      unsigned short maxheight = (int) rayheight;
      if (rayheight < hw [x] [z])
      {
        rayheight = hw [x] [z];
        maxheight = hw [x] [z];
      }
      rayheight -= ih;
      hray [x] [z] = maxheight;
    }
  }

  // precalculate water light, always the same angle
  int nlwater = 1200 - (int) (1000.0 * 2.0 * fabs ((90.0 - sungamma) * PI / 180.0) / PI);

  // precalculate a height average
  int midheight = (highestpoint + lowestpoint) / 2;

  // set minimum ambient light
  int minambient = (int) (100.0 + sungamma * 4);
  if (!day) minambient = 100;
  if (minambient > 350) minambient = 350;

  // Set the luminance of the landscape
  for (x = 0; x <= MAXX; x ++)
    for (z = 0; z <= MAXX; z ++)
    {
      int xm1 = GETCOORD(x - 1);
      int xp1 = GETCOORD(x + 1);
      int zm1 = GETCOORD(z - 1);
      int zp1 = GETCOORD(z + 1);

      // Calculate the normal vectors
      a[0] = 0;
      a[1] = (float) (hw[x][zm1] - hw[x][z]) * mzoom;
      a[2] = -hh;
      c[0] = -hh;
      c[1] = (float) (hw[xm1][z] - hw[x][z]) * mzoom;
      c[2] =  0;
      normalcrossproduct( a, c, nw );
      a[0] = hh;
      a[1] = (float) (hw[xp1][z] - hw[x][z]) * mzoom;
      a[2] = 0;
      c[0] = 0;
      c[1] = (float) (hw[x][zm1] - hw[x][z]) * mzoom;
      c[2] = -hh;
      normalcrossproduct( a, c, no );
      a[0] = 0;
      a[1] = (float) (hw[x][zp1] - hw[x][z]) * mzoom;
      a[2] = hh;
      c[0] = hh;
      c[1] = (float) (hw[xp1][z] - hw[x][z]) * mzoom;
      c[2] = 0;
      normalcrossproduct( a, c, so );
      a[0] = -hh;
      a[1] = (float) (hw[xm1][z] - hw[x][z]) * mzoom;
      a[2] =  0;
      c[0] = 0;
      c[1] = (float) (hw[x][zp1] - hw[x][z]) * mzoom;
      c[2] = hh;
      normalcrossproduct( a, c, sw );
      float normx = (no[0] + nw[0] + so[0] + sw[0]) / 4.0;
      float normy = (no[1] + nw[1] + so[1] + sw[1]) / 4.0;
      float normz = (no[2] + nw[2] + so[2] + sw[2]) / 4.0;

      // Calculate the light hitting the surface
      float gamma = (float) acos (normx * lv [0] + normy * lv [1] + normz * lv [2]); // angle
      if (!isWater (f [x] [z]))
      {
        nl [x] [z] = 1200 - (int) (900.0 * 2.0 * fabs (gamma) / PI); // calculate light
        if (type == LAND_CANYON) // in canyons more ambient light in higher regions
          nl [x] [z] += (h [x] [z] - midheight) / 40; // typical max height diff is 10000
      }
      else
      {
        nl [x] [z] = nlwater; // precalculated light (above)
      }
      if (nl [x] [z] < minambient) // minimum ambient light
        nl [x] [z] = minambient;
      // Check whether this point is in the shadow of some mountain
      if (hw [x] [z] < hray [x] [z])
      {
        nl [x] [z] /= 2;
        if (nl [x] [z] < minambient)
          nl [x] [z] = minambient; // minimum ambient light
      }
    }

  // Smooth the luminance (very important)
  long g3_1[3][3]={{1,2,1},
                   {2,4,2},
                   {1,2,1}};
  for (i = 1; i <= MAXX - 1; i ++)
  {
    for (i2 = 1; i2 <= MAXX - 1; i2 ++)
    {
      sum = 0;
      for (i3 = 0; i3 < 3; i3 ++)
        for (i4 = 0; i4 < 3; i4 ++)
        {
          sum += g3_1[i3][i4] * nl[i+i3-1][i2+i4-1];
        }
      sum /= 16;
      lg[i][i2] = (unsigned short) sum;
    }
  }
  for (i = 1; i <= MAXX - 1; i ++)
    for (i2 = 1; i2 <= MAXX - 1; i2 ++)
    {
      nl [i] [i2] = lg [i] [i2];
    }

  // Assign textures: tex1 = quad texture
  // if tex2 defined: tex1 = upper triangle texture, tex2 = lower triangle texture
  for (i = 0; i < MAXX; i ++)
    for (i2 = 0; i2 < MAXX; i2 ++)
    {
      drawrule [i] [i2] = 0;
      
      int f1 = f [i] [i2], f2 = f [i + 1] [i2], f3 = f [i] [i2 + 1], f4 = f [i + 1] [i2 + 1];
      if (isType (f1, GRASS))
      {
        tex1 [i] [i2] = texgrass->textureID;
      }
      else if (isWoods (f1) || isType (f1, MOONSAND) || isType (f1, REDSAND) || isType (f1, REDTREE0) || isType (f1, CACTUS0) || isType (f1, GREYSAND))
      {
        tex1 [i] [i2] = texredsand->textureID;
      }
      else if (isType (f1, GRAVEL))
      {
        tex1 [i] [i2] = texgravel1->textureID;
      }
      else if (isWater (f1))
      {
        if (type == 0 || type == 2)
          tex1 [i] [i2] = texgrass->textureID;
        else
          tex1 [i] [i2] = 0xFF;
      }
      else if (isType (f1, ROCKS) || isType (f1, ROCKS2))
      {
        tex1 [i] [i2] = texrocks->textureID;
      }
      else if (isType (f1, REDSTONE))
      {
        tex1 [i] [i2] = texredstone->textureID;
      }
      else if (isType (f1, DESERTSAND))
      {
        tex1 [i] [i2] = texsand->textureID;
      }
      else
      {
        tex1 [i] [i2] = 0xFF;
      }

      if (!isGlacier (f1) && isGlacier (f2) && isGlacier (f3) && isGlacier (f4))
      { drawrule [i] [i2] = 1; tex1 [i] [i2] = texrocks->textureID; tex2 [i] [i2] = 0xFF; }
      if (isGlacier (f1) && !isGlacier (f2) && !isGlacier (f3) && !isGlacier (f4))
      { drawrule [i] [i2] = 1; tex1 [i] [i2] = texrocks->textureID; tex2 [i] [i2] = texrocks->textureID; }
      if (!isGlacier (f1) && !isGlacier (f2) && !isGlacier (f3) && isGlacier (f4))
      { drawrule [i] [i2] = 1; tex1 [i] [i2] = texrocks->textureID; tex2 [i] [i2] = texrocks->textureID; }
      if (isGlacier (f1) && isGlacier (f2) && isGlacier (f3) && !isGlacier (f4))
      { drawrule [i] [i2] = 1; tex1 [i] [i2] = 0xFF; tex2 [i] [i2] = texrocks->textureID; }
      
      if (isGlacier (f1) && !isGlacier (f2) && isGlacier (f3) && isGlacier (f4))
      { drawrule [i] [i2] = 2; tex1 [i] [i2] = texrocks->textureID; tex2 [i] [i2] = 0xFF; }
      if (!isGlacier (f1) && isGlacier (f2) && !isGlacier (f3) && !isGlacier (f4))
      { drawrule [i] [i2] = 2; tex1 [i] [i2] = texrocks->textureID; tex2 [i] [i2] = texrocks->textureID; }
      if (!isGlacier (f1) && !isGlacier (f2) && isGlacier (f3) && !isGlacier (f4))
      { drawrule [i] [i2] = 2; tex1 [i] [i2] = texrocks->textureID; tex2 [i] [i2] = texrocks->textureID; }
      if (isGlacier (f1) && isGlacier (f2) && !isGlacier (f3) && isGlacier (f4))
      { drawrule [i] [i2] = 2; tex1 [i] [i2] = 0xFF; tex2 [i] [i2] = texrocks->textureID; }
    }

}

// Get height over landscape/water, no interpolation (fast)
float GLLandscape::getMinHeight (float x, float z)
{
  int mx = GETCOORD((int)floor (x));
  int mz = GETCOORD((int)floor (z));
  return (ZOOM * ((float)hcmin[mx/4][mz/4]*zoomz - zoomz2));
}

// Get height over landscape/water, no interpolation (fast)
float GLLandscape::getMaxHeight (float x, float z)
{
  int mx = GETCOORD((int)floor (x));
  int mz = GETCOORD((int)floor (z));
  return (ZOOM * ((float)hcmax[mx/4][mz/4]*zoomz - zoomz2));
}

// Get height over landscape/water, no interpolation (fast)
float GLLandscape::getHeight (float x, float z)
{
  int mx = GETCOORD((int)floor (x));
  int mz = GETCOORD((int)floor (z));
  return (ZOOM * ((float)hw[mx][mz]*zoomz - zoomz2));
}

// Get height over landscape/water, linear interpolation (slow)
float GLLandscape::getExactHeight2 (float x, float z)
{
  float mx = x;
  float mz = z;
  int mx1 = (int) floor (mx);
  int mz1 = (int) floor (mz);
  mx1 -= mx1 & 1;
  mz1 -= mz1 & 1;
  int mx2 = mx1 + 2;
  int mz2 = mz1 + 2;
  int ax1 = GETCOORD(mx1);
  int ax2 = GETCOORD(mx2);
  int az1 = GETCOORD(mz1);
  int az2 = GETCOORD(mz2);
  float h2 = (float)hw[ax1][az1]*((float)mx2-mx)*((float)mz2-mz) + (float)hw[ax2][az1]*(mx-mx1)*((float)mz2-mz) +
             (float)hw[ax1][az2]*((float)mx2-mx)*(mz-mz1) + (float)hw[ax2][az2]*(mx-mx1)*(mz-mz1);
  return (ZOOM * (h2/4*zoomz - zoomz2));
}

// Get height over landscape/water, linear interpolation (slow)
float GLLandscape::getExactHeight3 (float x, float z)
{
  float mx = x;
  float mz = z;
  int mx1 = (int) floor (mx);
  int mz1 = (int) floor (mz);
  mx1 -= mx1 % 3;
  mz1 -= mz1 % 3;
  int mx2 = mx1 + 3;
  int mz2 = mz1 + 3;
  int ax1 = GETCOORD(mx1);
  int ax2 = GETCOORD(mx2);
  int az1 = GETCOORD(mz1);
  int az2 = GETCOORD(mz2);
  float h2 = (float)hw[ax1][az1]*((float)mx2-mx)*((float)mz2-mz) + (float)hw[ax2][az1]*(mx-mx1)*((float)mz2-mz) +
             (float)hw[ax1][az2]*((float)mx2-mx)*(mz-mz1) + (float)hw[ax2][az2]*(mx-mx1)*(mz-mz1);
  return (ZOOM * (h2/9*zoomz - zoomz2));
}

// Get height over landscape/water, linear interpolation (slow)
float GLLandscape::getExactHeight4 (float x, float z)
{
  float mx = x;
  float mz = z;
  int mx1 = (int) floor (mx);
  int mz1 = (int) floor (mz);
  mx1 -= mx1 & 3;
  mz1 -= mz1 & 3;
  int mx2 = mx1 + 4;
  int mz2 = mz1 + 4;
  int ax1 = GETCOORD(mx1);
  int ax2 = GETCOORD(mx2);
  int az1 = GETCOORD(mz1);
  int az2 = GETCOORD(mz2);
  float h2 = (float)hw[ax1][az1]*((float)mx2-mx)*((float)mz2-mz) + (float)hw[ax2][az1]*(mx-mx1)*((float)mz2-mz) +
             (float)hw[ax1][az2]*((float)mx2-mx)*(mz-mz1) + (float)hw[ax2][az2]*(mx-mx1)*(mz-mz1);
  return (ZOOM * (h2/16*zoomz - zoomz2));
}

// Get height over landscape/water, linear interpolation (slow)
float GLLandscape::getExactHeight (float x, float z)
{
  if (gridstep == 2) return getExactHeight2 (x, z);
  else if (gridstep == 3) return getExactHeight3 (x, z);
  else if (gridstep == 4) return getExactHeight4 (x, z);
  float mx = x;
  float mz = z;
  int mx1 = (int) floor (mx);
  int mz1 = (int) floor (mz);
  int mx2 = mx1 + 1;
  int mz2 = mz1 + 1;
  int ax1 = GETCOORD(mx1);
  int ax2 = GETCOORD(mx2);
  int az1 = GETCOORD(mz1);
  int az2 = GETCOORD(mz2);
  float h2 = (float)hw[ax1][az1]*((float)mx2-mx)*((float)mz2-mz) + (float)hw[ax2][az1]*(mx-mx1)*((float)mz2-mz) +
             (float)hw[ax1][az2]*((float)mx2-mx)*(mz-mz1) + (float)hw[ax2][az2]*(mx-mx1)*(mz-mz1);
  return (ZOOM * (h2*zoomz - zoomz2));
}

// Get height over landscape/water without ZOOM scaling, linear interpolation (slow)
// Only used to draw trees
// Get height over landscape/water, linear interpolation (slow)
float GLLandscape::getExactLSHeight2 (float x, float z)
{
  float mx = x;
  float mz = z;
  int mx1 = (int) floor (mx);
  int mz1 = (int) floor (mz);
  mx1 -= mx1 & 1;
  mz1 -= mz1 & 1;
  int mx2 = mx1 + 2;
  int mz2 = mz1 + 2;
  int ax1 = GETCOORD(mx1);
  int ax2 = GETCOORD(mx2);
  int az1 = GETCOORD(mz1);
  int az2 = GETCOORD(mz2);
  float h2 = (float)hw[ax1][az1]*((float)mx2-mx)*((float)mz2-mz) + (float)hw[ax2][az1]*(mx-mx1)*((float)mz2-mz) +
             (float)hw[ax1][az2]*((float)mx2-mx)*(mz-mz1) + (float)hw[ax2][az2]*(mx-mx1)*(mz-mz1);
  return (h2/4*zoomz - zoomz2);
}

// Get height over landscape/water without ZOOM scaling, linear interpolation (slow)
// Only used to draw trees
// Get height over landscape/water, linear interpolation (slow)
float GLLandscape::getExactLSHeight3 (float x, float z)
{
  float mx = x;
  float mz = z;
  int mx1 = (int) floor (mx);
  int mz1 = (int) floor (mz);
  mx1 -= mx1 % 3;
  mz1 -= mz1 % 3;
  int mx2 = mx1 + 3;
  int mz2 = mz1 + 3;
  int ax1 = GETCOORD(mx1);
  int ax2 = GETCOORD(mx2);
  int az1 = GETCOORD(mz1);
  int az2 = GETCOORD(mz2);
  float h2 = (float)hw[ax1][az1]*((float)mx2-mx)*((float)mz2-mz) + (float)hw[ax2][az1]*(mx-mx1)*((float)mz2-mz) +
             (float)hw[ax1][az2]*((float)mx2-mx)*(mz-mz1) + (float)hw[ax2][az2]*(mx-mx1)*(mz-mz1);
  return (h2/9*zoomz - zoomz2);
}

// Get height over landscape/water without ZOOM scaling, linear interpolation (slow)
// Only used to draw trees
// Get height over landscape/water, linear interpolation (slow)
float GLLandscape::getExactLSHeight4 (float x, float z)
{
  float mx = x;
  float mz = z;
  int mx1 = (int) floor (mx);
  int mz1 = (int) floor (mz);
  mx1 -= mx1 & 3;
  mz1 -= mz1 & 3;
  int mx2 = mx1 + 4;
  int mz2 = mz1 + 4;
  int ax1 = GETCOORD(mx1);
  int ax2 = GETCOORD(mx2);
  int az1 = GETCOORD(mz1);
  int az2 = GETCOORD(mz2);
  float h2 = (float)hw[ax1][az1]*((float)mx2-mx)*((float)mz2-mz) + (float)hw[ax2][az1]*(mx-mx1)*((float)mz2-mz) +
             (float)hw[ax1][az2]*((float)mx2-mx)*(mz-mz1) + (float)hw[ax2][az2]*(mx-mx1)*(mz-mz1);
  return (h2/16*zoomz - zoomz2);
}

// Get height over landscape/water without ZOOM scaling, linear interpolation (slow)
// Only used to draw trees
// Get height over landscape/water, linear interpolation (slow)
float GLLandscape::getExactLSHeight (float x, float z)
{
  if (gridstep == 2) return getExactLSHeight2 (x, z);
  else if (gridstep == 3) return getExactLSHeight3 (x, z);
  else if (gridstep == 4) return getExactLSHeight4 (x, z);
  float mx = x;
  float mz = z;
  int mx1 = (int) floor (mx);
  int mz1 = (int) floor (mz);
  int mx2 = mx1 + 1;
  int mz2 = mz1 + 1;
  int ax1 = GETCOORD(mx1);
  int ax2 = GETCOORD(mx2);
  int az1 = GETCOORD(mz1);
  int az2 = GETCOORD(mz2);
  float h2 = (float)hw[ax1][az1]*((float)mx2-mx)*((float)mz2-mz) + (float)hw[ax2][az1]*(mx-mx1)*((float)mz2-mz) +
             (float)hw[ax1][az2]*((float)mx2-mx)*(mz-mz1) + (float)hw[ax2][az2]*(mx-mx1)*(mz-mz1);
  return (h2*zoomz - zoomz2);
}

// Get height of lowest sunray, no interpolation
float GLLandscape::getRayHeight (float x, float z)
{
  int mx = GETCOORD((int)floor (x));
  int mz = GETCOORD((int)floor (z));
  return (ZOOM * ((float)hray[mx][mz]*zoomz - zoomz2));
}

// Get height of lowest sunray, linear interpolation
float GLLandscape::getExactRayHeight (float x, float z)
{
  float mx = x;
  float mz = z;
  int mx1 = (int) floor (mx);
  int mx2 = mx1 + 1;
  int mz1 = (int) floor (mz);
  int mz2 = mz1 + 1;
  int ax1 = GETCOORD(mx1);
  int ax2 = GETCOORD(mx2);
  int az1 = GETCOORD(mz1);
  int az2 = GETCOORD(mz2);
  float h2 = (float)hray[ax1][az1]*((float)mx2-mx)*((float)mz2-mz) + (float)hray[ax2][az1]*(mx-mx1)*((float)mz2-mz) +
             (float)hray[ax1][az2]*((float)mx2-mx)*(mz-mz1) + (float)hray[ax2][az2]*(mx-mx1)*(mz-mz1);
  return (ZOOM * (h2*zoomz - zoomz2));
}

// Draw tree using two static quads (high quality, slow)
void GLLandscape::drawTree (float x, float y, float htree, float wtree, int phi)
{
  float ht = getExactLSHeight (x, y);
  phi = 359 - phi;
  if (phi < 0 || phi > 359)
  {
    printf ("Test: maybe a problem in drawTree!");
  }
  // Draw tree using a single rotated quad (low quality, fast)
  float ex1 = cosi [phi] * wtree, ey1 = sine [phi] * wtree;
  float ex2 = -ex1, ey2 = -ey1;
  int myticker;
  float zy = 0;
  if (weather == 1) // stormy weather
  {
    myticker = (int) (0.05 / htree * lsticker / timestep + 1000 * wtree + (x + y) * 50);
    if (myticker != 0)
      myticker %= 360;
    zy = 0.2 * (2.0 + sine [myticker]);
  }
  if (texturetree1 >= 0)
  {
//    glBindTexture (GL_TEXTURE_2D, texturetree1);
    va = &vertexarrayquad [texturetree1 + 1];
    va->glColor3ub (treecolor.c [0], treecolor.c [1], treecolor.c [2]);
    va->glTexCoord2d (0, 1);
    va->glVertex3f (hh2*(ex1+x), ht + htree, hh2*((ey1+y+zy)));
    va->glColor3ub (treecolor.c [0], treecolor.c [1], treecolor.c [2]);
    va->glTexCoord2d (1, 1);
    va->glVertex3f (hh2*(ex2+x), ht + htree, hh2*((ey2+y+zy)));
    va->glColor3ub (treecolor.c [0], treecolor.c [1], treecolor.c [2]);
    va->glTexCoord2d (1, 0);
    va->glVertex3f (hh2*(ex2+x), ht, hh2*((ey2+y)));
    va->glColor3ub (treecolor.c [0], treecolor.c [1], treecolor.c [2]);
    va->glTexCoord2d (0, 0);
    va->glVertex3f (hh2*(ex1+x), ht, hh2*((ey1+y)));
  }
  if (quality >= 2 && texturetree2 >= 0)
  {
    wtree *= 1.4F;
    ht += htree * 0.4F;
    zy *= 0.4F;
    phi += 45;
    if (phi >= 360) phi -= 360;
    ex1 = cosi [phi] * wtree; ey1 = sine [phi] * wtree;
    ex2 = -ex1; ey2 = -ey1;
    phi += 90;
    if (phi >= 360) phi -= 360;
    float ex3 = cosi [phi] * wtree, ey3 = sine [phi] * wtree;
    float ex4 = -ex3, ey4 = -ey3;
//    glBindTexture (GL_TEXTURE_2D, texturetree2);
    va = &vertexarrayquad [texturetree2 + 1];
    va->glColor3ub (treecolor.c [0], treecolor.c [1], treecolor.c [2]);
    va->glTexCoord2d (0, 1);
    va->glVertex3f (hh2*(ex1+x), ht, hh2*((ey1+y+zy)));
    va->glColor3ub (treecolor.c [0], treecolor.c [1], treecolor.c [2]);
    va->glTexCoord2d (1, 1);
    va->glVertex3f (hh2*(ex3+x), ht, hh2*((ey3+y+zy)));
    va->glColor3ub (treecolor.c [0], treecolor.c [1], treecolor.c [2]);
    va->glTexCoord2d (1, 0);
    va->glVertex3f (hh2*(ex2+x), ht, hh2*((ey2+y+zy)));
    va->glColor3ub (treecolor.c [0], treecolor.c [1], treecolor.c [2]);
    va->glTexCoord2d (0, 0);
    va->glVertex3f (hh2*(ex4+x), ht, hh2*((ey4+y+zy)));
  }
}

float xtree [256];
float ytree [256];

// Draw tree using two static quads (high quality, slow)
void GLLandscape::drawTreeQuad (int x, int y, int phi, bool hq)
{
  int i;
  int rotval = (x + 2 * y) & 127;
  int xs = getCoord (x);
  int ys = getCoord (y);
  if (f [xs] [ys] >= CONIFEROUSWOODS0 && f [xs] [ys] <= CONIFEROUSWOODS3)
  {
    texturetree1 = textree2->textureID;
    if (hq) texturetree2 = textreeu2->textureID;
    else texturetree2 = -1;
    int trees = CONIFEROUSWOODS3 - f [xs] [ys] + 1;
    if (hq && quality >= 2) trees += (trees - 1);
    for (i = 0; i < trees; i ++)
    {
      float htree = 0.0035 + 0.0002 * ((3 * y + 2 * x) & 7);
      drawTree (xtree [i + rotval] + x, ytree [i + rotval] + y, htree, htree * 70, phi);
    }
  }
  else if (f [xs] [ys] >= DECIDUOUSWOODS0 && f [xs] [ys] <= DECIDUOUSWOODS3)
  {
    texturetree1 = textree->textureID;
    if (hq) texturetree2 = textreeu->textureID;
    else texturetree2 = -1;
    int trees = DECIDUOUSWOODS3 - f [xs] [ys] + 1;
    if (hq && quality >= 2) trees += (trees - 1);
    for (i = 0; i < trees; i ++)
    {
      float htree = 0.0035 + 0.0003 * ((3 * y + 2 * x + 4 * i) & 7);
      drawTree (xtree [i + rotval] + x, ytree [i + rotval] + y, htree, htree * 60, phi);
    }
  }
  else if (f [xs] [ys] >= MIXEDWOODS0 && f [xs] [ys] <= MIXEDWOODS3)
  {
    texturetree1 = textree2->textureID;
    if (hq) texturetree2 = textreeu2->textureID;
    else texturetree2 = -1;
    int trees = MIXEDWOODS3 - f [xs] [ys] + 1;
    if (hq && quality >= 2) trees += (trees - 1);
    for (i = 0; i < 2; i ++)
    {
      float htree = 0.0035 + 0.0002 * ((3 * y + 2 * x + 4 * i) & 7);
      drawTree (xtree [i + rotval] + x, ytree [i + rotval] + y, htree, htree * 70, phi);
    }
    texturetree1 = textree5->textureID;
    if (hq) texturetree2 = textreeu5->textureID;
    else texturetree2 = -1;
    for (i = 2; i < 3; i ++)
    {
      float htree = 0.0025 + 0.00015 * ((3 * y + 2 * x + 4 * i) & 7);
      drawTree (xtree [i + rotval] + x, ytree [i + rotval] + y, htree, htree * 60, phi);
    }
    texturetree1 = textree->textureID;
    if (hq) texturetree2 = textreeu->textureID;
    else texturetree2 = -1;
    for (i = 3; i < trees; i ++)
    {
      float htree = 0.0035 + 0.0003 * ((3 * y + 2 * x + 4 * i) & 7);
      drawTree (xtree [i + rotval] + x, ytree [i + rotval] + y, htree, htree * 60, phi);
    }
  }
  else if (f [xs] [ys] >= DWARFPINES0 && f [xs] [ys] <= DWARFPINES3)
  {
    texturetree1 = textree3->textureID;
    if (hq) texturetree2 = textreeu3->textureID;
    else texturetree2 = -1;
    int trees = DWARFPINES3 - f [xs] [ys] + 1;
    if (hq && quality >= 2) trees += (trees - 1);
    for (i = 0; i < trees; i ++)
    {
      float htree = 0.0015 + 0.00015 * ((3 * y + 2 * x + 4 * i) & 7);
      drawTree (xtree [i + rotval] + x, ytree [i + rotval] + y, htree, htree * 150, phi);
    }
  }
  else if (f [xs] [ys] >= BUSHES0 && f [xs] [ys] <= BUSHES3)
  {
    texturetree1 = textree5->textureID;
    if (hq) texturetree2 = textreeu5->textureID;
    else texturetree2 = -1;
    int trees = BUSHES3 - f [xs] [ys] + 1;
    if (hq && quality >= 2) trees += (trees - 1);
    for (i = 0; i < trees; i ++)
    {
      float htree = 0.0025 + 0.00015 * ((3 * y + 2 * x + 4 * i) & 7);
      drawTree (xtree [i + rotval] + x, ytree [i + rotval] + y, htree, htree * 80, phi);
    }
  }
  else if (f [xs] [ys] == REDTREE0)
  {
    texturetree1 = textree4->textureID;
    if (hq) texturetree2 = textreeu4->textureID;
    else texturetree2 = -1;
    drawTree (x, y, 0.004, 0.35, phi);
  }
  else if (f [xs] [ys] == CACTUS0)
  {
    texturetree1 = texcactus1->textureID;
    if (hq) texturetree2 = texcactusu1->textureID;
    else texturetree2 = -1;
    drawTree (x, y, 0.004, 0.3, phi);
  }
}

void GLLandscape::drawTown (int x, int y)
{
  int xs = getCoord (x);
  int ys = getCoord (y);
  if (f [xs] [ys] == TOWN)
  {
    CVector3 tl;
    CRotation rot;
    rot.a = 270;
    rot.c = 90 * ((xs + ys / 3) & 3);
    tl.set (x + 0.5, getExactHeight ((float) xs + 0.5, (float) ys + 0.5) + 0.2, y + 0.5);
    glPushMatrix ();
    model_house1.draw (&tl, NULL, &rot, 0.3, 1, 0);
    glPopMatrix ();
    return;
  }
}

int visibility = 0;

// Fast landscape rendering without textures
void GLLandscape::drawQuadStrip (int x1, int y1, int x2, int y2)
{
  int x, y, xs, ys;
  float cr, cg, cb;
  bool water = false;
  bool last = false;
  int step = fargridstep;
  float texred, texgreen, texblue;

  va = &vertexarrayquadstrip;

  glDisable (GL_TEXTURE_2D);

  CTexture *tex;

  x1 -= x1 % step;
  y1 -= y1 % step;

  for (xs = x1; xs < x2;)
  {
    x = GETCOORD(xs);
    for (ys = y1; ys < y2;)
    {
      y = GETCOORD(ys);
      int xstep = GETCOORD(x + step);
      int y2 = GETCOORD(y + step);
      if (gl->isSphereInFrustum ((float) xs + 0.5F * step, (float)(hw[x][y]+hw[xstep][y])*0.5, (float) ys, step*8))
      {
        int a = f [x] [y];
        if (a >= 40 && a <= 49)
          water = true;
		    int x2 = xstep;
        int y0 = GETCOORD(y - step);
		    if (!(h [x] [y] < hw [x] [y] && h [x2] [y] < hw [x2] [y] &&
              h [x] [y2] < hw [x] [y2] && h [x2] [y2] < hw [x2] [y2] &&
              h [x] [y0] < hw [x] [y0] && h [x2] [y0] < hw [x2] [y0]))
        {
          if (!last)
          {
            last = true;
            va->glBegin (GL_QUAD_STRIP);
          }
          tex = texmap [a];
          if (tex == NULL)
          {
            texred = 1.0F;
            texgreen = 1.0F;
            texblue = 1.0F;
          }
          else
          {
            texred = tex->texred;
            texgreen = tex->texgreen;
            texblue = tex->texblue;
          }
          float fac = lightfac * (nl [x] [y] + (short) dl [x] [y] * 16) * sunlight;
          cr = (float) r [x] [y] * fac * texred;
          cg = (float) g [x] [y] * fac * texgreen;
          cb = (float) b [x] [y] * fac * texblue;
          if (cr >= texred) cr = texred;
          if (cg >= texgreen) cg = texgreen;
          if (cb >= texblue) cb = texblue;
          va->glColor3f (cr, cg, cb);
          va->glVertex3f (xs, (float)h[x][y], (ys));
          fac = lightfac * (nl [xstep] [y] + (short) dl [xstep] [y] * 16) * sunlight;
          cr = (float) r [x + step] [y] * fac * texred;
          cg = (float) g [x + step] [y] * fac * texgreen;
          cb = (float) b [x + step] [y] * fac * texblue;
          if (cr >= texred) cr = texred;
          if (cg >= texgreen) cg = texgreen;
          if (cb >= texblue) cb = texblue;
          va->glColor3f (cr, cg, cb);
          va->glVertex3f ((xs+step), (float)h[xstep][y], (ys));
        }
        else
        {
          if (last)
            va->glEnd ();
          last = false;
        }
      }
      ys += step;
    }
    if (last)
    {
      va->glEnd ();
      last = false;
    }
    xs += step;
  }

  last = false;
  if (water)
  {
    float texlight;
    float watergreen = 0.00025;
    if (day) watergreen = 0.0004;
    for (xs = x1; xs < x2;)
    {
      x = GETCOORD(xs);
      for (ys = y1; ys < y2;)
      {
        y = GETCOORD(ys);
        int xstep = GETCOORD(x + step);
        int ystep = GETCOORD(y + step);
        int ymstep = GETCOORD(y - step);
        if (isWater (f [x] [y]) || isWater (f [xstep] [y]) || isWater (f [xstep] [ystep]) || isWater (f [x] [ystep]) ||
            isWater (f [x] [ymstep]) || isWater (f [xstep] [ymstep]))
        {
          float h1 = hw [x] [y];
          if (hw [xstep] [ystep] < h1) h1 = hw [xstep] [ystep];
          if (hw [xstep] [y] < h1) h1 = hw [xstep] [y];
          if (hw [x] [ystep] < h1) h1 = hw [x] [ystep];
          if (hw [x] [ymstep] < h1) h1 = hw [x] [ymstep];
          if (hw [xstep] [ymstep] < h1) h1 = hw [xstep] [ymstep];
          if (gl->isSphereInFrustum ((xs), (float)h1, (ys), step*2))
          {
            if (!last)
            {
              last = true;
              va->glBegin (GL_QUAD_STRIP);
            }

            texlight = texwater->texlight;
            float d = watergreen * (h1 - h [x] [y]);
            if (d > 0.75) d = 0.75;
            if (type == LAND_ALPINE)
            {
              cr = 0.1 * 256;
              cg = (0.85 - d) * 256;
              cb = (0.6 + d) * 256;
            }
            else if (type == LAND_CANYON)
            {
              cr = (0.55 - d/2) * 256;
              cg = (0.55 - d/2) * 256;
              cb = (0.6 + d) * 256;
            }
            else if (type == LAND_ARCTIC)
            {
              cr = (0.7 - d/2) * 256;
              cg = (0.7 - d/2) * 256;
              cb = (0.7 + d / 2) * 256;
            }
            else
            {
              cr = 0; cg = 0; cb = 0;
            }
            float fac = lightfac * (nl [x] [y] + (short) dl [x] [y] * 16) * sunlight * texlight;
            cr = (float) cr * fac;
            cg = (float) cg * fac;
            cb = (float) cb * fac;
            if (cr > texlight) cr = texlight;
            if (cg < 0.1 * texlight) cg = 0.1 * texlight;
            if (cg > texlight) cg = texlight;
            if (cb > texlight) cb = texlight;
            va->glColor3f (cr, cg, cb);
            va->glVertex3f (xs, h1, (ys));

            d = watergreen * (h1 - h [xstep] [y]);
            if (d > 0.75) d = 0.75;
            if (type == LAND_ALPINE)
            {
              cr = 0.1 * 256;
              cg = (0.85 - d) * 256;
              cb = (0.6 + d) * 256;
            }
            else if (type == LAND_CANYON)
            {
              cr = (0.55 - d/2) * 256;
              cg = (0.55 - d/2) * 256;
              cb = (0.6 + d) * 256;
            }
            else if (type == LAND_ARCTIC)
            {
              cr = (0.7 - d/2) * 256;
              cg = (0.7 - d/2) * 256;
              cb = (0.7 + d / 2) * 256;
            }
            fac = lightfac * (nl [xstep] [y] + (short) dl [xstep] [y] * 16) * sunlight * texlight;
            cr = (float) cr * fac;
            cg = (float) cg * fac;
            cb = (float) cb * fac;
            if (cr > texlight) cr = texlight;
            if (cg < 0.1 * texlight) cg = 0.1 * texlight;
            if (cg > texlight) cg = texlight;
            if (cb > texlight) cb = texlight;
            va->glColor3f (cr, cg, cb);
            va->glVertex3f ((xs+step), h1, (ys));
          }
          else
          {
            if (last)
              va->glEnd ();
            last = false;
          }
        }
        ys += step;
      }
      if (last)
      {
        va->glEnd ();
        last = false;
      }
      xs += step;
    }
  }
}

// Draw a single untextured quad
void GLLandscape::drawQuad (int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
  int j;
  int step = fargridstep;
  float texred, texgreen, texblue;
  CTexture *tex = NULL;
  float col [4] [3];
  float pos [4] [3];
  float fac;
  int px [4], py [4];
  int xs = x1, ys = y1;
  int x = GETCOORD(xs);
  int y = GETCOORD(ys);
  px [0] = x1; py [0] = y1;
  px [1] = x2; py [1] = y2;
  px [2] = x3; py [2] = y3;
  px [3] = x4; py [3] = y4;
  float minh = h[x][y];
  float maxh = minh;
  for (j = 1; j < 4; j ++)
  {
    int h1 = h [GETCOORD(px [j])] [GETCOORD(py [j])];
    if (h1 > maxh) maxh = h1;
    else if (h1 < minh) minh = h1;
  }
  float midh = (minh + maxh) / 2;
  float size = (maxh - minh) * step; // exakt w�e mal 0.5
  if (size < 1.0 / 2 * step)
    size = 1.0 / 2 * step;
  if (!gl->isSphereInFrustum ((0.5+xs), midh, (0.5+ys), size * 2))
    return;

  va = &vertexarrayquad [0];

  int a = f [x] [y];
  tex = texmap [a];
  if (tex == NULL)
  {
    texred = 1.0F;
    texgreen = 1.0F;
    texblue = 1.0F;
  }
  else
  {
    texred = tex->texred;
    texgreen = tex->texgreen;
    texblue = tex->texblue;
  }
//  glDisable (GL_TEXTURE_2D);
  float fac2 = lightfac * sunlight;
  for (j = 0; j < 4; j ++)
  {
    int mx = GETCOORD(px [j]), my = GETCOORD(py [j]);
    fac = fac2 * (nl [mx] [my] + (short) dl [mx] [my] * 16);
    col [j] [0] = r [mx] [my] * fac * texred;
    col [j] [1] = g [mx] [my] * fac * texgreen;
    col [j] [2] = b [mx] [my] * fac * texblue;
    if (col [j] [0] >= texred) col [j] [0] = texred;
    if (col [j] [1] >= texgreen) col [j] [1] = texgreen;
    if (col [j] [2] >= texblue) col [j] [2] = texblue;
    pos [j] [0] = px[j];
    pos [j] [1] = (float)h[mx][my];
    pos [j] [2] = py[j];
  }
  for (j = 0; j < 4; j ++)
  {
    va->glColor3fv (col [j]);
    va->glVertex3fv (pos [j]);
  }
}

// Draw a single untextured triangle
void GLLandscape::drawTriangle (int x1, int y1, int x2, int y2, int x3, int y3)
{
  int j;
  int step = fargridstep;
  float texred, texgreen, texblue;
  CTexture *tex = NULL;
  float col [3] [3];
  float pos [3] [3];
  float fac;
  int px [3], py [3];
  int xs = x1, ys = y1;
  int x = GETCOORD(xs);
  int y = GETCOORD(ys);
  px [0] = x1; py [0] = y1;
  px [1] = x2; py [1] = y2;
  px [2] = x3; py [2] = y3;
  float minh = h[x][y];
  float maxh = minh;
  for (j = 1; j < 3; j ++)
  {
    int h1 = h [GETCOORD(px [j])] [GETCOORD(py [j])];
    if (h1 > maxh) maxh = h1;
    else if (h1 < minh) minh = h1;
  }
  float midh = (minh + maxh) / 2;
  float size = (maxh - minh) * step; // exakt w�e mal 0.5
  if (size < 1.0 / 2 * step)
    size = 1.0 / 2 * step;
  if (!gl->isSphereInFrustum ((0.5+xs), midh, (0.5+ys), size * 2))
    return;

  va = &vertexarraytriangle [0];

  int a = f [x] [y];
  tex = texmap [a];
  if (tex == NULL)
  {
    texred = 1.0F;
    texgreen = 1.0F;
    texblue = 1.0F;
  }
  else
  {
    texred = tex->texred;
    texgreen = tex->texgreen;
    texblue = tex->texblue;
  }
//  glDisable (GL_TEXTURE_2D);
  float fac2 = lightfac * sunlight;
  for (j = 0; j < 3; j ++)
  {
    int mx = GETCOORD(px [j]), my = GETCOORD(py [j]);
    fac = fac2 * (nl [mx] [my] + (short) dl [mx] [my] * 16);
    col [j] [0] = r [mx] [my] * fac * texred;
    col [j] [1] = g [mx] [my] * fac * texgreen;
    col [j] [2] = b [mx] [my] * fac * texblue;
    if (col [j] [0] >= texred) col [j] [0] = texred;
    if (col [j] [1] >= texgreen) col [j] [1] = texgreen;
    if (col [j] [2] >= texblue) col [j] [2] = texblue;
    pos [j] [0] = px[j];
    pos [j] [1] = (float)h[mx][my];
    pos [j] [2] = py[j];
  }
//  glBegin (GL_TRIANGLES);
  for (j = 0; j < 3; j ++)
  {
    va->glColor3fv (col [j]);
    va->glVertex3fv (pos [j]);
  }
//  glEnd();
}

// Draw a single textured quad
void GLLandscape::drawTexturedQuad (int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
  int j;
  int step = gridstep;
  bool texture = false;
  float col [4] [3];
  float pos [4] [3];
  float tf [4] [2];
  float fac;
  float texzoom;
  int px [4], py [4];
  int x = GETCOORD(x2);
  int y = GETCOORD(y2);
  px [0] = x1; py [0] = y1;
  px [1] = x2; py [1] = y2;
  px [2] = x3; py [2] = y3;
  px [3] = x4; py [3] = y4;
  float minh = h[x][y];
  float maxh = minh;
  for (j = 1; j < 4; j ++)
  {
    int h1 = h [GETCOORD(px [j])] [GETCOORD(py [j])];
    if (h1 > maxh) maxh = h1;
    else if (h1 < minh) minh = h1;
  }
  float midh = (minh + maxh) / 2;
  float size = (maxh - minh) * step; // exakt w�e mal 0.5
  if (size < 1.0 / 2 * step)
    size = 1.0 / 2 * step;
  if (!gl->isSphereInFrustum ((0.5+x2), midh, (0.5+y2), size * 2))
    return;
  if (tex1 [x] [y] == 0xFF)
  {
    texture = false;
    va = &vertexarrayquad [0];
//    glDisable (GL_TEXTURE_2D);
  }
  else
  {
    texture = true;
    va = &vertexarrayquad [tex1 [x] [y] + 1];
//    gl->enableTextures (tex1 [x] [y]);
  }
  int texcoord = 0;
  if (tex1 [x] [y] == texredstone->textureID)
  {
    texzoom = 0.5;
    texcoord = 1;
  }
  else if (tex1 [x] [y] != texgrass->textureID)
  {
    texzoom = 0.5;
  }
  else
  {
    texzoom = 0.25;
  }
  float fac2 = lightfac * sunlight;
  for (j = 0; j < 4; j ++)
  {
    int mx = GETCOORD(px [j]), my = GETCOORD(py [j]);
    fac = fac2 * (nl [mx] [my] + (short) dl [mx] [my] * 16);
    col [j] [0] = r [mx] [my] * fac;
    col [j] [1] = g [mx] [my] * fac;
    col [j] [2] = b [mx] [my] * fac;
    if (col [j] [0] >= 1.0) col [j] [0] = 1.0;
    if (col [j] [1] >= 1.0) col [j] [1] = 1.0;
    if (col [j] [2] >= 1.0) col [j] [2] = 1.0;
    pos [j] [0] = px[j];
    pos [j] [1] = (float)h[mx][my];
    pos [j] [2] = py[j];
  }
  for (j = 0; j < 4; j ++)
  {
    if (texcoord == 0)
    {
      tf [j] [0] = (float) px [j] * texzoom;
      tf [j] [1] = (float) py [j] * texzoom;
    }
    else
    {
      tf [j] [0] = (float) px [j] * texzoom;
      tf [j] [1] = (float) h [GETCOORD(px [j])] [GETCOORD(py [j])] * texzoom / 400.0;
    }
  }
  for (j = 0; j < 4; j ++)
  {
    if (texture)
      va->glTexCoord2fv (tf [j]);
    va->glColor3fv (col [j]);
    va->glVertex3fv (pos [j]);
  }
}

// Draw a single textured triangle
void GLLandscape::drawTexturedTriangle (int x1, int y1, int x2, int y2, int x3, int y3)
{
  int j;
  int step = gridstep;
  bool texture = false;
  float col [3] [3];
  float pos [3] [3];
  float tf [3] [2];
  float fac;
  float texzoom;
  int px [3], py [3];
  int x = GETCOORD(x2);
  int y = GETCOORD(y2);
  px [0] = x1; py [0] = y1;
  px [1] = x2; py [1] = y2;
  px [2] = x3; py [2] = y3;
  float minh = h[x][y];
  float maxh = minh;
  for (j = 1; j < 3; j ++)
  {
    int h1 = h [GETCOORD(px [j])] [GETCOORD(py [j])];
    if (h1 > maxh) maxh = h1;
    else if (h1 < minh) minh = h1;
  }
  float midh = (minh + maxh) / 2;
  float size = (maxh - minh) * step; // exakt w�e mal 0.5
  if (size < 1.0 / 2 * step)
    size = 1.0 / 2 * step;
  if (!gl->isSphereInFrustum ((0.5+x2), midh, (0.5+y2), size * 2))
    return;
  if (tex1 [x] [y] == 0xFF)
  {
    texture = false;
    va = &vertexarraytriangle [0];
//    glDisable (GL_TEXTURE_2D);
  }
  else
  {
    texture = true;
    va = &vertexarraytriangle [tex1 [x] [y] + 1];
//    gl->enableTextures (tex1 [x] [y]);
  }
  int texcoord = 0;
  if (tex1 [x] [y] == texredstone->textureID)
  {
    texzoom = 0.5;
    texcoord = 1;
  }
  else if (tex1 [x] [y] != texgrass->textureID)
  {
    texzoom = 0.5;
  }
  else
  {
    texzoom = 0.25;
  }
  float fac2 = lightfac * sunlight;
  for (j = 0; j < 3; j ++)
  {
    int mx = GETCOORD(px [j]), my = GETCOORD(py [j]);
    fac = fac2 * (nl [mx] [my] + (short) dl [mx] [my] * 16);
    col [j] [0] = r [mx] [my] * fac;
    col [j] [1] = g [mx] [my] * fac;
    col [j] [2] = b [mx] [my] * fac;
    if (col [j] [0] >= 1.0) col [j] [0] = 1.0;
    if (col [j] [1] >= 1.0) col [j] [1] = 1.0;
    if (col [j] [2] >= 1.0) col [j] [2] = 1.0;
    pos [j] [0] = px[j];
    pos [j] [1] = (float)h[mx][my];
    pos [j] [2] = py[j];
  }
  for (j = 0; j < 3; j ++)
  {
    if (texcoord == 0)
    {
      tf [j] [0] = (float) px [j] * texzoom;
      tf [j] [1] = (float) py [j] * texzoom;
    }
    else
    {
      tf [j] [0] = (float) px [j] * texzoom;
      tf [j] [1] = (float) h [GETCOORD(px [j])] [GETCOORD(py [j])] * texzoom / 400.0;
    }
  }
  for (j = 0; j < 3; j ++)
  {
    if (texture)
      va->glTexCoord2fv (tf [j]);
    va->glColor3fv (col [j]);
    va->glVertex3fv (pos [j]);
  }
}

// Draw a single textured quad
void GLLandscape::drawTexturedQuad (int xs, int ys)
{
  int j;
  int step = gridstep;
  bool texture = false;
  float col [4] [3];
  float pos [4] [3];
  float tf [4] [2];
  float fac;
  float texzoom;
  int px [4], py [4];
  int pcx [4], pcy [4];
  px [0] = xs; py [0] = ys;
  px [1] = xs + step; py [1] = ys;
  px [2] = xs + step; py [2] = ys + step;
  px [3] = xs; py [3] = ys + step;
  for (j = 0; j < 4; j ++)
  {
    pcx [j] = GETCOORD(px [j]);
    pcy [j] = GETCOORD(py [j]);
  }
  int x = GETCOORD(pcx [0]);
  int y = GETCOORD(pcy [0]);
  float minh = h [x] [y];
  float maxh = minh;
  for (j = 1; j < 4; j ++)
  {
    int h1 = h [pcx [j]] [pcy [j]];
    if (h1 > maxh) maxh = h1;
    else if (h1 < minh) minh = h1;
  }
  float midh = (minh + maxh) / 2;
  float size = (maxh - minh) * step; // exakt w�e mal 0.5
  if (size < 1.0 / 2 * step)
    size = 1.0 / 2 * step;
  if (!gl->isSphereInFrustum ((0.5+xs), midh, (0.5+ys), size * 2))
    return;
  if (tex1 [x] [y] == 0xFF)
  {
    texture = false;
    va = &vertexarrayquad [0];
//    glDisable (GL_TEXTURE_2D);
  }
  else
  {
    texture = true;
    va = &vertexarrayquad [tex1 [x] [y] + 1];
//    gl->enableTextures (tex1 [x] [y]);
  }
  int texcoord = 0;
  if (tex1 [x] [y] == texredstone->textureID)
  {
    texzoom = 0.5;
    texcoord = 1;
  }
  else if (tex1 [x] [y] != texgrass->textureID)
  {
    texzoom = 0.5;
  }
  else
  {
    texzoom = 0.25;
  }
  float fac2 = lightfac * sunlight;
  for (j = 0; j < 4; j ++)
  {
    int mx = pcx [j], my = pcy [j];
    fac = fac2 * (nl [mx] [my] + (short) dl [mx] [my] * 16);
    col [j] [0] = (float) r [mx] [my] * fac;
    col [j] [1] = (float) g [mx] [my] * fac;
    col [j] [2] = (float) b [mx] [my] * fac;
    if (col [j] [0] >= 1.0) col [j] [0] = 1.0;
    if (col [j] [1] >= 1.0) col [j] [1] = 1.0;
    if (col [j] [2] >= 1.0) col [j] [2] = 1.0;
    pos [j] [0] = px[j];
    pos [j] [1] = (float)h[mx][my];
    pos [j] [2] = py[j];
  }
  for (j = 0; j < 4; j ++)
  {
    if (texcoord == 0)
    {
      tf [j] [0] = (float) px [j] * texzoom;
      tf [j] [1] = (float) py [j] * texzoom;
    }
    else
    {
      tf [j] [0] = (float) px [j] * texzoom;
      tf [j] [1] = (float) h [pcx [j]] [pcy [j]] * texzoom / 400.0;
    }
  }
  for (j = 0; j < 4; j ++)
  {
    if (texture)
      va->glTexCoord2fv (tf [j]);
    va->glColor3fv (col [j]);
    va->glVertex3fv (pos [j]);
  }
}

// Draw a single textured water quad
void GLLandscape::drawWaterTexturedQuad (int xs, int ys)
{
  int i, j;
  int step = gridstep;
  bool texture = false;
  float col [4] [4];
  float pos [4] [3];
  float tf [4] [2];
  float li [4];
  float fac;
  float texzoom;
  int px [4], py [4];
  int x = GETCOORD(xs);
  int y = GETCOORD(ys);
  int xstep = GETCOORD(xs + step);
  int ystep = GETCOORD(ys + step);
  px [0] = xs; py [0] = ys; li [0] = (nl [x] [y] + (short) dl [x] [y] * 16);
  px [1] = xs + step; py [1] = ys; li [1] = (nl [xstep] [y] + (short) dl [xstep] [y] * 16);
  px [2] = xs + step; py [2] = ys + step; li [2] = (nl [xstep] [ystep] + (short) dl [xstep] [ystep] * 16);
  px [3] = xs; py [3] = ys + step; li [3] = (nl [x] [ystep] + (short) dl [x] [ystep] * 16);

  float h1 = hw [x] [y];
  for (i = 1; i <= 3; i ++)
  {
    int mx = GETCOORD(px [i]);
    int my = GETCOORD(py [i]);
    if (hw [mx] [my] < h1)
    {
      h1 = hw [mx] [my];
    }
  }

  if (!gl->isSphereInFrustum ((0.5+xs), (float) h1, (0.5+ys), step))
    return;

  float quadglittering = 0;
  float glitter [4] = { 1, 1, 1, 1 };
  if (specialeffects)
  if (weather == WEATHER_SUNNY || weather == WEATHER_CLOUDY)
  {
    float dz1 = fabs ((float) camx - xs);
    float dz2 = fabs ((float) camx - xs - step);
    float dy = fabs (camy - (h1*zoomz - zoomz2) * ZOOM);
    float dtheta1 = fabs (atan (dy / dz1) * 180.0 / PI - 90);
    float dtheta2 = fabs (atan (dy / dz2) * 180.0 / PI - 90);
    dtheta1 /= 4; dtheta2 /= 4;
//    if (lz1 <= 5 || lz2 <= 5)
    {
      float divdy = 1.0F / dy * 200;
      float dx1 = ((float) -camz + ys);
      float dx2 = ((float) -camz + ys + step);
//      float dy = fabs (camy - (h1*zoomz - zoomz2) * ZOOM);
      float dgamma1 = fabs (atan (dy / dx1) * 180.0 / PI - sungamma);
      float dgamma2 = fabs (atan (dy / dx2) * 180.0 / PI - sungamma);
      dgamma1 /= 4; dgamma2 /= 4;
      float sc = 1.0;
      float test;
      if (dx1 < 0) dgamma1 += 90;
      if (dx2 < 0) dgamma2 += 90;
      if (h1 >= hray [x] [y])
      {
        test = sc * exp ((-dgamma1 * dgamma1 - dtheta1 * dtheta1) / divdy) + 0.98;
        if (test > 1.0)
        {
          glitter [0] = test;
          if (test > glittering) glittering = test;
          if (test > quadglittering) quadglittering = test;
        }
      }
      if (h1 >= hray [xstep] [y])
      {
        test = sc * exp ((-dgamma1 * dgamma1 - dtheta2 * dtheta2) / divdy) + 0.98;
        if (test > 1.0)
        {
          glitter [1] = test;
          if (test > glittering) glittering = test;
          if (test > quadglittering) quadglittering = test;
        }
      }
      if (h1 >= hray [x] [ystep])
      {
        test = sc * exp ((-dgamma2 * dgamma2 - dtheta1 * dtheta1) / divdy) + 0.98;
        if (test > 1.0)
        {
          glitter [3] = test;
          if (test > glittering) glittering = test;
          if (test > quadglittering) quadglittering = test;
        }
      }
      if (h1 >= hray [xstep] [ystep])
      {
        test = sc * exp ((-dgamma2 * dgamma2 - dtheta2 * dtheta2) / divdy) + 0.98;
        if (test > 1.0)
        {
          glitter [2] = test;
          if (test > glittering) glittering = test;
          if (test > quadglittering) quadglittering = test;
        }
      }
    }
  }

  texture = true;
  va = &vertexarrayquad [texwater->textureID + 1];
  gl->enableTextures (texwater->textureID);
  texzoom = 0.5;
  float watergreen = 0.00025;
  if (day) watergreen = 0.0004;
  float fac2 = lightfac * sunlight * 256.0;
  for (j = 0; j < 4; j ++)
  {
    int mx = GETCOORD(px [j]), my = GETCOORD(py [j]);
    float d = watergreen * (h1 - h [mx] [my]);
    if (d > 0.75) d = 0.75;
    fac = fac2 * li [j];
    if (type == LAND_ALPINE)
    {
      col [j] [0] = 0.1 * fac;
      col [j] [1] = (0.85 - d) * fac;
      col [j] [2] = (0.6 + d) * fac;
    }
    else if (type == LAND_CANYON)
    {
      col [j] [0] = (0.55 - d/2) * fac;
      col [j] [1] = (0.55 - d/2) * fac;
      col [j] [2] = (0.6 + d) * fac;
    }
    else if (type == LAND_ARCTIC)
    {
      col [j] [0] = (0.7 - d/2) * fac;
      col [j] [1] = (0.7 - d/2) * fac;
      col [j] [2] = (0.7 + d / 2) * fac;
    }
    if (col [j] [0] >= 1.0) col [j] [0] = 1.0;
    if (col [j] [1] <= 0.1) col [j] [1] = 0.1;
    if (col [j] [1] >= 1.0) col [j] [1] = 1.0;
    if (col [j] [2] >= 1.0) col [j] [2] = 1.0;
    pos [j] [0] = px[j];
    pos [j] [1] = (float)h1;
    pos [j] [2] = py[j];
  }
  for (j = 0; j < 4; j ++)
  {
    float waterspeed = 0.008;
    if (weather == 1) waterspeed = 0.016;
    tf [j] [0] = (float) px [j] * texzoom + waterspeed * lsticker / timestep;
    tf [j] [1] = (float) py [j] * texzoom;
  }
  
  for (j = 0; j < 4; j ++)
  {
    if (texture)
    {
      va->glTexCoord2fv (tf [j]);
    }
    va->glColor4fv (col [j]);
    va->glVertex3fv (pos [j]);
  }

  va = &vertexarrayglitter [0];
  if (specialeffects && quadglittering > 1.2)
  {
    glEnable (GL_BLEND);
    glDepthFunc (GL_LEQUAL);
    glBlendFunc (GL_ONE, GL_SRC_ALPHA);
    glEnable (GL_ALPHA_TEST);
    glAlphaFunc (GL_GEQUAL, 0.2);
    gl->enableTextures (texglitter1->textureID);
    gl->enableLinearTexture (texglitter1->textureID);
    for (j = 0; j < 4; j ++)
    {
      if (texture)
      {
        tf [j] [0] = (px [j] * texzoom) + (float) ((lsticker / timestep / 2) & 7) * 0.6;
        tf [j] [1] = (py [j] * texzoom) + (float) ((lsticker / timestep / 2) & 7) * 0.6;
        va->glTexCoord2fv (tf [j]);
      }
      col [j] [3] = glitter [j] - 1.0;
      col [j] [0] = 1.0;
      col [j] [1] = 1.0;
      col [j] [2] = 1.0;
      va->glColor4fv (col [j]);
      va->glVertex3fv (pos [j]);
    }
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable (GL_ALPHA_TEST);
    glDisable (GL_BLEND);
  }

  va = &vertexarrayglitter [1];
  if (specialeffects && quadglittering > 1.02)
  {
    glEnable (GL_BLEND);
    glDepthFunc (GL_LEQUAL);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable (GL_ALPHA_TEST);
    glAlphaFunc (GL_GEQUAL, 0.02);
    glDisable (GL_TEXTURE_2D);
    for (j = 0; j < 4; j ++)
    {
      col [j] [3] = glitter [j] - 1.0;
      col [j] [0] = 1.0;
      col [j] [1] = 1.0;
      col [j] [2] = 1.0;
      va->glColor4fv (col [j]);
      va->glVertex3fv (pos [j]);
    }
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable (GL_ALPHA_TEST);
    glDisable (GL_BLEND);
  }

}

// Draw two textured triangles (quad)
void GLLandscape::drawTexturedTriangle1 (int xs, int ys)
{
  int j;
  int step = gridstep;
  bool texture = false;
  float col [4] [3];
  float pos [4] [3];
  float tf [4] [2];
  float fac;
  float texzoom;
  int px [4], py [4];
  int x = GETCOORD(xs);
  int y = GETCOORD(ys);
  px [0] = xs; py [0] = ys;
  px [1] = xs + step; py [1] = ys;
  px [2] = xs + step; py [2] = ys + step;
  px [3] = xs; py [3] = ys + step;

  if (!gl->isSphereInFrustum ((0.5+xs), (float)h[x][y], (0.5+ys), 2*step))
    return;

  if (tex1 [x] [y] == 0xFF)
  {
    texture = false;
    va = &vertexarraytriangle [0];
//    glDisable (GL_TEXTURE_2D);
  }
  else
  {
    texture = true;
    va = &vertexarraytriangle [tex1 [x] [y] + 1];
//    gl->enableTextures (tex1 [x] [y]);
  }
  if (tex1 [x] [y] != texgrass->textureID && tex1 [x] [y] != texgrass->textureID)
  {
    texzoom = 0.5;
  }
  else
  {
    texzoom = 0.25;
  }

  float fac2 = lightfac * sunlight;

  for (j = 0; j < 4; j ++)
  {
    int mx = GETCOORD(px [j]), my = GETCOORD(py [j]);
    fac = fac2 * (nl [mx] [my] + (short) dl [mx] [my] * 16);
    col [j] [0] = r [mx] [my] * fac;
    col [j] [1] = g [mx] [my] * fac;
    col [j] [2] = b [mx] [my] * fac;
    if (col [j] [0] >= 1.0) col [j] [0] = 1.0;
    if (col [j] [1] >= 1.0) col [j] [1] = 1.0;
    if (col [j] [2] >= 1.0) col [j] [2] = 1.0;
    pos [j] [0] = px[j];
    pos [j] [1] = (float)h[mx][my];
    pos [j] [2] = py[j];
  }

  for (j = 0; j < 4; j ++)
  {
    tf [j] [0] = (float) px [j] * texzoom;
    tf [j] [1] = (float) py [j] * texzoom;
  }

  for (j = 0; j < 3; j ++)
  {
    if (texture)
      va->glTexCoord2fv (tf [j]);
    va->glColor3fv (col [j]);
    va->glVertex3fv (pos [j]);
  }

  if (tex2 [x] [y] == 0xFF)
  {
    texture = false;
    va = &vertexarraytriangle [0];
  }
  else
  {
    texture = true;
    va = &vertexarraytriangle [tex2 [x] [y] + 1];
  }

  if (tex2 [x] [y] != texgrass->textureID && tex2 [x] [y] != texredstone->textureID)
  {
    texzoom = 0.5;
  }
  else
  {
    texzoom = 0.25;
  }

  for (j = 0; j < 4; j ++)
  {
    tf [j] [0] = (float) px [j] * texzoom;
    tf [j] [1] = (float) py [j] * texzoom;
  }

  if (texture)
    va->glTexCoord2fv (tf [0]);
  va->glColor3fv (col [0]);
  va->glVertex3fv (pos [0]);
  for (j = 2; j < 4; j ++)
  {
    if (texture)
      va->glTexCoord2fv (tf [j]);
    va->glColor3fv (col [j]);
    va->glVertex3fv (pos [j]);
  }
}

// Draw two textured triangles (quad)
void GLLandscape::drawTexturedTriangle2 (int xs, int ys)
{
  int j;
  int step = gridstep;
  bool texture = false;
  float col [4] [3];
  float pos [4] [3];
  float tf [4] [2];
  float fac;
  float texzoom;
  int px [4], py [4];
  int x = GETCOORD(xs);
  int y = GETCOORD(ys);
  px [0] = xs; py [0] = ys;
  px [1] = xs + step; py [1] = ys;
  px [2] = xs + step; py [2] = ys + step;
  px [3] = xs; py [3] = ys + step;

  if (!gl->isSphereInFrustum ((0.5+xs), (float)h[x][y], (0.5+ys), 2*step))
    return;

  if (tex1 [x] [y] == 0xFF)
  {
    texture = false;
    va = &vertexarraytriangle [0];
//    glDisable (GL_TEXTURE_2D);
  }
  else
  {
    texture = true;
    va = &vertexarraytriangle [tex1 [x] [y] + 1];
//    gl->enableTextures (tex1 [x] [y]);
  }

  if (tex1 [x] [y] != texgrass->textureID && tex1 [x] [y] != texredstone->textureID)
  {
    texzoom = 0.5;
  }
  else
  {
    texzoom = 0.25;
  }

  float fac2 = lightfac * sunlight;
  for (j = 0; j < 4; j ++)
  {
    int mx = GETCOORD(px [j]), my = GETCOORD(py [j]);
    fac = fac2 * (nl [mx] [my] + (short) dl [mx] [my]);
    col [j] [0] = r [mx] [my] * fac;
    col [j] [1] = g [mx] [my] * fac;
    col [j] [2] = b [mx] [my] * fac;
    if (col [j] [0] >= 1.0) col [j] [0] = 1.0;
    if (col [j] [1] >= 1.0) col [j] [1] = 1.0;
    if (col [j] [2] >= 1.0) col [j] [2] = 1.0;
    pos [j] [0] = px[j];
    pos [j] [1] = (float)h[mx][my];
    pos [j] [2] = py[j];
  }

  for (j = 0; j < 4; j ++)
  {
    tf [j] [0] = (float) px [j] * texzoom;
    tf [j] [1] = (float) py [j] * texzoom;
  }

//  glBegin (GL_TRIANGLES);
  for (j = 0; j < 2; j ++)
  {
    if (texture)
      va->glTexCoord2fv (tf [j]);
    va->glColor3fv (col [j]);
    va->glVertex3fv (pos [j]);
  }
  if (texture)
    va->glTexCoord2fv (tf [3]);
  va->glColor3fv (col [3]);
  va->glVertex3fv (pos [3]);
//  glEnd();

  if (tex2 [x] [y] == 0xFF)
  {
    texture = false;
    va = &vertexarraytriangle [0];
//    glDisable (GL_TEXTURE_2D);
  }
  else
  {
    texture = true;
    va = &vertexarraytriangle [tex2 [x] [y] + 1];
//    gl->enableTextures (tex2 [x] [y]);
  }

  if (tex2 [x] [y] != texgrass->textureID && tex2 [x] [y] != texredstone->textureID)
  {
    texzoom = 0.5;
  }
  else
  {
    texzoom = 0.25;
  }

  for (j = 0; j < 4; j ++)
  {
    tf [j] [0] = (float) px [j] * texzoom;
    tf [j] [1] = (float) py [j] * texzoom;
  }

//  glBegin (GL_TRIANGLES);
  for (j = 1; j < 4; j ++)
  {
    if (texture)
      va->glTexCoord2fv (tf [j]);
    va->glColor3fv (col [j]);
    va->glVertex3fv (pos [j]);
  }
//  glEnd();
}

void GLLandscape::draw (int phi, int gamma)
{
  char buf [STDSIZE];
  int i, i2, i3, x, y;
  int xs, ys;

  float fac;

  int fardetail = quality;
  int middetail = quality;
  int lineardetail = -1;

// until v1.2 (no vertex arrays)
/*  if (quality == 0)
  { neargridstep = 3; fargridstep = 3; middetail = -1; fardetail = -1; lineardetail = -1; }
  else if (quality == 1)
  { neargridstep = 2; fargridstep = 4; middetail = 2; fardetail = 2; lineardetail = -1; }
  else if (quality == 2)
  { neargridstep = 2; fargridstep = 4; middetail = 5; fardetail = 5; lineardetail = -1; }
  else if (quality == 3)
  { neargridstep = 2; fargridstep = 4; middetail = 7; fardetail = 7; lineardetail = 0; }
  else if (quality == 4)
  { neargridstep = 2; fargridstep = 2; middetail = -1; fardetail = 7; lineardetail = 0; }
  else
  { neargridstep = 1; fargridstep = 2; middetail = 3; fardetail = 7; lineardetail = 0; }*/

  if (quality == 0)
  { neargridstep = 3; fargridstep = 3; middetail = -1; fardetail = -1; lineardetail = -1; }
  else if (quality == 1)
  { neargridstep = 2; fargridstep = 4; middetail = 3; fardetail = 3; lineardetail = -1; }
  else if (quality == 2)
  { neargridstep = 2; fargridstep = 4; middetail = 6; fardetail = 6; lineardetail = -1; }
  else if (quality == 3)
  { neargridstep = 2; fargridstep = 2; middetail = -1; fardetail = 7; lineardetail = 0; }
  else if (quality == 4)
  { neargridstep = 1; fargridstep = 2; middetail = 3; fardetail = 7; lineardetail = 0; }
  else
  { neargridstep = 1; fargridstep = 2; middetail = 5; fardetail = 7; lineardetail = 0; }

  if (phi < 0) phi += 360;
  else if (phi >= 360) phi -= 360;

  if (gamma < 0) gamma += 360;
  else if (gamma >= 360) gamma -= 360;

  if (phi < 0 || phi >= 360)
  {
    sprintf (buf, "Phi exceeds in file %s, line %d, val %d", __FILE__, __LINE__, phi);
    display (buf, LOG_ERROR);
  }

  if (gamma < 0 || gamma >= 360)
  {
    sprintf (buf, "Gamma exceeds in file %s, line %d, val %d", __FILE__, __LINE__, gamma);
    display (buf, LOG_ERROR);
  }

  glPushMatrix ();

  glScalef (MAXX2, ZOOM, MAXX2);
  glPushMatrix ();
  glTranslatef (0, -zoomz2, 0);
  glScalef (hh2, zoomz, hh2);

  gl->extractFrustum ();
    
  float pseudoview = getView ();
  float radius = pseudoview / cosi [45];

  int minx = (int) (camx - radius);
  int miny = (int) (camz - radius);
  int maxx = (int) (minx + radius * 2);
  int maxy = (int) (miny + radius * 2);

  space->z1->x = minx - MAXX2;
  space->z1->y = -MAXX2;
  space->z1->z = maxy - MAXX2;
  space->z2->x = maxx - MAXX2;
  space->z2->y = MAXX2;
  space->z2->z = miny - MAXX2;

  if (camera == 50)
  {
    minx = 0; maxx = MAXX;
    miny = 0; maxy = MAXX;
  }

  int parts = (int) (view / 13);
  parts *= 2;
  parts ++;
  if (parts >= PARTS)
  {
    display ("view exceeds ray casting blocks - not implemented", LOG_FATAL);
    exit (6);
  }

  int mp = parts / 2;
  for (i = 0; i < parts; i ++)
    for (i2 = 0; i2 < parts; i2 ++)
    {
      float d = dist (mp - i, mp - i2);
      detail [i] [i2] = (int) (d * 200.0F / view); // do not use pseudoview
    }

  float dx = (float) (maxx - minx + 1) / parts;
  float dy = (float) (maxy - miny + 1) / parts;

  // Efficient occlusion culling (kind of ray casting technique):
  // Run from inner grid point (viewer) to outer grid parts and check if grid points are hidden
  // This is currently not completely correct (needs two comparisons of inner fields), but
  // it already works very well, so I negligate the second comparison, as it would double the code

  for (i = 0; i < parts; i ++)
    for (i2 = 0; i2 < parts; i2 ++)
    {
      int ax = minx + (int) (dx * (float) i2);
      int ay = miny + (int) (dy * (float) i);
      int zx = minx + (int) (dx * (float) (i2 + 1));
      int zy = miny + (int) (dy * (float) (i + 1));
      vmin [i] [i2] = 65535;
      vmax [i] [i2] = 0;
      for (int i3 = 0; i3 < zy - ay + 1; i3 += 4)
        for (int i4 = 0; i4 < zx - ax + 1; i4 += 4)
        {
          int by = getCoord (ay + i3) / 4;
          int bx = getCoord (ax + i4) / 4;
          if (hcmin [bx] [by] < vmin [i] [i2]) vmin [i] [i2] = hcmin [bx] [by];
          if (hcmax [bx] [by] > vmax [i] [i2]) vmax [i] [i2] = hcmax [bx] [by];
        }
    }

  // ray casting
  bool dosecondtest = false;
  int count = 0;
  bool set = true;
  memset (vis, 0, PARTS * PARTS * sizeof (bool));
  int cx = parts / 2, cy = parts / 2;
  float ch = (unsigned int) ((camy / ZOOM + zoomz2) / zoomz); // - (h1*zoomz - zoomz2) * ZOOM;
  vh [cy] [cx] = ch;
  vis [cy] [cx] = set;
  for (i = cy + 1; i < parts; i ++)
    for (i2 = parts - i - 1; i2 < i + 1; i2 ++)
    {
      int lasty = 1;
      int lastx = 0;
      if (i2 < cx) lastx = -1;
      if (i2 > cx) lastx = 1;
//      if (i2 == cx - 1 && i2 > parts - i - 1) lastx = 0;
//      if (i2 == cx + 1 && i2 < i - 1) lastx = 0;
      int vminref = (int) vh [i - lasty] [i2 - lastx];
      int deltax = cx - i2 + lastx;
      int deltay = cy - i + lasty;
      float dist1 = sqrt ((float) deltax * deltax + deltay * deltay);
      float dist2 = sqrt ((float) lastx * lastx + lasty * lasty);
      int dh1 = vminref - (int) ch;
      int dhp;
      if (dist1 > 1E-4)
        dhp = (int) (dist2 * dh1 / dist1);
      else
        dhp = -30000;
      int h1 = vminref + dhp;
      if (h1 < vmin [i] [i2]) h1 = vmin [i] [i2];

      // also test non-diagonal element if available
      if (dosecondtest)
      {
        bool secondtest = false;
        if (i2 < cx && i2 > parts - i - 1)
        {
          lastx = 0;
          secondtest = true;
        }
        if (i2 > cx && i2 < i)
        {
          lastx = 0;
          secondtest = true;
        }
        if (secondtest)
        {
          vminref = (int) vh [i - lasty] [i2 - lastx];
          deltax = cx - i2 + lastx;
          deltay = cy - i + lasty;
          dist1 = sqrt ((float) deltax * deltax + deltay * deltay);
          dist2 = sqrt ((float) lastx * lastx + lasty * lasty);
          dh1 = vminref - (int) ch;
          if (dist1 > 1E-4)
            dhp = (int) (dist2 * dh1 / dist1);
          else
            dhp = -30000;
          int h11 = vminref + dhp;
          if (h11 < vmin [i] [i2]) h11 = vmin [i] [i2];
          if (h11 < h1) h1 = h11;
        }
      }

      vh [i] [i2] = h1;
      if (vmax [i] [i2] >= h1) vis [i] [i2] = set;
      else
      { vis [i] [i2] = !set; count ++; }
    }
  for (i = cy - 1; i >= 0; i --)
    for (i2 = i; i2 < parts - i; i2 ++)
    {
      int lasty = -1;
      int lastx = 0;
      if (i2 < cx) lastx = -1;
      if (i2 > cx) lastx = 1;
      if (i2 == cx - 1 && i2 > i) lastx = 0;
      if (i2 == cx + 1 && i2 < parts - 1) lastx = 0;
      int vminref = (int) vh [i - lasty] [i2 - lastx];
      int deltax = cx - i2 + lastx;
      int deltay = cy - i + lasty;
      float dist1 = sqrt ((float) deltax * deltax + deltay * deltay);
      float dist2 = sqrt ((float) lastx * lastx + lasty * lasty);
      int dh1 = vminref - (int) ch;
      int dhp;
      if (dist1 > 1E-4)
        dhp = (int) (dist2 * dh1 / dist1);
      else
        dhp = -30000;
      int h1 = vminref + dhp;
      if (h1 < vmin [i] [i2]) h1 = vmin [i] [i2];

      // also test non-diagonal element if available
      if (dosecondtest)
      {
        bool secondtest = false;
        if (i2 < cx && i2 > i)
        {
          lastx = 0;
          secondtest = true;
        }
        if (i2 > cx && i2 < parts - i - 1)
        {
          lastx = 0;
          secondtest = true;
        }
        if (secondtest)
        {
          vminref = (int) vh [i - lasty] [i2 - lastx];
          deltax = cx - i2 + lastx;
          deltay = cy - i + lasty;
          dist1 = sqrt ((float) deltax * deltax + deltay * deltay);
          dist2 = sqrt ((float) lastx * lastx + lasty * lasty);
          dh1 = vminref - (int) ch;
          if (dist1 > 1E-4)
            dhp = (int) (dist2 * dh1 / dist1);
          else
            dhp = -30000;
          int h11 = vminref + dhp;
          if (h11 < vmin [i] [i2]) h11 = vmin [i] [i2];
          if (h11 < h1) h1 = h11;
        }
      }

      vh [i] [i2] = h1;
      if (vmax [i] [i2] >= h1) vis [i] [i2] = set;
      else
      { vis [i] [i2] = !set; count ++; }
    }
  for (i2 = cx + 1; i2 < parts; i2 ++)
    for (i = parts - i2; i < i2; i ++)
    {
      int lasty = 0;
      int lastx = 1;
      if (i < cy) lasty = -1;
      if (i > cy) lasty = 1;
      if (i == cy - 1 && i > parts - i2) lasty = 0;
      if (i == cy + 1 && i < i2) lasty = 0;
      int vminref = (int) vh [i - lasty] [i2 - lastx];
      int deltax = cx - i2 + lastx;
      int deltay = cy - i + lasty;
      float dist1 = sqrt ((float) deltax * deltax + deltay * deltay);
      float dist2 = sqrt ((float) lastx * lastx + lasty * lasty);
      int dh1 = vminref - (int) ch;
      int dhp;
      if (dist1 > 1E-4)
        dhp = (int) (dist2 * dh1 / dist1);
      else
        dhp = -30000;
      int h1 = vminref + dhp;
      if (h1 < vmin [i] [i2]) h1 = vmin [i] [i2];

      // also test non-diagonal element if available
      if (dosecondtest)
      {
        bool secondtest = false;
        if (i < cy && i > parts - i2)
        {
          lasty = 0;
          secondtest = true;
        }
        if (i > cy && i < i2 - 1)
        {
          lasty = 0;
          secondtest = true;
        }
        if (secondtest)
        {
          vminref = (int) vh [i - lasty] [i2 - lastx];
          deltax = cx - i2 + lastx;
          deltay = cy - i + lasty;
          dist1 = sqrt ((float) deltax * deltax + deltay * deltay);
          dist2 = sqrt ((float) lastx * lastx + lasty * lasty);
          dh1 = vminref - (int) ch;
          if (dist1 > 1E-4)
            dhp = (int) (dist2 * dh1 / dist1);
          else
            dhp = -30000;
          int h11 = vminref + dhp;
          if (h11 < vmin [i] [i2]) h11 = vmin [i] [i2];
          if (h11 < h1) h1 = h11;
        }
      }

      vh [i] [i2] = h1;
      if (vmax [i] [i2] >= h1) vis [i] [i2] = set;
      else
      { vis [i] [i2] = !set; count ++; }
    }
  for (i2 = cx - 1; i2 >= 0; i2 --)
    for (i = i2 + 1; i < parts - i2 - 1; i ++)
    {
      int lasty = 0;
      int lastx = -1;
      if (i < cy) lasty = -1;
      if (i > cy) lasty = 1;
      if (i == cy - 1 && i > i2 - 1) lasty = 0;
      if (i == cy + 1 && i < parts - i2 - 1) lasty = 0;
      int vminref = (int) vh [i - lasty] [i2 - lastx];
      int deltax = cx - i2 + lastx;
      int deltay = cy - i + lasty;
      float dist1 = sqrt ((float) deltax * deltax + deltay * deltay);
      float dist2 = sqrt ((float) lastx * lastx + lasty * lasty);
      int dh1 = vminref - (int) ch;
      int dhp;
      if (dist1 > 1E-4)
        dhp = (int) (dist2 * dh1 / dist1);
      else
        dhp = -30000;
      int h1 = vminref + dhp;
      if (h1 < vmin [i] [i2]) h1 = vmin [i] [i2];

      // also test non-diagonal element if available
      if (dosecondtest)
      {
        bool secondtest = false;
        if (i < cy && i > i2 + 1)
        {
          lasty = 0;
          secondtest = true;
        }
        if (i > cy && i < parts - i2 - 2)
        {
          lasty = 0;
          secondtest = true;
        }
        if (secondtest)
        {
          vminref = (int) vh [i - lasty] [i2 - lastx];
          deltax = cx - i2 + lastx;
          deltay = cy - i + lasty;
          dist1 = sqrt ((float) deltax * deltax + deltay * deltay);
          dist2 = sqrt ((float) lastx * lastx + lasty * lasty);
          dh1 = vminref - (int) ch;
          if (dist1 > 1E-4)
            dhp = (int) (dist2 * dh1 / dist1);
          else
            dhp = -30000;
          int h11 = vminref + dhp;
          if (h11 < vmin [i] [i2]) h11 = vmin [i] [i2];
          if (h11 < h1) h1 = h11;
        }
      }

      vh [i] [i2] = h1;
      if (vmax [i] [i2] >= h1) vis [i] [i2] = set;
      else
      { vis [i] [i2] = !set; count ++; }
    }
//  printf ("c=%d ", count);

//  memset (vis, 0xFF, PARTS * PARTS * sizeof (bool));


  for (i = 0; i < 20; i ++)
    vertexarrayquad [i].glBegin (GL_QUADS);
  for (i = 0; i < 20; i ++)
    vertexarraytriangle [i].glBegin (GL_TRIANGLES);

  vertexarrayglitter [0].glBegin (GL_QUADS);
  vertexarrayglitter [1].glBegin (GL_QUADS);



  int zz1 = 0, zz = 0;

  if (quality <= 0 || camera == 50)
  {

    drawQuadStrip (minx, miny, maxx, maxy);

  }
  else
  {

    if (mode == 0)
    {

      for (i = 0; i < parts; i ++)
        for (i2 = 0; i2 < parts; i2 ++)
        if (vis [i] [i2])
        {
          int ax = (minx + (int) (dx * (float) i2));
          int ay = (miny + (int) (dy * (float) i));
          int zx = (minx + (int) (dx * (float) (i2 + 1)));
          int zy = (miny + (int) (dy * (float) (i + 1))/* + gridstep*/);

          if (fargridstep == 2)
          {
            ax -= ax & 1; ay -= ay & 1;
            zx -= zx & 1; zy -= zy & 1;
          }
          else if (fargridstep == 3)
          {
            ax -= ax % 3; ay -= ay % 3;
            zx -= zx % 3; zy -= zy % 3;
          }
          else if (fargridstep == 4)
          {
            ax -= ax & 3; ay -= ay & 3;
            zx -= zx & 3; zy -= zy & 3;
          }

          if (detail [i] [i2] > fardetail)
          {
            zy += fargridstep;
//            if (gl->isSphereInFrustum ((ax+zx)/2, (float)hw[getCoord((ax+zx)/2)][getCoord((ay+zy)/2)], ((float)(ay+zy)/2), (zx-ax)*2.0))
            if (gl->isSphereInFrustum (ax, (float)hw[getCoord(ax)][getCoord(ay)], (float)(ay), 0.00001) ||
                gl->isSphereInFrustum (ax, (float)hw[getCoord(ax)][getCoord(zy)], (float)(zy), 0.00001) ||
                gl->isSphereInFrustum (zx, (float)hw[getCoord(zx)][getCoord(ay)], (float)(ay), 0.00001) ||
                gl->isSphereInFrustum (zx, (float)hw[getCoord(zx)][getCoord(zy)], (float)(zy), 0.00001))
            {
              drawQuadStrip (ax, ay, zx, zy);
            }
            else
            {
              float sl = sunlight;
              sunlight = 10.0;
//              drawQuadStrip (ax, ay, zx, zy);
              sunlight = sl;
            }
          }
          else
          {
            if (detail [i] [i2] <= lineardetail)
            {
              gl->enableLinearTexture (texgrass->textureID);
              gl->enableLinearTexture (texgravel1->textureID);
              gl->enableLinearTexture (texredsand->textureID);
              gl->enableLinearTexture (texrocks->textureID);
              gl->enableLinearTexture (texwater->textureID);
              gl->enableLinearTexture (texredstone->textureID);
            }
            else
            {
              gl->disableLinearTexture (texgrass->textureID);
              gl->disableLinearTexture (texgravel1->textureID);
              gl->disableLinearTexture (texredsand->textureID);
              gl->disableLinearTexture (texrocks->textureID);
              gl->disableLinearTexture (texwater->textureID);
              gl->disableLinearTexture (texredstone->textureID);
            }
            if (detail [i] [i2] <= middetail)
            {
            if (i > 0) // south
            {
              if (detail [i - 1] [i2] > middetail)
              {
                if (fargridstep == 3 * neargridstep)
                {
                  ys = ay;
                  for (xs = ax; xs < zx;)
                  {
                    drawTexturedQuad (xs, ys, xs + 3 * neargridstep, ys, xs + 2 * neargridstep, ys, xs + 1 * neargridstep, ys);
                    xs += fargridstep;
                  }
                }
                else if (fargridstep == 2 * neargridstep)
                {
                  ys = ay;
                  for (xs = ax; xs < zx;)
                  {
                    drawTexturedTriangle (xs, ys, xs + 2 * neargridstep, ys, xs + 1 * neargridstep, ys);
                    xs += fargridstep;
                  }
                }
              }
            }
            if (i < parts - 1) // north
            {
              if (detail [i + 1] [i2] > middetail)
              {
                if (fargridstep == 3 * neargridstep)
                {
                  ys = zy;
                  for (xs = ax; xs < zx;)
                  {
                    drawTexturedQuad (xs, ys, xs + 1 * neargridstep, ys, xs + 2 * neargridstep, ys, xs + 3 * neargridstep, ys);
                    xs += fargridstep;
                  }
                }
                else if (fargridstep == 2 * neargridstep)
                {
                  ys = zy;
                  for (xs = ax; xs < zx;)
                  {
                    drawTexturedTriangle (xs, ys, xs + 1 * neargridstep, ys, xs + 2 * neargridstep, ys);
                    xs += fargridstep;
                  }
                }
              }
            }
            if (i2 > 0) // east
            {
              if (detail [i] [i2 - 1] > middetail)
              {
                if (fargridstep == 3 * neargridstep)
                {
                  xs = ax;
                  for (ys = ay; ys < zy;)
                  {
                    drawTexturedQuad (xs, ys, xs, ys + 1 * neargridstep, xs, ys + 2 * neargridstep, xs, ys + 3 * neargridstep);
                    ys += fargridstep;
                  }
                }
                else if (fargridstep == 2 * neargridstep)
                {
                  xs = ax;
                  for (ys = ay; ys < zy;)
                  {
                    drawTexturedTriangle (xs, ys, xs, ys + 1 * neargridstep, xs, ys + 2 * neargridstep);
                    ys += fargridstep;
                  }
                }
              }
            }
            if (i2 < parts - 1) // west
            {
              if (detail [i] [i2 + 1] > middetail)
              {
                if (fargridstep == 3 * neargridstep)
                {
                  xs = zx;
                  for (ys = ay; ys < zy;)
                  {
                    drawTexturedQuad (xs, ys, xs, ys + 3 * neargridstep, xs, ys + 2 * neargridstep, xs, ys + 1 * neargridstep);
                    ys += fargridstep;
                  }
                }
                else if (fargridstep == 2 * neargridstep)
                {
                  xs = zx;
                  for (ys = ay; ys < zy;)
                  {
                    drawTexturedTriangle (xs, ys, xs, ys + 2 * neargridstep, xs, ys + 1 * neargridstep);
                    ys += fargridstep;
                  }
                }
              }
            }
              gridstep = neargridstep;
            }
            else
            {
              gridstep = fargridstep;
            }
            for (xs = ax; xs < zx;)
            {
              x = GETCOORD(xs);
              for (ys = ay; ys < zy;)
              {
                y = GETCOORD(ys);
                zz1 ++;
				        int x2 = GETCOORD(xs+gridstep);
				        int y2 = GETCOORD(ys+gridstep);
				        if (h [x] [y] < hw [x] [y] && h [x2] [y] < hw [x2] [y] && h [x] [y2] < hw [x] [y2] && h [x2] [y2] < hw [x2] [y2])
			              ; // water
                else
                {
                  if (drawrule [x] [y] == 0)
                    drawTexturedQuad (xs, ys);
                  else if (drawrule [x] [y] == 2)
                    drawTexturedTriangle1 (xs, ys);
                  else
                    drawTexturedTriangle2 (xs, ys);
                }

                ys += gridstep;
                zz ++;
              }
              xs += gridstep;
            }
            for (xs = ax; xs < zx;)
            {
              x = GETCOORD(xs);
              for (ys = ay; ys < zy;)
              {
                y = GETCOORD(ys);
                zz1 ++;
                int xstep = GETCOORD(xs + gridstep);
                int ystep = GETCOORD(ys + gridstep);
                if (isWater (f [x] [y]) || isWater (f [xstep] [y]) || isWater (f [xstep] [ystep]) || isWater (f [x] [ystep]))
                {
                  drawWaterTexturedQuad (xs, ys);
                }
                ys += gridstep;
                zz ++;
              }
              xs += gridstep;
            }
          }

        } // for i2

    } // if mode

  }



  glDisable (GL_TEXTURE_2D);
  vertexarrayquad [0].glEnd ();
  vertexarraytriangle [0].glEnd ();
  for (i = 1; i < 20; i ++)
  {
    gl->enableTextures (i - 1);
    vertexarrayquad [i].glEnd ();
    vertexarraytriangle [i].glEnd ();
  }


  glEnable (GL_BLEND);
  glDepthFunc (GL_LEQUAL);
  glBlendFunc (GL_ONE, GL_SRC_ALPHA);
  glEnable (GL_ALPHA_TEST);
  glAlphaFunc (GL_GEQUAL, 0.2);
  gl->enableTextures (texglitter1->textureID);
  gl->enableLinearTexture (texglitter1->textureID);
  vertexarrayglitter [0].glEnd ();
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable (GL_ALPHA_TEST);
  glDisable (GL_BLEND);

  glEnable (GL_BLEND);
  glDepthFunc (GL_LEQUAL);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable (GL_ALPHA_TEST);
  glAlphaFunc (GL_GEQUAL, 0.02);
  glDisable (GL_TEXTURE_2D);
  vertexarrayglitter [1].glEnd ();
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable (GL_ALPHA_TEST);
  glDisable (GL_BLEND);



  glPopMatrix ();
  gl->extractFrustum ();



  int treestep = 2;
  if (quality >= 2) treestep = 1;

// Draw trees, bushes
  if (quality >= 1)
  {
    glPushMatrix ();
    glDisable (GL_CULL_FACE);
    if (quality >= 6)
    {
      gl->enableAlphaBlending ();
      glEnable (GL_ALPHA_TEST);
      glAlphaFunc (GL_GEQUAL, 0.1);
    }
    else
    {
      glEnable (GL_ALPHA_TEST);
      glAlphaFunc (GL_GEQUAL, 0.5);
    }
    gl->enableTextures (textree->textureID);
    glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    gl->enableTextures (textree2->textureID);
    glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    gl->enableTextures (textree3->textureID);
    glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    gl->enableTextures (textree4->textureID);
    glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    gl->enableTextures (textree5->textureID);
    glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    gl->enableTextures (texcactus1->textureID);
    glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    float mydep = 1000;
    if (quality == 2) mydep = 1800;
    else if (quality == 3) mydep = 2500;
    else if (quality == 4) mydep = 3200;
    else if (quality == 5) mydep = 3800;
    if (mydep > view * view) mydep = view * view;
    int cutdep = 800;

    int lineartree = -1;
    if (antialiasing) lineartree = 0;

    float treelightfac = lightfac * 1000.0 * 256.0 * 0.00085;

    for (i = 0; i < parts; i ++)
      for (i2 = 0; i2 < parts; i2 ++)
      {
        if (detail [i] [i2] > middetail)
          gridstep = fargridstep;
        else
          gridstep = neargridstep;
        if (detail [i] [i2] <= lineartree)
        {
          gl->enableLinearTexture (textree->textureID);
          gl->enableLinearTexture (textree2->textureID);
          gl->enableLinearTexture (textree3->textureID);
          gl->enableLinearTexture (textree4->textureID);
          gl->enableLinearTexture (textree5->textureID);
          gl->enableLinearTexture (texcactus1->textureID);
          gl->enableLinearTexture (textreeu->textureID);
          gl->enableLinearTexture (textreeu2->textureID);
          gl->enableLinearTexture (textreeu3->textureID);
          gl->enableLinearTexture (textreeu4->textureID);
          gl->enableLinearTexture (textreeu5->textureID);
          gl->enableLinearTexture (texcactusu1->textureID);
        }
        else
        {
          gl->disableLinearTexture (textree->textureID);
          gl->disableLinearTexture (textree2->textureID);
          gl->disableLinearTexture (textree3->textureID);
          gl->disableLinearTexture (textree4->textureID);
          gl->disableLinearTexture (textree5->textureID);
          gl->disableLinearTexture (texcactus1->textureID);
          gl->disableLinearTexture (textreeu->textureID);
          gl->disableLinearTexture (textreeu2->textureID);
          gl->disableLinearTexture (textreeu3->textureID);
          gl->disableLinearTexture (textreeu4->textureID);
          gl->disableLinearTexture (textreeu5->textureID);
          gl->disableLinearTexture (texcactusu1->textureID);
        }
        int ax = minx + (int) (dx * (float) i2);
        int ay = miny + (int) (dy * (float) i);
        int ex = minx + (int) (dx * (float) (i2 + 1));
        int ey = miny + (int) (dy * (float) (i + 1)) + treestep;
        float dep;
        if (treestep == 2)
        {
          ax -= ax & 1; ay -= ay & 1;
        }

        for (i3 = 1; i3 < 20; i3 ++)
          vertexarrayquad [i3].glBegin (GL_QUADS);
        for (i3 = 1; i3 < 20; i3 ++)
          vertexarraytriangle [i3].glBegin (GL_TRIANGLES);

        for (xs = ax; xs < ex;)
        {
          x = GETCOORD(xs);
          zz = 0;
          for (ys = ay; ys <= ey;)
          {
            y = GETCOORD(ys);
            float tdx = camx - xs;
            float tdy = camz - ys;
            dep = tdx * tdx + tdy * tdy;
            if (dep < mydep)
              if (isWoods (f [x] [y]) || isType (f [x] [y], REDTREE0) || isType (f [x] [y], CACTUS0))
                if (gl->isSphereInFrustum (hh2*(xs), (float)h[x][y]*zoomz - zoomz2, hh2*((ys)), hh2*2))
                {
                  float cg = g [x] [y];
                  fac = treelightfac * (nl [x] [y] + (short) dl [x] [y] * 16) * sunlight;
                  cg = (float) cg * fac;
                  if (cg >= 256.0) cg = 255.0;
                  treecolor.c [0] = treecolor.c [1] = treecolor.c [2] = (int) cg;
                  drawTreeQuad (xs, ys, phi, dep < cutdep);
                }
            ys += treestep;
          } // ys for
          xs += treestep;
        } // xs for

  for (i3 = 1; i3 < 20; i3 ++)
  {
    glBindTexture (GL_TEXTURE_2D, i3 - 1);
    vertexarrayquad [i3].glEnd ();
    vertexarraytriangle [i3].glEnd ();
  }

      }

    glDisable (GL_ALPHA_TEST);
    glPopMatrix ();
    gl->disableAlphaBlending ();

  }



  glDisable (GL_TEXTURE_2D);

  glPopMatrix ();

  if (quality >= 1 && hastowns)
  {
    float mydep = 1000;
    if (quality == 2) mydep = 1800;
    else if (quality == 3) mydep = 2600;
    else if (quality == 4) mydep = 3300;
    else if (quality == 5) mydep = 4000;
    if (mydep > view * view) mydep = view * view;

    for (i = 0; i < parts; i ++)
      for (i2 = 0; i2 < parts; i2 ++)
      {
        int ax = minx + (int) (dx * (float) i2);
        int ay = miny + (int) (dy * (float) i);
        int ex = minx + (int) (dx * (float) (i2 + 1));
        int ey = miny + (int) (dy * (float) (i + 1));
        float dep;
        for (xs = ax; xs < ex;)
        {
          x = GETCOORD(xs);
          zz = 0;
          for (ys = ay; ys <= ey;)
          {
            y = GETCOORD(ys);
            if (f [x] [y] == TOWN)
            {
              float tdx = camx - xs;
              float tdy = camz - ys;
              dep = tdx * tdx + tdy * tdy;
              if (dep < mydep)
                if (gl->isSphereInFrustum (hh2*(xs), (float)h[x][y]*zoomz - zoomz2, hh2*((ys)), hh2*2))
                {
                  drawTown (xs, ys);
                }
            }
            ys += 1;
          } // ys for
          xs += 1;
        } // xs for
      }
  }

  gridstep = neargridstep; // set to finer grid for ground collision detection
}

void GLLandscape::calcDynamicLight (CExplosion **explo, DynamicObj **cannon, DynamicObj **missile, DynamicObj **flare)
{
  int i, x, y;
  memset (dl, 0, (MAXX + 1) * (MAXX + 1));
  if (!dynamiclighting) return;
  for (i = 0; i < maxexplosion; i ++)
  {
    if (explo [i]->ttl > 0)
    {
      int mx = (int) explo [i]->tl->x;
      int mz = (int) explo [i]->tl->z;
      float h = explo [i]->tl->y - getHeight (explo [i]->tl->x, explo [i]->tl->z);
      if (h < 0) h = 0;
      float radius = h / 2 + 3;
      if (h < 50)
      {
        float intens = 100.0 - 2 * h;
        for (x = mx - (int) radius; x <= mx + (int) radius; x ++)
          for (y = mz - (int) radius; y <= mz + (int) radius; y ++)
          {
            int xn = GETCOORD(x);
            int yn = GETCOORD(y);
            int dx = x - mx, dy = y - mz;
            float dist = sqrt (dx*dx + dy*dy);
            if (dist < radius)
            {
              int light = (int) ((radius - dist) * intens / radius * explo [i]->zoom) + dl [xn] [yn];
              if (light > 255) light = 255;
              dl [xn] [yn] = light;
            }
          }
      }
    }
  }
  if (!day)
    for (i = 0; i < maxlaser; i ++)
    {
      if (cannon [i]->draw && cannon [i]->active)
      {
        int mx = (int) cannon [i]->tl->x;
        int mz = (int) cannon [i]->tl->z;
        float h = cannon [i]->tl->y - getHeight (cannon [i]->tl->x, cannon [i]->tl->z);
        if (h < 0) h = 0;
        float radius = h / 2 + 3;
        if (h < 15)
        {
          float intens = 75.0 - 5 * h;
          for (x = mx - (int) radius; x <= mx + (int) radius; x ++)
            for (y = mz - (int) radius; y <= mz + (int) radius; y ++)
            {
              int xn = GETCOORD(x);
              int yn = GETCOORD(y);
              int dx = x - mx, dy = y - mz;
              float dist = sqrt (dx*dx + dy*dy);
              if (dist < radius)
              {
                int light = (int) ((radius - dist) * intens / radius * cannon [i]->zoom) + dl [xn] [yn];
                if (light > 255) light = 255;
                dl [xn] [yn] = light;
              }
            }
        }
      }
    }
  for (i = 0; i < maxmissile; i ++)
  {
    if (missile [i]->draw && missile [i]->active)
    {
      int mx = (int) missile [i]->tl->x;
      int mz = (int) missile [i]->tl->z;
      float h = missile [i]->tl->y - getHeight (missile [i]->tl->x, missile [i]->tl->z);
      if (h < 0) h = 0;
      float radius = h / 2 + 3;
      if (h < 15)
      {
        float intens = 150.0 - 10 * h;
        for (x = mx - (int) radius; x <= mx + (int) radius; x ++)
          for (y = mz - (int) radius; y <= mz + (int) radius; y ++)
          {
            int xn = GETCOORD(x);
            int yn = GETCOORD(y);
            int dx = x - mx, dy = y - mz;
            float dist = sqrt (dx*dx + dy*dy);
            if (dist < radius)
            {
              int light = (int) ((radius - dist) * intens / radius * missile [i]->zoom) + dl [xn] [yn];
              if (light > 255) light = 255;
              dl [xn] [yn] = light;
            }
          }
      }
    }
  }
  float flarezoom = 0.2F;
  for (i = 0; i < maxflare; i ++)
  {
    if (flare [i]->draw && flare [i]->active)
    {
      int mx = (int) flare [i]->tl->x;
      int mz = (int) flare [i]->tl->z;
      float h = flare [i]->tl->y - getHeight (flare [i]->tl->x, flare [i]->tl->z);
      if (h < 0) h = 0;
      float radius = h / 2 + 3;
      if (h < 50)
      {
        float intens = 200.0 - 2 * h;
        for (x = mx - (int) radius; x <= mx + (int) radius; x ++)
          for (y = mz - (int) radius; y <= mz + (int) radius; y ++)
          {
            int xn = GETCOORD(x);
            int yn = GETCOORD(y);
            int dx = x - mx, dy = y - mz;
            float dist = sqrt (dx*dx + dy*dy);
            if (dist < radius)
            {
              int light = (int) ((radius - dist) * intens / radius * flarezoom) + dl [xn] [yn];
              if (light > 255) light = 255;
              dl [xn] [yn] = light;
            }
          }
      }
    }
  }
}

void GLLandscape::setMaterial (int n, float r, float g, float b, CTexture *tex)
{
  mat [n] [0] = r;
  mat [n] [1] = g;
  mat [n] [2] = b;
  mat [n] [3] = 1.0;
  texmap [n] = tex;
}

GLLandscape::GLLandscape (Space *space2, int type, int *heightmask)
{
  int i, i2;
  lsticker = 0;
  space = space2;
  randptr = 0;
//  if (!multiplayer || isserver)
  {
    if (type == LANDSCAPE_ALPINE || type == LANDSCAPE_ALPINE_NOLAKE || type == LANDSCAPE_LOW_ALPINE)
    {
      if (type == 0 || type == 1)
      {
        genSurface (60, heightmask);
        genRocks (30, 40);
      }
      else
      {
        genSurface (40, heightmask);
        genRocks (30, 10);
      }
      if (type == 0 || type == 2)
      {
        int lakes = myrandom (20) + 20;
        genLake (lakes);
        genLake (lakes / 3);
        genLake (lakes / 4);
        genLake (lakes / 4);
        genLake (2);
        genLake (2);
        genLake (2);
        genLake (2);
        genLake (2);
      }
      calcWoods (150);
    }
    else if (type == LANDSCAPE_ALPINE_EROSION)
    {
      genErosionSurface (50, heightmask);
      genRocks (30, 25);
      calcWoods (150);
    }
    else if (type == LANDSCAPE_ALPINE_SEA)
    {
      genSurface (60, heightmask);
      genRocks (30, 70);
      int diff = lowestpoint + (highestpoint - lowestpoint) * 3 / 4;
      for (i = 0; i <= MAXX; i ++)
        for (i2 = 0; i2 <= MAXX; i2 ++)
        {
          if (h [i] [i2] < diff)
          {
            hw [i] [i2] = diff;
            if (diff - h [i] [i2] < 1000)
              f [i] [i2] = SHALLOWWATER;
            else
              f [i] [i2] = DEEPWATER;
          }
        }
    }
    else if (type == LANDSCAPE_ALPINE_ROCKY)
    {
      genSurface (60, heightmask);
      genRocks (1, 99);
      genLake (10);
      genLake (10);
      genLake (10);
      genLake (10);
      genLake (10);
      genLake (10);
      genLake (10);
      genLake (10);
      calcWoods (200);
    }
    else if (type == LANDSCAPE_SEA)
    {
      for (i = 0; i <= MAXX; i ++)
        for (i2 = 0; i2 <= MAXX; i2 ++)
        {
          f [i] [i2] = DEEPWATER;
          h [i] [i2] = 10000;
          hw [i] [i2] = 30000;
        }
    }
    else if (type == LANDSCAPE_MOON)
    {
      genMoonSurface (60);
      for (i = 0; i <= MAXX; i ++)
        for (i2 = 0; i2 <= MAXX; i2 ++)
        {
          f [i] [i2] = MOONSAND;
        }
    }
    else if (type == LANDSCAPE_FLAT_MOON)
    {
      genMoonSurface (30);
      for (i = 0; i <= MAXX; i ++)
        for (i2 = 0; i2 <= MAXX; i2 ++)
        {
          f [i] [i2] = MOONSAND;
        }
    }
    else if (type == LANDSCAPE_CANYON)
    {
      genCanyonSurface (120);
    }
    else if (type == LANDSCAPE_ARCTIC)
    {
      genArcticSurface (60, NULL);
    }
    else if (type == LANDSCAPE_CANYON_TRENCH)
    {
      genCanyonSurface (10);
      genTrench (22, 3800);
    }
    else if (type == LANDSCAPE_DESERT)
    {
      genDesertSurface (20);
    }
  }
/*#ifdef HAVE_SDL_NET
  if (isserver)
  {
// Send map data to all clients
    char buf [10];
    for (i = 0; i <= MAXX; i ++)
    {
    printf (" %d ", i);
      server->sendMessage (1, (char *) h [i], (MAXX + 1) * 2);
      server->sendMessage (1, (char *) hw [i], (MAXX + 1) * 2);
      server->sendMessage (1, (char *) f [i], (MAXX + 1) * 1);
      while (!server->getMessage (1, buf)) ;
    }
  }
  if (multiplayer && !isserver)
  {
//    for (;;)
//    {
    for (i = 0; i <= MAXX; i ++)
    {
//    printf (" %d ", i);
      while (!client->getMessage ((char *) h [i])) ;
      while (!client->getMessage ((char *) hw [i])) ;
      while (!client->getMessage ((char *) f [i])) ;
      client->sendMessage (".", 1);
    }
  
  }
#endif*/

  lv [0] = 0.0; lv [1] = 1.0; lv [2] = 1.0;
  for (i = 0; i < MAXMATERIAL; i ++)
  {
    if (i == GRASS) { setMaterial (i, 0.4, 0.8, 0.3, texgrass); }
    else if (i >= CONIFEROUSWOODS1 && i <= MIXEDWOODS3) { setMaterial (i, 0.3, 0.55, 0.2, texgrass); }
    else if (i == ROCKS) { setMaterial (i, 0.7, 0.7, 0.7, texrocks); }
    else if (i == GLACIER) { setMaterial (i, 1.0, 1.0, 1.0, NULL); }
    else if (i >= DWARFPINES1 && i <= BUSHES3) { setMaterial (i, 0.3, 0.55, 0.2, texgrass); }
    else if (i == WATER) { setMaterial (i, 0.2, 1.0, 0.2, texwater); }
    else if (i == SHALLOWWATER) { setMaterial (i, 0.25, 1.0, 0.25, texwater); }
    else if (i == DEEPWATER) { setMaterial (i, 0.1, 0.25, 1.0, texwater); }
    else if (i == ROCKS2) { setMaterial (i, 0.5, 0.5, 0.5, texrocks); }
    else if (i == XSHALLOWWATER) { setMaterial (i, 0.3, 1.0, 0.3, texwater); }
    else if (i == XDEEPWATER) { setMaterial (i, 0.1, 0.15, 1.0, texwater); }
    else if (i == MOONSAND) { setMaterial (i, 0.8, 0.8, 0.8, texgrass); }
    else if (i == REDSTONE) { setMaterial (i, 0.95, 0.6, 0.4, texredstone); }
    else if (i == REDSAND || i == REDTREE0) { setMaterial (i, 0.9, 0.75, 0.55, texgrass); }
    else if (i == DESERTSAND || i == CACTUS0) { setMaterial (i, 1.0, 0.76, 0.35, texgrass); }
    else if (i == GREYSAND) { setMaterial (i, 0.7, 0.7, 0.65, texgrass); }
    else if (i == GRAVEL) { setMaterial (i, 0.75, 0.78, 0.68, texgravel1); }
    else if (i == TOWN) { setMaterial (i, 0.7, 0.7, 0.7, texgrass); }
    else { setMaterial (i, 0.4, 0.8, 0.3, texgrass); }
  }

/*  texmap [0] = texmap [1] = texmap [10] = texmap [12] = texmap [13] = texgrass;
  texmap [2] = texmap [7] = texrocks;
  texmap [4] = texmap [5] = texmap [6] = texmap [8] = texmap [9] = texwater;
  texmap [11] = texredstone;
  texmap [13] = texsand;
  texmap [3] = texmap [14] = texmap [15] = texmap [16] = NULL;

  if (f [x] [y] == GRASS) return 0;
  else if (f [x] [y] >= CONIFEROUSWOODS1 && f [x] [y] <= MIXEDWOODS3) return 1;
  else if (f [x] [y] == ROCKS) return 2;
  else if (f [x] [y] == GLACIER) return 3;
  else if (f [x] [y] >= DWARFPINES1 && f [x] [y] <= BUSHES3) return 1;
  else if (f [x] [y] == WATER) return 5;
  else if (f [x] [y] == SHALLOWWATER) return 4;
  else if (f [x] [y] == DEEPWATER) return 6;
  else if (f [x] [y] == ROCKS2) return 7;
  else if (f [x] [y] == XSHALLOWWATER) return 8;
  else if (f [x] [y] == XDEEPWATER) return 9;
  else if (f [x] [y] == MOONSAND) return 10;
  else if (f [x] [y] == REDSTONE) return 11;
  else if (f [x] [y] == REDSAND || f [x] [y] == REDTREE0) return 12;
  else if (f [x] [y] == DESERTSAND || f [x] [y] == CACTUS0) return 13;
  else if (f [x] [y] == GREYSAND) return 14;
  else if (f [x] [y] == GRAVEL) return 15;
  else if (f [x] [y] == TOWN) return 16;
  else return 0;

  mat [0] [0] = 0.4; mat [0] [1] = 0.8; mat [0] [2] = 0.3; mat [0] [3] = 1.0;
  mat [1] [0] = 0.3; mat [1] [1] = 0.55; mat [1] [2] = 0.2; mat [1] [3] = 1.0;
  mat [2] [0] = 0.7; mat [2] [1] = 0.7; mat [2] [2] = 0.7; mat [2] [3] = 1.0;
  mat [3] [0] = 1.0; mat [3] [1] = 1.0; mat [3] [2] = 1.0; mat [3] [3] = 1.0;
  mat [4] [0] = 0.25; mat [4] [1] = 1.0; mat [4] [2] = 0.25; mat [4] [3] = 1.0;
  mat [5] [0] = 0.2; mat [5] [1] = 1.0; mat [5] [2] = 0.2; mat [5] [3] = 1.0;
  mat [6] [0] = 0.1; mat [6] [1] = 0.25; mat [6] [2] = 1.0; mat [6] [3] = 1.0;
  mat [7] [0] = 0.5; mat [7] [1] = 0.5; mat [7] [2] = 0.5; mat [7] [3] = 1.0;
  mat [8] [0] = 0.3; mat [8] [1] = 1.0; mat [8] [2] = 0.3; mat [8] [3] = 1.0;
  mat [9] [0] = 0.1; mat [9] [1] = 0.15; mat [9] [2] = 1.0; mat [9] [3] = 1.0;
  mat [10] [0] = 0.8; mat [10] [1] = 0.8; mat [10] [2] = 0.8; mat [10] [3] = 1.0;
  mat [11] [0] = 0.95; mat [11] [1] = 0.6; mat [11] [2] = 0.4; mat [11] [3] = 1.0;
  mat [12] [0] = 0.9; mat [12] [1] = 0.75; mat [12] [2] = 0.55; mat [12] [3] = 1.0;
  mat [13] [0] = 1.0; mat [13] [1] = 0.76; mat [13] [2] = 0.35; mat [13] [3] = 1.0;
  mat [14] [0] = 0.7; mat [14] [1] = 0.7; mat [14] [2] = 0.65; mat [14] [3] = 1.0;
  mat [15] [0] = 0.75; mat [15] [1] = 0.78; mat [15] [2] = 0.68; mat [15] [3] = 1.0;
  mat [16] [0] = 0.7; mat [16] [1] = 0.7; mat [16] [2] = 0.7; mat [16] [3] = 1.0;*/

  for (i = 0; i <= MAXX; i ++)
    for (i2 = 0; i2 <= MAXX; i2 ++)
      if (hw [i] [i2] == 0)
      {
        hw [i] [i2] = h [i] [i2];
      }

  i = 0;
  while (i < 256)
  {
    bool again = false;
    xtree [i] = -0.48 + 0.001 * myrandom (960);
    ytree [i] = -0.48 + 0.001 * myrandom (960);
    for (i2 = i - 1; i2 >= 0 && i2 >= i - 6; i2 --)
    {
      if (fabs (xtree [i] - xtree [i2]) + fabs (ytree [i] - ytree [i2]) < 0.08)
      {
        again = true;
        break;
      }
    }
    if (!again) i ++;
  }

  if (type >= 0) precalculate (); // do not precalculate anything for custom height maps
}

#endif
