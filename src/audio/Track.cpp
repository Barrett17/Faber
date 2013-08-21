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

#include "Track.h"

Track::Track()
	:
	fSelectionPointer(0),
	fCurrentPointer(0),
	fChanged(false),
	fMuted(false),
	fSolo(false),
	fPlaying(false),
	fRecording(false),
	fStart(0),
	fEnd(0)
{

}


int64
Track::Pointer() const
{
	return fCurrentPointer;
}


int64
Track::SelectionPointer() const
{
	return fSelectionPointer;
}


int64
Track::Start() const
{
	return 0;
}


int64
Track::End() const
{
	return fEnd;
}


void
Track::SetPointer(int64 pointer)
{
	fCurrentPointer = pointer;
}


void
Track::SetSelectionPointer(int64 pointer)
{
	fSelectionPointer = pointer;
}


void
Track::SetStart(int64 start)
{
	fStart = start;
}


void
Track::SetEnd(int64 end)
{
	fEnd = end;
}


void
Track::SetFormat(media_format format)
{
	fFormat.SpecializeTo(&format);
}


media_format
Track::Format() const
{
	return fFormat;
}


const BString&
Track::Name() const
{
	return fName;
}


void
Track::SetName(const char* name)
{
	fName.SetTo(name);
}


float
Track::Volume() const
{
	return fVolume;
}


void
Track::SetVolume(float volume)
{
	fVolume = volume;
}


float
Track::Balance() const
{
	return fBalance;
}


void
Track::SetBalance(float balance)
{
	fBalance = balance;
}


void
Track::MuteTrack(bool mute)
{
	fMuted = mute;
}


bool
Track::IsPlaying() const
{
	return fPlaying;
}


void
Track::SetPlaying(bool playing)
{
	fPlaying = playing;
}


bool
Track::IsRecording() const
{
	return fRecording;
}


void
Track::SetRecording(bool recording)
{
	fRecording = recording;
}


bool
Track::IsMute() const
{
	return fMuted;
}


void
Track::SetSolo(bool solo)
{
	fSolo = solo;
}


bool
Track::IsSolo() const
{
	return fSolo;
}
