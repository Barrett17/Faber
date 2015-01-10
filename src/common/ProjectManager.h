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
#ifndef _PROJECT_MANAGER
#define _PROJECT_MANAGER


#include <Entry.h>
#include <MediaFile.h>
#include <Message.h>
#include <Path.h>
#include <String.h>
#include <SupportDefs.h>

#include "AudioTrack.h"
#include "FaberView.h"
#include "FaberWindow.h"


class ProjectManager {
public:
			static void					Init();

			static status_t				SaveProject();
			static status_t				LoadProject(entry_ref ref);

			static status_t				LoadFile(entry_ref ref);
			static status_t				LoadMediaFile(BMediaFile* mediaFile,
											const char* name);

			static bool					HasChanged();
			static bool					WasSaved();

			static bool					HasRedo();
			static bool					HasUndo();

			static void					Undo();
			static void					Redo();

			static BString				Name();
			static void					SetName(const char* name);

			static BPath				GetProjectPath();

			static status_t				RegisterTrack(Track* track);
			static status_t				UnregisterTrack(Track* track);

private:
			static void					_NotifyTracksContainer(Track* track);

			static BString				fName;
			static bool					fWasSaved;
			static BPath				fProjectPath;
			static BFile*				fProjectFile;
};

#endif
