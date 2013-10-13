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

// ============================================================ Trim
void DoTrim()
{
	/*if (Pool.sample_type == NONE || Pool.selection == NONE)	return;
	WindowsManager::MainWinMessenger()->SendMessage(TRANSPORT_STOP);		// stop playing

	//Hist.Save(H_FULL, 0, Pool.size);	// full undo

	if (Pool.pointer != 0){			// copy to begin of memory
		float *src = Pool.sample_memory + Pool.pointer*Pool.sample_type;
		float *dst = Pool.sample_memory;
		float *end = Pool.sample_memory + Pool.r_sel_pointer*Pool.sample_type;
		while(src<=end){
			*dst++ = *src++;
		}
	}
	// resize memory
	Pool.size = (Pool.r_sel_pointer - Pool.pointer);
	Pool.r_sel_pointer = Pool.size;
	Pool.r_pointer = Pool.size;
	Pool.pointer = 0;
	Pool.l_pointer = 0;

	Pool.sample_memory = (float*)realloc(Pool.sample_memory, Pool.size*Pool.sample_type*4 +1024);

	// wipe last piece
	float *p = Pool.sample_memory + (Pool.size+1)*Pool.sample_type;
	for (int i=0; i<256; i++)		// now delete 1Kb additional memory as reserved to prevent crashes
		*p++=0;

	Pool.changed = true;
	WindowsManager::MainWindow()->ResetIndexView();
	WindowsManager::MainWindow()->UpdateMenu();
	WindowsManager::MainWindow()->RedrawWindow();*/
}
