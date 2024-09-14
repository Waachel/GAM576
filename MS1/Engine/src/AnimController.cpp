//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "AnimController.h"
#include "Clip.h"
namespace Azul
{
	AnimController::AnimController(AnimTime delta, AnimController::Type type)
	{

		// Animation object
		if (type == AnimController::Type::WALK)
		{
			this->poAnimA = new AnimA();
			assert(this->poAnimA);

			// TimerController
			AnimTime maxTime = poAnimA->FindMaxTime();
			AnimTime minTime = AnimTime(AnimTime::Duration::ZERO);
			this->poTimerControl = new TimerController(delta, minTime, maxTime);
			assert(this->poTimerControl);
			this->animType = 0;
		}

		// Animation object
		if (type == AnimController::Type::SIDE_STEP_RIGHT)
		{
			this->poAnimB = new AnimB();
			assert(this->poAnimB);

			// TimerController
			AnimTime maxTime = poAnimB->FindMaxTime();
			AnimTime minTime = AnimTime(AnimTime::Duration::ZERO);
			this->poTimerControl = new TimerController(delta, minTime, maxTime);
			assert(this->poTimerControl);
			this->animType = 1;
		}



	}

	AnimController::~AnimController()
	{
		if (this->animType == 0)
		{
			delete poAnimA;
		}
		if (this->animType == 1)
		{
			delete poAnimB;
		}
		delete poTimerControl;
		this->animType = 0;
	}

	void AnimController::Update()
	{
		this->poTimerControl->Update();

		if (this->animType == 0)
		{
			this->poAnimA->Animate(this->poTimerControl->GetCurrTime());
		}
		if (this->animType == 1)
		{
			this->poAnimB->Animate(this->poTimerControl->GetCurrTime());
		}		
		
	}
}

// --- End of File ---
