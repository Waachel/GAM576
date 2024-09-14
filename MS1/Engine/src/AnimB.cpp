//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "AnimB.h"
#include "Constants.h"
#include "MathEngine.h"
#include "MathApp.h"
#include "Mixer.h"
#include "Clip.h"

namespace Azul
{
	extern Clip *pClipB;

	AnimB::AnimB()
		: pClip(nullptr),
		poBoneResult(nullptr),
		poSkeleton(nullptr),
		numBones(NUM_BONES)
	{
		this->poBoneResult = new Bone[(unsigned int)numBones]();
		assert(poBoneResult);

		this->poSkeleton = new SkeletonB(this->poBoneResult, numBones);
		assert(this->poSkeleton);

		// Get this from the AnimMan
		this->pClip = pClipB;
		assert(pClip);


	}

	AnimB::~AnimB()
	{
		delete[] this->poBoneResult;
		delete   this->poSkeleton;
	}

	AnimTime AnimB::FindMaxTime()
	{
		assert(pClip);
		return this->pClip->GetTotalTime();
	}

	void AnimB::Animate(AnimTime tCurr)
	{
		this->pClip->AnimateBones(tCurr, this->poBoneResult);
	}


	void AnimB::PoseSkeleton(GameObjectAnimSkeleton *node)
	{
		// safety
		assert(node);

		// Now get the world matrices
		GameObjectAnimSkeleton *childNode = (GameObjectAnimSkeleton *)node->GetChild();
		GameObjectAnimSkeleton *parentNode = node;

		// Normal.. valid child
		if(parentNode != nullptr && childNode != nullptr)
		{
			// starting point
			Vec3 start(0.0f, 0.0f, 0.0f);

			//  At this point, Find the two bones initial positions in world space
			//       Now get the length and directions
			Vec3 ptA(Vec4(start, 1) * *parentNode->GetWorld());
			Vec3 ptB(Vec4(start, 1) * *childNode->GetWorld());

			// direction between the anchor points of the respective bones
			Vec3 dir = ptB - ptA;

			// length of the bone 0
			float mag = dir.len();

			Scale S(1, 1, mag);
			Quat Q(Orient::LocalToWorld, dir.getNorm(), Vec3(0.0f, 1.0f, 0.0f));
			Trans T(ptA);
			Mat4 BoneOrient = S * Q * T;
			parentNode->SetBoneOrientation(BoneOrient);
		}

		// deal with last node, when there isn't a terminal node
		if(parentNode != nullptr && childNode == nullptr)
		{
			// get the parent's parent  -> grandParent
			GameObjectAnimSkin *grandParentNode = (GameObjectAnimSkin *)parentNode->GetParent();
			assert(grandParentNode);
			Vec3 pTmp1(Vec4(0, 0, 0, 1) * *grandParentNode->GetWorld());
			Vec3 pTmp2(Vec4(0, 0, 0, 1) * *parentNode->GetWorld());
			Vec3 dir = pTmp2 - pTmp1;
			float mag = dir.len();

			Vec3 pTmp3(Vec4(0, 0, 0, 1) * *parentNode->GetWorld());
			Vec3 pTmp4(Vec4(1, 0, 0, 1) * *parentNode->GetWorld());
			dir = pTmp4 - pTmp3;

			Scale S(1.0f, 1.0f, mag);
			Rot R(Orient::LocalToWorld, dir.getNorm(), Vec3(0.0f, 1.0f, 0.0f));
			Trans T(pTmp2);

			Mat4 mTmp = S * R * T;
			parentNode->SetBoneOrientation(mTmp);
		}

	}

}

//--- End of File ----