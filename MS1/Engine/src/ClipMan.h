//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ANIM_MANAGER_H
#define ANIM_MANAGER_H

#include "ManBase.h"
#include "Anim.h"
#include "AnimController.h"

namespace Azul
{
    class AnimMan : public ManBase
    {
        //----------------------------------------------------------------------
        // Static Methods
        //----------------------------------------------------------------------
    public:
        static void Create(int reserveNum = 0, int reserveGrow = 1);
        static void Destroy();
        static Clip *AddClip(Clip::Type type, Clip *pClip, const char* const pClipFileName, int _numBones);
        static Clip *FindClip(Clip::Type clipName);
        static void RemoveClip(Clip* pNode);


        static AnimController* AddController(AnimController::Type controllerType, AnimController* pController, AnimTime delta);
        static AnimController* FindController(AnimController::Type controllerType);
        static void RemoveAnimController(AnimController* pNode);

        static void Dump();

        //----------------------------------------------------------------------
        // Private methods
        //----------------------------------------------------------------------
    private:
        static AnimMan *privGetInstance();

        AnimMan() = delete;
        AnimMan(const AnimMan &) = delete;
        AnimMan &operator=(const AnimMan &) = delete;
        virtual ~AnimMan();

        AnimMan(int reserveNum, int reserveGrow);

        //----------------------------------------------------------------------
        // Override Abstract methods
        //----------------------------------------------------------------------
    protected:
        DLink *derivedCreateNode() override;

        //----------------------------------------------------------------------
        // Data: unique data for this manager 
        //----------------------------------------------------------------------
    private:
        AnimController *poControllerCompare;
        static AnimMan *posInstance;

        Clip* poClipCompare;
        Anim* poNodeCompare;

    };
}

#endif

//--- End of File ---
