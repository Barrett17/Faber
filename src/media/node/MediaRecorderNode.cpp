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

#include "MediaRecorderNode.h"

MediaRecorderNode::MediaRecorderNode(const char* name,
	const media_format format, int32 priority)
{
}


MediaRecorderNode::~MediaRecorderNode()
{
}


void
MediaRecorderNode::SetFormat(const media_format format)
{
}


media_format&
MediaRecorderNode::Format() const
{
}


status_t
MediaRecorderNode::GetInput(media_input* out_input) const
{
}


void
MediaRecorderNode::SetDataEnabled(bool enabled)
{
}


void
MediaRecorderNode::NodeRegistered()
{
}


BMediaAddOn*
MediaRecorderNode::AddOn(int32* id) const
{
}


status_t
MediaRecorderNode::HandleMessage(int32 code,
	const void* data, size_t size)
{
}


void
MediaRecorderNode::BufferReceived(BBuffer* buffer)
{
}


status_t
MediaRecorderNode::AcceptFormat(const media_destination& dst,
	media_format* format)
{
}


status_t
MediaRecorderNode::GetNextInput(int32* cookie, media_input* input)
{
}


void
MediaRecorderNode::DisposeInputCookie(int32 cookie)
{
}


status_t
MediaRecorderNode::FormatChanged(const media_source& src,
	const media_destination& dst, int32 change_tag,
	const media_format& format)
{
}


void
MediaRecorderNode::ProducerDataStatus(const media_destination& dst,
	int32 status, bigtime_t when)
{
}


status_t
MediaRecorderNode::GetLatencyFor(const media_destination& dst,
	bigtime_t* latency, media_node_id* time_src)
{
}


status_t
MediaRecorderNode::Connected(const media_source& src,
	const media_destination& dst,
	const media_format& format, media_input* input)
{
}


void
MediaRecorderNode::Disconnected(const media_source& src,
	const media_destination& where)
{
}
