//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "GraphicsObject_LightTexture.h"
#include "GraphicsObject_FlatTexture.h"
#include "GameObjectBasic.h"
#include "GameObjectMan.h"
#include "ShaderObjectNodeMan.h"
#include "GameObjectRigid.h"
#include "GraphicsObject_Null.h"
#include "Skeleton.h"
#include "MeshNodeMan.h"
#include "GameObjectAnimSkeleton.h"
#include "GameObjectAnimSkin.h"
#include "TexNodeMan.h"
#include "PCSTreeForwardIterator.h"

namespace Azul
{
	// move to proto buff
	Skeleton::Data skel[]
	{
		{ 4, -1, "Bone" },
		{ 5,  4, "Bone_L" },
		{ 6,  5, "Bone_L_001" },
		{ 7,  6, "Bone_L_002" },
		{ 8,  4, "Bone_R" },
		{ 9,  8, "Bone_R_001" },
		{10,  9, "Bone_R_002" },
		{11,  4, "Bone_001" }
	};

	Skeleton::Skeleton(Bone *pBone, int _numBones)
		: pFirstBone(nullptr),
		numBones(_numBones)
	{
		this->privSetAnimationHierarchy(pBone);
		assert(pFirstBone);
	}

	Skeleton::~Skeleton()
	{

	}

	GameObjectAnim *Skeleton::GetFirstBone()
	{
		assert(pFirstBone);
		return this->pFirstBone;
	}

	GameObjectAnim *Skeleton::privFindBoneByIndex(int index)
	{
		GameObjectAnim *pFound = nullptr;

		GameObjectAnim *pObj = this->GetFirstBone();

		// TODO - add test bed for an interator of a tree with only one node
		if(pObj->index == index)
		{
			pFound = pObj;
		}
		else
		{
			PCSTreeForwardIterator pIter(pObj);
			PCSNode *pNode = pIter.First();
			GameObjectAnim *pGameObj = nullptr;

			// walks the anim node does the pose for everything that
			while(pNode != nullptr)
			{
				assert(pNode);
				// Update the game object
				pGameObj = (GameObjectAnim *)pNode;
				if(pGameObj->index == index)
				{
					pFound = pGameObj;
					break;
				}
				pNode = pIter.Next();
			}
		}

		return pFound;
	}

	void Skeleton::privSetAnimationHierarchy(Bone *pBoneResult)
	{
		assert(pBoneResult);

		// Mesh
		Mesh *MeshArray[8];

		MeshArray[0] = MeshNodeMan::Find(Mesh::Name::Bone);
		MeshArray[1] = MeshNodeMan::Find(Mesh::Name::Bone_L);
		MeshArray[2] = MeshNodeMan::Find(Mesh::Name::Bone_L_001);
		MeshArray[3] = MeshNodeMan::Find(Mesh::Name::Bone_L_002);
		MeshArray[4] = MeshNodeMan::Find(Mesh::Name::Bone_R);
		MeshArray[5] = MeshNodeMan::Find(Mesh::Name::Bone_R_001);
		MeshArray[6] = MeshNodeMan::Find(Mesh::Name::Bone_R_002);
		MeshArray[7] = MeshNodeMan::Find(Mesh::Name::Bone_001);

		// Load Shader 
		ShaderObject *pShaderObject_textureLight = ShaderObjectNodeMan::Find(ShaderObject::Name::LightTexture);
		assert(pShaderObject_textureLight);

		// GraphicsObject for a specific instance
		GraphicsObject *pGraphicsObj;

		// Create GameObject
		Vec3 color(1, 1, 1);
		Vec3 pos(1, 1, 1);

		//---------------------------------------
		// ChickenBot
		//---------------------------------------

		// Add A Dummy pivot object
		pGraphicsObj = new GraphicsObject_Null(Mesh::Name::NULL_MESH, ShaderObject::Name::NullShader);
		GameObjectBasic *pPivot = new GameObjectBasic(pGraphicsObj);
		pPivot->SetName("Pivot");
		pPivot->DrawDisable();
		pPivot->delta_x = 0.003f;
		pPivot->SetScale(70,70,70);
		pPivot->SetTrans(-4, 0, 3);

		// Add Bones in Hierarchy
		GameObjectMan::Add(pPivot, GameObjectMan::GetRoot());
		GameObjectAnim *pGameObj;

		// Root animation is treated differently
		for(int i = 0; i < 1; i++)
		{
			pGraphicsObj = new GraphicsObject_LightTexture(MeshArray[i], 
														   ShaderObject::Name::LightTexture, 
														   TextureObject::Name::ChickenBot, 
														   color, 
														   pos);
			pGameObj = new GameObjectAnimSkin(pGraphicsObj, pBoneResult);
			pGameObj->SetIndex(skel[i].index);
			pGameObj->SetName(skel[i].name);

			GameObjectMan::Add(pGameObj, pPivot);
			this->pFirstBone = pGameObj;
		}

		for(int i = 1; i < this->numBones - 4; i++)
		{
			pGraphicsObj = new GraphicsObject_LightTexture(MeshArray[i], 
														   ShaderObject::Name::LightTexture, 
														   TextureObject::Name::ChickenBot, 
														   color, 
														   pos);
			pGameObj = new GameObjectAnimSkin(pGraphicsObj, pBoneResult);
			pGameObj->SetIndex(skel[i].index);
			pGameObj->SetName(skel[i].name);

			GameObjectAnim *pParent = this->privFindBoneByIndex(skel[i].parentIndex);

			GameObjectMan::Add(pGameObj, pParent);
		}
		}

}

// --- End of File ---
