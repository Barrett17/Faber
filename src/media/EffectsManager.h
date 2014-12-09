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
#ifndef _EFFECTS_MANAGER_H_
#define _EFFECTS_MANAGER_H_


#include "AudioEffects.h"
#include "FaberEffect.h"


class EffectsManager {
public:
								EffectsManager();
	virtual 					~EffectsManager();

	static EffectsManager*		Get();

	const char*					EffectToString(int32 index) const;
	//FaberEffectList&			StandardEffects();
	//FaberEffectList&			EffectsByFlag(int32 flag);

	int32						CountEffects() const;
	FaberEffect*				GetEffect(int32 index) const;
	FaberEffect*				GetEffect(const char* name) const;

private:
	static EffectsManager*		fInstance;

	FaberEffectList				fEffectsList;
};

#endif	// _EFFECTS_MANAGER_H_
