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

#include "AudioEngine.h"

#include <Buffer.h>
#include <BufferGroup.h>
#include <SupportDefs.h>

#include <stdio.h>

#define ENGINE_FORMAT media_raw_audio_format::B_AUDIO_FLOAT
#define ENGINE_BUFSIZE 1024


AudioEngine::AudioEngine()
	:
	BMediaNode("Faber Audio Engine"),
	BBufferConsumer(B_MEDIA_RAW_AUDIO),
	BBufferProducer(B_MEDIA_RAW_AUDIO),
	BMediaEventLooper()
{
}


AudioEngine::~AudioEngine()
{
	BMediaEventLooper::Quit();
}


// *** BMediaAddon ***


BMediaAddOn*
AudioEngine::AddOn(int32* id) const
{
	return NULL;
}


void
AudioEngine::NodeRegistered()
{
	SetPriority(B_REAL_TIME_PRIORITY);

	fFormat.type = B_MEDIA_RAW_AUDIO;
	fFormat.u.raw_audio = media_raw_audio_format::wildcard;

	fFormat.u.raw_audio.channel_count = 1;
	fFormat.u.raw_audio.frame_rate = 44100.0;
	fFormat.u.raw_audio.byte_order
		= (B_HOST_IS_BENDIAN) ? B_MEDIA_BIG_ENDIAN : B_MEDIA_LITTLE_ENDIAN;

	fFormat.u.raw_audio.buffer_size = ENGINE_BUFSIZE;

	fFormat.u.raw_audio.channel_count = 1;
	fFormat.u.raw_audio.format = ENGINE_FORMAT;

	//fOwner->SetFormat(fFormat);

	Run();
}


status_t
AudioEngine::HandleMessage(int32 code,
	const void *data, size_t size)
{
	return B_OK;
}


// *** BBufferConsumer ***


void
AudioEngine::BufferReceived(BBuffer* buffer)
{
	media_timed_event event(buffer->Header()->start_time,
		BTimedEventQueue::B_HANDLE_BUFFER, buffer,
		BTimedEventQueue::B_RECYCLE_BUFFER);
	EventQueue()->AddEvent(event);
}


status_t
AudioEngine::AcceptFormat(const media_destination& dst, media_format* format)
{
	if (dst.port != ControlPort())
		return B_MEDIA_BAD_DESTINATION;

	if (format->type == B_MEDIA_UNKNOWN_TYPE)
		format->type = B_MEDIA_RAW_AUDIO;

	if (format->type != B_MEDIA_RAW_AUDIO)
		return B_MEDIA_BAD_FORMAT;

	if (format->u.raw_audio.format != media_raw_audio_format::ENGINE_FORMAT)
		return B_MEDIA_BAD_FORMAT;

	// TODO finish it
	format->u.raw_audio.format = media_raw_audio_format::ENGINE_FORMAT;
	return B_OK;
}


status_t
AudioEngine::GetNextInput(int32* cookie, media_input* input)
{
	/*MediaEndPointMap* endPoints = fAudioGate->GetInputPorts();

	if (*cookie >= endPoints->CountItems())
		return B_BAD_INDEX;

	MediaEndPoint* endpoint = endPoints->ItemAt(*cookie);
	if (endpoint == NULL)
		return B_BAD_INDEX;

	*input = endpoint->Input();
	*cookie++;

	return B_OK;*/
	return B_BAD_INDEX;
}


void
AudioEngine::DisposeInputCookie(int32 cookie)
{
}


status_t
AudioEngine::FormatChanged(const media_source& src, 
	const media_destination& dst, int32 change_tag,
	const media_format& format)
{
	return B_MEDIA_BAD_FORMAT;
}


void
AudioEngine::ProducerDataStatus(const media_destination& dst,
	int32 status, bigtime_t when)
{
	if (dst != media_destination::null)
		SendDataStatus(status, dst, when);
}


status_t
AudioEngine::GetLatencyFor(const media_destination& dst,
	bigtime_t* latency, media_node_id* time_src)
{
	//printf("AudioEngine::GetLatencyFor\n");
	*latency = fDownstreamLatency + fProcessLatency;
	// we have multiple inputs with different IDs, but
	// the port number must match our ControlPort()
	if (dst.port != ControlPort())
		return B_MEDIA_BAD_DESTINATION;

	// return our event latency - this includes our internal + downstream
	// latency, but _not_ the scheduling latency
	*latency += EventLatency();
	*time_src += TimeSource()->ID();

	//printf("AudioEngine::GetLatencyFor %Ld, timesource is %ld\n",
	//	*latency, *time_src);

	return B_OK;
}


status_t
AudioEngine::Connected(const media_source& src,
	const media_destination& dst, const media_format& format,
	media_input* input)
{
	if (src.id != 0 || src.port != ControlPort())
		return B_MEDIA_BAD_DESTINATION;
	
	fFormat = format;

	return B_OK;
}


void
AudioEngine::Disconnected(const media_source& src,
	const media_destination& where)
{
	if (where.port != ControlPort())
		return;
}


// *** BBufferProducer ***


status_t
AudioEngine::FormatSuggestionRequested(media_type type,
	int32 quality, media_format* format)
{
	if (type != B_MEDIA_RAW_AUDIO)
		return B_MEDIA_BAD_FORMAT;

	*format = fFormat;
	
	return B_OK;
}


status_t
AudioEngine::FormatProposal(const media_source& src,
	media_format* format)
{
	if (src.id != 0 || src.port != ControlPort())
		return B_MEDIA_BAD_SOURCE;

	media_raw_audio_format* raw = &format->u.raw_audio;

	if (format->type == B_MEDIA_UNKNOWN_TYPE) {
		format->type = B_MEDIA_RAW_AUDIO;
	} else if (format->type != B_MEDIA_RAW_AUDIO ||
		format->u.raw_audio.format != ENGINE_FORMAT)
		return B_MEDIA_BAD_FORMAT;

	if (raw->format == 0)
		raw->format = fFormat.u.raw_audio.format;

 	if (raw->frame_rate == 0)
		raw->frame_rate = fFormat.u.raw_audio.frame_rate;

	if (raw->buffer_size == 0)
		raw->buffer_size = fFormat.u.raw_audio.buffer_size;

	if (raw->channel_count == 0)
		raw->channel_count = fFormat.u.raw_audio.channel_count;

	if (format->u.raw_audio.channel_count > 1
		|| raw->frame_rate != fFormat.u.raw_audio.frame_rate
		|| raw->buffer_size != fFormat.u.raw_audio.buffer_size)
		return B_MEDIA_BAD_FORMAT;

	return B_OK;
}


status_t
AudioEngine::FormatChangeRequested(const media_source& src,
	const media_destination& dst, media_format* format,
	int32* _deprecated_)
{
	return B_MEDIA_BAD_FORMAT;
}


status_t
AudioEngine::GetNextOutput(int32* cookie,
	media_output* output)
{
	/*MediaEndPointMap* endPoints = fAudioGate->GetOutputPorts();

	if (*cookie >= endPoints->CountItems())
		return B_BAD_INDEX;

	MediaEndPoint* endpoint = endPoints->ItemAt(*cookie);

	if (endpoint == NULL)
		return B_BAD_INDEX;

	*output = *endpoint->Output();

	*cookie += 1;

	return B_OK;*/
	return B_BAD_INDEX;
}


status_t
AudioEngine::DisposeOutputCookie(int32 cookie)
{
	return B_OK;
}


status_t
AudioEngine::SetBufferGroup(const media_source& src,
	BBufferGroup* group)
{
	if (src.port != ControlPort() || src.id != 0)
		return B_MEDIA_BAD_SOURCE;

	fBufferGroup = group;

	return B_OK;
}


status_t
AudioEngine::PrepareToConnect(const media_source& src,
	const media_destination& dst, media_format* format,
	media_source* out_source, char* name)
{
	if (dst.port == ControlPort())
		return B_MEDIA_BAD_SOURCE;

	if (src.port != ControlPort() || src.id != 0)
		return B_MEDIA_BAD_SOURCE;

	if (format->type != B_MEDIA_RAW_AUDIO
		&& format->type != B_MEDIA_UNKNOWN_TYPE) {
		return B_MEDIA_BAD_FORMAT;
	}

	// TODO do other controls here

	return B_MEDIA_BAD_SOURCE;
}


void
AudioEngine::Connect(status_t status,
	const media_source& src, const media_destination& dst,
	const media_format& format, char *name)
{
	if (status != B_OK)
		return;

	media_node_id id;
	FindLatencyFor(dst, &fDownstreamLatency, &id);

}


void
AudioEngine::Disconnect(const media_source& src,
	const media_destination& dst)
{
}


void
AudioEngine::EnableOutput(const media_source& src, bool enabled,
	int32* _deprecated_)
{
	if (src.id != 0 || src.port != ControlPort())
		return;
}


status_t
AudioEngine::GetLatency(bigtime_t* outLatency)
{
	*outLatency = EventLatency() + SchedulingLatency();
	return B_OK;
}


void
AudioEngine::LatencyChanged(const media_source& src,
	const media_destination& dst, bigtime_t latency, uint32 flags)
{
	if (src.port != ControlPort() || src.id != 0) {
		printf("Error: wrong source");
		return;
	}

	fDownstreamLatency = latency;
	SetEventLatency(fDownstreamLatency + fProcessLatency);

	/*MediaEndPoint* endpoint;
	MediaEndPointMap* inputs = fAudioGate->GetInputPorts();
	for (int i = 0; i < inputs->CountItems(); i++) {
		input = inputs->ItemAt(i)->Input();

		// consumer disabled
		//SendLatencyChange(input->source,
		//	input->destination, EventLatency());
	}*/

}


void
AudioEngine::LateNoticeReceived(const media_source& src,
	bigtime_t late,	bigtime_t when)
{
	NotifyLateProducer(src, late, when);
}


// *** BMediaEventLooper ***


void
AudioEngine::HandleEvent(const media_timed_event *event,
	bigtime_t late, bool realTimeEvent)
{
	/*//printf("ClientNode::HandleEvent %d\n", event->type);
	switch (event->type) {
		case BTimedEventQueue::B_HANDLE_BUFFER:
		{
			printf("BTimedEventQueue::B_HANDLE_BUFFER\n");

			break;
		}
		case BTimedEventQueue::B_START:
		{
			printf("BTimedEventQueue::B_START\n");
			if (RunState() != B_STARTED) {
			
			}
			break;
		}

		case BTimedEventQueue::B_STOP:
		{
			// stopped - don't process any more buffers, flush all buffers
			// from event queue
			EventQueue()->FlushEvents(0, BTimedEventQueue::B_ALWAYS, true,
				BTimedEventQueue::B_HANDLE_BUFFER);

			Stop(TimeSource()->Now(), true);
			NodeStopped(TimeSource()->Now());

			break;
		}

		case BTimedEventQueue::B_DATA_STATUS:
		{
			break;
		}

		case NEW_BUFFER_EVENT:
		{
			ComputeCycle();
			_DataAvailable(event->event_time);

			// Now we schedule the next event
			bigtime_t nextEvent = fTime + bigtime_t((1000000LL * fFramesSent)
				/ (int32)fFormat.u.raw_audio.frame_rate)+EventLatency();
			_ScheduleOutputEvent(nextEvent);

			break;
		}

		default:
			break;
	}*/
}
