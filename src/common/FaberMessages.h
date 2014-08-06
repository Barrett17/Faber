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


const uint32 FABER_COMMAND				= 	'FBCm';

// GUI

const uint32 FABER_ABOUT					= 	'abou';
const uint32 FABER_OPEN_HOMEPAGE			=	'hmpg';
const uint32 FABER_SETTINGS					=	'STng';

const uint32 FABER_FILE_OPEN				=	'open';

const uint32 FABER_EXPORT_PROJECT   		=	'fepj';

const uint32 FABER_NEW_PROJECT				=	'fnew';
const uint32 FABER_SAVE_PROJECT				=	'save';
const uint32 FABER_SAVE_AS_PROJECT			=	'svas';

const uint32 FABER_UNDO						=	'undo';
const uint32 FABER_REDO						=	'redo';

const uint32 FABER_OPEN_MIXER				=	'fbOM';

const uint32 FABER_NEW_MONO_TRACK			=   'FNat';
const uint32 FABER_NEW_STEREO_TRACK			=   'FNst';

const uint32 FABER_NEW_MULTICHANNEL_TRACK	=   'FNmt';

const uint32 FABER_NEW_LABEL_TRACK			=   'FNlt';

const uint32 FABER_REMOVE_TRACK				=	'Frtk';

const uint32 FABER_PASTE_NEW				=	'pnnw';
const uint32 FABER_DROP_PASTE				=	'drps';

const uint32 FABER_COPY_SILENCE				=	'cpsl';

const uint32 FABER_INVERT_SELECTION			=	'unsl';

const uint32 FABER_UNSELECT_ALL				=	'unsl';

const uint32 FABER_CLEAR					=	'cler';

const uint32 FABER_MUTE_ALL					=	'mtal';
const uint32 FABER_UNMUTE_ALL				=	'umal';

const uint32 FABER_SORT_BY_NAME				=	'srbn';
const uint32 FABER_SORT_BY_DURATION			=	'srbd';

// Track contextual menu

const uint32 FABER_TRACK_SET_NAME			=	'Trsn';
const uint32 FABER_TRACK_GET_INFO			=	'Trgi';

const uint32 FABER_TRACK_SPLIT_CHAN			=	'Trsc';
const uint32 FABER_TRACK_ADD_CHAN			=	'Tadc';
const uint32 FABER_TRACK_RM_CHAN			=	'Trmc';

const uint32 FABER_TRACK_MERGE_WITH			=	'Trmw';
const uint32 FABER_TRACK_MOVE_UP			=	'Trmu';
const uint32 FABER_TRACK_MOVE_DOWN			=	'Trmd';

// Effects and audio manipulation

const uint32 FABER_EFFECT_CALL				=	'fecl';
const uint32 FABER_EFFECT_EXECUTE			=	'feex';
const uint32 FABER_EFFECT_ABORT				=	'feab';

const uint32 FABER_SET_PROJECT_FREQ			=	'setf';

// Transport

const uint32 FABER_TRANSPORT_REW			=	'trew';
const uint32 FABER_TRANSPORT_REW_ALL		=	'trwa';
const uint32 FABER_TRANSPORT_FWD			=	'trwd';
const uint32 FABER_TRANSPORT_FWD_ALL		=	'trfa';
const uint32 FABER_TRANSPORT_PLAY			=	'trly';
const uint32 FABER_TRANSPORT_STOP			=	'trtp';
const uint32 FABER_TRANSPORT_REC			=	'trec';
const uint32 FABER_TRANSPORT_LOOP			=	'trop';

// Zoom

const uint32 FABER_ZOOM_IN					=	'zooi';
const uint32 FABER_ZOOM_OUT					=	'zoot';
const uint32 FABER_ZOOM_FULL				=	'zofl';
const uint32 FABER_ZOOM_SELECTION			=	'zosl';

// Tools

const uint32 FABER_SELECTION_TOOL			=	'fstl';
const uint32 FABER_DRAW_TOOL				=	'fdwt';
const uint32 FABER_PLAY_TOOL				=	'fplt';
const uint32 FABER_TEMPORAL_TOOL			= 	'ftmt';

// Panels et similia

const uint32 FABER_APPLY					=	'fbaP';
const uint32 FABER_REVERT					=	'fbrv';
const uint32 FABER_CANCEL					=	'fbcn';

// Menu structure definitions

const uint32 FABER_ITEM_START				=	'fits';
const uint32 FABER_ITEM_END					=	'fite';

const uint32 FABER_CUSTOM_ITEM				=	'fitc';

const uint32 FABER_SUBITEM					=	'fsbi';

const uint32 FABER_EOF						=	'feof';
const uint32 FABER_SPLITTER					=	'spli';

// Menu IDs

const uint32 FABER_EDIT_MENU				=	'FMED';
const uint32 FABER_RECENTS_MENU				=	'FMRE';
const uint32 FABER_FILE_MENU				=	'FMFI';

#endif
