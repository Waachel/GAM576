//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef Anim_NULL_H
#define Anim_NULL_H
	
#include "Anim.h"

namespace Azul
{
	class AnimNull : public Anim
	{
	public:
		AnimNull();
		AnimNull(const AnimNull &) = delete;
		AnimNull &operator=(const AnimNull &other) = delete;
		virtual ~AnimNull();


	private:

	};
}

#endif

// --- End of File ---
