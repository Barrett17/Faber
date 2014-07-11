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


#include <Archivable.h>


class MessageBuilder {
public:

	MessageBuilder(uint32 what)
	{
		msg = new BMessage(what);
		code = what;
	}

	MessageBuilder(uint32 what, BArchivable* archivable)
	{
		msg = new BMessage(what);
		code = what;

		archivable->Archive(msg);
	}

	void SetTo(uint32 what)
	{
		delete msg;
		msg = new BMessage(what);
		code = what;
	}

	BMessage* ToMessage() const
	{
		return msg;
	}

	operator BMessage* () const
	{
    	return this->ToMessage();
	}

	operator uint32 () const
	{
    	return this->code;
	}

	bool operator!=(const MessageBuilder& from)
  	{
		if (this->code != from.code)
			return true;

		return false;
	}

	bool operator==(const MessageBuilder& from)
  	{
		if (this->code == from.code)
			return true;

		return false;
	}

	BMessage* msg;
	uint32 code;
};

// GUI

const uint32 FABER_ABOUT				= 	'abou';
const uint32 FABER_OPEN_HOMEPAGE		=	'hmpg';
const uint32 FABER_SETTINGS				=	'stng';

const uint32 FABER_FILE_OPEN			=	'open';

const uint32 FABER_EXPORT_PROJECT   	=	'fepj';

const uint32 FABER_NEW_PROJECT			=	'fnew';
const uint32 FABER_SAVE_PROJECT			=	'save';
const uint32 FABER_SAVE_AS_PROJECT		=	'svas';

const uint32 FABER_UNDO					=	'undo';
const uint32 FABER_REDO					=	'redo';

const uint32 FABER_OPEN_MIXER			=	'fbOM';

const uint32 FABER_NEW_AUDIO_TRACK		=   'FNat';
const uint32 FABER_NEW_LABEL_TRACK		=   'FNlt';

const uint32 FABER_REMOVE_TRACK			=	'Frtk';

const uint32 FABER_PASTE_NEW			=	'pnnw';
const uint32 FABER_DROP_PASTE			=	'drps';

const uint32 FABER_COPY_SILENCE			=	'cpsl';

const uint32 FABER_INVERT_SELECTION		=	'unsl';

const uint32 FABER_UNSELECT_ALL			=	'unsl';
const uint32 FABER_CLEAR				=	'cler';

const uint32 FABER_MUTE_ALL				=	'mtal';
const uint32 FABER_UNMUTE_ALL			=	'umal';

const uint32 FABER_SORT_BY_NAME			=	'srbn';
const uint32 FABER_SORT_BY_DURATION		=	'srbd';

// Track contextual menu

const uint32 FABER_TRACK_SET_NAME		=	'Trsn';
const uint32 FABER_TRACK_GET_INFO		=	'Trgi';

const uint32 FABER_TRACK_SPLIT_MONO		=	'Trsm';
const uint32 FABER_TRACK_SPLIT_CHAN		=	'Trsc';
const uint32 FABER_TRACK_MERGE_WITH		=	'Trmw';

// Effects and audio manipulation

const uint32 FABER_EFFECT_CALL			=	'fecl';
const uint32 FABER_EFFECT_EXECUTE		=	'feex';
const uint32 FABER_EFFECT_ABORT			=	'feab';

const uint32 FABER_SET_PROJECT_FREQ		=	'setf';

// Transport

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

// Tools

const uint32 FABER_SELECTION_TOOL		=	'fstl';
const uint32 FABER_DRAW_TOOL			=	'fdwt';
const uint32 FABER_PLAY_TOOL			=	'fplt';
const uint32 FABER_TEMPORAL_TOOL		= 	'ftmt';

// Panels et similia

const uint32 FABER_APPLY				=	'fbaP';
const uint32 FABER_REVERT				=	'fbrv';
const uint32 FABER_CANCEL				=	'fbcn';

// Menu definitions
const uint32 FABER_ITEM_START			=	'fits';
const uint32 FABER_ITEM_END				=	'fite';
const uint32 FABER_EOF					=	'feof';
const uint32 FABER_SPLITTER				=	'spli';

#endif
