//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Game.h"
#include "MathEngine.h"
#include "GameObjectAnimSkin.h"
#include "GraphicsObject.h"
#include "Mesh.h"
#include "AnimA.h"

namespace Azul
{

	GameObjectAnimSkin::GameObjectAnimSkin(GraphicsObject *pGraphicsObject, Bone *_pBoneResult)
		: GameObjectAnim(pGraphicsObject)
	{
		assert(pGraphicsObject);
		this->poTrans = new Vec3(0.0f, 0.0f, 0.0f);
		this->poScale = new Vec3(1.0f, 1.0f, 1.0f);
		this->poQuat = new Quat(0.0f, 0.0f, 0.0f, 1.0f);
		this->poLocal = new Mat4(Special::Identity);
	//	this->index = -1;
		this->poBoneOrientation = new Mat4(Special::Identity);
		this->pBoneResult = _pBoneResult;
		assert(_pBoneResult);
		assert(poBoneOrientation);
		assert(this->poTrans);
		assert(this->poScale);
		assert(this->poQuat);
		assert(this->poLocal);
	}

	GameObjectAnimSkin::~GameObjectAnimSkin()
	{
		delete this->poTrans;
		delete this->poScale;
		delete this->poQuat;
		delete this->poLocal;
		delete this->poBoneOrientation;
	}

	void GameObjectAnimSkin::privUpdate(AnimTime currentTime)
	{
		AZUL_UNUSED_VAR(currentTime);

		//Trace::out("index: %d \n", index);

		Trans T;
		Scale S;
		Quat  Q;

		// Get the parent matrix... if root, set to identity
		GameObjectAnimSkin *pParentObject = (GameObjectAnimSkin *)this->GetParent();
		Mat4 matParent(Special::Identity);
		if(pParentObject)
		{
			matParent = *pParentObject->poWorld;

			// REMEMBER this is for Animation and hierachy, you need to handle models differently
			// Get the result bone array, from there make the matrix
			T.set(this->pBoneResult[index].T);
			S.set(this->pBoneResult[index].S);
			Q = this->pBoneResult[index].Q;
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

	void GameObjectAnimSkin::Update(AnimTime currentTime)
	{
		// Goal: update the world matrix
		this->privUpdate(currentTime);

		// update the bounding volume based on world matrix
		this->poGraphicsObject->SetWorld(*this->poWorld);
	}

	void GameObjectAnimSkin::SetQuat(float qx, float qy, float qz, float qw)
	{
		this->poQuat->set(qx, qy, qz, qw);
	}

	void GameObjectAnimSkin::SetScale(float sx, float sy, float sz)
	{
		this->poScale->set(sx, sy, sz);
	}

	void GameObjectAnimSkin::SetTrans(float x, float y, float z)
	{
		this->poTrans->set(x, y, z);
	}

	void GameObjectAnimSkin::SetIndex(int i)
	{
		this->index = i;
	}

	void GameObjectAnimSkin::SetScale(Vec3 &r)
	{
		*this->poScale = r;
	}

	void GameObjectAnimSkin::SetQuat(Quat &r)
	{
		*this->poQuat = r;
	}

	void GameObjectAnimSkin::SetTrans(Vec3 &r)
	{
		*this->poTrans = r;
	}

	void GameObjectAnimSkin::SetBoneOrientation(const Mat4 &tmp)
	{
		*this->poBoneOrientation = tmp;
	}

	Mat4 GameObjectAnimSkin::GetBoneOrientation(void) const
	{
		// return a copy...
		return Mat4(*this->poBoneOrientation);
	}
}

// --- End of File ---
