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

#include <Entry.h>
#include <MediaFile.h>
#include <Message.h>
#include <Path.h>
#include <String.h>
#include <SupportDefs.h>

#include "AudioTrack.h"
#include "FaberWindow.h"


class ProjectManager {
public:
										ProjectManager();
	virtual								~ProjectManager();

			static ProjectManager*		Get();

			status_t					SaveProject();
			status_t					LoadProject(entry_ref ref);

			status_t					LoadFile(entry_ref ref);
			status_t					LoadMediaFile(BMediaFile* mediaFile,
											const char* name);

			bool						HasChanged() const;
			bool						WasSaved() const;
			
			bool						HasRedo() const;
			bool						HasUndo() const;

			void						Undo();
			void						Redo();

			BString						Name() const;
			void						SetName(const char* name);

			//							CurrentClipboard();
			//void						CleanClipboard();
private:
			status_t					_SaveTrack(AudioTrack* track);

			static ProjectManager*		fInstance;

			BString						fName;
			bool						fWasSaved;
			BPath						fProjectPath;
};
