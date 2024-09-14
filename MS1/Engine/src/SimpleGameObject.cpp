//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Game.h"
#include "MathEngine.h"
#include "SimpleGameObject.h"
#include "GameObject.h"
#include "GraphicsObject.h"
#include "Mesh.h"
#include "FrameBucket.h"
#include "Anim.h"

namespace Azul
{
	extern FrameBucket *pHead;

	SimpleGameObject::SimpleGameObject(GraphicsObject *pGraphicsObject)
		: GameObject(pGraphicsObject)
	{
		assert(pGraphicsObject);
		this->poTrans = new Vec3(0.0f, 0.0f, 0.0f);
		this->poScale = new Vec3(1.0f, 1.0f, 1.0f);
		this->poQuat = new Quat(0.0f, 0.0f, 0.0f, 1.0f);
		this->poLocal = new Mat4(Special::Identity);
		this->index = -1;
		this->poBoneOrientation = new Mat4(Special::Identity);
		assert(poBoneOrientation);
		assert(this->poTrans);
		assert(this->poScale);
		assert(this->poQuat);
		assert(this->poLocal);
	}

	SimpleGameObject::~SimpleGameObject()
	{
		delete this->poTrans;
		delete this->poScale;
		delete this->poQuat;
		delete this->poLocal;
		delete this->poBoneOrientation;
	}

	void SimpleGameObject::privUpdate(AnimTime currentTime)
	{
		AZUL_UNUSED_VAR(currentTime);

		//Trace::out("index: %d \n", index);

		Trans T;
		Scale S;
		Quat  Q;

		// Get the parent matrix... if root, set to identity
		SimpleGameObject *pParentObject = (SimpleGameObject *)this->GetParent();
		Mat4 matParent(Special::Identity);
		if(pParentObject)
		{
			matParent = *pParentObject->poWorld;

			// REMEMBER this is for Animation and hierachy, you need to handle models differently
			// Get the result bone array, from there make the matrix
			Bone *bResult = pHead->poBone;

			T.set(bResult[index].T);
			S.set(bResult[index].S);
			Q = bResult[index].Q;
		}
		else
		{
			T.set(0, 0, 0);
			S.set(1, 1, 1);
			Q.set(Special::Identity);
		}

		// world matrix
		*this->poLocal = S * Q * T;
		*this->poWorld = *poLocal * matParent;

	}

	void SimpleGameObject::Update(AnimTime currentTime)
	{
		// Goal: update the world matrix
		this->privUpdate(currentTime);

		SetAnimationPose(this);

		// push to graphics object
		//Mat4 mTmp = *this->poBoneOrientation;

		// push to graphics object
		//this->poGraphicsObject->SetWorld(mTmp);

		this->poGraphicsObject->SetWorld(*this->poWorld);
	}

	void SimpleGameObject::SetQuat(float qx, float qy, float qz, float qw)
	{
		this->poQuat->set(qx, qy, qz, qw);
	}

	void SimpleGameObject::SetScale(float sx, float sy, float sz)
	{
		this->poScale->set(sx, sy, sz);
	}

	void SimpleGameObject::SetTrans(float x, float y, float z)
	{
		this->poTrans->set(x, y, z);
	}

	void SimpleGameObject::SetIndex(int i)
	{
		this->index = i;
	}

	void SimpleGameObject::SetScale(Vec3 &r)
	{
		*this->poScale = r;
	}

	void SimpleGameObject::SetQuat(Quat &r)
	{
		*this->poQuat = r;
	}

	void SimpleGameObject::SetTrans(Vec3 &r)
	{
		*this->poTrans = r;
	}

	void SimpleGameObject::SetBoneOrientation(const Mat4 &tmp)
	{
		*this->poBoneOrientation = tmp;
	}

	Mat4 SimpleGameObject::GetBoneOrientation(void) const
	{
		// return a copy...
		return Mat4(*this->poBoneOrientation);
	}
}

// --- End of File ---
