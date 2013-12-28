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

#include "AudioConsumer.h"


AudioConsumer::AudioConsumer(const char* name,
	const media_format format)
{
}


void
AudioConsumer::SetFormat(const media_format format)
{
}


media_format&
AudioConsumer::Format() const
{
}


AudioConsumer::~AudioConsumer()
{
}


void
AudioConsumer::NodeRegistered()
{
}


BMediaAddOn*
AudioConsumer::AddOn(int32 *id) const
{
}


status_t
AudioConsumer::HandleMessage(int32 code, const void* data, size_t size)
{
}


void
AudioConsumer::BufferReceived(BBuffer* buffer)
{
}


status_t
AudioConsumer::AcceptFormat(const media_destination& dst, media_format* format)
{
}


status_t
AudioConsumer::GetNextInput(int32* cookie, media_input* input)
{
}


void
AudioConsumer::DisposeInputCookie(int32 cookie)
{
}


status_t
AudioConsumer::FormatChanged(const media_source& src, 
	const media_destination& dst, int32 change_tag,
	const media_format& format)
{
}


void
AudioConsumer::ProducerDataStatus(const media_destination& dst,
	int32 status, bigtime_t when)
{
}


status_t
AudioConsumer::GetLatencyFor(const media_destination& dst,
	bigtime_t* latency, media_node_id* time_src)
{
}


status_t
AudioConsumer::Connected(const media_source& src,
	const media_destination& dst, const media_format& format,
	media_input* input)
{
}


void
AudioConsumer::Disconnected(const media_source& src,
	const media_destination& where)
{
}
