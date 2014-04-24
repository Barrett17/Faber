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

#include "StorageUtils.h"

#include <Entry.h>
#include <Directory.h>
#include <FindDirectory.h>
#include <String.h>

#include "ProjectManager.h"


int32 StorageUtils::fLastCode = 00;

#define DEFAULT_BLOCK_FILENAME "block"


BEntry*
StorageUtils::BlockFileRequested()
{
	BPath path = ProjectManager::Get()->GetProjectPath();

	BString str;

	str << DEFAULT_BLOCK_FILENAME;

	fLastCode += 1;

	str << fLastCode;

	path.Append(str.String());

	return new BEntry(path.Path());
}


BPath
StorageUtils::TemporaryProjectDirRequested()
{
	BPath path;

	find_directory(B_SYSTEM_TEMP_DIRECTORY, &path);

	path.Append("project");

	create_directory(path.Path(), 0777);

	return path;
}


BFile*
StorageUtils::GetProjectFile(BPath path, bool create)
{
	return NULL;
}


// NOTE ATM we support only float, also PCM16 and PCM24 are planned.
int64
StorageUtils::SizeToFrames(ssize_t size)
{
	return (int64)size/sizeof(float);
}


ssize_t
StorageUtils::FramesToSize(int64 frames)
{
	return (size_t)frames*sizeof(float);
}
