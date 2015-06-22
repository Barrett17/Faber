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
#ifndef _STORAGE_UTILS_H
#define _STORAGE_UTILS_H


#include <File.h>
#include <Path.h>


class StorageUtils {
public:
		// Get a new BlockFile for the current project
		static status_t			BlockEntryRequested(entry_ref* entry);

		// Get the current project file, there's only one project
		// per window.
		static BFile*			GetProjectFile(BPath path, bool create);

		// Get a temporary dir to use by the program, for example
		// the project temporary dir.
		static BPath			TemporaryDirRequested();

		// Move the current project dir to a new path, used only to move
		// the temporary files for a new project.
		// static status_t			MoveToPath(BPath old, BPath newPath);

		// Copy the project files to a new location, used by save as.
		// static status_t			CopyToPath(BPath old, BPath newPath);

		static status_t			DeleteDirectory(BPath path);

		// Disk storage / media conversion
		static size_t			FramesToSize(int64 frames);
		static int64			SizeToFrames(size_t size);

private:
		// TODO randomize
		static int32			fLastBlockID;
		static int32			fLastProjectID;

};

#endif
