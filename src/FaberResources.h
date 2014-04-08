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

#ifndef FABER_RESOURCES_H
#define FABER_RESOURCES_H

#define FABER_APP_NAME "Faber"
#define FABER_SIGNATURE "application/x-vnd.Faber"
#define FABER_PROJECT_MIME "application/vnd-FaberProject"

enum {
	kPlayIcon = 0,
	kNewDocumentIcon,
	kOpenDocumentIcon,
	kDocumentSaveIcon,

	kCutIcon,
	kCopyIcon,
	kPasteIcon,
	kUndoIcon,
	kRedoIcon,

	kDrawingToolIcon,
	kSelectionToolIcon,
	kPlayingToolIcon,

	kStopIcon,
	kPauseIcon,
	kRecordIcon,
	kLoopIcon,
	kSeekForwardIcon,
	kSeekForwardAllIcon,
	kSeekBackwardIcon,
	kSeekBackwardAllIcon,

	kZoomInIcon,
	kZoomOutIcon,
	kZoomToSelectionIcon,
	kZoomFullWaveIcon,

	// track icons
	kMuteIcon,
	kSoloIcon,
	kTrackMenuIcon,

	kCloseTrackIcon,
	kMiniRecordIcon
};

#endif
