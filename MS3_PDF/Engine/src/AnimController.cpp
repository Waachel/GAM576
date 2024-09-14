//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "AnimController.h"
#include "TimerController.h"
#include "Anim.h"
#include <FontSprite.h>

namespace Azul
{
	AnimController::AnimController(Anim *pAnim, AnimTime delta)
	{
		// Animation object
		this->poAnim = pAnim;
		assert(this->poAnim);

		// TimerController
		AnimTime maxTime = poAnim->FindMaxTime();
		AnimTime minTime = AnimTime(AnimTime::Duration::ZERO);
		this->poTimerControl = new TimerController(delta, minTime, maxTime);
		assert(this->poTimerControl);

	}

	void AnimController::ChangeAnim(bool wantsFirstAnim, FontSprite* f)
	{

		//is Walking but wants die left
		if (this->poAnim->pClip->mName == Clip::Name::Walk_ChickenBot && !wantsFirstAnim)
		{
			this->poAnim->ChangeClip(Clip::Name::DieLeft_ChickenBot);

			f->ChangeMessage("Die", 450, 290);
		}

		//is Running but wants side step
		else if (this->poAnim->pClip->mName == Clip::Name::Dance_Mousey && !wantsFirstAnim)
		{
			this->poAnim->ChangeClip(Clip::Name::ZombieWalk_Mousey);

			f->ChangeMessage("Zombie Walk", 180, 290);
		}

		//is Die left but Wants walking
		else if (this->poAnim->pClip->mName == Clip::Name::DieLeft_ChickenBot && wantsFirstAnim)
		{
			this->poAnim->ChangeClip(Clip::Name::Walk_ChickenBot);

			f->ChangeMessage("Walk", 425, 290);
		}

		//is Side step but Wants running
		else if (this->poAnim->pClip->mName == Clip::Name::ZombieWalk_Mousey && wantsFirstAnim)
		{
			this->poAnim->ChangeClip(Clip::Name::Dance_Mousey);

			f->ChangeMessage("Dance", 260, 290);
		}
	}

	AnimController::~AnimController()
	{
		delete poAnim;
		delete poTimerControl;
	}

	void AnimController::Update()
	{
		this->poTimerControl->Update();

		// interpolate all bones
		this->poAnim->Animate(this->poTimerControl->GetCurrTime());

	}
}

// --- End of File ---
