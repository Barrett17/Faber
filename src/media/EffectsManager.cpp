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


#include "AudioEffects.h"
#include "EffectsManager.h"

#include <String.h>

#include <stdio.h>


BObjectList<FaberEffect> gEffectsList;

void
EffectsManager::InitEffects()
{
	for (int32 i = 0; gStandardAudioEffects[i].effect != NULL; i++) {
		gEffectsList.AddItem(gStandardAudioEffects[i].effect);
	}
}


const char*
EffectsManager::EffectToString(int32 index)
{
	return gEffectsList.ItemAt(index)->Name();
}


int32
EffectsManager::CountEffects()
{
	return gEffectsList.CountItems();
}


FaberEffect*
EffectsManager::GetEffect(int32 index)
{
	return gEffectsList.ItemAt(index);
}
