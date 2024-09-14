//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "AnimDemoInput.h"
#include "AnimA.h"
#include "AnimB.h"
#include "TimerController.h"

namespace Azul
{
	void AnimDemoInput(Clip * c, AnimController * a, FontSprite * f)
	{
		//Play
		if ((GetKeyState('G') & 0x8000))
		{
			a->poTimerControl->SetDeltaTime(0.2f * AnimTime(AnimTime::Duration::FILM_24_FRAME));
		}

		//Pause
		if ((GetKeyState('P') & 0x8000))
		{
			a->poTimerControl->SetDeltaTime(0.0f * AnimTime(AnimTime::Duration::FILM_24_FRAME));
		}

		//Forward
		if ((GetKeyState('F') & 0x8000))
		{
			a->poTimerControl->SetForRev(0);
		}
		//Backward
		if ((GetKeyState('B') & 0x8000))
		{
			a->poTimerControl->SetForRev(1);
		}

		//Slow
		if ((GetKeyState('L') & 0x8000))
		{
			a->poTimerControl->SetDeltaTime(0.1f * AnimTime(AnimTime::Duration::FILM_24_FRAME));
		}

		//Normal
		if ((GetKeyState('N') & 0x8000))
		{
			a->poTimerControl->SetDeltaTime(0.2f * AnimTime(AnimTime::Duration::FILM_24_FRAME));
		}

		//Fast
		if ((GetKeyState('T') & 0x8000))
		{
			a->poTimerControl->SetDeltaTime(0.4f * AnimTime(AnimTime::Duration::FILM_24_FRAME));
		}

		//Looping
		
		//Switching Animations
		//Primary Animation
		if((GetKeyState('Q') & 0x8000))
		{
			//Skeleton A - Walk
			if (a->animType == 0)
			{
				//wants first animation of Skeleton A
				c->ChangeAnim(true, true, f);
			}

			//Skeleton B - Die
			else
			{
				//wants first animation of Skeleton B
				c->ChangeAnim(true, false, f);
			}
		}

		//Secondary Animation
		if ((GetKeyState('W') & 0x8000))
		{
			//Skeleton A - Run
			if (a->animType == 0)
			{
				//wants secondary animation of Skeleton A
				c->ChangeAnim(false, true, f);
			}

			//Skeleton B - Side Step
			else
			{
				//wants secondary animation of Skeleton A
				c->ChangeAnim(false, false, f);
			}
		}

	}

}

// --- End of File ---
