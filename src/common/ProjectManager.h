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

#include <Message.h>
#include <String.h>
#include <SupportDefs.h>


class ProjectManager {
public:
										ProjectManager();
	virtual								~ProjectManager();

			static ProjectManager*		Get();

			bool						HasChanged() const;
			bool						HasRedo() const;
			bool						HasUndo() const;

			bool						WasSaved() const;

			status_t					SaveProject(BMessage* message);
			status_t					LoadProject(BMessage* message);

			status_t					LoadTrack(BMessage* message);
			status_t					SaveTrack(BMessage* message);

			void						Undo();
			void						Redo();

			BString					Name() const;
			void						SetName(const char* name);

			//							CurrentClipboard();
			//void						EmptyClipboard();
private:
			static ProjectManager*		fInstance;

			BString						fName;

			bool						fWasSaved;
};
