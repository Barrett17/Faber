
#include <MediaFile.h>

class WavePeak
{
					WavePeak(AudioTrack* track);

	BPositionIO*	PreviewRequested();

private:
	void			_RenderPeaks();

	AudioTrack*		fTrack;
	BMallocIO*		fPreview;
}
