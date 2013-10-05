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

public class EffectsManager
{
public:
								EffectsManager();
	virtual 					~EffectsManager();

	const char*					EffectToString(int32 index) const;

	BMenuItem*					FilterItemAt(int32 index) const;
	int32						CountFilterItems() const;

	FaberEffect*				GetEffect(int32 id) const;

	status_t					RunEffect(AudioTrack* track,
									float* buffer, size_t size
									int32 id);

	status_t					RunEffect(AudioTrack* track,
									float* buffer, size_t size
									FaberEffect* effect);
private:

	BObjectList<FaberEffect*>	fEffectsList;
}

#endif	// _EFFECTSM_ANAGER_H_
