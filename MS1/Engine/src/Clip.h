//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef CLIP_H
#define CLIP_H

#include "AnimTime.h"
#include "Bone.h"
#include "FontSprite.h"

namespace Azul
{
	class Clip
	{
	public:

		enum class Type
		{
			WALK,
			RUN,
			DIE_LEFT,
			SIDE_STEP_RIGHT,
			NOT_INITIALIZED
		};


		struct FrameBucket
		{
			~FrameBucket()
			{
				delete[] poBone;
			}

			FrameBucket *nextBucket;
			FrameBucket *prevBucket;
			AnimTime      KeyTime;
			Bone         *poBone;
			char          pad[4];
		};

	public:
		Clip(const char* const pClipFileName, int _numBones, int _numFrames);

		Clip() = delete;
		Clip(const Clip &) = delete;
		Clip &operator = (const Clip &) = delete;
		~Clip();

		void ChangeAnim(bool isFirstAnim, bool isSkeletonA, FontSprite* f);
		AnimTime GetTotalTime();
		void privSetAnimationData(const char* clipFile);
		void AnimateBones(AnimTime tCurr, Bone *pResult);

	private:
		//void privSetAnimationData();
		AnimTime privFindMaxTime();
		int  privFindNumFrames();

	private:
		int          numBones;
		int          numFrames;
		AnimTime     TotalTime;
		FrameBucket *poHead;

		int animType = 0;
	};
}

#endif

//--- End of File ---
