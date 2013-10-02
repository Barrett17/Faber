/*
    Copyright 2013 Dario Casalinuovo. All rights reserved.

    This file is part of Faber.

    Faber is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Faber is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Faber.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef FABER_DEFS_H
#define FABER_DEFS_H

#include <SupportDefs.h>
#include <Locale.h>
#include <Catalog.h>

#include "FaberMessages.h"
#include "FaberResources.h"
#include "Preferences.h"

#include <stdio.h>

// Debug

//#define FABER_DEBUG 0
//#define DEBUG_DISABLE_DRAWPART 0

// Translation

#define B_TRANSLATION_CONTEXT "Faber"

#define catSig "x-vnd.Be.locale.Faber"
#define catName catSig".catalog"

// Enums

enum {
	FABER_SELECTION_TOOL,
	FABER_DRAW_TOOL,
	FABER_PLAY_TOOL	
};


#endif
