//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SkeletonA_H
#define SkeletonA_H

#include "GameObjectAnim.h"
#include "Bone.h"

namespace Azul
{
	class SkeletonA
	{
	public:
		struct Data
		{
			int  index;
			int  parentIndex;
			char name[64];
		};

	public:
		SkeletonA(Bone *pBone, int numBones);

		SkeletonA() = delete;
		SkeletonA(const SkeletonA &) = delete;
		SkeletonA &operator = (const SkeletonA &) = delete;
		~SkeletonA();

		GameObjectAnim *GetFirstBone();

	private:
		void privSetAnimationHierarchy(Bone *pBoneResult);
		GameObjectAnim *privFindBoneByIndex(int index);

	private:
		GameObjectAnim *pFirstBone;
		int             numBones;
	};
}

#endif

// --- End of File ---
