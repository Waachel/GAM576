//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ANIM_CONTROLLER_H
#define ANIM_CONTROLLER_H

#include "TimerController.h"
#include "GameObjectAnim.h"
#include "AnimA.h"
#include "AnimB.h"

namespace Azul
{
	class AnimController
	{
	public:

		enum class Type
		{
			MAIN,
			WALK,
			RUN,
			DIE_LEFT,
			SIDE_STEP_RIGHT,
			NOT_INITIALIZED
		};

		AnimController(AnimTime delta, AnimController::Type type);
		AnimController(const AnimController &) = delete;
		AnimController &operator = (const AnimController &) = delete;
		~AnimController();

		void Update();
		TimerController* poTimerControl;

	private:
		
		AnimA *poAnimA;

		AnimB* poAnimB;
	public:
		int animType = 0;
		bool isSkeletonA = true;
	};
}

#endif

// --- End of File ---
