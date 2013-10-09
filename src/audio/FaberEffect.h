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

#ifndef _FABER_EFFECT_H_
#define _FABER_EFFECT_H_

#include <MenuItem.h>

#include "AudioTrack.h"

enum FaberEffectsKind {
	FABER_AUDIO_EFFECT,
	FABER_CONSUMER,
	FABER_FILTER,
	FABER_MULTIPASS_EFFECT,
	FABER_PRODUCER,
	FABER_REALTIME_EFFECT
};

#define CONTROL_CHANGED	'chgd'


class FaberEffect
{
public:
							FaberEffect(const char* name, uint32 flags);
	virtual 				~FaberEffect();

	const char*				Name() const;

	virtual BView*			SettingsPanel() = 0;
	BMenuItem*				BuildItem();

	int32					Flags() const;

	status_t				FilterTrack(AudioTrack* track,
								int64 start, size_t size);

	virtual void			FilterBuffer(float* buffer, size_t size) = 0;

	status_t				FlattenSettings(BMessage* message);
	status_t				UnflattenSettings(BMessage* message);

	//int32					Pass() const;
	//void					SetPass(int32 pass);

private:
	uint32					fFlags;
	const char*				fName;
};

#endif	// _FABER_EFFECT_H_
