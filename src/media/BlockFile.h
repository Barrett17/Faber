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

class BlockFile
{
public:

	int32					ID() const;

	BPath					GetPath() const;
	const char*				Name() const;

	MediaBlock*				ReadNextBlock();
	MediaBlock*				BlockAt(int32 index);
	int32					CountBlocks();

	status_t				AppendBlock(MediaBlock* block);
	status_t				PrependBlock(MediaBlock* block);

	status_t				WriteBlock(MediaBlock* block, int32 index);
	status_t				WriteBlock(MediaBlock* block,
								int64 start);

	status_t				CommitChanges();
	status_t				CleanAll();

	BMessage*				ArchiveStructure();

private:
	void					_LoadBlocks();

	MediaBlock*				fLastBlock;

	BObjectList<MediaBlock>	fWriteQueue;

	int64					fSize;

	int64					fStart;
	int64					fEnd;

};
