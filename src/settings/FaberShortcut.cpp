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

#include "DefaultKeymap.h"

FaberShortcut* FaberShortcut::fInstance = NULL;

// TODO using HashMap we can go from an O(n) execution time to O(log n)


FaberShortcut*
FaberShortcut::Get()
{
	if (fInstance == NULL)
		fInstance = new FaberShortcut();

	return fInstance;	
}


FaberShortcut::FaberShortcut()
	:
	fBinds(true)
{
	CreateDefaultKeys();
}


FaberShortcut::~FaberShortcut()
{
}


void
FaberShortcut::CreateDefaultKeys()
{
	for (int i = 0; kDefaultKeymap[i] != NULL; i++) {
		KeyBind* items = kDefaultKeymap[i];
		for (int j = 0; items[j].itemType != FABER_EOF; j++) {
			KeyBind* bind = new KeyBind();

			if (items[j].itemType == FABER_SPLITTER)
				continue;

			_CopyObj(bind, &items[j]);

			fBinds.AddItem(bind);
		}
	}
}


KeyBind*
FaberShortcut::FindKeyBind(uint32 code)
{
	for (int i = 0; i < CountKeys(); i++) {
		KeyBind* bind = fBinds.ItemAt(i);
		if (bind->message == code)
			return bind;
	}
	return NULL;
}


KeyBind*
FaberShortcut::KeyBindAt(int32 index)
{
	return fBinds.ItemAt(index);
}


int32
FaberShortcut::CountKeys()
{
	return fBinds.CountItems();
}


void
FaberShortcut::AddKeyBind(KeyBind* keybind)
{
	KeyBind* bind = FindKeyBind(keybind->message);
	if (bind != NULL) {
		fBinds.RemoveItem(bind);	
		delete bind;
	}

	fBinds.AddItem(keybind);
}


void
FaberShortcut::_CopyObj(KeyBind* bind, KeyBind* from)
{
	bind->label = from->label;
	bind->key = from->key;
	bind->mod = from->mod;
	bind->message = from->message;
	bind->itemType = from->itemType;
}
