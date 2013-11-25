
#include <BView.h>

#include "WavePeaks.h"


class WaveRender : public BView
{
public:
							WaveRender(AudioTrack* track);	

	virtual void			Draw(BRect rect);
	virtual void			MouseDown(BPoint point);
	virtual void			MouseUp(BPoint point);

	virtual void			MouseMoved(BPoint point, uint32 ,
								const BMessage* message);

	virtual void			FrameResized(float width, float height);

			void			RedrawRequested();
			void			UpdateRequested(BRect rect);

			void			ScrollBy(int64 value);

			bool			IsSelected();
			void			CurrentSelection(int64 start, int64 end);

			int64			Pointer();

			void			ZoomIn();
			void			ZoomOut();
			void			ZoomFull();
			void			ZoomSelection();			

private:
			void			_Render();
			void			_UpdatePeaks();

			bool			fIsSelected;

			WavePeaks*		fWavePeaks;
}
