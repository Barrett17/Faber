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
#include "Compressor.h"

#include <LayoutBuilder.h>
#include <View.h>

#include "FaberDefs.h"


CompressorEffect::CompressorEffect(uint32 flags)
	:
	AudioEffect(B_TRANSLATE("Compressor"), flags)
{

}


BView*
CompressorEffect::SettingsPanel()
{
	BRect r(0,0,280,280);

	BView *view = new BView(r, NULL, B_FOLLOW_ALL, B_WILL_DRAW);
	view->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	rms = new BCheckBox("calcrms", B_TRANSLATE("Calculate in RMS"),
		new BMessage(CONTROL_CHANGED));

	//rms->SetValue(Prefs.filter_compressor_rms);

	attac = new SpinSlider("attacktime", B_TRANSLATE("Attack Time (ms)"),
		new BMessage(CONTROL_CHANGED), 0, 1000);

	//attac->SetValue(Prefs.filter_compressor_attac * 1000);

	decay = new SpinSlider("decay", B_TRANSLATE("Decay time (ms)"),
		new BMessage(CONTROL_CHANGED), 0, 1000);

	//decay->SetValue(Prefs.filter_compressor_decay * 1000);

	treshold = new SpinSlider("threshold", B_TRANSLATE("Threshold (dB)"),
		new BMessage(CONTROL_CHANGED), -60, 18);

	//treshold->SetValue(Prefs.filter_compressor_treshold);

	ratio = new SpinSlider("ratio", B_TRANSLATE("Ratio"),
		new BMessage(CONTROL_CHANGED), 1, 100);

	//ratio->SetValue(Prefs.filter_compressor_ratio);

	gain = new SpinSlider("gain", B_TRANSLATE("Gain (dB)"), new BMessage(CONTROL_CHANGED), 0, 24);

	//gain->SetValue(Prefs.filter_compressor_gain);

	BLayoutBuilder::Group<>(view, B_VERTICAL, 5)
		.Add(rms, 0)
		.Add(attac, 1)
		.Add(decay, 2)
		.Add(treshold, 3)
		.Add(ratio, 4)
		.Add(gain, 5)
	.End();

	return view;
}


status_t
CompressorEffect::ArchiveSettings(BMessage* message)
{
	/*Prefs.filter_compressor_rms = rms->Value();
	Prefs.filter_compressor_attac = attac->Value()/1000.0;
	Prefs.filter_compressor_decay = decay->Value()/1000.0;
	Prefs.filter_compressor_treshold = treshold->Value();
	Prefs.filter_compressor_ratio = ratio->Value();
	Prefs.filter_compressor_gain = gain->Value();

	for (int32 i=0; i<buffer_size; i++)
		delay_buffer[i] = 0;*/
	return B_OK;
}



status_t
CompressorEffect::UpdateSettings(BMessage* msg)
{
	return B_ERROR;
}


status_t
CompressorEffect::SettingsChanged()
{
	return B_OK;
}


bool
CompressorEffect::InitFilter(float f, int32 c, int32 pass, int32 size)
{
	/*RealtimeFilter::InitFilter(f, c, pass, size);

	buffer_size = (int32)(m_frequency +.5);

	delay_buffer = new float[ buffer_size ];
	for (int32 i=0; i<buffer_size; i++)	
		delay_buffer[i] = 0.0;

	mRMSSumL = 0.0;
	mMultL = 1.0;
	mRMSSumR = 0.0;
	mMultR = 1.0;
	pBuffer = 0;
*/
	return true;
}


void
CompressorEffect::DeAllocate()
{
	delete[] delay_buffer;
}


void
CompressorEffect::FilterBuffer(float* buffer, int64 frames)
{
	/*mDecayMult = exp(log(0.1)/(Prefs.filter_compressor_decay*m_frequency));
	mThreshold = pow(10.0, Prefs.filter_compressor_treshold/10);
	mGain = pow(10.0, Prefs.filter_compressor_gain/10);
	mInvRatio = 1.0 - 1.0 / Prefs.filter_compressor_ratio;


	float mult, levelL, levelR;
 
		for (size_t i=0; i<size; i++){
			if (Prefs.filter_compressor_rms) {
				// Calculate current level from root-mean-squared of
				// circular buffer ("RMS")
				mRMSSumL -= delay_buffer[pBuffer];
				delay_buffer[pBuffer] = buffer[i] * buffer[i];
				mRMSSumL += delay_buffer[pBuffer];
				levelL = sqrt(mRMSSumL/buffer_size);
				pBuffer = (pBuffer+1)%buffer_size;
			}else{
				// Calculate current level from value at other end of
				// circular buffer ("Peak")
				levelL = delay_buffer[pBuffer];
				delay_buffer[pBuffer] = buffer[i]>0? buffer[i]: -buffer[i];
				pBuffer = (pBuffer+1)%buffer_size;
			}
   
			if (levelL > mThreshold)	mult = mGain * pow(mThreshold/levelL, mInvRatio);
			else						mult = 1.0;
			mMultL = mult*mDecayMult + mMultL*(1.0-mDecayMult);

			buffer[i] *= mMultL;
		}*/
}
