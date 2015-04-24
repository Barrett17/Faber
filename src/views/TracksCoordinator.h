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

#ifndef TRACKS_COORDINATOR
#define TRACKS_COORDINATOR

#include <Message.h>
#include <ObjectList.h>
#include <Rect.h>
#include <SupportDefs.h>


class TracksContainer;
class TrackView;
class Render;


class TracksCoordinator
{
public:
										TracksCoordinator(TracksContainer* owner);

			int64						Pointer() const;
			int64						ZoomFactor() const;

			bool						SelectionActive() const;
			void						CurrentSelection(int64* start, int64* end);

			void						SelectAll();
			void						UnselectAll();

			void						ZoomIn();
			void						ZoomOut();
			void						ZoomFull();
			void						ZoomSelection();

			int64						ScreenToFrame(int64 value);
			int64						FrameToScreen(int64 value);

			void						InvalidateSelection();

			// Tracks callbacks
			void						NotifySelect(int64 start, int64 end,
											Render* who);

			void						NotifySelectAll(Render* who);
			void						NotifyUnselect(Render* who);

			void						NotifyMouseDown(BPoint point,
											BMessage* msg, Render* who);
			void						NotifyMouseUp(BPoint point,
											Render* who);

			void						NotifyMouseMoved(BPoint point, uint32 data,
											const BMessage* message, Render* who);
private:
			void						_UpdateSelection(int64 start, int64 end);
			void 						_AddToSelection(Render* who);
			void 						_RemoveFromSelection(Render* who);
			void						_CleanupSelection();

			TracksContainer*			fOwner;

			int64						fPointer;
			int64						fPlayPointer;

			int64						fStart;
			int64						fEnd;

			int64						fSelectionStart;
			int64						fSelectionEnd;

			int64						fDuration;

			int32						fZoomFactor;

			bool						fPrimaryButton;
			bool						fSecondaryButton;

			BObjectList<Render>			fSelectionQueue;
			Render*						fCurrentRender;
			
};

#endif
