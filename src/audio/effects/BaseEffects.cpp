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

#include "SimpleEffects.h"


SwapFilter::SwapFilter()
	:
	FaberEffect(NULL, false)
{
}


void
SwapFilter::FilterBuffer(float *buffer, size_t size)
{
	for (size_t i=0; i<size; i+=2) {
		float tmp = buffer[i+0];
		buffer[i+0] = buffer[i+1];
		buffer[i+1] = tmp;
	}
}


InvertFilter::InvertFilter()
	:
	FaberEffect(NULL, false)
{
}


void
InvertFilter::FilterBuffer(float *buffer, size_t size)
{
	for (size_t i=0; i<size; i++) {
		buffer++;
		*buffer = -*buffer;
	}
}


SilenceFilter::SilenceFilter()
	:
	FaberEffect(NULL, false)
{
}


void
SilenceFilter::FilterBuffer(float *buffer, size_t size)
{
	for (size_t i=0; i<size; i++) {
		*buffer++ = 0.0f;
	}
}


FadeInFilter::FadeInFilter()
	:
	FaberEffect(NULL, false)
{
}


void
FadeInFilter::FilterBuffer(float *buffer, size_t size)
{
	for (size_t i=0; i<size; i++) {
		buffer++;
		*buffer = (*buffer)*count / m_total;
		count++;
	}
}


bool
FadeInFilter::InitFilter(float f, int32 c, int32 pass, int32 size)
{
	FaberEffect::InitFilter(f, c, pass, size);
	count = 0;
	return true;
}


FadeOutFilter::FadeOutFilter()
	:
	FaberEffect(NULL, false)
{
}


void
FadeOutFilter::FilterBuffer(float *buffer, size_t size)
{
	for (size_t i=0; i<size; i++) {
		buffer++;
		*buffer = (*buffer)*count / m_total;
		count--;
	}
}


bool
FadeOutFilter::InitFilter(float f, int32 c, int32 pass, int32 size)
{
	FaberEffect::InitFilter(f, c, pass, size);
	count = size;
	return true;
}
