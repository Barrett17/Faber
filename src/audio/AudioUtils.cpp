/*
 * Copyright 2003-2009 Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Marcus Overhagen
 */


#include <MediaDefs.h>
#include <OS.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "AudioUtils.h"

const char *StringForFormat(char *str, const media_format & format);


int
count_nonzero_bits(uint32 value)
{
	int count = 0;
	for (int i = 0; i < 32; i++)
		if (value & (1 << i))
			count++;
	return count;
}


uint32
GetChannelMask(int channel, uint32 all_channel_masks)
{
	if (all_channel_masks == 0) {
		debugger("Mixer: GetChannelMask: all_channel_masks == 0\n");
		return 0;
	}
	if (channel > count_nonzero_bits(all_channel_masks)) {
		debugger("Mixer: GetChannelMask: channel > count_nonzero_bits(all_channel_masks)\n");
		return 0;
	}

	uint32 mask = 1;
	int pos = 0;
	for (;;) {
		while ((all_channel_masks & mask) == 0)
			mask <<= 1;
		if (pos == channel)
			return mask;
		pos++;
		mask <<= 1;
		if (mask == 0)
			return 0;
	}
}

int ChannelMaskToChannelType(uint32 mask)
{
	for (int i = 0; i < 32; i++)
		if (mask & (1 << i))
			return i;
	return -1;
}

uint32 ChannelTypeToChannelMask(int type)
{
	if (type < 0 || type > 31)
		return 0;
	return 1 << type;
}

int
GetChannelType(int channel, uint32 all_channel_masks)
{
	return ChannelMaskToChannelType(GetChannelMask(channel, all_channel_masks));
}

bool
HasKawamba()
{
	team_info i;
	int32 c = 0;
	while (!get_next_team_info(&c, &i))
		if (i.argc && strstr(i.args, "\x42\x65\x54\x75\x6e\x65\x73"))
			return true;
	return false;
}

void
ZeroFill(float *_dst, int32 _dst_sample_offset, int32 _sample_count)
{
	register char * dst = (char *) _dst;
	register int32 sample_count = _sample_count;
	register int32 dst_sample_offset = _dst_sample_offset;
	while (sample_count--) {
		*(float *)dst = 0.0f;
		dst += dst_sample_offset;
	}
}

int64
frames_for_duration(double framerate, bigtime_t duration)
{
	if (duration <= 0 || framerate <= 0.0)
		return 0;
	return (int64) ceil(framerate * double(duration) / 1000000.0);
}

bigtime_t
duration_for_frames(double framerate, int64 frames)
{
	if (frames <= 0 || framerate <= 0.0)
		return 0;
	return (bigtime_t)((1000000.0 * frames) / framerate);
}

int
bytes_per_sample(const media_multi_audio_format & format)
{
	return format.format & 0xf;
}

int
bytes_per_frame(const media_multi_audio_format & format)
{
	return format.channel_count * (format.format & 0xf);
}

int
frames_per_buffer(const media_multi_audio_format & format)
{
	int frames = 0;
	if (bytes_per_frame(format) > 0) {
		frames = format.buffer_size / bytes_per_frame(format);
	}
	return frames;
}

bigtime_t
buffer_duration(const media_multi_audio_format & format)
{
	bigtime_t duration = 0;
	if (format.buffer_size > 0 && format.frame_rate > 0 && bytes_per_frame(format) > 0) {
		duration = s_to_us((format.buffer_size / bytes_per_frame(format)) / format.frame_rate);
	}
	return duration;
}

double
us_to_s(bigtime_t usecs)
{
	return (usecs / 1000000.0);
}

bigtime_t
s_to_us(double secs)
{
	return (bigtime_t) (secs * 1000000.0);
}

const char *StringForFormat(char *str, const media_format & format)
{
	char fmtstr[20];
	const char *fmt;
	switch (format.u.raw_audio.format) {
		case media_raw_audio_format::B_AUDIO_FLOAT:
			fmt = "float";
			break;
		case media_raw_audio_format::B_AUDIO_INT:
			if (format.u.raw_audio.valid_bits != 0) {
				sprintf(fmtstr, "%d bit", format.u.raw_audio.valid_bits);
				fmt = fmtstr;
			} else {
				fmt = "32 bit";
			}
			break;
		case media_raw_audio_format::B_AUDIO_SHORT:
			fmt = "16 bit";
			break;
		case media_raw_audio_format::B_AUDIO_CHAR:
			fmt = "8 bit";
			break;
		case media_raw_audio_format::B_AUDIO_UCHAR:
			fmt = "8 bit unsigned";
			break;
		default:
			fmt = "unknown";
			break;
	}
	int a,b;
	a = int(format.u.raw_audio.frame_rate + 0.05) / 1000;
	b = int(format.u.raw_audio.frame_rate + 0.05) % 1000;
	if (b)
		sprintf(str, "%d.%d kHz %s", a, b / 100, fmt);
	else
		sprintf(str, "%d kHz %s", a, fmt);
	return str;
}
