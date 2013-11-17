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

#include <stdio.h>

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
		BMessage* ret = new BMessage(code);
		ret->AddUInt32(FABER_WHAT, what);
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

const GeneralMessage FABER_ABOUT				= 	'abou';

const GeneralMessage FABER_OPEN_HOMEPAGE		=	'hmpg';

const GeneralMessage FABER_EXPORT_PROJECT   	=	'fepj';
const GeneralMessage FABER_NEW_PROJECT			=	'fnew';
const GeneralMessage FABER_SAVE_PROJECT			=	'save';

const GeneralMessage FABER_NEW_EMPTY_TRACK		=   'FNEt';
const GeneralMessage FABER_REMOVE_TRACK			=	'Frtk';

const GeneralMessage FABER_FILE_OPEN			=	'open';

const GeneralMessage FABER_EXPORT_SELECTION		=	'svsl';

const GeneralMessage FABER_INSERT				=	'insr';
const GeneralMessage FABER_APPEND				=	'appn';

const GeneralMessage FABER_UNDO					=	'undo';
const GeneralMessage FABER_REDO					=	'redo';

const GeneralMessage FABER_PASTE_NEW			=	'pnnw';
const GeneralMessage FABER_DROP_PASTE			=	'drps';

const GeneralMessage FABER_PREFERENCES			=	'prfs';

const GeneralMessage FABER_QUIT_REQUESTED		=	B_QUIT_REQUESTED;

const GeneralMessage FABER_COPY					=	B_COPY;
const GeneralMessage FABER_CUT					=	B_CUT;
const GeneralMessage FABER_PASTE				=	B_PASTE;
const GeneralMessage FABER_SELECT_ALL			=	B_SELECT_ALL;

// Transport

const uint32 FABER_TRANSPORT_MESSAGE	=	'TRMS';

class TransportMessage : public FaberMessage {
public:
				TransportMessage(uint32 code)
					:
					FaberMessage(FABER_TRANSPORT_MESSAGE, code) {};
};

const TransportMessage FABER_TRANSPORT_REW		=	'trew';
const TransportMessage FABER_TRANSPORT_REW_ALL	=	'trwa';
const TransportMessage FABER_TRANSPORT_FWD		=	'trwd';
const TransportMessage FABER_TRANSPORT_FWD_ALL	=	'trfa';
const TransportMessage FABER_TRANSPORT_PLAY		=	'trly';
const TransportMessage FABER_TRANSPORT_STOP		=	'trtp';
const TransportMessage FABER_TRANSPORT_REC		=	'trec';
const TransportMessage FABER_TRANSPORT_LOOP		=	'trop';

// Zoom

const uint32 FABER_TRACK_MESSAGE		=	'FTRM';

class TrackMessage : public FaberMessage {
public:
				TrackMessage(uint32 code)
					:
					FaberMessage(FABER_TRACK_MESSAGE, code) {};
};

const TrackMessage FABER_ZOOM_IN				=	'zooi';
const TrackMessage FABER_ZOOM_OUT				=	'zoot';
const TrackMessage FABER_ZOOM_FULL			=	'zofl';
const TrackMessage FABER_ZOOM_SELECTION		=	'zosl';

// Effects and audio manipulation

const TrackMessage FABER_EFFECT_CALL			=	'fecl';
const TrackMessage FABER_EFFECT_EXECUTE		=	'feex';
const TrackMessage FABER_EFFECT_ABORT			=	'feab';

const TrackMessage FABER_CLEAR				=	'cler';

const TrackMessage FABER_UNSELECT_ALL			=	'unsl';
const TrackMessage FABER_COPY_SILENCE			=	'cpsl';

const TrackMessage FABER_SET_FREQUENCY		=	'setf';
const TrackMessage FABER_RESAMPLE				=	'resm';
const TrackMessage FABER_RESAMPLE_EXECUTE		=	'resd';

const TrackMessage FABER_SELECTION_TOOL			=	'fstl';
const TrackMessage FABER_DRAW_TOOL				=	'fdwt';
const TrackMessage FABER_PLAY_TOOL				=	'fplt';
const TrackMessage FABER_TEMPORAL_TOOL			= 	'ftmt';

// TODO those will be effects, remove them
const TrackMessage FABER_TRIM					=	'trim';
const TrackMessage FABER_ZERO_IN				=	'zrin';
const TrackMessage FABER_ZERO_OUT				=	'zrot';
const TrackMessage FABER_ZERO_LL				=	'zrll';
const TrackMessage FABER_ZERO_LR				=	'zrlr';
const TrackMessage FABER_ZERO_RL				=	'zrlr';
const TrackMessage FABER_ZERO_RR				=	'zrrr';

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


#endif
