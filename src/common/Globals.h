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
 * Copyright 2011 Davide Gessa. All rights reserved.
 * Copyright 2013 Dario Casalinuovo. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Davide Gessa, dak.linux@gmail.com
 *		Dario Casalinuovo, b.vitruvio@gmail.com
 */
 
#ifndef GLOBALS_H
#define GLOBALS_H

// Various settings

#define WINDOW_DEFAULT_SIZE_X	800
#define WINDOW_DEFAULT_SIZE_Y	600

#define WINDOW_MIN_SIZE_X		530
#define WINDOW_MIN_SIZE_Y		256

#define WINDOW_MAX_SIZE_X		2048
#define WINDOW_MAX_SIZE_Y		1536

// TODO replace them where needed in the code
#define VERSUT_HOMEPAGE "http://www.versut.com/"
#define FABER_MIMETYPE 	"application/x-vnd.Faber"
#define FABER_HOMEPAGE	"http://www.versut.com/Faber"

#define COPY_RIGHT 		"Copyright 2002 Xentronix\n Copyright 2013 Versut\n"

#define SETTINGS_DIR    "Faber"
#define FABER_CONF_NAME	"FaberSettings"
#define KEYBINDINGS_CONF_NAME "KeyBindings"

// Enums

enum {
	SELECT_TOOL,
	DRAW_TOOL,
	PLAY_TOOL,
};

// Messages

#define UNDO				'undo'
#define REDO				'redo'

#define TRIM				'trim'
#define SET_FREQUENCY		'setF'
#define RESAMPLE			'rspl'
#define RESAMPLE_DO			'rspD'
#define CLEAR				'clrD'
#define UNSELECT_ALL		'usel'
#define COPY_SILENCE		'cpyS'

#define TRANSPORT_REW		'Trew'
#define TRANSPORT_REW_ALL	'Trwa'
#define TRANSPORT_FWD		'Tfwd'
#define TRANSPORT_FWD_ALL	'Tfwa'
#define TRANSPORT_PLAY		'Tply'
#define TRANSPORT_STOP		'Tstp'
#define TRANSPORT_REC		'Trec'
#define TRANSPORT_LOOP		'Tlop'

#define RUN_FILTER			'FXrn'
#define RUN_FILTER_KEY		'FXky'
#define EXE_FILTER			'FXex'
#define CANCEL_FILTER		'FXcl'
#define RUN_LAST_FILTER		'FXlt'

enum {
	TOOL_SELECT	= 0,
	TOOL_DRAW	= 1,
	TOOL_PLAY	= 2
};

#endif
