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

#include "Shortcut.h"

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


KeyBind*
FaberShortcut::FindKeyBind(uint32 code)
{
	for (int i = 0; i < CountKeys(); i++) {
		KeyBind* bind = fBinds.ItemAt(i);
		if (bind->code == code)
			return bind;
	}
	return NULL;
}


const char*
FaberShortcut::GetLabel(uint32 code)
{
	KeyBind* bind = FindKeyBind(code);
	return bind->label;
}


const char*
FaberShortcut::GetLabel(int32 index)
{
	return fBinds.ItemAt(index)->label;
}


char
FaberShortcut::GetKey(uint32 code)
{
	KeyBind* bind = FindKeyBind(code);
	return bind->key;
}


char
FaberShortcut::GetMod(uint32 code)
{
	KeyBind* bind = FindKeyBind(code);
	return bind->mod;
}


char
FaberShortcut::GetKeyAlt(uint32 code)
{
	KeyBind* bind = FindKeyBind(code);
	return bind->altKey;
}


char
FaberShortcut::GetModAlt(uint32 code)
{
	KeyBind* bind = FindKeyBind(code);
	return bind->altMod;
}


uint32
FaberShortcut::GetCode(int32 index)
{
	return fBinds.ItemAt(index)->code;
}


uint32
FaberShortcut::GetCode(char key, int32 mod)
{

}


uint32
FaberShortcut::GetMessage(int32 key, int32 mod)
{

}


int32
FaberShortcut::CountKeys()
{
	return fBinds.CountItems();
}


bool
FaberShortcut::IsMenuItem(uint32 code)
{
	KeyBind* bind = FindKeyBind(code);
	return bind->isMenuItem;
}


void
FaberShortcut::CreateDefaultKeys()
{
	for (int i = 0; kDefaultKeymap[i] != NULL; i++) {
		KeyBind* items = kDefaultKeymap[i]; 
		for (int j = 0;  items[j].code != FABER_EOF; j++) {
			KeyBind* bind = new KeyBind();

			if (bind->code == FABER_SPLITTER)
				continue;

			_CopyObj(bind, &items[j]);
			fBinds.AddItem(bind);
		}
	}
}


void
FaberShortcut::AddKeyBind(KeyBind* keybind)
{
	KeyBind* bind = FindKeyBind(keybind->code);
	if (bind != NULL) {
		fBinds.RemoveItem(bind);	
		delete bind;
	}

	fBinds.AddItem(keybind);
}


void
FaberShortcut::_CopyObj(KeyBind* bind, KeyBind* from)
{
	bind->isMenuItem = from->isMenuItem;
	bind->label = from->label;
	bind->key = from->key;
	bind->mod = from->mod;
	bind->altMod = from->altMod;
	bind->altKey = from->altKey;
	bind->code = from->code;
}
