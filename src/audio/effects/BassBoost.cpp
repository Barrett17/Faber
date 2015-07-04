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

#include "BassBoost.h"

#include <LayoutBuilder.h>
#include <StringView.h>
#include <View.h>

#include "FaberDefs.h"


BassBoostEffect::BassBoostEffect(uint32 flags)
	:
	AudioEffect(B_TRANSLATE("Bass Boost"), flags)
{
	xn1 = 0, xn2 = 0, yn1 = 0, yn2 = 0;
	xn3 = 0, xn4 = 0, yn3 = 0, yn4 = 0;
}


BView*
BassBoostEffect::SettingsPanel()
{
	BRect r(0,0,200,100);

	BView *view = new BView(r, "BassBoost", B_FOLLOW_ALL, B_WILL_DRAW);
	view->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	freq = new SpinSlider("frequency", B_TRANSLATE("Frequency (Hz)"),
		new BMessage(CONTROL_CHANGED), 1, 1000);

	//freq->SetValue(Prefs.filter_bassboost_frequency);

	boost = new SpinSlider("frequency", B_TRANSLATE("Boost (dB)"),
		new BMessage(CONTROL_CHANGED), 0, 24);

//	boost->SetValue(Prefs.filter_bassboost_boost);

	BLayoutBuilder::Group<>(view, B_VERTICAL, 0)
		.Add(freq, 0)
		.Add(boost, 1)
	.End();

	return view;
}


status_t
BassBoostEffect::ArchiveSettings(BMessage* message)
{
	return B_ERROR;
}


status_t
BassBoostEffect::UpdateSettings(BMessage* message)
{
	return B_ERROR;
}


status_t
BassBoostEffect::SettingsChanged()
{
	return B_OK;
}


/*
status_t
BassBoostEffect::FlattenSettings(BMessage* message)
{
	//Prefs.filter_bassboost_frequency = freq->Value();
	//Prefs.filter_bassboost_boost = boost->Value();
}*/


void
BassBoostEffect::FilterBuffer(float *buffer, int64 frames)
{
	// Compute coefficents of the biquand IIR filter
	/*float omega = 2 * 3.141592653589 * Prefs.filter_bassboost_frequency / m_frequency;

	float sn = sin(omega);
	float cs = cos(omega);

	float a = exp(log(10) * Prefs.filter_bassboost_boost / 40);

	//Low Shelf filter's shape, if this is too large
	// or too small it will result an unstable filter
	float shape = 1.0;
	float beta = sqrt((a * a + 1) / shape - (pow((a - 1), 2)));

	//  Coefficients  
	float b0 = a * ((a + 1) - (a - 1) * cs + beta * sn);
	float b1 = 2 * a * ((a - 1) - (a + 1) * cs);
	float b2 = a * ((a + 1) - (a - 1) * cs - beta * sn);
	float a0 = ((a + 1) + (a - 1) * cs + beta * sn);
	float a1 = -2 * ((a - 1) + (a + 1) * cs);
	float a2 = (a + 1) + (a - 1) * cs - beta * sn;

	// initialise the filter
	float out, in = 0;

		// Mono	
		for (size_t i=0; i<size; i++){
			in = buffer[i];
			out = (b0 * in + b1 * xn1 + b2 * xn2 - a1 * yn1 - a2 * yn2) / a0;
			xn2 = xn1;
			xn1 = in;
			yn2 = yn1;
			yn1 = out;

			if (out < -1.0)
				out = -1.0;
			else if (out > 1.0)
				out = 1.0;        //Prevents clipping

			buffer[i] = out;
		}*/
}
