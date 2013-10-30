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

#ifndef _AUDIO_EFFECT_H_
#define _AUDIO_EFFECT_H_

#include "FaberEffect.h"


class AudioEffect : public FaberEffect
{
public:
							AudioEffect(const char* name, uint32 flags);
	virtual 				~AudioEffect();

	virtual status_t		FilterTrack(AudioTrack* track,
								int64 start, size_t size);
};

#endif
