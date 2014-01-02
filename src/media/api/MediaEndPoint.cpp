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

#include "MediaEndPoint.h"

#include <MediaNode.h>


MediaEndPoint::MediaEndPoint(int32 id, const char* name,
	MediaEndPointKind kind)
	:
	fID(id),
	fName(name),
	fEndPointKind(kind)
	
{
}


MediaEndPoint::~MediaEndPoint()
{
}


const int32
MediaEndPoint::ID() const
{
	return fID;
}


const char*
MediaEndPoint::Name() const
{
	return fName;
}


const media_node&
MediaEndPoint::Node() const
{
	return fNode;
}


status_t
MediaEndPoint::SetOutput(media_output* out)
{
	if (out == NULL)
		return B_ERROR;

	fOutput = *out;
}


media_output
MediaEndPoint::Output() const
{
	return fOutput;
}


status_t
MediaEndPoint::SetInput(media_input* in)
{
	if (in == NULL)
		return B_ERROR;

	fInput = *in;
}


media_input
MediaEndPoint::Input() const
{
	return fInput;
}


bool
MediaEndPoint::IsInput() const
{
	return fEndPointKind == MEDIA_INPUT;
}


bool
MediaEndPoint::IsOutput() const
{
	return fEndPointKind == MEDIA_OUTPUT;
}


status_t
MediaEndPoint::SetSource(media_source& source)
{
	fMediaSource = source;
}


const media_source&
MediaEndPoint::Source() const
{
	return fMediaSource;
}


status_t
MediaEndPoint::SetDestination(media_destination& dest)
{
	fMediaDestination = dest;
}


const media_destination&
MediaEndPoint::Destination() const
{
	return fMediaDestination;
}


status_t
MediaEndPoint::SetFormat(media_format* format)
{
	if (format == NULL)
		return B_MEDIA_BAD_FORMAT;

	fFormat = *format;

	return B_OK;
}


const media_format&
MediaEndPoint::Format() const
{
	return fFormat;
}


void
MediaEndPoint::SetDataStatus(int32 status)
{
	fDataStatus = status;
}


int32
MediaEndPoint::DataStatus() const
{
	return fDataStatus;
}


void
MediaEndPoint::SetOutputEnabled(bool enabled)
{
	fOutputEnabled = enabled;
}


bool
MediaEndPoint::OutputEnabled() const
{
	return fOutputEnabled;
}


void
MediaEndPoint::SetBufferGroup(BBufferGroup* buffers, bool own)
{
	if (buffers == NULL)
		return;

	fOwnedBuffers = own;
}


const BBufferGroup*
MediaEndPoint::Buffers() const
{
	return fBufferGroup;
}


void
MediaEndPoint::SetNextBuffer(BBuffer *buffer)
{
	if (buffer == NULL)
		return;

	fNextBuffer = buffer;
}


BBuffer *
MediaEndPoint::NextBuffer() const
{
	return fNextBuffer;
}


void
MediaEndPoint::SetLatencies(bigtime_t* process, bigtime_t* downstream)
{
	if (process == NULL &&
		downstream == NULL)
		return;

	fProcessLatency = *process;
	fDownstreamLatency = *downstream;
}


bool
MediaEndPoint::BuffersOwned() const
{
	return fOwnedBuffers;
}


bigtime_t
MediaEndPoint::ProcessLatency() const
{
	return fProcessLatency;
}


bigtime_t
MediaEndPoint::DownstreamLatency() const
{
	return fDownstreamLatency;
}


void
MediaEndPoint::GetLatencies(bigtime_t* process, bigtime_t* downstream)
{
	*process = fProcessLatency;
	*downstream = fDownstreamLatency;
}
