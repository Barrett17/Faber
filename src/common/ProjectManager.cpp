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

#include "AudioGate.h"
#include "AutoDeleter.h"
#include "FaberDefs.h"
#include "StorageUtils.h"
#include "TrackIO.h"


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
	fProjectPath = StorageUtils::TemporaryProjectDirRequested();
	fProjectFile = StorageUtils::GetProjectFile(fProjectPath, true);
}


ProjectManager::~ProjectManager()
{
}


bool
ProjectManager::HasChanged() const
{
	//return SnanpshotManager::HasChanged();

	return false;
}


bool
ProjectManager::HasRedo() const
{
	//return SnapshotManager::HasRedo();

	return false;
}


bool
ProjectManager::HasUndo() const
{
	//return SnapshotManager::HasUndo();

	return false;
}


void
ProjectManager::Undo()
{
	//return SnapshotManager::Undo();
}


void
ProjectManager::Redo()
{
	//return SnapshotManager::Redo();
}


bool
ProjectManager::WasSaved() const
{
	return fWasSaved;
}


status_t
ProjectManager::SaveProject()
{
	BMessage* msg = AudioGate::ArchiveTracks();

	msg->Flatten(fProjectFile);

	delete msg;

	fWasSaved = true;
	return B_OK;
}


status_t
ProjectManager::LoadProject(entry_ref ref)
{
	_Cleanup();

	BPath path(ref.name);

	fProjectPath.SetTo(path.Path());
	fProjectFile = new BFile(&ref, B_READ_WRITE);

	BMessage* msg = new BMessage();

	if (msg->Unflatten(fProjectFile) == B_OK
		&& AudioGate::UnarchiveTracks(msg) != B_OK) {
		fWasSaved = true;
		return B_OK;
	}
	return B_ERROR;
}


status_t
ProjectManager::LoadFile(entry_ref ref)
{
	// TODO maybe use B_MEDIA_FILE_BIG_BUFFERS !?
	BMediaFile* mediaFile = new BMediaFile(&ref);
	ObjectDeleter<BMediaFile> deleter(mediaFile);

	if (mediaFile->InitCheck() == B_OK)
		return LoadMediaFile(mediaFile, ref.name);

	printf("Loading project\n");
	return LoadProject(ref);
}


status_t
ProjectManager::LoadMediaFile(BMediaFile* mediaFile, const char* name)
{
	AudioTrack* track = TrackIO::ImportAudio(mediaFile, name);

	if (track == NULL)
		return B_ERROR;

	printf("Faber: Loading media file %s\n", name);

	status_t ret = track->InitCheck();

	if (ret != B_OK) {
		delete track;
		printf("Error loading track: %s\n", strerror(ret));
		return ret;
	}

  	WindowsManager::MainWindow()->MainView()->AddTrack(track);

	return B_OK;
}


BPath
ProjectManager::GetProjectPath() const
{
	return fProjectPath;
}


status_t
ProjectManager::RegisterTrack(Track* track)
{
	if (track->IsAudio()) {
		return AudioGate::RegisterTrack((AudioTrack*)track);
	}

	return B_ERROR;
}


status_t
ProjectManager::UnregisterTrack(Track* track)
{
	if (track->IsAudio()) {
		return AudioGate::UnregisterTrack((AudioTrack*)track);
	}

	return B_ERROR;
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


void
ProjectManager::_Cleanup()
{

}
