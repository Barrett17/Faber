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


AudioProducer::AudioProducer(const char* name,
	const media_format format)
	:
	BMediaNode(name),
	BBufferProducer(B_MEDIA_RAW_AUDIO)
{
}


AudioProducer::~AudioProducer()
{
}


void
AudioProducer::SetFormat(const media_format format)
{
}


media_format&
AudioProducer::Format() const
{
}

/*
BMediaAddOn*
AudioProducer::AddOn(int32* id) const
{
	return NULL;
}


void
AudioProducer::NodeRegistered()
{
	//Run();
}


status_t
AudioProducer::HandleMessage(int32 code,
	const void *data, size_t size)
{
	return B_OK;
}
*/

status_t
AudioProducer::FormatSuggestionRequested(media_type type,
	int32 quality, media_format* format)
{
	if (type != B_MEDIA_RAW_AUDIO)
		return B_MEDIA_BAD_FORMAT;

	//*format = fFormat;
	
	return B_OK;
}


status_t
AudioProducer::FormatProposal(const media_source& src,
	media_format* format)
{
	if (src.id != 0 || src.port != ControlPort())
		return B_MEDIA_BAD_SOURCE;

	/*media_raw_audio_format* raw = &format->u.raw_audio;

	if (format->type == B_MEDIA_UNKNOWN_TYPE) {
		format->type = B_MEDIA_RAW_AUDIO;
	} else if (format->type != B_MEDIA_RAW_AUDIO ||
		format->u.raw_audio.format != FABER_AUDIO_FORMAT)
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
*/
	return B_OK;
}


status_t
AudioProducer::FormatChangeRequested(const media_source& src,
	const media_destination& dst, media_format* format,
	int32* _deprecated_)
{
	return B_MEDIA_BAD_FORMAT;
}


status_t
AudioProducer::GetNextOutput(int32* cookie,
	media_output* output)
{
	/*MediaEndPointMap* endPoints = fAudioGate->GetOutputPorts();

	if (*cookie >= endPoints->CountItems())
		return B_BAD_INDEX;

	MediaEndPoint* endpoint = endPoints->ItemAt(*cookie);

	if (endpoint == NULL)
		return B_BAD_INDEX;

	*output = *endpoint->Output();

	*cookie += 1;*/

	return B_OK;
}


status_t
AudioProducer::DisposeOutputCookie(int32 cookie)
{
	return B_OK;
}


status_t
AudioProducer::SetBufferGroup(const media_source& src,
	BBufferGroup* group)
{
	/*if (src.port != ControlPort() || src.id != 0)
		return B_MEDIA_BAD_SOURCE;

	fBufferGroup = group;*/

	return B_OK;
}


status_t
AudioProducer::PrepareToConnect(const media_source& src,
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
AudioProducer::Connect(status_t status,
	const media_source& src, const media_destination& dst,
	const media_format& format, char *name)
{
	/*if (status != B_OK)
		return;

	media_node_id id;
	FindLatencyFor(dst, &fDownstreamLatency, &id);*/

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
	if (src.id != 0 || src.port != ControlPort())
		return;
}


status_t
AudioProducer::GetLatency(bigtime_t* outLatency)
{
	//*outLatency = EventLatency() + SchedulingLatency();
	return B_OK;
}


void
AudioProducer::LatencyChanged(const media_source& src,
	const media_destination& dst, bigtime_t latency, uint32 flags)
{
	/*if (src.port != ControlPort() || src.id != 0) {
		printf("Error: wrong source");
		return;
	}

	fDownstreamLatency = latency;
	SetEventLatency(fDownstreamLatency + fProcessLatency);*/
}


void 
AudioProducer::ProducerDataStatus(const media_destination &dst, int32 status,
	bigtime_t when)
{
	//printf("ClientNode::ProducerDataStatus(\n");
	/*MediaEndPointMap* endPoints = fAudioGate->GetOutputPorts();

	for (int i = 0; i < endPoints->CountItems(); i++) {
		MediaEndPoint* endPoint = ports->ItemAt(i);
		if (endPoint->Destination() != media_destination::null)
			SendDataStatus(status, endPoint->Destination(), when);
	}*/
}

/*
void
AudioProducer::HandleEvent(const media_timed_event *event,
	bigtime_t late, bool realTimeEvent)
{
	//printf("ClientNode::HandleEvent %d\n", event->type);
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
	}
}*/
