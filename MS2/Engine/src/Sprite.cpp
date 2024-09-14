//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Game.h"
#include "MathEngine.h"
#include "Sprite.h"
#include "GraphicsObject_Sprite.h"
#include "GraphicsObject.h"
#include "Mesh.h"
#include "Anim.h"

namespace Azul
{
	Sprite::Sprite(GraphicsObject *pGraphicsObject)
		: GameObjectRigid(pGraphicsObject),
		scaleX(1.0f),
		scaleY(1.0f),
		posX(0.0f),
		posY(0.0f),
		angle(0.0f)
	{
		assert(pGraphicsObject);
		
		this->pGraphicsObjectSprite = (GraphicsObject_Sprite *)poGraphicsObject;

		this->angle = 0.0f;
		this->posX = pGraphicsObjectSprite->origPosX;
		this->posY = pGraphicsObjectSprite->origPosY;
		this->scaleX = 1.0f;
		this->scaleY = 1.0f;

		this->delta_x = 0.0f;
		this->delta_y = 0.0f;
		this->delta_z = 0.0f;

		this->cur_rot_x = 0.0f;
		this->cur_rot_y = 0.0f;
		this->cur_rot_z = 0.0f;

	}

	Sprite::~Sprite()
	{
		
	}

	void Sprite::SetImage(Image::Name imageName)
	{
		pGraphicsObjectSprite->SetImage(imageName);
	}

	void Sprite::privUpdate(AnimTime currentTime)
	{
		AZUL_UNUSED_VAR(currentTime);

		cur_rot_x += this->delta_x;
		cur_rot_y += this->delta_y;
		cur_rot_z += this->delta_z;

		// Goal: update the world matrix
		Scale S(this->scaleX, this->scaleY, 1.0f);
		Rot Rx(Rot1::X, cur_rot_x);
		Rot Ry(Rot1::Y, cur_rot_y);
		Rot Rz(Rot1::Z, cur_rot_z);
		Trans T(this->posX, this->posY, 0.0f);

		*this->poWorld = S * Rx * Ry * Rz * T;
	}

	void Sprite::Update(AnimTime currentTime)
	{
		// Goal: update the world matrix
		this->privUpdate(currentTime);

		// update the bounding volume based on world matrix
		this->poGraphicsObject->SetWorld(*this->poWorld);
	}


}

// --- End of File ---
