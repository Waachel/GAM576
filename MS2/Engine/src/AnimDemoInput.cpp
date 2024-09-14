//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "AnimDemoInput.h"
#include "Anim.h"
#include "Anim.h"
#include "TimerController.h"
#include "ClipMan.h"

namespace Azul
{
	void AnimDemoInput(AnimController * a, FontSprite * f)
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
		if ((GetKeyState('Q') & 0x8000))
		{
			//Skeleton A - Walk
			if (a->poAnim->pClip->mName == Clip::Name::Walk_ChickenBot)
			{
				//wants first animation of Skeleton A - Walk
				a->ChangeAnim(true, f);
			}

			//Skeleton B - Die
			else if (a->poAnim->pClip->mName == Clip::Name::DieLeft_ChickenBot)
			{
				//wants first animation of Skeleton B
				a->ChangeAnim(true, f);
			}

			else if (a->poAnim->pClip->mName == Clip::Name::Dance_Mousey)
			{
				a->ChangeAnim(true, f);
			}

			else if (a->poAnim->pClip->mName == Clip::Name::ZombieWalk_Mousey)
			{
				a->ChangeAnim(true, f);
			}
		}

		//Secondary Animation
		if ((GetKeyState('W') & 0x8000))
		{
			//Skeleton A - Walk
			if (a->poAnim->pClip->mName == Clip::Name::Walk_ChickenBot)
			{
				//wants first animation of Skeleton A - Walk
				a->ChangeAnim(false, f);
			}

			//Skeleton B - Die
			else if (a->poAnim->pClip->mName == Clip::Name::DieLeft_ChickenBot)
			{
				//wants first animation of Skeleton B
				a->ChangeAnim(false, f);
			}

			else if (a->poAnim->pClip->mName == Clip::Name::Dance_Mousey)
			{
				a->ChangeAnim(false, f);
			}

			else if (a->poAnim->pClip->mName == Clip::Name::ZombieWalk_Mousey)
			{
				a->ChangeAnim(false, f);
			}
		}

	}

}

// --- End of File ---
