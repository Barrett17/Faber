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

#ifndef FABER_SHORTCUT_H
#define FABER_SHORTCUT_H

#include <ObjectList.h>
#include <SupportDefs.h>

#include "FaberShortcut.h"


class KeyBind {
public:
	const char*		label;

	char			key;
	int32			mod;

	uint32			message;
	uint32			itemType;
};


class FaberShortcut {
public:
	static void						LoadKeys();

	static void						AddKeyBind(KeyBind* bind);
	static KeyBind*					FindKeyBind(uint32 code);
	static KeyBind*					KeyBindAt(int32 index);
	static int32					CountKeys();

private:
	static void						CreateDefaultKeys();

	static void						_CopyObj(KeyBind* bind,
										KeyBind* from);

	static BObjectList<KeyBind>		fBinds;
};

#endif
