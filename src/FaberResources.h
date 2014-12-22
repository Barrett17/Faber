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
	// Transport toolbar
	kPlayIcon = 0,
	kStopIcon,
	kPauseIcon,
	kRecordIcon,
	kLoopIcon,
	kSeekForwardIcon,
	kSeekForwardAllIcon,
	kSeekBackwardIcon,
	kSeekBackwardAllIcon,

	// Zoom toolbar
	kZoomInIcon,
	kZoomOutIcon,
	kZoomToSelectionIcon,
	kZoomFullWaveIcon,

	// Track
	kMiniRecordIcon,
	kMuteIcon,
	kSoloIcon,
	kCloseTrackIcon,

	// Editor tools
	kSelectionToolIcon,
	kDrawingToolIcon,
	kPlayingToolIcon
};

#endif
