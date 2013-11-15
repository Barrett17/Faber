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


class KeyBind {
public:
	bool			isMenuItem;
	const char*		label;
	char			key;
	int32			mod;
	char			altKey;
	int32			altMod;
	uint32			code;
};

class FaberShortcut {
public:
							FaberShortcut();
							~FaberShortcut();

	static FaberShortcut* 	Get();

	KeyBind*				FindKeyBind(uint32 code);

	const char*				GetLabel(uint32 code);
	const char*				GetLabel(int32 index);

	char					GetKey(uint32 code);
	char					GetMod(uint32 code);

	char					GetKeyAlt(uint32 code);
	char					GetModAlt(uint32 code);

	uint32					GetCode(int32 index);
	uint32					GetCode(char key, int32 mod);

	uint32					GetMessage(int32 key, int32 mod);

	bool					IsMenuItem(uint32 code);
	int32					CountKeys();

	void					CreateDefaultKeys();

	void					AddKeyBind(KeyBind* bind);

private:
	void					_CopyObj(KeyBind* bind, KeyBind* from);

	static FaberShortcut*	fInstance;

	BObjectList<KeyBind>	fBinds;
};

#endif
