//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Clip.h"
#include "AnimTime.h"
#include "Bone.h"
#include "Mixer.h"
#include "File.h"

namespace Azul
{
	Clip::Clip(const char* const pClipFileName, int _numBones, int _numFrames)
		: numBones(_numBones),
		numFrames(_numFrames),
		TotalTime(AnimTime::Duration::ZERO),
		poHead(nullptr)
	{
		this->privSetAnimationData(pClipFileName);
		this->TotalTime = this->privFindMaxTime();
	}

	Clip::~Clip()
	{
		FrameBucket* pTmp = poHead;

		while (pTmp != nullptr)
		{
			FrameBucket* pDeleteMe = pTmp;
			pTmp = pTmp->nextBucket;
			delete pDeleteMe;
		}
	}

	//wants first animation state and is the first Skeleton
	void Clip::ChangeAnim(bool wantsFirstAnim, bool isSkeletonA, FontSprite* f)
	{
		//they want the firt animation of Skeleton A
		if (wantsFirstAnim && isSkeletonA)
		{
			Clip::~Clip();
			privSetAnimationData("walk_mesh.clip");

			f->ChangeMessage("Walk", 425, 290);
		}

		//they want the first animation of Skeleton B
		else if (wantsFirstAnim && !isSkeletonA)
		{
			Clip::~Clip();
			privSetAnimationData("run+mesh.clip");
			f->ChangeMessage("Run", 275, 290);
		}

		//they want the second animation of Skeleton A
		else if (!wantsFirstAnim && isSkeletonA)
		{
			Clip::~Clip();
			privSetAnimationData("die_left+mesh.clip");
			f->ChangeMessage("Die", 450, 290);
		}

		//they want the second animation of Skeleton B
		else
		{
			Clip::~Clip();
			privSetAnimationData("sidestep_right+mesh.clip");
			f->ChangeMessage("Side Step", 200, 290);
		}

	}

	void Clip::AnimateBones(AnimTime tCurr, Bone* pResult)
	{
		// First one 
		FrameBucket* pTmp = this->poHead;

		// Find which key frames
		while (tCurr >= pTmp->KeyTime && pTmp->nextBucket != nullptr)
		{
			pTmp = pTmp->nextBucket;
		}

		// pTmp is the "B" key frame
		// pTmp->prev is the "A" key frame
		FrameBucket* pA = pTmp->prevBucket;
		FrameBucket* pB = pTmp;

		// find the "S" of the time
		float tS = (tCurr - pA->KeyTime) / (pB->KeyTime - pA->KeyTime);

		Mixer::Blend(pResult, pA->poBone, pB->poBone, tS, this->numBones);

	}

	int Clip::privFindNumFrames()
	{
		int count = 0;
		FrameBucket* pTmp = this->poHead;

		while (pTmp->nextBucket != nullptr)
		{
			count++;
			pTmp = pTmp->nextBucket;
		}
		return count;
	}

	AnimTime Clip::privFindMaxTime()
	{
		AnimTime tMax;
		FrameBucket* pTmp = this->poHead;

		while (pTmp->nextBucket != nullptr)
		{
			pTmp = pTmp->nextBucket;
		}

		tMax = pTmp->KeyTime;

		return tMax;
	}

	AnimTime Clip::GetTotalTime()
	{
		return this->TotalTime;
	}

	float scale = 1.0f;



	void Clip::privSetAnimationData(const char* pClipFileName)
	{
		struct NewVect
		{
			NewVect(float _x, float _y, float _z)
				:x(_x), y(_y), z(_z)
			{
			}
			float x;
			float y;
			float z;
		};

		// File stuff
		File::Handle fh;
		File::Error  ferror;

		ferror = File::Open(fh, pClipFileName, File::Mode::READ);
		assert(ferror == File::Error::SUCCESS);


		// Read number of bones.
		// Get the size
		ferror = File::Seek(fh, File::Position::END, 0);
		assert(ferror == File::Error::SUCCESS);

		DWORD length;
		ferror = File::Tell(fh, length);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Seek(fh, File::Position::BEGIN, 0);
		assert(ferror == File::Error::SUCCESS);

		//name * 3(T,Q,S) * Vect size * number of bones * number of frames.
		//         which bone         T                   S                  Q                   12         Frames
		//int move = clipIndex * ((int)sizeof(Vec3) + (int)sizeof(Vec3) + (int)sizeof(Vec4)) * this->numBones * 25;
		// 1 Bon(12 Tof0 + 12 Sof0 + 16Qof0) * 12
		//ferror = File::Seek(fh, File::Position::END, move);
		//assert(status == File::Error::SUCCESS);

		Vec3* newVect3 = new Vec3(0.0f, 0.0f, 0.0f);
		Vec4* newVect4 = new Vec4(0.0f, 0.0f, 0.0f, 1.0f);

		Bone* pBone = new Bone[(unsigned int)this->numBones]();

		FrameBucket* pTmp = nullptr;
		FrameBucket* pTmpX = nullptr;

		for (int i = 0; i < this->numFrames; i++)
		{
			pTmpX = new FrameBucket();
			pTmpX->prevBucket = pTmp;
			pTmpX->nextBucket = nullptr;
			pTmpX->KeyTime = i * AnimTime(AnimTime::Duration::FILM_24_FRAME);
			pTmpX->poBone = new Bone[(unsigned int)this->numBones];
			//pTmp->nextBucket = pTmpX;
			if (i == 0)
			{
				this->poHead = pTmpX;
			}
			else
			{
				pTmp->nextBucket = pTmpX;
			}
			pTmp = pTmpX;
			
			// Bone: 0 
			pTmp->poBone[0].T = Vec3(0.000000f, 0.000000f, 0.000000f);
			pTmp->poBone[0].Q = Quat(0.000000f, 0.000000f, 0.000000f, 1.000000f);
			pTmp->poBone[0].S = Vec3(1.000000f, 1.000000f, 1.000000f);

			// Bone: 1 
			pTmp->poBone[1].T = Vec3(0.000000f, 0.000000f, 0.000000f);
			pTmp->poBone[1].Q = Quat(0.000000f, 0.000000f, 0.000000f, 1.000000f);
			pTmp->poBone[1].S = Vec3(1.000000f, 1.000000f, 1.000000f);

			// Bone: 2 
			pTmp->poBone[2].T = Vec3(0.000000f, 0.000000f, 0.000000f);
			pTmp->poBone[2].Q = Quat(-0.707107f, -0.000000f, 0.000000f, 0.707107f);
			pTmp->poBone[2].S = Vec3(1.000000f, 1.000000f, 1.000000f);

			// Bone: 3 
			pTmp->poBone[3].T = Vec3(0.000000f, 0.000000f, 0.000000f);
			pTmp->poBone[3].Q = Quat(0.500000f, -0.500000f, 0.500000f, 0.500000f);
			pTmp->poBone[3].S = Vec3(1.000000f, 1.000000f, 1.000000f);


			//Translation
			pTmp->poBone[4].T = Vec3(0.023785f, 0.000156f, -0.002526f);
			pTmp->poBone[5].T = Vec3(0.000030f, -0.008568f, 0.000247f);
			pTmp->poBone[6].T = Vec3(0.015902f, 0.000000f, -0.000000f);
			pTmp->poBone[7].T = Vec3(0.014309f, 0.000000f, 0.000000f);
			pTmp->poBone[8].T = Vec3(0.000030f, 0.008568f, 0.000247f);
			pTmp->poBone[9].T = Vec3(0.015902f, 0.000000f, 0.000000f);
			pTmp->poBone[10].T = Vec3(0.014309f, 0.000000f, -0.000000f);
			pTmp->poBone[11].T = Vec3(0.003649f, 0.000000f, 0.002990f);

			//Bone 4 - 10
			for (int j = 4; j < this->numBones - 1; j++)
			{
				// Trans read.
				//ferror = File::Read(fh, newVect3, sizeof(Vec3));
				//assert(ferror == File::Error::SUCCESS);
				//pTmp->poBone[j].T = Vec3(newVect3->x(), newVect3->y(), newVect3->z());

				// Rotation read.
				ferror = File::Read(fh, newVect4, sizeof(Vec4));
				assert(ferror == File::Error::SUCCESS);
				pTmp->poBone[j].Q = Quat(newVect4->x(), newVect4->y(), newVect4->z(), newVect4->w());
			
				//hard coded for now
				pTmp->poBone[j].S = Vec3(1.000000f, 1.000000f, 1.000000f);
			}

			// Bone: 11 
			pTmp->poBone[11].T = Vec3(0.003649f, 0.000000f, 0.002990f);
			pTmp->poBone[11].Q = Quat(0.707107f, 0.000000f, 0.707107f, -0.000000f);
			pTmp->poBone[11].S = Vec3(1.000000f, 1.000000f, 1.000000f);

		}

		delete[] pBone;

		ferror = File::Close(fh);
		assert(ferror == File::Error::SUCCESS);

		delete newVect3;
		delete newVect4;
	}
}

// --- End of File ---
