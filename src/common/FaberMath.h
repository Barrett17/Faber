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

#define INT_MUL(a,b,t)				((t) = (a) * (b) + 0x80, ((((t) >> 8) + (t)) >> 8))
#define INT_MUL3(a,b,c,t)			((t) = (a) * (b) * (c)+ 0x7F5B, ((((t) >> 7) + (t)) >> 16))
#define INT_BLEND(a,b,alpha,tmp)	(INT_MUL((a)-(b), alpha, tmp) + (b))
#define WITHIN(a, b, c)				((((a) <= (b)) && ((b) <= (c))) ? 1 : 0)
#define ROUND(a)					(( ( (a)-long((a)) ) >=.5 ) ? (long((a))+1) : long((a)))
#define CLAMP(a, b, c)				( (a) = (a)<(b) ? (b) : (  (a)>(c) ? (c) : (a) ) )

#ifndef PI
	#define PI 3.14159265358979323846
#endif
