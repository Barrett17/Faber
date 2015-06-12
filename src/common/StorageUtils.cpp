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

#include <stdio.h>

#include "ProjectManager.h"


int32 StorageUtils::fLastBlockID = 0;
int32 StorageUtils::fLastProjectID = 0;

#define DEFAULT_BLOCK_FILENAME "block"
#define DEFAULT_PROJECT_FILENAME "faber_proj"
#define DEFAULT_TEMPORARY_DIR_NAME "faber_temp"


status_t
StorageUtils::BlockEntryRequested(entry_ref* ref)
{
	fLastBlockID += 1;
	BPath path = ProjectManager::GetProjectPath();
	// Ensure that we will not owerwrite some other block
	while (true) {
		BPath temp(path.Path());
		BString name;
		name << DEFAULT_BLOCK_FILENAME << fLastBlockID;
		temp.Append(name.String());
		BEntry block(temp.Path());
		if (block.Exists()) {
			printf("exists\n");
			fLastBlockID++;
			continue;
		} else
			return get_ref_for_path(temp.Path(), ref);
	}
}


BFile*
StorageUtils::GetProjectFile(BPath path, bool create)
{
	return NULL;
}


BPath
StorageUtils::TemporaryDirRequested()
{
	BPath path;
	find_directory(B_SYSTEM_TEMP_DIRECTORY, &path);
	while (true) {
		BPath temp(path.Path());
		BString name(DEFAULT_TEMPORARY_DIR_NAME);
		name << fLastProjectID;
		temp.Append(name);
		BEntry dir(temp.Path());
		if (dir.Exists()) {
			fLastProjectID++;
			continue;
		} else {
			create_directory(temp.Path(), 0777);
			fLastProjectID++;
			return temp;
		}
	}
}


status_t
StorageUtils::DeleteDirectory(BPath path)
{
	BDirectory dir(path.Path());
	status_t err = dir.InitCheck();
	if (err == B_OK) {
		BEntry entry;
		while (dir.GetNextEntry(&entry) == B_OK)
			entry.Remove();

		err = dir.GetEntry(&entry);
		if (err == B_OK)
			return entry.Remove();
	}
	return err;
}


// TODO Deprecated, remove it as planned
size_t
StorageUtils::FramesToSize(int64 frames)
{
	return (size_t)frames*sizeof(float);
}
