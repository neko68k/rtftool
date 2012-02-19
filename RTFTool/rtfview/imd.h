/*  imd.h

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

#ifndef __IMD_H__
#define __IMD_H__
#include <stdio.h>
#include "../binfile.h"
#include "common.h"


void IMD_Load0x01(BINHANDLE *infile, IMD_SEG *seg);
void IMD_Load0x02(BINHANDLE *infile);


#endif // __IMD_H__
