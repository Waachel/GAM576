//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SkeletonB_H
#define SkeletonB_H

#include "GameObjectAnim.h"
#include "Bone.h"

namespace Azul
{
	class SkeletonB
	{
	public:
		struct Data
		{
			int  index;
			int  parentIndex;
			char name[64];
		};

	public:
		SkeletonB(Bone *pBone, int numBones);

		SkeletonB() = delete;
		SkeletonB(const SkeletonB &) = delete;
		SkeletonB &operator = (const SkeletonB &) = delete;
		~SkeletonB();

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
