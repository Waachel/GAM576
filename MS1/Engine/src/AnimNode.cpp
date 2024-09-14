//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "AnimNode.h"
#include "StringThis.h"

namespace Azul
{
    AnimNode::AnimNode()
        : DLink(),
        poAnim(nullptr)
    {
        this->privClear();
    }

    AnimNode::~AnimNode()
    {
        this->privClear();
    }

    void AnimNode::Set(Anim::Name name, Anim *pAnim)
    {
        assert(pAnim);
        this->poAnim = pAnim;

        this->poAnim->SetName(name);
    }

    Anim *AnimNode::GetAnim()
    {
        return this->poAnim;
    }

    void AnimNode::privClear()
    {
        if(this->poAnim)
        {
            delete this->poAnim;
        }
        this->poAnim = nullptr;
    }

    char *AnimNode::GetName()
    {
        char *pName = nullptr;
        if(this->poAnim)
        {
            pName = this->poAnim->GetName();
        }

        return pName;
    }

    void AnimNode::Wash()
    {
        this->privClear();
    }

    bool AnimNode::Compare(DLink *pTargetNode)
    {
        // This is used in ManBase.Find() 
        assert(pTargetNode);

        AnimNode *pDataB = (AnimNode *)pTargetNode;

        bool status = false;

        assert(pDataB->poAnim);
        assert(this->poAnim);

        if(this->poAnim->name == pDataB->poAnim->name)
        {
            status = true;
        }

        return status;
    }

    void AnimNode::Dump()
    {
        Trace::out("      AnimNode(%p)\n", this);

        // Data:
        if(this->poAnim)
        {
            Trace::out("      Anim(%p) \n", this->poAnim);
            Trace::out("      Name: %s \n", StringMe(this->poAnim->name));
        }
        else
        {
            Trace::out("      Name: %s \n", "null");
        }

        DLink::Dump();
    }
}

// --- End of File ---
