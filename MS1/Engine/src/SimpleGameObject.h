//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SIMPLE_GAME_OBJECT_H
#define SIMPLE_GAME_OBJECT_H

#include "GameObject.h"
#include "MathEngine.h"
#include "ShaderObject.h"
#include "Mesh.h"
#include "GraphicsObject.h"
#include "PCSNode.h"
#include "AnimTime.h"

namespace Azul
{
	class SimpleGameObject : public GameObject
	{
	public:
		SimpleGameObject(GraphicsObject *graphicsObject);

		// Big four
		SimpleGameObject() = delete;
		SimpleGameObject(const SimpleGameObject &) = delete;
		SimpleGameObject &operator=(SimpleGameObject &) = delete;
		virtual ~SimpleGameObject();

		virtual void Update(AnimTime currTime);

		void SetScale(float sx, float sy, float sz);
		void SetQuat(float qx, float qy, float qz, float qw);
		void SetTrans(float x, float y, float z);

		void SetScale(Vec3 &r);
		void SetQuat(Quat &r);
		void SetTrans(Vec3 &r);

		void SetIndex(int i);

		Mat4 GetBoneOrientation(void) const;
		void SetBoneOrientation(const Mat4 &);

	private:
		void privUpdate(AnimTime currTime);

	public:   // add accessors later
		Vec3 *poScale;
		Quat *poQuat;
		Vec3 *poTrans;
		Mat4 *poLocal;
		int index;

		Mat4 *poBoneOrientation;
	};

}

#endif

// --- End of File ---
