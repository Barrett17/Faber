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
	:
	BMediaNode(name),
	BBufferConsumer(B_MEDIA_RAW_AUDIO)
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


/*
void
AudioConsumer::BufferReceived(BBuffer* buffer)
{
}*/


status_t
AudioConsumer::AcceptFormat(const media_destination& dst, media_format* format)
{
	if (dst.port != ControlPort())
		return B_MEDIA_BAD_DESTINATION;

	if (format->type == B_MEDIA_UNKNOWN_TYPE)
		format->type = B_MEDIA_RAW_AUDIO;

	if (format->type != B_MEDIA_RAW_AUDIO)
		return B_MEDIA_BAD_FORMAT;

	/*if (format->u.raw_audio.format != media_raw_audio_format::B_AUDIO_INT)
		return B_MEDIA_BAD_FORMAT;

	// TODO finish it
	format->u.raw_audio.format = media_raw_audio_format::B_AUDIO_INT;*/
	return B_OK;
}


status_t
AudioConsumer::GetNextInput(int32* cookie, media_input* input)
{
	/*MediaEndPointMap* endPoints = fAudioGate->GetInputPorts();

	if (*cookie >= endPoints->CountItems())
		return B_BAD_INDEX;

	MediaEndPoint* endpoint = endPoints->ItemAt(*cookie);
	if (endpoint == NULL)
		return B_BAD_INDEX;

	*input = endpoint->Input();
	*cookie++;*/

	return B_OK;
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
	return B_MEDIA_BAD_FORMAT;
}


void
AudioConsumer::ProducerDataStatus(const media_destination& dst,
	int32 status, bigtime_t when)
{
	//if (dst != media_destination::null)
	//	SendDataStatus(status, dst, when);
}


status_t
AudioConsumer::GetLatencyFor(const media_destination& dst,
	bigtime_t* latency, media_node_id* time_src)
{
	/*//printf("AudioConsumer::GetLatencyFor\n");
	*latency = fDownstreamLatency + fProcessLatency;
	// we have multiple inputs with different IDs, but
	// the port number must match our ControlPort()
	if (dst.port != ControlPort())
		return B_MEDIA_BAD_DESTINATION;

	// return our event latency - this includes our internal + downstream
	// latency, but _not_ the scheduling latency
	*latency += EventLatency();
	*time_src += TimeSource()->ID();

	//printf("AudioConsumer::GetLatencyFor %Ld, timesource is %ld\n",
	//	*latency, *time_src);*/

	return B_OK;
}


status_t
AudioConsumer::Connected(const media_source& src,
	const media_destination& dst, const media_format& format,
	media_input* input)
{
	if (src.id != 0 || src.port != ControlPort())
		return B_MEDIA_BAD_DESTINATION;
	
	//fFormat = format;

	return B_OK;
}


void
AudioConsumer::Disconnected(const media_source& src,
	const media_destination& where)
{
	if (where.port != ControlPort())
		return;
}
