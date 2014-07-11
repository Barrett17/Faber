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
#include "FaberShortcut.h"
#include "SettingsManager.h"
#include "WindowsManager.h"

#include <stdio.h>


#define FABER_HELP_HOMEPAGE "http://www.versut.com/Faber"

#define FABER_DESCRIPTION "Faber is a multitrack audio editor for Haiku."

#define FABER_VERSION "R1 Alpha1"

#define FABER_COPYRIGHT "Dario Casalinuovo"

// Debug

//#define FABER_DEBUG 0

// Translation

#define B_TRANSLATION_CONTEXT "Faber"

#define catSig "x-vnd.Be.locale.Faber"
#define catName catSig".catalog"

// Settings

#define FABER_SETTINGS_DIR "Faber"
#define FABER_SETTINGS_FILE "Faber_Settings"
#define FABER_KEYMAP_CONF "Keymap_Conf"

// UI settings

#define WINDOW_DEFAULT_SIZE_X	800
#define WINDOW_DEFAULT_SIZE_Y	600

#define WINDOW_MIN_SIZE_X		530
#define WINDOW_MIN_SIZE_Y		256

#define WINDOW_MAX_SIZE_X		2048
#define WINDOW_MAX_SIZE_Y		1536

#endif
