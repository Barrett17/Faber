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

#ifndef _BASE_EFFECTS_H_
#define _BASE_EFFECTS_H_

#include "AudioEffect.h"


class SwapEffect : public AudioEffect {
public:
					SwapEffect();
	virtual void	FilterBuffer(float* buf, int64 frames);
};


class InvertEffect : public AudioEffect {
public:
					InvertEffect();
	virtual void	FilterBuffer(float* buf, int64 frames);
};


class SilenceEffect : public AudioEffect {
public:
					SilenceEffect();
	virtual void	FilterBuffer(float* buf, int64 frames);
};


class FadeInEffect : public AudioEffect {
public:
					FadeInEffect();
	virtual void	FilterBuffer(float* buf, int64 frames);

private:
			int64	count;
};

class FadeOutEffect : public AudioEffect {
public:
					FadeOutEffect();
	virtual void	FilterBuffer(float* buf, int64 frames);

  private:
			int64	count;
};

#endif
