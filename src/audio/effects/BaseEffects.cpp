/*
   	Copyright (c) 2003, Xentronix
	Author: Frans van Nispen (frans@xentronix.com)
	All rights reserved.
	
	Redistribution and use in source and binary forms, with or without modification,
	are permitted provided that the following conditions are met:
	
	Redistributions of source code must retain the above copyright notice, this list
	of conditions and the following disclaimer. Redistributions in binary form must
	reproduce the above copyright notice, this list of conditions and the following
	disclaimer in the documentation and/or other materials provided with the distribution. 
	
	Neither the name of Xentronix nor the names of its contributors may be used
	to endorse or promote products derived from this software without specific prior
	written permission. 
	
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
	EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
	SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
	PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
	INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/*
 * Copyright 2013-2015 Dario Casalinuovo
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#include "BaseEffects.h"

#include "FaberDefs.h"


SwapEffect::SwapEffect()
	:
	AudioEffect(B_TRANSLATE("Swap"),
		FABER_REALTIME_EFFECT | FABER_FILTER)
{
}


void
SwapEffect::FilterBuffer(float* buffer, int64 frames)
{
	for (int64 i = 0; i < frames; i+=2) {
		float tmp = buffer[i+0];
		buffer[i+0] = buffer[i+1];
		buffer[i+1] = tmp;
	}
}


InvertEffect::InvertEffect()
	:
	AudioEffect(B_TRANSLATE("Invert"),
		FABER_REALTIME_EFFECT | FABER_FILTER)
{
}


void
InvertEffect::FilterBuffer(float* buffer, int64 frames)
{
	for (int64 i = 0; i < frames; i++) {
		buffer++;
		*buffer = -*buffer;
	}
}


SilenceEffect::SilenceEffect()
	:
	AudioEffect(B_TRANSLATE("Silence"),
		FABER_REALTIME_EFFECT | FABER_FILTER)
{
}


void
SilenceEffect::FilterBuffer(float* buffer, int64 frames)
{
	for (int64 i = 0; i < frames; i++)
		*buffer++ = 0.0f;
}


FadeInEffect::FadeInEffect()
	:
	AudioEffect(B_TRANSLATE("Fade-In"),
		FABER_REALTIME_EFFECT | FABER_FILTER)
{
	count = 0;
}


void
FadeInEffect::FilterBuffer(float* buffer, int64 frames)
{
	for (int64 i = 0; i < frames; i++) {
		buffer++;
		*buffer = (*buffer)*(float)count / (float)frames/*/ m_total*/;
		count++;
	}
}


FadeOutEffect::FadeOutEffect()
	:
	AudioEffect(B_TRANSLATE("FadeOut"),
		FABER_REALTIME_EFFECT | FABER_FILTER)
{
}


void
FadeOutEffect::FilterBuffer(float* buffer, int64 frames)
{
	count = frames;
	for (int64 i = 0; i < frames; i++) {
		buffer++;
		*buffer = (*buffer)*(float)count / (float)frames/*/ m_total*/;
		count--;
	}
}
