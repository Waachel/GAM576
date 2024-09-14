//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "AnimNodeMan.h"
#include "AnimNull.h"
#include "ManBase.h"
#include "DLinkMan.h"

namespace Azul
{
	AnimNodeMan *AnimNodeMan::posInstance = nullptr;

	//----------------------------------------------------------------------
	// Constructor
	//----------------------------------------------------------------------
	AnimNodeMan::AnimNodeMan(int reserveNum, int reserveGrow)
		: ManBase(new DLinkMan(), new DLinkMan(), reserveNum, reserveGrow)
	{
		// Preload the reserve
		this->proFillReservedPool(reserveNum);

		// initialize derived data here
		Anim *pAnim = new AnimNull();
		assert(pAnim);
		this->poNodeCompare = new AnimNode();
		assert(this->poNodeCompare);
		this->poNodeCompare->Set(Anim::Name::NULL_ANIM, pAnim);
	}

	AnimNodeMan::~AnimNodeMan()
	{
		delete this->poNodeCompare;
		this->poNodeCompare = nullptr;

		// iterate through the list and delete
		Iterator *pIt = this->baseGetActiveIterator();

		DLink *pNode = pIt->First();

		// Walk through the nodes
		while(!pIt->IsDone())
		{
			AnimNode *pDeleteMe = (AnimNode *)pIt->Curr();
			pNode = pIt->Next();
			delete pDeleteMe;
		}

		pIt = this->baseGetReserveIterator();

		pNode = pIt->First();

		// Walk through the nodes
		while(!pIt->IsDone())
		{
			AnimNode *pDeleteMe = (AnimNode *)pIt->Curr();
			pNode = pIt->Next();
			delete pDeleteMe;
		}
	}

	//----------------------------------------------------------------------
	// Static Methods
	//----------------------------------------------------------------------
	void AnimNodeMan::Create(int reserveNum, int reserveGrow)
	{
		// make sure values are ressonable 
		assert(reserveNum >= 0);
		assert(reserveGrow > 0);

		// initialize the singleton here
		assert(posInstance == nullptr);

		// Do the initialization
		if(posInstance == nullptr)
		{
			posInstance = new AnimNodeMan(reserveNum, reserveGrow);
		}
	}

	void AnimNodeMan::Destroy()
	{
		AnimNodeMan *pMan = AnimNodeMan::privGetInstance();
		assert(pMan != nullptr);
		AZUL_UNUSED_VAR(pMan);

		delete AnimNodeMan::posInstance;
		AnimNodeMan::posInstance = nullptr;
	}

	AnimNode *AnimNodeMan::Add(Anim::Name name, AnimController *pAnim)
	{
		AnimNodeMan *pMan = AnimNodeMan::privGetInstance();

		assert(pAnim);

		AnimNode *pNode = (AnimNode *)pMan->baseAddToFront();
		assert(pNode != nullptr);

		// Initialize the date
		pNode->Set(name, (Anim*)pAnim);

		return pNode;
	}

Anim *AnimNodeMan::Find(Anim::Name _name)
	{
		AnimNodeMan *pMan = AnimNodeMan::privGetInstance();
		assert(pMan != nullptr);

		// Compare functions only compares two Nodes

		// So:  Use the Compare Node - as a reference
		//      use in the Compare() function
		Anim *pAnim = pMan->poNodeCompare->GetAnim();
		assert(pAnim);

		pAnim->name = _name;

		AnimNode *pData = (AnimNode *)pMan->baseFind(pMan->poNodeCompare);

    if(pData)
    {
        pAnim = pData->GetAnim();
    }
    else
    {
        pAnim = nullptr;
    }

    return pAnim;
	}

	void AnimNodeMan::Remove(AnimNode *pNode)
	{
		assert(pNode != nullptr);

		AnimNodeMan *pMan = AnimNodeMan::privGetInstance();
		assert(pMan != nullptr);

		pMan->baseRemove(pNode);
	}

	void AnimNodeMan::Dump()
	{
		AnimNodeMan *pMan = AnimNodeMan::privGetInstance();
		assert(pMan != nullptr);

		pMan->baseDump();
	}

	AnimNode* AnimNodeMan::AddClip(Anim::Name name, Clip* pClip)
	{
		AnimNodeMan* pMan = AnimNodeMan::privGetInstance();

		assert(pClip);

		AnimNode* pNode = (AnimNode*)pMan->baseAddToFront();
		assert(pNode != nullptr);

		// Initialize the date
		pNode->Set(name, (Anim*) pClip);

		return pNode;
	}

	//----------------------------------------------------------------------
	// Private methods
	//----------------------------------------------------------------------
	AnimNodeMan *AnimNodeMan::privGetInstance()
	{
		// Safety - this forces users to call Create() first before using class
		assert(posInstance != nullptr);

		return posInstance;
	}

	//----------------------------------------------------------------------
	// Override Abstract methods
	//----------------------------------------------------------------------
	DLink *AnimNodeMan::derivedCreateNode()
	{
		DLink *pNodeBase = new AnimNode();
		assert(pNodeBase != nullptr);

		return pNodeBase;
	}
}

// --- End of File ---




