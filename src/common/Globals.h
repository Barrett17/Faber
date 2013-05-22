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


#include <SupportDefs.h>
#include <Locale.h>
#include <Catalog.h>

#include "CommonPool.h"
#include "Preferences.h"
#include "History.h"
#include "MouseIcons.h"

#include <stdio.h>

// Translation

#define B_TRANSLATION_CONTEXT "Faber"

#define catSig "x-vnd.Be.locale.Faber"
#define catName catSig".catalog"

// Various settings

const int32 WINDOW_DEFAULT_SIZE_X			= 800;
const int32 WINDOW_DEFAULT_SIZE_Y			= 600;

#define MIN_W 	530
#define MIN_H	256
#define MAX_W 	2048
#define MAX_H	1536

// TODO replace them where needed in the code
#define VERSUT_HOMEPAGE "http://www.versut.com/"
#define FABER_MIMETYPE 	"application/x-vnd.Faber"

#define FABER_HOMEPAGE	"http://www.versut.com/Faber"

#define COPY_RIGHT 		"Copyright 2002 Xentronix\n Copyright 2013 Versut"

#define SETTINGS_DIR    "Faber"
#define FABER_CONF_NAME	"Faber"
#define KEYBINDINGS_CONF_NAME "KeyBindings"
#define LANGUAGE_CONF	"Languages"

// Math
#define INT_MUL(a,b,t)				((t) = (a) * (b) + 0x80, ((((t) >> 8) + (t)) >> 8))
#define INT_MUL3(a,b,c,t)			((t) = (a) * (b) * (c)+ 0x7F5B, ((((t) >> 7) + (t)) >> 16))
#define INT_BLEND(a,b,alpha,tmp)	(INT_MUL((a)-(b), alpha, tmp) + (b))
#define WITHIN(a, b, c)				((((a) <= (b)) && ((b) <= (c))) ? 1 : 0)
#define ROUND(a)					(( ( (a)-long((a)) ) >=.5 ) ? (long((a))+1) : long((a)))
#define CLAMP(a, b, c)				( (a) = (a)<(b) ? (b) : (  (a)>(c) ? (c) : (a) ) )

#ifndef PI
	#define PI 3.14159265358979323846
#endif

// Enums
enum {
	NONE=0,
	LEFT,
	RIGHT,
	BOTH
};

enum {
	SAMPLES,
	TIME
};

enum {
	PLAY=1,
	RECORD,
	PLAY_PAUSE,
	RECORD_PAUSE
};

enum {
	MONO=1,
	STEREO=2
};

enum {
	SELECT_TOOL,
	DRAW_TOOL,
	PLAY_TOOL,
};

// Messages
#define SAVE_AS   			'ssam'
#define NEW					'Bnew'
#define OPEN				'Bopn'
#define DO_OPEN				'Dopn'
#define SAVE				'Bsav'
#define SAVE_SELECTION		'Bsva'
#define INSERT				'Bins'
#define APPEND				'apnd'
#define UNDO				'undo'
#define REDO				'redo'

#define PASTE_NEW			'pnew'
#define DROP_PASTE			'drpP'

#define PASTE_MIXED			'pmix'
#define PREFERENCES			'pref'
#define OPEN_MIX			'opnM'
#define HELP				'help'
#define ABOUT				'bout'
#define HOMEPAGE			'hmep'
#define ZERO_IN				'zrIn'
#define ZERO_OUT			'zrOt'
#define ZERO_LL				'zrLL'
#define ZERO_LR				'zrLR'
#define ZERO_RL				'zrRL'
#define ZERO_RR				'zrRR'
#define TRIM				'trim'
#define SET_FREQUENCY		'setF'
#define RESAMPLE			'rspl'
#define RESAMPLE_DO			'rspD'
#define CLEAR				'clrD'
#define UNSELECT_ALL		'usel'
#define COPY_SILENCE		'cpyS'

#define EDIT_L				'EdiL'
#define EDIT_R				'EdiR'
#define EDIT_B				'EdiB'

#define TRANSPORT_REW		'Trew'
#define TRANSPORT_REW_ALL	'Trwa'
#define TRANSPORT_FWD		'Tfwd'
#define TRANSPORT_FWD_ALL	'Tfwa'
#define TRANSPORT_PLAY		'Tply'
#define TRANSPORT_STOP		'Tstp'
#define TRANSPORT_REC		'Trec'
#define TRANSPORT_PAUSE		'Tpse'
#define TRANSPORT_LOOP		'Tlop'
#define TRANSPORT_HOME		'Tlhm'
#define TRANSPORT_END		'Tled'
#define TRANSPORT_LEFT		'Tllf'
#define TRANSPORT_RIGHT		'Tlrg'
#define TRANSPORT_TOGGLE	'Tltg'
#define TRANSPORT_SET		'Tlst'

#define ZOOM_IN				'zmIn'
#define ZOOM_OUT			'zmOt'
#define ZOOM_FULL			'zmFl'
#define ZOOM_SELECTION		'zmSl'
#define ZOOM_LEFT			'zmLf'
#define ZOOM_RIGHT			'zmRg'

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

#define SPECTRUM			'spct'
#define SAMPLE_SCOPE		'spls'

#define SET_TIME			'setT'

#define UPDATE_MENU			'_udm'
#define REDRAW				'rdrw'

#endif
