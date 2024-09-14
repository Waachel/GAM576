//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "AnimMan.h"
#include "DLinkMan.h"

namespace Azul
{

    AnimMan *AnimMan::posInstance = nullptr;

    //----------------------------------------------------------------------
    // Constructor
    //----------------------------------------------------------------------
    AnimMan::AnimMan(int reserveNum, int reserveGrow)
        : ManBase(new DLinkMan(), new DLinkMan(), reserveNum, reserveGrow)
    {
        // Preload the reserve
        this->proFillReservedPool(reserveNum);

        // initialize derived data here
        this->poNodeCompare = new Anim();
    }

    DLink* AnimMan::derivedCreateNode()
    {
        DLink* pNodeBaseClip = new Anim();
        assert(pNodeBase != nullptr);

        return pNodeBase;
    }

    AnimMan::~AnimMan()
    {
        delete this->poNodeCompare;
        this->poNodeCompare = nullptr;

        // iterate through the list and delete
        Iterator *pIt = this->baseGetActiveIterator();

        DLink *pNode = pIt->First();

        // Walk through the nodes
        while(!pIt->IsDone())
        {
            Anim *pDeleteMe = (Anim *)pIt->Curr();
            pNode = pIt->Next();
            delete pDeleteMe;
        }

        pIt = this->baseGetReserveIterator();

        pNode = pIt->First();

        // Walk through the nodes
        while(!pIt->IsDone())
        {
            Anim *pDeleteMe = (Anim *)pIt->Curr();
            pNode = pIt->Next();
            delete pDeleteMe;
        }
    }

    //----------------------------------------------------------------------
    // Static Methods
    //----------------------------------------------------------------------
    void AnimMan::Create(int reserveNum, int reserveGrow)
    {
        // make sure values are ressonable 
        assert(reserveNum >= 0);
        assert(reserveGrow > 0);

        // initialize the singleton here
        assert(posInstance == nullptr);

        // Do the initialization
        if(posInstance == nullptr)
        {
            posInstance = new AnimMan(reserveNum, reserveGrow);
        }

    }

    void AnimMan::Destroy()
    {
        AnimMan *pMan = AnimMan::privGetInstance();
        assert(pMan != nullptr);
        AZUL_UNUSED_VAR(pMan);

        delete AnimMan::posInstance;
        AnimMan::posInstance = nullptr;
    }

    Clip* AnimMan::AddClip(Clip::Type type, Clip* pClip, const char* const pClipFileName, int _numBones)
    {
        AnimMan* pMan = AnimMan::privGetInstance();

        Clip* pNode = (Clip*)pMan->baseAddToFront();
        assert(pNode != nullptr);

        // Initialize the date
        pNode->Set(pClip, pClipFileName, _numBones, type);

        assert(pNode);
        return pNode;
    }

    Clip* AnimMan::FindClip(Clip::Type clipType)
    {
        AnimMan* pMan = AnimMan::privGetInstance();
        assert(pMan != nullptr);

        // Compare functions only compares two Nodes

        // So:  Use the Compare Node - as a reference
        //      use in the Compare() function
        pMan->poClipCompare->_type = clipType;

        Clip* pData = (Clip*)pMan->baseFind(pMan->poClipCompare);
        return pData;
    }

    void AnimMan::RemoveClip(Clip* pNode)
    {
        assert(pNode != nullptr);

        AnimMan* pMan = AnimMan::privGetInstance();
        assert(pMan != nullptr);

        pMan->baseRemove(pNode);
    }

    AnimController* AnimMan::AddController(AnimController::Type controllerType, AnimController* pController, AnimTime delta)
    {
        AnimMan* pMan = AnimMan::privGetInstance();

        AnimController* pNode = (AnimController*)pMan->baseAddToFront();
        assert(pNode != nullptr);

        // Initialize the date
        pNode->Set(pController, delta, controllerType);

        assert(pNode);
        return pNode;
    }

    AnimController* AnimMan::FindController(AnimController::Type controllerType)
    {
        AnimMan* pMan = AnimMan::privGetInstance();
        assert(pMan != nullptr);

        // Compare functions only compares two Nodes

        // So:  Use the Compare Node - as a reference
        //      use in the Compare() function
        pMan->poControllerCompare->_type = controllerType;

        AnimController* pData = (AnimController*)pMan->baseFind(pMan->poControllerCompare);
        return pData;
    }

    void AnimMan::RemoveAnimController(AnimController* pNode)
    {
        assert(pNode != nullptr);

        AnimMan* pMan = AnimMan::privGetInstance();
        assert(pMan != nullptr);

        pMan->baseRemove(pNode);
    }



    void AnimMan::Dump()
    {
        AnimMan *pMan = AnimMan::privGetInstance();
        assert(pMan != nullptr);

        pMan->baseDump();
    }

    //----------------------------------------------------------------------
    // Private methods
    //----------------------------------------------------------------------
    AnimMan *AnimMan::privGetInstance()
    {
        // Safety - this forces users to call Create() first before using class
        assert(posInstance != nullptr);

        return posInstance;
    }

    //----------------------------------------------------------------------
    // Override Abstract methods
    //----------------------------------------------------------------------

}

// --- End of File ---
