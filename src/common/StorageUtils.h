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
		static BEntry*			BlockFileRequested();

		static BFile*			GetProjectFile(BPath path, bool create);

		static BPath			TemporaryProjectDirRequested();
		static status_t			DeleteDirectory(BPath path);

		static size_t			FramesToSize(int64 frames);
		static int64			SizeToFrames(size_t size);
private:
		// TODO randomize
		static int32			fLastCode;

};

#endif
