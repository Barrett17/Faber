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


#include "EffectsManager.h"

#include <String.h>


EffectsManager* EffectsManager::fInstance = NULL;


EffectsManager*
EffectsManager::Get()
{
	if (fInstance == NULL)
		fInstance = new EffectsManager();

	return fInstance;	
}


EffectsManager::EffectsManager()
	:
	fEffectsList(true)
{
	for (int32 i = 0; gStandardAudioEffects[i].effect != NULL; i++) {
		fEffectsList.AddItem(gStandardAudioEffects[i].effect);
	}
}


EffectsManager::~EffectsManager()
{

}


const char*
EffectsManager::EffectToString(int32 index) const
{
	return fEffectsList.ItemAt(index)->Name();
}


FaberEffectList&
EffectsManager::StandardEffects()
{
	return EffectsByFlag(FABER_BUILTIN_EFFECT);
}


FaberEffectList&
EffectsManager::EffectsByFlag(int32 flag)
{
}


int32
EffectsManager::CountEffects() const
{
	return fEffectsList.CountItems();
}


FaberEffect*
EffectsManager::GetEffect(int32 index) const
{
	return fEffectsList.ItemAt(index);
}


FaberEffect*
EffectsManager::GetEffect(const char* name) const
{
	for (int32 i = 0; i < CountEffects(); i++) {
		FaberEffect* effect = GetEffect(i);
		BString str(effect->Name());
		if (str.Compare(name) == 0)
			return effect;
	}
	return NULL;
}


status_t
EffectsManager::RunEffect(int32 id, AudioTrack* track, size_t size)
{
	return B_ERROR;
}


status_t
EffectsManager::RunEffect(FaberEffect* effect, AudioTrack* track, size_t size)
{
	return B_ERROR;
}
