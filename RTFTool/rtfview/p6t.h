/*  p6t.h

    Copyright 2010 Shaun Thompson

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

#ifndef __P6T_H__
#define __P6T_H__
#ifdef WIN32

#else
#include <SDL/SDL.h>
#endif
#include "common.h"

P6T_EXPORT P6T_LoadTexture(char *fn, GLuint texID, char *exePath, BYTE stat);
P6T_EXPORT P6T_LoadTexture(BYTE *data, GLuint texID, BYTE stat, bool flip);

#endif // __P6T_H__
