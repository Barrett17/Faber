

class BlockFile
{
public:
	MediaBlock*				ReadNextBlock();
	MediaBlock*				BlockAt(int32 index);
	int32					CountBlocks();

	status_t				AppendBlock(MediaBlock* block);
	status_t				PrependBlock(MediaBlock* block);

	status_t				WriteBlock(MediaBlock* block, int32 index);
	status_t				WriteBlock(MediaBlock* block,
								int64 start);

	status_t				Commit();

private:
	void					_LoadBlocks();

	BObjectList<MediaBlock>	fWriteQueue;

	int64					fSize;

	int64					fStart;
	int64					fEnd;

};
