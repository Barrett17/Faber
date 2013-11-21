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

#include "ProjectManager.h"

ProjectManager* ProjectManager::fInstance = NULL;


ProjectManager*
ProjectManager::Get()
{
	if (fInstance == NULL)
		fInstance = new ProjectManager();

	return fInstance;	
}

ProjectManager::ProjectManager()
	:
	fWasSaved(false)
{
}


ProjectManager::~ProjectManager()
{
}


bool
ProjectManager::HasChanged() const
{
	//return fHistory->HasChanged();
}


bool
ProjectManager::HasRedo() const
{
	//return fHistory->HasRedo();
}


bool
ProjectManager::HasUndo() const
{
	//return fHistory->HasUndo();
}


bool
ProjectManager::WasSaved() const
{
	return fWasSaved;
}


status_t
ProjectManager::SaveProject(BMessage* message)
{
}


status_t
ProjectManager::LoadProject(BMessage* message)
{
}


status_t
ProjectManager::LoadTrack(BMessage* message)
{
}


status_t
ProjectManager::SaveTrack(BMessage* message)
{
}


void
ProjectManager::Undo()
{
	//return fHistory->Undo();
}


void
ProjectManager::Redo()
{
	//return fHistory->Redo();
}


BString
ProjectManager::Name() const
{
	return fName;
}


void
ProjectManager::SetName(const char* name)
{
	fName = BString(name);
}
