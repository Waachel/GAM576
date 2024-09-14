//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef Anim_NODE_H
#define Anim_NODE_H

#include "DLink.h"
#include "Anim.h"

namespace Azul
{
	class AnimNode : public DLink
	{
	public:
		AnimNode();
		AnimNode(const Anim &) = delete;
		AnimNode &operator = (const Anim &) = delete;
		virtual ~AnimNode();

		void Set(Anim::Name name, Anim *pAnim);
		Anim *GetAnim();

		virtual char *GetName() override;
		virtual void Wash() override;
		virtual bool Compare(DLink *pTargetNode) override;
		virtual void Dump() override;

	private:
		void privClear();

		//--------------------------------------------------
		// Data
		//--------------------------------------------------
		Anim *poAnim;
	};
}

#endif

// --- End of File ---
