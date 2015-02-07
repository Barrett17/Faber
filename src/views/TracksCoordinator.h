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

#ifndef TRACKS_COMMON
#define TRACKS_COMMON

#include <SupportDefs.h>


class TracksCoordinator
{
public:
										TracksCoordinator();
	virtual								~TracksCoordinator();

			static TracksCoordinator*		Get();

protected:
			friend class				TracksContainer;
			friend class				WaveRender;

			void						ScrollBy(int64 value);

			int64						Pointer();

			bool						IsSelected();
			void						CurrentSelection(int64* start, int64* end);
			void						Select(int64 start, int64 end);
			void						SelectAll();
			void						Unselect();

			void						ZoomIn();
			void						ZoomOut();
			void						ZoomFull();
			void						ZoomSelection();

			int64						pointer;
			int64						playPointer;

			int64						start;
			int64						end;

			int64						selectionStart;
			int64						selectionEnd;

			int64						duration;

			int32						zoomFactor;

			bool						primaryButton;
			bool						secondaryButton;
			bool						scrollButton;

			bool						isSelected;
			bool						multipleSelection;

private:
			int64						_DisplaySize();

			static TracksCoordinator*	fInstance;
};


static TracksCoordinator& GetCoords()
{
	return *TracksCoordinator::Get();
}


#endif
