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

#ifndef AUDIO_EFFECTS_H
#define AUDIO_EFFECTS_H

#include "FaberEffect.h"

// effects headers
#include "Amplifier.h"
#include "BassBoost.h"
#include "Compressor.h"
#include "Delay.h"


struct StandardAudioEffect {
	FaberEffect* effect;
};

static StandardAudioEffect gStandardAudioEffects[] = {

	{ new AmplifierEffect() },
	{ new BassBoostEffect() },
	{ new CompressorEffect() },
	{ new DelayEffect() },
	{ NULL }
};

#endif // AUDIO_EFFECTS_H
