//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef Anim_MAN_H
#define Anim_MAN_H 

#include "ManBase.h"
#include "AnimNode.h"
#include "Anim.h"
#include "AnimController.h"

namespace Azul
{
	class AnimNodeMan : public ManBase
	{
		//----------------------------------------------------------------------
		// Static Methods
		//----------------------------------------------------------------------
	public:
		static void Create(int reserveNum = 0, int reserveGrow = 1);
		static void Destroy();

		static AnimNode *Add(Anim::Name name, AnimController* pAnim);
	    static Anim *Find(Anim::Name name);
		static void Remove(AnimNode *pNode);
		static void Dump();

		static AnimNode* AddClip(Anim::Name name, Clip* pClip);

		//----------------------------------------------------------------------
		// Private methods
		//----------------------------------------------------------------------
	private:
		static AnimNodeMan *privGetInstance();

		AnimNodeMan() = delete;
		AnimNodeMan(const AnimNodeMan &) = delete;
		AnimNodeMan &operator = (const AnimNodeMan &) = delete;

		~AnimNodeMan();

		AnimNodeMan(int reserveNum, int reserveGrow);

		//----------------------------------------------------------------------
		// Override Abstract methods
		//----------------------------------------------------------------------
	protected:
		DLink *derivedCreateNode() override;

		//----------------------------------------------------------------------
		// Data: unique data for this manager 
		//----------------------------------------------------------------------
	private:
		AnimNode *poNodeCompare;
		static AnimNodeMan *posInstance;

	};
}

#endif

// --- End of File ---
