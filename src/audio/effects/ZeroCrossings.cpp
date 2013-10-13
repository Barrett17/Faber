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

// ========================= Zero crossings
void ZeroLL()
{
	float left = 0, right = 0, tmpL = 0, tmpR = 0;

	/*switch(Pool.sample_type){
	case MONO:
		left = Pool.sample_memory[Pool.pointer];
		while (Pool.pointer>0){
			tmpL = Pool.sample_memory[Pool.pointer];
			if (left>0 && tmpL<0)	break;
			if (left<0 && tmpL>0)	break;
			Pool.pointer--;
		}
		break;
	case STEREO:
		left = Pool.sample_memory[Pool.pointer*2];
		right = Pool.sample_memory[Pool.pointer*2+1];
		while (Pool.pointer>0){
			tmpL = Pool.sample_memory[Pool.pointer*2];
			tmpR = Pool.sample_memory[Pool.pointer*2+1];
			if ((left>0 && tmpL<0) && (right>0 && tmpR<0))	break;
			if ((left<0 && tmpL>0) && (right<0 && tmpR>0))	break;
			Pool.pointer--;
		}
		break;
	}*/
}

void ZeroLR()
{
	/*float left = 0, right = 0, tmpL = 0, tmpR = 0;

	switch(Pool.sample_type){
	case MONO:
		left = Pool.sample_memory[Pool.pointer];
		while (Pool.pointer<Pool.size){
			tmpL = Pool.sample_memory[Pool.pointer];
			if (left>0 && tmpL<0)	break;
			if (left<0 && tmpL>0)	break;
			Pool.pointer++;
		}
		break;
	case STEREO:
		left = Pool.sample_memory[Pool.pointer*2];
		right = Pool.sample_memory[Pool.pointer*2+1];
		while (Pool.pointer<Pool.size){
			tmpL = Pool.sample_memory[Pool.pointer*2];
			tmpR = Pool.sample_memory[Pool.pointer*2+1];
			if ((left>0 && tmpL<0) && (right>0 && tmpR<0))	break;
			if ((left<0 && tmpL>0) && (right<0 && tmpR>0))	break;
			Pool.pointer++;
		}
		break;
	}
	if (Pool.pointer > Pool.r_sel_pointer)	Pool.selection = NONE;*/
}

void ZeroRL()
{
	/*if (Pool.selection==NONE)
		return;

	float left = 0, right = 0, tmpL = 0, tmpR = 0;

	switch(Pool.sample_type){
	case MONO:
		left = Pool.sample_memory[Pool.r_sel_pointer];
		while (Pool.r_sel_pointer>Pool.pointer){
			tmpL = Pool.sample_memory[Pool.r_sel_pointer];
			if (left>0 && tmpL<0)	break;
			if (left<0 && tmpL>0)	break;
			Pool.r_sel_pointer--;
		}
		break;
	case STEREO:
		left = Pool.sample_memory[Pool.r_sel_pointer*2];
		right = Pool.sample_memory[Pool.r_sel_pointer*2+1];
		while (Pool.r_sel_pointer>Pool.pointer){
			tmpL = Pool.sample_memory[Pool.r_sel_pointer*2];
			tmpR = Pool.sample_memory[Pool.r_sel_pointer*2+1];
			if ((left>0 && tmpL<0) && (right>0 && tmpR<0))	break;
			if ((left<0 && tmpL>0) && (right<0 && tmpR>0))	break;
			Pool.r_sel_pointer--;
		}
		break;
	}*/
}

void ZeroRR()
{
	/*if (Pool.selection==NONE){
		Pool.selection = BOTH;
		Pool.r_sel_pointer = Pool.pointer;
	}
	float left = 0, right = 0, tmpL = 0, tmpR = 0;

	switch(Pool.sample_type){
	case MONO:
		left = Pool.sample_memory[Pool.r_sel_pointer];
		while (Pool.r_sel_pointer<Pool.size){
			tmpL = Pool.sample_memory[Pool.r_sel_pointer];
			if (left>0 && tmpL<0)	break;
			if (left<0 && tmpL>0)	break;
			Pool.r_sel_pointer++;
		}
		break;
	case STEREO:
		left = Pool.sample_memory[Pool.r_sel_pointer*2];
		right = Pool.sample_memory[Pool.r_sel_pointer*2+1];
		while (Pool.r_sel_pointer<Pool.size){
			tmpL = Pool.sample_memory[Pool.r_sel_pointer*2];
			tmpR = Pool.sample_memory[Pool.r_sel_pointer*2+1];
			if ((left>0 && tmpL<0) && (right>0 && tmpR<0))	break;
			if ((left<0 && tmpL>0) && (right<0 && tmpR>0))	break;
			Pool.r_sel_pointer++;
		}
		break;
	}*/
}
