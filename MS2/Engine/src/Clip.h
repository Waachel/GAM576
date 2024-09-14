//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef CLIP_H
#define CLIP_H

#include "AnimTime.h"
#include "Skel.h"

namespace Azul
{
	class Mixer;
	class FrameBucket;
	class FrameBucketEntry;

	class Clip : public DLink
	{
	public:
		enum Name
		{
			Walk_ChickenBot,
			Run_ChickenBot,
			DieLeft_ChickenBot,
			SideStepRight_ChickenBot,
			Dance_Mousey,
			ZombieWalk_Mousey,
			Not_Initialized
		};

	public:
		Clip();
		Clip(const Clip &) = delete;
		Clip &operator = (const Clip &) = delete;
		virtual ~Clip();

		AnimTime GetTotalTime();
		size_t GetNumBones();
		Skel::Name GetSkelName();
		void SetClipName(Name clipName);

		void Set(Clip::Name clipName,
				 unsigned int numBones,
				 unsigned int numKeyFrames,
				 FrameBucketEntry *pFrameBucket,
				 Skel::Name skelName);

		void AnimateBones(AnimTime tCurr, Mixer *pMixer);

		virtual bool Compare(DLink *pTarget) override;
		virtual void Dump() override;
		virtual void Wash() override;

		Name         mName;

	private:
		AnimTime privFindMaxTime();
		int  privFindNumFrames();

	private:
		
		size_t       numBones;
		size_t       numFrames;
		Skel::Name   skelName;
		AnimTime     TotalTime;
		FrameBucket *poHead;
	};
}

#endif

//--- End of File ---
