//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "GraphicsObject_LightTexture.h"
#include "GameObjectMan.h"
#include "MeshNodeMan.h"
#include "ShaderObjectNodeMan.h"
#include "SimpleGameObject.h"
#include "Anim.h"
#include "FrameBucket.h"
#include "TexNodeMan.h"


namespace Azul
{
	FrameBucket* pHead = nullptr;

	SimpleGameObject* p0 = nullptr;
	SimpleGameObject* p1 = nullptr;
	SimpleGameObject* p2 = nullptr;
	SimpleGameObject* p3 = nullptr;
	SimpleGameObject* p4 = nullptr;
	SimpleGameObject* p5 = nullptr;
	SimpleGameObject* p6 = nullptr;
	SimpleGameObject* p7 = nullptr;

	void SetAnimationHierarchy()
	{
		// Load the mesh
		Mesh* pPyramidMesh = MeshNodeMan::Find(Mesh::Name::PYRAMID);
		assert(pPyramidMesh);

		// Load Shader 
		ShaderObject* pShaderObject_textureLight = ShaderObjectNodeMan::Find(ShaderObject::Name::LightTexture);
		assert(pShaderObject_textureLight);

		// GraphicsObject for a specific instance
		GraphicsObject_LightTexture* pGraphics_LightTexture;

		// Create GameObject
		Vec3 color;
		Vec3 pos(1, 1, 1);

		// RED - bone 0
		//color.set(1.50f, 0.50f, 0.50f);
		//TextureObject *pTex = TexNodeMan::Find(TextureObject::Name::Duckweed);
		//pGraphics_LightTexture = new GraphicsObject_LightTexture(pPyramidMesh,
		//														 pShaderObject_textureLight,
		//														 pTex,
		//														 color,
		//														 pos);
		//p0 = new SimpleGameObject(pGraphics_LightTexture);
		//p0->SetName("Red");
		//p0->SetIndex(0);

		//GameObjectMan::Add(p0, GameObjectMan::GetRoot());

		// Green - bone 1
		//color.set(1.0f, 1.0f, 1.0f);
		//pGraphics_LightTexture = new GraphicsObject_LightTexture(pPyramidMesh,
		//														 pShaderObject_textureLight,
		//														 pTex,
		//														 color,
		//														 pos);
		//p1 = new SimpleGameObject(pGraphics_LightTexture);
		//p1->SetName("Green");
		//p1->SetIndex(1);

		// Add Bones in Hierarchy
		//GameObjectMan::Add(p1, p0);

		//PCSTree *pTree = GameObjectMan::GetPCSTree();
		//pTree->Print();


		//---------------------------------------
		// ChickenBot
		//---------------------------------------

			//0: RootNode
			//child : (1) Chickenbot
			//child : (2) chicken_rig
			//1 : Chickenbot
			//2 : chicken_rig
			//child : (3) root
			//3 : root
			//child : (4) Bone
			//child : (12) ik_root
			//4 : Bone
			//child : (5) Bone_L
			//child : (8) Bone_R
			//child : (11) Bone_001
			//5 : Bone_L
			//child : (6) Bone_L_001
			//6 : Bone_L_001
			//child : (7) Bone_L_002
			//7 : Bone_L_002
			//8 : Bone_R
			//child : (9) Bone_R_001
			//9 : Bone_R_001
			//child : (10) Bone_R_002
			//10 : Bone_R_002
			//11 : Bone_001
			//12 : ik_root
			//child : (13) ikTarget_R
			//child : (14) ikTarget_L
			//13 : ikTarget_R
			//14 : ikTarget_L

		TextureObject* pTex = TexNodeMan::Find(TextureObject::Name::Duckweed);

		// Yellow
		color.set(1.50f, 1.50f, 0.0f);
		pGraphics_LightTexture = new GraphicsObject_LightTexture(pPyramidMesh, pShaderObject_textureLight, pTex, color, pos);
		p0 = new SimpleGameObject(pGraphics_LightTexture);
		p0->SetName("Bone");
		p0->SetIndex(4);

		color.set(1.50f, 0.50f, 0.50f);
		pGraphics_LightTexture = new GraphicsObject_LightTexture(pPyramidMesh, pShaderObject_textureLight, pTex, color, pos);
		p1 = new SimpleGameObject(pGraphics_LightTexture);
		p1->SetName("Bone_R");
		p1->SetIndex(8);

		color.set(1.50f, 0.50f, 0.50f);
		pGraphics_LightTexture = new GraphicsObject_LightTexture(pPyramidMesh, pShaderObject_textureLight, pTex, color, pos);
		p2 = new SimpleGameObject(pGraphics_LightTexture);
		p2->SetName("Bone_R_001");
		p2->SetIndex(9);

		color.set(1.50f, 0.50f, 0.50f);
		pGraphics_LightTexture = new GraphicsObject_LightTexture(pPyramidMesh, pShaderObject_textureLight, pTex, color, pos);
		p3 = new SimpleGameObject(pGraphics_LightTexture);
		p3->SetName("Bone_R_002");
		p3->SetIndex(10);

		color.set(0.50f, 1.50f, 0.50f);
		pGraphics_LightTexture = new GraphicsObject_LightTexture(pPyramidMesh, pShaderObject_textureLight, pTex, color, pos);
		p4 = new SimpleGameObject(pGraphics_LightTexture);
		p4->SetName("Bone_L");
		p4->SetIndex(5);

		color.set(0.50f, 1.50f, 0.50f);
		pGraphics_LightTexture = new GraphicsObject_LightTexture(pPyramidMesh, pShaderObject_textureLight, pTex, color, pos);
		p5 = new SimpleGameObject(pGraphics_LightTexture);
		p5->SetName("Bone_L_001");
		p5->SetIndex(6);

		color.set(0.50f, 1.50f, 0.50f);
		pGraphics_LightTexture = new GraphicsObject_LightTexture(pPyramidMesh, pShaderObject_textureLight, pTex, color, pos);
		p6 = new SimpleGameObject(pGraphics_LightTexture);
		p6->SetName("Bone_L_002");
		p6->SetIndex(7);

		color.set(0.50f, 0.50f, 1.50f);
		pGraphics_LightTexture = new GraphicsObject_LightTexture(pPyramidMesh, pShaderObject_textureLight, pTex, color, pos);
		p7 = new SimpleGameObject(pGraphics_LightTexture);
		p7->SetName("Bone_01");
		p7->SetIndex(11);


		// Add Bones in Hierarchy
		GameObjectMan::Add(p0, GameObjectMan::GetRoot());
		GameObjectMan::Add(p1, p0);
		GameObjectMan::Add(p2, p1);
		GameObjectMan::Add(p3, p2);
		GameObjectMan::Add(p4, p0);
		GameObjectMan::Add(p5, p4);
		GameObjectMan::Add(p6, p5);
		GameObjectMan::Add(p7, p0);

		PCSTree* pTree = GameObjectMan::GetPCSTree();
		pTree->Print();

	}


	void SetAnimationPose(SimpleGameObject* node)
	{
		// safety
		assert(node);

		// Now get the world matrices
		SimpleGameObject* childNode = (SimpleGameObject*)node->GetChild();
		SimpleGameObject* parentNode = node;

		// Normal.. valid child
		if (parentNode != nullptr && childNode != nullptr)
		{
			// starting point
			Vec3 start(0.0f, 0.0f, 0.0f);

			//  At this point, Find the two bones initial positions in world space
			//       Now get the length and directions
			Vec3 ptA(Vec4(start, 1) * *parentNode->GetWorld());
			Vec3 ptB(Vec4(start, 1) * *childNode->GetWorld());

			// direction between the anchor points of the respective bones
			Vec3 dir = ptB - ptA;

			// length of the bone 0
			float mag = dir.len();

			Scale S(1, 1, mag);
			Quat Q(Orient::LocalToWorld, dir.getNorm(), Vec3(0.0f, 1.0f, 0.0f));
			Trans T(ptA);
			Mat4 BoneOrient = S * Q * T;
			parentNode->SetBoneOrientation(BoneOrient);
		}

		// deal with last node, when there isn't a terminal node
		if (parentNode != nullptr && childNode == nullptr)
		{
			// get the parent's parent  -> grandParent
			SimpleGameObject* grandParentNode = (SimpleGameObject*)parentNode->GetParent();
			assert(grandParentNode);
			Vec3 pTmp1(Vec4(0, 0, 0, 1) * *grandParentNode->GetWorld());
			Vec3 pTmp2(Vec4(0, 0, 0, 1) * *parentNode->GetWorld());
			Vec3 dir = pTmp2 - pTmp1;
			float mag = dir.len();

			Vec3 pTmp3(Vec4(0, 0, 0, 1) * *parentNode->GetWorld());
			Vec3 pTmp4(Vec4(1, 0, 0, 1) * *parentNode->GetWorld());
			dir = pTmp4 - pTmp3;

			Scale S(1.0f, 1.0f, mag);
			Rot R(Orient::LocalToWorld, dir.getNorm(), Vec3(0.0f, 1.0f, 0.0f));
			Trans T(pTmp2);

			Mat4 mTmp = S * R * T;
			parentNode->SetBoneOrientation(mTmp);
		}

	}




	void SetAnimationData()
	{
		// --------  Result Frame  ----------------------

		pHead = new FrameBucket();
		pHead->prevBucket = nullptr;
		pHead->nextBucket = nullptr;
		pHead->KeyTime = AnimTime(AnimTime::Duration::ZERO);
		pHead->poBone = new Bone[NUM_BONES];

		FrameBucket* pTmp;

		// --------  Frame 0  ----------------------------

		FrameBucket* pTmp0 = new FrameBucket();
		pTmp0->prevBucket = pHead;
		pTmp0->nextBucket = nullptr;
		pTmp0->KeyTime = 0 * AnimTime(AnimTime::Duration::FILM_24_FRAME);
		pTmp0->poBone = new Bone[NUM_BONES];
		pHead->nextBucket = pTmp0;

		pTmp = pTmp0;

		pTmp->poBone[0].T = Vec3(0.0f, 0.0f, 0.0f);         // Padding avoids remapping
		pTmp->poBone[0].Q = Quat(0.0f, 0.0f, 0.0f, 1.0f);   // Padding avoids remapping
		pTmp->poBone[0].S = Vec3(0.0f, 0.0f, 0.0f);         // Padding avoids remapping

		pTmp->poBone[1].T = Vec3(0.0f, 0.0f, 0.0f);         // Padding avoids remapping
		pTmp->poBone[1].Q = Quat(0.0f, 0.0f, 0.0f, 1.0f);	// Padding avoids remapping
		pTmp->poBone[1].S = Vec3(0.0f, 0.0f, 0.0f);			// Padding avoids remapping

		pTmp->poBone[2].T = Vec3(0.0f, 0.0f, 0.0f);         // Padding avoids remapping
		pTmp->poBone[2].Q = Quat(0.0f, 0.0f, 0.0f, 1.0f);	// Padding avoids remapping
		pTmp->poBone[2].S = Vec3(0.0f, 0.0f, 0.0f);			// Padding avoids remapping

		pTmp->poBone[3].T = Vec3(0.0f, 0.0f, 0.0f);         // Padding avoids remapping
		pTmp->poBone[3].Q = Quat(0.0f, 0.0f, 0.0f, 1.0f);	// Padding avoids remapping
		pTmp->poBone[3].S = Vec3(0.0f, 0.0f, 0.0f);			// Padding avoids remapping

		//Trans bone : 4 Frame : 0
		//	Vec3(0.026428f, 0.001437f, -0.000000f);

		//Quat bone : 4 Frame : 0
		//	Quat(0.000500f, -0.020833f, 0.007737f, 0.999753f);

		//Quat bone : 5 Frame : 0
		//	Quat(-0.273342f, 0.650441f, 0.641298f, 0.301576f);

		//Quat bone : 6 Frame : 0
		//	Quat(-0.480434f, -0.518829f, 0.518829f, 0.480434f);

		//Quat bone : 7 Frame : 0
		//	Quat(0.926996f, 0.010783f, -0.374829f, -0.008046f);

		//Quat bone : 8 Frame : 0
		//	Quat(0.142469f, 0.693114f, -0.668960f, 0.227570f);

		//Quat bone : 9 Frame : 0
		//	Quat(0.571355f, -0.416597f, -0.416598f, 0.571355f);

		//Quat bone : 10 Frame : 0
		//	Quat(0.939128f, -0.065612f, -0.336303f, 0.025158f);

		pTmp->poBone[4].T = Vec3(0.023792f, 0.000000f, -0.002410f);
		pTmp->poBone[4].Q = Quat(0.000000f, -0.000000f, 0.000000f, 1.000000f);
		pTmp->poBone[4].S = 100.0f * Vec3(1.000000f, 1.000000f, 1.000000f);  // Stationary

		pTmp->poBone[5].T = Vec3(0.000030f, -0.008568f, 0.000247f); // Stationary
		pTmp->poBone[5].Q = Quat(-0.247495f, 0.662379f, 0.662379f, 0.247496f);
		pTmp->poBone[5].S = Vec3(1.000000f, 1.000000f, 1.000000f);  // Stationary

		pTmp->poBone[6].T = Vec3(0.015902f, 0.000000f, -0.000000f); // Stationary
		pTmp->poBone[6].Q = Quat(-0.507033f, -0.492866f, 0.492867f, 0.507033f);
		pTmp->poBone[6].S = Vec3(1.000000f, 1.000000f, 1.000000f);  // Stationary

		pTmp->poBone[7].T = Vec3(0.014309f, 0.000000f, 0.000000f);  // Stationary
		pTmp->poBone[7].Q = Quat(0.915661f, 0.000000f, -0.401952f, 0.000000f);
		pTmp->poBone[7].S = Vec3(1.000000f, 1.000000f, 0.999999f);  // Stationary

		pTmp->poBone[8].T = Vec3(0.000030f, 0.008568f, 0.000247f);  // Stationary
		pTmp->poBone[8].Q = Quat(0.247495f, 0.662379f, -0.662379f, 0.247496f);
		pTmp->poBone[8].S = Vec3(1.000000f, 1.000000f, 1.000000f);  // Stationary

		pTmp->poBone[9].T = Vec3(0.015902f, 0.000000f, 0.000000f);  // Stationary
		pTmp->poBone[9].Q = Quat(0.507033f, -0.492866f, -0.492867f, 0.507033f);
		pTmp->poBone[9].S = Vec3(1.000000f, 1.000000f, 1.000000f);  // Stationary

		pTmp->poBone[10].T = Vec3(0.014309f, 0.000000f, -0.000000f); // Stationary
		pTmp->poBone[10].Q = Quat(0.915661f, 0.000000f, -0.401952f, 0.000000f);
		pTmp->poBone[10].S = Vec3(1.000000f, 1.000000f, 0.999999f);  // Stationary

		pTmp->poBone[11].T = Vec3(0.003649f, 0.000000f, 0.002990f);  // Stationary
		pTmp->poBone[11].Q = Quat(0.707107f, 0.000000f, 0.707107f, -0.000000f); // Stationary
		pTmp->poBone[11].S = Vec3(1.000000f, 1.000000f, 1.000000f);  // Stationary



		// --------  Frame 10  ----------------------------

		FrameBucket *pTmp2 = new FrameBucket();
		pTmp2->prevBucket = pTmp0;
		pTmp2->nextBucket = nullptr;
		pTmp2->KeyTime = 10 * AnimTime(AnimTime::Duration::FILM_24_FRAME);
		pTmp2->poBone = new Bone[NUM_BONES];
		pTmp0->nextBucket = pTmp2;

		pTmp = pTmp2;

		pTmp->poBone[0].T = Vec3(0.0f, 0.0f, 0.0f);         // Padding avoids remapping
		pTmp->poBone[0].Q = Quat(0.0f, 0.0f, 0.0f, 1.0f);   // Padding avoids remapping
		pTmp->poBone[0].S = Vec3(0.0f, 0.0f, 0.0f);         // Padding avoids remapping

		pTmp->poBone[1].T = Vec3(0.0f, 0.0f, 0.0f);         // Padding avoids remapping
		pTmp->poBone[1].Q = Quat(0.0f, 0.0f, 0.0f, 1.0f);	// Padding avoids remapping
		pTmp->poBone[1].S = Vec3(0.0f, 0.0f, 0.0f);			// Padding avoids remapping

		pTmp->poBone[2].T = Vec3(0.0f, 0.0f, 0.0f);         // Padding avoids remapping
		pTmp->poBone[2].Q = Quat(0.0f, 0.0f, 0.0f, 1.0f);	// Padding avoids remapping
		pTmp->poBone[2].S = Vec3(0.0f, 0.0f, 0.0f);			// Padding avoids remapping

		pTmp->poBone[3].T = Vec3(0.0f, 0.0f, 0.0f);         // Padding avoids remapping
		pTmp->poBone[3].Q = Quat(0.0f, 0.0f, 0.0f, 1.0f);	// Padding avoids remapping
		pTmp->poBone[3].S = Vec3(0.0f, 0.0f, 0.0f);			// Padding avoids remapping

		//Trans bone : 4 Frame : 10
		//	//Vec3(0.025098f, 0.000986f, -0.000000f);

		//Quat bone : 4 Frame : 10
		//	//Quat(0.000444f, 0.015755f, 0.022881f, 0.999614f);

		//Quat bone : 5 Frame : 10
		//	//Quat(0.008006f, 0.712285f, 0.700156f, 0.048655f);

		//Quat bone : 6 Frame : 10
		//	//Quat(-0.612932f, -0.352582f, 0.352583f, 0.612932f);

		//Quat bone : 7 Frame : 10
		//	//Quat(0.873157f, 0.018841f, -0.487073f, -0.001489f);

		//Quat bone : 8 Frame : 10
		//	//Quat(0.316669f, 0.624309f, -0.609920f, 0.371424f);

		//Quat bone : 9 Frame : 10
		////	Quat(0.580290f, -0.404060f, -0.404060f, 0.580289f);

		//Quat bone : 10 Frame : 10
		////	Quat(0.991437f, -0.060584f, -0.114703f, 0.015010f);

		pTmp->poBone[4].T = Vec3(0.023522f, 0.005539f, -0.006575f);
		pTmp->poBone[4].Q = Quat(-0.004145f, 0.038622f, 0.104662f, 0.993749f);
		pTmp->poBone[4].S = 100.0f * Vec3(1.000000f, 1.000000f, 1.000000f);  // Stationary

		pTmp->poBone[5].T = Vec3(0.000030f, -0.008568f, 0.000247f); // Stationary
		pTmp->poBone[5].Q = Quat(-0.173702f, 0.680534f, 0.639492f, 0.312653f);
		pTmp->poBone[5].S = Vec3(1.000000f, 1.000000f, 1.000000f);  // Stationary

		pTmp->poBone[6].T = Vec3(0.015902f, 0.000000f, -0.000000f); // Stationary
		pTmp->poBone[6].Q = Quat(-0.543178f, -0.452723f, 0.452723f, 0.543178f);
		pTmp->poBone[6].S = Vec3(1.000000f, 1.000000f, 1.000000f);  // Stationary

		pTmp->poBone[7].T = Vec3(0.014309f, 0.000000f, 0.000000f);  // Stationary
		pTmp->poBone[7].Q = Quat(0.928469f, -0.001671f, -0.371400f, 0.002055f);
		pTmp->poBone[7].S = Vec3(1.000000f, 1.000000f, 0.999999f);  // Stationary

		pTmp->poBone[8].T = Vec3(0.000030f, 0.008568f, 0.000247f);  // Stationary
		pTmp->poBone[8].Q = Quat(0.201934f, 0.684298f, -0.660492f, 0.233901f);
		pTmp->poBone[8].S = Vec3(1.000000f, 1.000000f, 1.000000f);  // Stationary

		pTmp->poBone[9].T = Vec3(0.015902f, 0.000000f, 0.000000f);  // Stationary
		pTmp->poBone[9].Q = Quat(-0.486447f, 0.513195f, 0.513196f, -0.486446f);
		pTmp->poBone[9].S = Vec3(1.000000f, 1.000000f, 1.000000f);  // Stationary

		pTmp->poBone[10].T = Vec3(0.014309f, 0.000000f, -0.000000f); // Stationary
		pTmp->poBone[10].Q = Quat(0.850957f, -0.117674f, -0.508655f, 0.057403f);
		pTmp->poBone[10].S = Vec3(1.000000f, 1.000000f, 0.999999f);  // Stationary

		pTmp->poBone[11].T = Vec3(0.003649f, 0.000000f, 0.002990f);  // Stationary
		pTmp->poBone[11].Q = Quat(0.707107f, 0.000000f, 0.707107f, -0.000000f); // Stationary
		pTmp->poBone[11].S = Vec3(1.000000f, 1.000000f, 1.000000f);  // Stationary


		// --------  Frame 20  ----------------------------

		FrameBucket *pTmp3 = new FrameBucket();
		pTmp3->prevBucket = pTmp2;
		pTmp3->nextBucket = nullptr;
		pTmp3->KeyTime = 20 * AnimTime(AnimTime::Duration::FILM_24_FRAME);
		pTmp3->poBone = new Bone[NUM_BONES];
		pTmp2->nextBucket = pTmp3;

		pTmp = pTmp3;

		pTmp->poBone[0].T = Vec3(0.0f, 0.0f, 0.0f);         // Padding avoids remapping
		pTmp->poBone[0].Q = Quat(0.0f, 0.0f, 0.0f, 1.0f);   // Padding avoids remapping
		pTmp->poBone[0].S = Vec3(0.0f, 0.0f, 0.0f);         // Padding avoids remapping

		pTmp->poBone[1].T = Vec3(0.0f, 0.0f, 0.0f);         // Padding avoids remapping
		pTmp->poBone[1].Q = Quat(0.0f, 0.0f, 0.0f, 1.0f);	// Padding avoids remapping
		pTmp->poBone[1].S = Vec3(0.0f, 0.0f, 0.0f);			// Padding avoids remapping

		pTmp->poBone[2].T = Vec3(0.0f, 0.0f, 0.0f);         // Padding avoids remapping
		pTmp->poBone[2].Q = Quat(0.0f, 0.0f, 0.0f, 1.0f);	// Padding avoids remapping
		pTmp->poBone[2].S = Vec3(0.0f, 0.0f, 0.0f);			// Padding avoids remapping

		pTmp->poBone[3].T = Vec3(0.0f, 0.0f, 0.0f);         // Padding avoids remapping
		pTmp->poBone[3].Q = Quat(0.0f, 0.0f, 0.0f, 1.0f);	// Padding avoids remapping
		pTmp->poBone[3].S = Vec3(0.0f, 0.0f, 0.0f);			// Padding avoids remapping



		//Trans bone : 4 Frame : 20
		//	Vec3(0.026878f, -0.001554f, -0.000000f);

		//Quat bone : 4 Frame : 20
		//	Quat(-0.000189f, -0.015880f, -0.010030f, 0.999824f);

		//Quat bone : 5 Frame : 20
		//	Quat(-0.156065f, 0.689287f, 0.665692f, 0.239543f);

		//Quat bone : 6 Frame : 20
		//	Quat(-0.581520f, -0.402286f, 0.402287f, 0.581520f);

		//Quat bone : 7 Frame : 20
		//	Quat(0.951353f, 0.067269f, -0.299730f, -0.023760f);

		//Quat bone : 8 Frame : 20
		//	Quat(0.251491f, 0.659718f, -0.651004f, 0.278780f);

		//Quat bone : 9 Frame : 20
		//	Quat(-0.491527f, 0.508331f, 0.508332f, -0.491527f);

		//Quat bone : 10 Frame : 20
		//	Quat(0.920266f, -0.008445f, -0.391156f, 0.005974f);


		pTmp->poBone[4].T = Vec3(0.022906f, -0.005516f, 0.001433f);
		pTmp->poBone[4].Q = Quat(-0.008317f, -0.228717f, -0.023362f, 0.973177f);
		pTmp->poBone[4].S = 100.0f * Vec3(1.000000f, 1.000000f, 1.000000f);  // Stationary

		pTmp->poBone[5].T = Vec3(0.000030f, -0.008568f, 0.000247f); // Stationary
		pTmp->poBone[5].Q = Quat(-0.037195f, 0.707493f, 0.635513f, 0.306911f);
		pTmp->poBone[5].S = Vec3(1.000000f, 1.000000f, 1.000000f);  // Stationary

		pTmp->poBone[6].T = Vec3(0.015902f, 0.000000f, -0.000000f); // Stationary
		pTmp->poBone[6].Q = Quat(-0.566214f, -0.423558f, 0.423559f, 0.566214f);
		pTmp->poBone[6].S = Vec3(1.000000f, 1.000000f, 1.000000f);  // Stationary

		pTmp->poBone[7].T = Vec3(0.014309f, 0.000000f, 0.000000f);  // Stationary
		pTmp->poBone[7].Q = Quat(0.963743f, 0.185763f, -0.153473f, -0.114616f);
		pTmp->poBone[7].S = Vec3(1.000000f, 1.000000f, 0.999999f);  // Stationary

		pTmp->poBone[8].T = Vec3(0.000030f, 0.008568f, 0.000247f);  // Stationary
		pTmp->poBone[8].Q = Quat(-0.215159f, -0.667758f, 0.708826f, -0.073284f);
		pTmp->poBone[8].S = Vec3(1.000000f, 1.000000f, 1.000000f);  // Stationary

		pTmp->poBone[9].T = Vec3(0.015902f, 0.000000f, 0.000000f);  // Stationary
		pTmp->poBone[9].Q = Quat(0.517383f, -0.481990f, -0.481991f, 0.517383f);
		pTmp->poBone[9].S = Vec3(1.000000f, 1.000000f, 1.000000f);  // Stationary

		pTmp->poBone[10].T = Vec3(0.014309f, 0.000000f, -0.000000f); // Stationary
		pTmp->poBone[10].Q = Quat(0.944251f, 0.104083f, -0.303900f, -0.072121f);
		pTmp->poBone[10].S = Vec3(1.000000f, 1.000000f, 0.999999f);  // Stationary

		pTmp->poBone[11].T = Vec3(0.003649f, 0.000000f, 0.002990f);  // Stationary
		pTmp->poBone[11].Q = Quat(0.707107f, 0.000000f, 0.707107f, -0.000000f); // Stationary
		pTmp->poBone[11].S = Vec3(1.000000f, 1.000000f, 1.000000f);  // Stationary

	}

}

//--- End of File ---
