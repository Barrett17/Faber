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

#include "FaberDefs.h"

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
ProjectManager::SaveProject()
{


	fWasSaved = true;
	return B_OK;
}


status_t
ProjectManager::LoadProject(entry_ref ref)
{
	/*
	ProjectFile* project = new ProjectFile(ref);
	if (project->InitCheck() != B_OK)
		return B_ERROR;

	
	*/
	return B_ERROR;
}


status_t
ProjectManager::LoadFile(entry_ref ref)
{
	printf("file\n");

	BMediaFile* mediaFile = new BMediaFile(&ref);
	if (mediaFile->InitCheck() == B_OK) {
		return LoadMediaFile(mediaFile, ref.name);
	}

	delete mediaFile;

	return LoadProject(ref);
}


status_t
ProjectManager::LoadMediaFile(BMediaFile* mediaFile, const char* name)
{
	AudioTrack* track = new AudioTrack(mediaFile);
	track->SetName(name);

	printf("loading media file\n");
	status_t ret = track->InitCheck();
	if (ret < B_OK) {
		delete track;
		printf("%s\n", strerror(ret));
		return ret;
	}

  	WindowsManager::MainWindow()->MainView()->AddTrack(track);

	return B_OK;
}


status_t
ProjectManager::_SaveTrack(AudioTrack* track)
{

	return B_OK;
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
	BString title("Faber - ");
	title.Append(fName);

	FaberWindow* win = WindowsManager::MainWindow();
	if (win->Lock()) {
		win->SetTitle(title);
		win->Unlock();
	}
}
