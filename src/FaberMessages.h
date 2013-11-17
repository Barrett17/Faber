/*
    Copyright 2013 Dario Casalinuovo. All rights reserved.

    This file is part of Faber.

    Faber is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Faber is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Faber.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef FABER_MESSAGES_H
#define FABER_MESSAGES_H

#define FABER_WHAT "faber_what?"

class FaberMessage {
public:

	FaberMessage()
	{
		this->code = 0;
		this->what = 0;
	}

	FaberMessage(uint32 what, uint32 code)
	{
		this->code = code;
		this->what = what;
	}

	BMessage* ToMessage() const
	{
		BMessage* ret = new BMessage(what);
		ret->AddUInt32(FABER_WHAT, code);
		return ret;
	}

	operator BMessage* () const
	{
    	return this->ToMessage();
	}

	operator uint32 () const
	{
    	return this->code;
	}

	bool operator!=(const FaberMessage& from)
  	{
		if (this->code != from.code)
			return true;

		return false;
	}

	bool operator==(const FaberMessage& from)
  	{
		if (this->code == from.code)
			return true;

		return false;
	}

	uint32 code;
	uint32 what;
};

// GUI

const uint32 FABER_GENERAL_MESSAGE = 'FAGM';

class GeneralMessage : public FaberMessage {
public:
				GeneralMessage(uint32 code)
					:
					FaberMessage(FABER_GENERAL_MESSAGE, code) {};
};

const uint32 FABER_ABOUT				= 	'abou';

const uint32 FABER_OPEN_HOMEPAGE		=	'hmpg';

const uint32 FABER_EXPORT_PROJECT   	=	'fepj';
const uint32 FABER_NEW_PROJECT			=	'fnew';
const uint32 FABER_SAVE_PROJECT			=	'save';

const uint32 FABER_NEW_EMPTY_TRACK		=   'FNEt';
const uint32 FABER_REMOVE_TRACK			=	'Frtk';

const uint32 FABER_FILE_OPEN			=	'open';

const uint32 FABER_EXPORT_SELECTION		=	'svsl';

const uint32 FABER_INSERT				=	'insr';
const uint32 FABER_APPEND				=	'appn';

const uint32 FABER_UNDO					=	'undo';
const uint32 FABER_REDO					=	'redo';

const uint32 FABER_PASTE_NEW			=	'pnnw';
const uint32 FABER_DROP_PASTE			=	'drps';

const uint32 FABER_SETTINGS				=	'stng';

const uint32 FABER_QUIT_REQUESTED		=	B_QUIT_REQUESTED;

const uint32 FABER_COPY					=	B_COPY;
const uint32 FABER_CUT					=	B_CUT;
const uint32 FABER_PASTE				=	B_PASTE;
const uint32 FABER_SELECT_ALL			=	B_SELECT_ALL;
const uint32 FABER_UNSELECT_ALL			=	'unsl';

const uint32 FABER_TRANSPORT_REW		=	'trew';
const uint32 FABER_TRANSPORT_REW_ALL	=	'trwa';
const uint32 FABER_TRANSPORT_FWD		=	'trwd';
const uint32 FABER_TRANSPORT_FWD_ALL	=	'trfa';
const uint32 FABER_TRANSPORT_PLAY		=	'trly';
const uint32 FABER_TRANSPORT_STOP		=	'trtp';
const uint32 FABER_TRANSPORT_REC		=	'trec';
const uint32 FABER_TRANSPORT_LOOP		=	'trop';

// Zoom

const uint32 FABER_ZOOM_IN				=	'zooi';
const uint32 FABER_ZOOM_OUT				=	'zoot';
const uint32 FABER_ZOOM_FULL			=	'zofl';
const uint32 FABER_ZOOM_SELECTION		=	'zosl';

// Effects and audio manipulation

const uint32 FABER_EFFECT_CALL			=	'fecl';
const uint32 FABER_EFFECT_EXECUTE		=	'feex';
const uint32 FABER_EFFECT_ABORT			=	'feab';

const uint32 FABER_CLEAR				=	'cler';

const uint32 FABER_COPY_SILENCE			=	'cpsl';

const uint32 FABER_SET_FREQUENCY		=	'setf';
const uint32 FABER_RESAMPLE				=	'resm';
const uint32 FABER_RESAMPLE_EXECUTE		=	'resd';

const uint32 FABER_SELECTION_TOOL		=	'fstl';
const uint32 FABER_DRAW_TOOL			=	'fdwt';
const uint32 FABER_PLAY_TOOL			=	'fplt';
const uint32 FABER_TEMPORAL_TOOL		= 	'ftmt';
const uint32 FABER_TRIM					=	'trim';

// TODO those will be effects, remove them

const uint32 FABER_ZERO_IN				=	'zrin';
const uint32 FABER_ZERO_OUT				=	'zrot';
const uint32 FABER_ZERO_LL				=	'zrll';
const uint32 FABER_ZERO_LR				=	'zrlr';
const uint32 FABER_ZERO_RL				=	'zrlr';
const uint32 FABER_ZERO_RR				=	'zrrr';

// Other defs

const uint32 FABER_ITEM_MESSAGE		=	'FITM';

class ItemMessage : public FaberMessage {
public:
				ItemMessage(uint32 code)
					:
					FaberMessage(FABER_ITEM_MESSAGE, code) {};
};

// Menu definitions
const ItemMessage FABER_ITEM_START			=	'fits';
const ItemMessage FABER_ITEM_END			=	'fite';
const ItemMessage FABER_EOF					=	'feof';
const ItemMessage FABER_SPLITTER			=	'spli';

const uint32 FABER_APPLY					=	'fbaP';
const uint32 FABER_REVERT					=	'fbrv';

#endif
