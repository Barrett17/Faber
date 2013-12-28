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

#include "AudioProducer.h"


AudioProducer::~AudioProducer()
{
}


void
AudioProducer::NodeRegistered()
{
}


BMediaAddOn*
AudioProducer::AddOn(int32* id) const
{
}


status_t
AudioProducer::HandleMessage(int32 code,
	const void* data, size_t size)
{
}


status_t
AudioProducer::FormatSuggestionRequested(media_type type,
	int32 quality, media_format* format)
{
}


status_t
AudioProducer::FormatProposal(const media_source& src,
	media_format* format)
{
}


status_t
AudioProducer::FormatChangeRequested(const media_source& src,
	const media_destination& dst, media_format* format,
	int32* _deprecated_)
{
}


void
AudioProducer::LateNoticeReceived(const media_source& src,
	bigtime_t late, bigtime_t when)
{
}


status_t
AudioProducer::GetNextOutput(int32* cookie,
	media_output* output)
{
}


status_t
AudioProducer::DisposeOutputCookie(int32 cookie)
{
}


status_t
AudioProducer::SetBufferGroup(const media_source& src,
	BBufferGroup* group)
{
}


status_t
AudioProducer::PrepareToConnect(const media_source& src,
	const media_destination& dst, media_format* format,
	media_source* out_source, char* name)
{
}


void
AudioProducer::Connect(status_t status,
	const media_source& src, const media_destination& dst,
	const media_format& format, char *name)
{
}


void
AudioProducer::Disconnect(const media_source& src,
	const media_destination& dst)
{
}


void
AudioProducer::EnableOutput(const media_source& src, bool enabled,
	int32* _deprecated_)
{
}


status_t
AudioProducer::GetLatency(bigtime_t* outLatency)
{
}


void
AudioProducer::LatencyChanged(const media_source& src,
	const media_destination& dst, bigtime_t latency, uint32 flags)
{
}


AudioProducer::AudioProducer(const char* name, const media_format format)
{
}
