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

#ifndef COMPRESSOR_EFFECT_H
#define COMPRESSOR_EFFECT_H

#include <CheckBox.h>

#include "AudioEffect.h"
#include "SpinSlider.h"


class CompressorEffect : public AudioEffect
{
public:
	CompressorEffect(uint32 flags = FABER_REALTIME_EFFECT
		| FABER_FILTER | FABER_GUI_EFFECT);

	virtual bool InitFilter(float f, int32 channels = 2,
		int32 pass = 0, int32 size = 0);

	virtual void				DeAllocate();
	virtual void 				FilterBuffer(float *, int64 frames);

	virtual BView*				SettingsPanel();
			status_t			ArchiveSettings(BMessage* message);
			status_t			UpdateSettings(BMessage* message);
			status_t			SettingsChanged();
   
private:
	float *delay_buffer;
	float mRMSSumL, mMultL, mDecayMult;
	float mRMSSumR, mMultR;
	float mThreshold, mGain, mInvRatio;
	int32 buffer_size, pBuffer;
	SpinSlider *attac, *decay, *ratio, *gain, *treshold;
	BCheckBox *rms;
};

#endif
