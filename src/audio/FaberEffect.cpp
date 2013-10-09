
#include "FaberEffect.h"


FaberEffect::FaberEffect(const char* name, uint32 flags)
	:
	fName(name),
	fFlags(flags)
{
}


FaberEffect::~FaberEffect()
{
}



const char*
FaberEffect::Name() const
{
	return fName;
}


BMenuItem*
FaberEffect::BuildItem()
{
	return new BMenuItem(Name(), NULL, 0, 0);
}



int32
FaberEffect::Flags() const
{
	return fFlags;
}



status_t
FaberEffect::FilterTrack(AudioTrack* track,
	int64 start, size_t size)
{

}


status_t
FaberEffect::FlattenSettings(BMessage* message)
{

}


status_t
FaberEffect::UnflattenSettings(BMessage* message)
{

}
