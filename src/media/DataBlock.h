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
#ifndef _DATA_BLOCK_H
#define _DATA_BLOCK_H


#include <DataIO.h>

class TrackIndex;


enum FaberDataBlockKinds {
	FABER_RAW_DATA = 1001,
	FABER_AUDIO_DATA,
	FABER_MIDI_DATA
};


// DataBlock i/o methods are protected, only MediaBlockVisitor
// can use them, through MediaBlock.
class DataBlock {
public:
								DataBlock(BPositionIO* data,
									FaberDataBlockKinds kind);	

	virtual	uint32				Kind() const;

protected:
	// BDataIO
	virtual	ssize_t				Read(void* buffer, size_t size);
	virtual	ssize_t				Write(const void* buffer, size_t size);

	// BPositionIO
	virtual	ssize_t				ReadAt(off_t position, void* buffer,
									size_t size);
	virtual	ssize_t				WriteAt(off_t position, const void* buffer,
									size_t size);

	virtual	off_t				Seek(off_t position, uint32 seekMode);
	virtual	off_t				Position() const;

	virtual	status_t			SetSize(off_t size);
	virtual	status_t			GetSize(off_t* size) const;

			status_t			_FlattenReservedData();
			status_t			_LoadReservedData();

private:
			BPositionIO*		fData;
			FaberDataBlockKinds	fKind;
};


// This block is meant to represent empty data.
// Useful to save space.
/*
class EmptyDataBlock : public DataBlock {

};

*/

#endif
