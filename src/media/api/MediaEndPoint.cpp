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


MediaEndPoint::MediaEndPoint(int32 id, const char* name)
{
}


MediaEndPoint::~MediaEndPoint()
{
}


const int32
MediaEndPoint::ID() const
{
}


const char*
MediaEndPoint::Name() const
{
}


const media_node&
MediaEndPoint::Node() const
{
}


status_t
MediaEndPoint::SetOutput(media_output* out)
{
}


media_output
MediaEndPoint::Output() const
{
}


status_t
MediaEndPoint::SetInput(media_input* in)
{
}


media_input
MediaEndPoint::Input() const
{
}


status_t
MediaEndPoint::SetSource(media_source& source)
{
}


const media_source&
MediaEndPoint::Source() const
{
}


status_t
MediaEndPoint::SetDestination(media_destination& dest)
{
}


const media_destination&
MediaEndPoint::Destination() const
{
}


status_t
MediaEndPoint::SetFormat(media_format* format)
{
}


const media_format&
MediaEndPoint::Format() const
{
}


void
MediaEndPoint::SetDataStatus(int32 status)
{
}


int32
MediaEndPoint::DataStatus() const
{
}


void
MediaEndPoint::SetOutputEnabled(bool enabled)
{
}


bool
MediaEndPoint::OutputEnabled() const
{
}


void
MediaEndPoint::SetBufferGroup(BBufferGroup* buffers, bool own)
{
}


const BBufferGroup*
MediaEndPoint::Buffers() const
{
}


void
MediaEndPoint::SetNextBuffer(BBuffer *buffer)
{
}


BBuffer *
MediaEndPoint::NextBuffer() const
{
}


void
MediaEndPoint::SetLatencies(bigtime_t* process, bigtime_t* downstream)
{
}


bool
MediaEndPoint::BuffersOwned() const
{
}


bigtime_t
MediaEndPoint::ProcessLatency() const
{
}


bigtime_t
MediaEndPoint::DownstreamLatency() const
{
}


void
MediaEndPoint::GetLatencies(bigtime_t* process, bigtime_t* downstream)
{
}
