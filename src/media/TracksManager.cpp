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

#include "TracksManager.h"

#include "FaberDefs.h"

TracksManager* TracksManager::fInstance = NULL;


TracksManager*
TracksManager::Get()
{
	if (fInstance == NULL)
		fInstance = new TracksManager();

	return fInstance;	
}

TracksManager::TracksManager()
	:
	fLastID(-1)
{
}


TracksManager::~TracksManager()
{
}


status_t
TracksManager::RegisterTrack(AudioTrack* track)
{
	Get()->fLastID++;
	track->SetID(Get()->fLastID);

	Get()->fTracks.AddItem(track);

	return B_OK;
}


void
TracksManager::UnregisterTrack(AudioTrack* track)
{
	Get()->fTracks.RemoveItem(track);
}
