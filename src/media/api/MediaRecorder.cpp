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

#include "MediaRecorder.h"


MediaRecorder::MediaRecorder(const char* name,
	media_format format, int32 priority)
{
}


MediaRecorder::~MediaRecorder()
{
}


status_t
MediaRecorder::InitCheck() const
{
}


const media_node &
MediaRecorder::Node() const
{
}


status_t
MediaRecorder::Connect(const dormant_node_info& info,
	const media_format* format, uint32 flags)
{
}


status_t
MediaRecorder::Connect(const media_node& node,
	const media_output* useOutput,
	const media_format* format, uint32 flags)
{
}


bool
MediaRecorder::IsConnected() const
{
}


status_t
MediaRecorder::Disconnect()
{
}


status_t
MediaRecorder::Start()
{
}


status_t
MediaRecorder::Stop()
{
}


bool
MediaRecorder::IsStarted() const
{
}


const media_output&
MediaRecorder::Input() const
{
}


const media_input&
MediaRecorder::Output() const
{
}


const media_format&
MediaRecorder::Format() const
{
}


void
MediaRecorder::BufferReceived(void* data,
	size_t size, const media_header& header)
{
}
