//--------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include <d3d11.h>
#include <d3dcompiler.h>
#include <WinUser.h>
#include "Game.h"
#include "Engine.h"
#include "MathEngine.h"
#include "Camera.h"
#include "Colors.h"
#include "GameObject.h"
#include "ShaderObject.h"
#include "ShaderObject_ColorByVertex.h"
#include "ShaderObject_FlatTexture.h"
#include "ShaderObject_LightTexture.h"
#include "ShaderObject_ConstColor.h"
#include "ShaderObject_Sprite.h"
#include "Mesh.h"
#include "MeshProto.h"
#include "GraphicsObject_ColorByVertex.h"
#include "GraphicsObject_Wireframe.h"
#include "GraphicsObject_FlatTexture.h"
#include "GraphicsObject_LightTexture.h"
#include "GraphicsObject_ConstColor.h"
#include "GraphicsObject_Sprite.h"
#include "GameObjectMan.h"
#include "MeshNodeMan.h"
#include "ShaderObjectNodeMan.h"
#include "TextureObject.h"
#include "TextureProto.h"
#include "TexNodeMan.h"
#include "DirectXDeviceMan.h"
#include "CameraMan.h"
#include "CameraDemoInput.h"
#include "CameraMesh.h"
#include "AnimTimer.h"
#include "MathApp.h"
#include "AnimA.h"
#include "AnimB.h"
#include "GameObjectBasic.h"
#include "TimerController.h"
#include "AnimController.h"
#include "Clip.h"
#include "Image.h"
#include "ImageMan.h"
#include "Sprite.h"
#include "GlyphMan.h"

#include "FontSprite.h"
#include <AnimDemoInput.h>

using namespace Azul;

namespace Azul
{
	// Todo: clean up
	AnimController *pAnimControllerA;
	AnimController* pAnimControllerB;
	Clip *pClipA = nullptr;
	Clip* pClipB = nullptr;
	Sprite *pSprite(nullptr);
	FontSprite* pFontSpriteA = nullptr;
	FontSprite* pFontSpriteB = nullptr;
	FontSprite* pFontSpriteC = nullptr;

	//-----------------------------------------------------------------------------
	//  Game::Game()
	//		Game Engine Constructor
	//-----------------------------------------------------------------------------

	Game::Game(const char *const pName, int width, int height)
		: Engine(pName, width, height)
	{
		this->globalTimer.Tic();
		this->intervalTimer.Tic();
	}

	//-----------------------------------------------------------------------------
	//  Game::~Game()
	//		Game Engine destructor
	//-----------------------------------------------------------------------------
	Game::~Game()
	{
	}

	//-----------------------------------------------------------------------------
	// Game::LoadContent()
	//		Allows you to load all content needed for your engine,
	//	    such as objects, graphics, etc.
	//-----------------------------------------------------------------------------
	bool Game::LoadContent()
	{
		ShaderObjectNodeMan::Create();
		MeshNodeMan::Create();
		GameObjectMan::Create();
		TexNodeMan::Create();
		CameraMan::Create();
		ImageMan::Create();
		GlyphMan::Create();

		// --------------------------
		// Camera setup
		// --------------------------

		// Camera_0		

		//---------------------------------------------------------------------------------------------------------
		// Setup the current 3D perspective Camera
		//---------------------------------------------------------------------------------------------------------
		Camera *pCam0 = new Camera(Camera::Type::PERSPECTIVE_3D);

		pCam0->setViewport(0, 0, this->WindowWidth, this->WindowHeight);
		pCam0->setPerspective(40.0f, float(pCam0->getScreenWidth()) / float(pCam0->getScreenHeight()), 1.0f, 100000.0f);
		pCam0->setOrientAndPosition(Vec3(0.0f, 1.0f, 0.0f),
									Vec3(0.0f, 0.0f, 0.0f),
									Vec3(0.0f, 0.0f, 6.0f));
		pCam0->updateCamera();
		CameraMan::Add(Camera::Name::CAMERA_0, pCam0);
		CameraMan::SetCurrent(Camera::Name::CAMERA_0, Camera::Type::PERSPECTIVE_3D);

		//---------------------------------------------------------------------------------------------------------
		// Setup the current 2D orthographic Camera
		//---------------------------------------------------------------------------------------------------------
		Camera *pCam2D = new Camera(Camera::Type::ORTHOGRAPHIC_2D);

		pCam2D->setViewport(0, 0, this->WindowWidth, this->WindowHeight);
		pCam2D->setOrthographic((float)-pCam2D->getScreenWidth() / 2.0f, (float)pCam2D->getScreenWidth() / 2.0f,
								(float)-pCam2D->getScreenHeight() / 2.0f, (float)pCam2D->getScreenHeight() / 2.0f,
								1.0f, 1000.0f);
		pCam2D->setOrientAndPosition(Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(0.0f, 0.0f, 2.0f));

		// Holder for the current 2D  cameras
		CameraMan::Add(Camera::Name::CAMERA_1, pCam2D);
		CameraMan::SetCurrent(Camera::Name::CAMERA_1, Camera::Type::ORTHOGRAPHIC_2D);


		// --------------------------------
		//  Mesh
		// --------------------------------

		Mesh *poSpriteMesh = new MeshProto("SpriteMesh.m.proto.azul");
		MeshNodeMan::Add(Mesh::Name::SPRITE, poSpriteMesh);

		Mesh *pPyramidMesh = new MeshProto("PyramidMesh.m.proto.azul");
		MeshNodeMan::Add(Mesh::Name::PYRAMID, pPyramidMesh);

		Mesh *pCubeMesh = new MeshProto("CubeMesh.m.proto.azul");
		MeshNodeMan::Add(Mesh::Name::CUBE, pCubeMesh);

		Mesh* pCrateMesh = new MeshProto("crate.mt.proto.azul");
		MeshNodeMan::Add(Mesh::Name::CRATE, pCrateMesh);

		Mesh* pFrigateMesh = new MeshProto("space_frigate.mt.proto.azul");
		MeshNodeMan::Add(Mesh::Name::SPACE_FRIGATE, pFrigateMesh);

		TextureObject *pTexChickenBot = new TextureProto("ChickenbotDiffuse.t.proto.azul");
		TexNodeMan::Add(TextureObject::Name::ChickenBot, pTexChickenBot);


		Mesh *pBone_Mesh = new MeshProto("Bone.m.proto.azul");
		Mesh *pBone_001_Mesh = new MeshProto("Bone_001.m.proto.azul");
		Mesh *pBone_L_Mesh = new MeshProto("Bone_L.m.proto.azul");
		Mesh *pBone_L_001_Mesh = new MeshProto("Bone_L_001.m.proto.azul");
		Mesh *pBone_L_002_Mesh = new MeshProto("Bone_L_002.m.proto.azul");
		Mesh *pBone_R_Mesh = new MeshProto("Bone_R.m.proto.azul");
		Mesh *pBone_R_001_Mesh = new MeshProto("Bone_R_001.m.proto.azul");
		Mesh *pBone_R_002_Mesh = new MeshProto("Bone_R_002.m.proto.azul");

		MeshNodeMan::Add(Mesh::Name::Bone, pBone_Mesh);
		MeshNodeMan::Add(Mesh::Name::Bone_001, pBone_001_Mesh);
		MeshNodeMan::Add(Mesh::Name::Bone_L, pBone_L_Mesh);
		MeshNodeMan::Add(Mesh::Name::Bone_L_001, pBone_L_001_Mesh);
		MeshNodeMan::Add(Mesh::Name::Bone_L_002, pBone_L_002_Mesh);
		MeshNodeMan::Add(Mesh::Name::Bone_R, pBone_R_Mesh);
		MeshNodeMan::Add(Mesh::Name::Bone_R_001, pBone_R_001_Mesh);
		MeshNodeMan::Add(Mesh::Name::Bone_R_002, pBone_R_002_Mesh);

		// --------------------------------
		//  Shader
		// --------------------------------
		ShaderObject *poShaderA = new ShaderObject_ColorByVertex(ShaderObject::Name::ColorByVertex);
		ShaderObject *poShaderB = new ShaderObject_FlatTexture(ShaderObject::Name::FlatTexture);
		ShaderObject *poShaderC = new ShaderObject_LightTexture(ShaderObject::Name::LightTexture);
		ShaderObject *poShaderD = new ShaderObject_ConstColor(ShaderObject::Name::ConstColor);

		ShaderObjectNodeMan::Add(poShaderA);
		ShaderObjectNodeMan::Add(poShaderB);
		ShaderObjectNodeMan::Add(poShaderC);
		ShaderObjectNodeMan::Add(poShaderD);

		ShaderObject *poShaderE = new ShaderObject_Sprite(ShaderObject::Name::Sprite);
		ShaderObjectNodeMan::Add(poShaderE);

		// -----------------------------------
		//  Texture
		// -----------------------------------

		TextureObject *pTex0 = new TextureProto("TEST_PNG_RGB.t.proto.azul");
		TextureObject *pTex1 = new TextureProto("TEST_PNG_RGBA.t.proto.azul");
		TextureObject *pTex2 = new TextureProto("TEST_TGA_BGR.t.proto.azul");
		TextureObject *pTex3 = new TextureProto("TEST_TGA_BGRA.t.proto.azul");
		TextureObject* pTex4 = new TextureProto("R2D2_BaseColor.t.proto.azul");
		TextureObject* pTex5 = new TextureProto("crate.mt.proto.azul");
		TextureObject* pTex6 = new TextureProto("space_frigate.mt.proto.azul");
		TextureObject* pTex7 = new TextureProto("StarCute.t.proto.azul");
		TextureObject* pTex8 = new TextureProto("MudkipEmotes.t.proto.azul");

		TexNodeMan::Add(TextureObject::Name::Test0, pTex0);
		TexNodeMan::Add(TextureObject::Name::Test1, pTex1);
		TexNodeMan::Add(TextureObject::Name::Test2, pTex2);
		TexNodeMan::Add(TextureObject::Name::Test3, pTex3);
		TexNodeMan::Add(TextureObject::Name::Test4, pTex4);
		TexNodeMan::Add(TextureObject::Name::Test5, pTex5);
		TexNodeMan::Add(TextureObject::Name::Test6, pTex6);
		TexNodeMan::Add(TextureObject::Name::Test7, pTex7);
		TexNodeMan::Add(TextureObject::Name::Test8, pTex8);

		// -----------------------------------
		//  Image
		// -----------------------------------

		ImageMan::Add(Image::Name::WhiteBird,
					  TextureObject::Name::Test1,
					  Rect(139.0f, 131.0f, 84.0f, 97.0f));
		ImageMan::Add(Image::Name::ALLBirds,
					  TextureObject::Name::Test1,
					  Rect(0.0f, 0.0f, 377.0f, 234.0f));
		ImageMan::Add(Image::Name::GreenBird,
					  TextureObject::Name::Test1,
					  Rect(244.0f, 134.0f, 102.0f, 75.0f));
		ImageMan::Add(Image::Name::Star,
					  TextureObject::Name::Test7,
				      Rect(0.0f, 0.0f, 360.0f, 344.0f));
		ImageMan::Add(Image::Name::Mudkip,
					  TextureObject::Name::Test8,
				      Rect(889.0f, 429.0f, 360.0f, 344.0f));

		// ---------------------------------------------
		//  Font - load xml
		// ---------------------------------------------
		TextureObject *pTmp = new TextureProto("FontArial36.t.proto.azul");
		TexNodeMan::Add(TextureObject::Name::FontAriel36, pTmp);

		GlyphMan::Add("MetricsArial36.xml.proto.azul", TextureObject::Name::FontAriel36);


		// ---------------------------------------------
		//  GraphicsObject -- needs model + shader
		// ---------------------------------------------

		GraphicsObject *pGraphicsObject(nullptr);

		// Star Sprite
		pGraphicsObject = new GraphicsObject_Sprite(Mesh::Name::SPRITE,
													ShaderObject::Name::Sprite,
													Image::Star,
													Rect(500, 150, 100,100));
		pSprite = new Sprite(pGraphicsObject);
		pSprite->delta_z = -0.01f;
		GameObjectMan::Add(pSprite, GameObjectMan::GetRoot());


		// Mudkip Sprite
		pGraphicsObject = new GraphicsObject_Sprite(Mesh::Name::SPRITE,
			ShaderObject::Name::Sprite,
			Image::Mudkip,
			Rect(350, 150, 100, 100));
		pSprite = new Sprite(pGraphicsObject);
		GameObjectMan::Add(pSprite, GameObjectMan::GetRoot());


		// FontSprite
		pGraphicsObject = new GraphicsObject_Sprite(Mesh::Name::SPRITE,
													ShaderObject::Name::Sprite,
													Image::GreenBird,  //anything
													Rect(100, 100, 100, 100));
		pFontSpriteA = new FontSprite(pGraphicsObject);
		GameObjectMan::Add(pFontSpriteA, GameObjectMan::GetRoot());

		pFontSpriteA->Set(FontSprite::Name::TestMessage, "Walk", Glyph::Name::Arial36pt, 425, 290);


		GraphicsObject* pGraphicsObjectB = new GraphicsObject_Sprite(Mesh::Name::SPRITE,
			ShaderObject::Name::Sprite,
			Image::GreenBird,  //anything
			Rect(100, 100, 100, 100));
		pFontSpriteB = new FontSprite(pGraphicsObjectB);
		GameObjectMan::Add(pFontSpriteB, GameObjectMan::GetRoot());

		pFontSpriteB->Set(FontSprite::Name::TestMessage, "Run", Glyph::Name::Arial36pt, 275, 290);


		GraphicsObject* pGraphicsObjectC = new GraphicsObject_Sprite(Mesh::Name::SPRITE,
			ShaderObject::Name::Sprite,
			Image::GreenBird,  //anything
			Rect(100, 100, 100, 100));
		pFontSpriteC = new FontSprite(pGraphicsObjectC);
		GameObjectMan::Add(pFontSpriteC, GameObjectMan::GetRoot());

		pFontSpriteC->Set(FontSprite::Name::TestMessage, "Count", Glyph::Name::Arial36pt, 310, 550);

		//R2-D2 model
		GameObjectBasic* pGameObj1(nullptr);
		GraphicsObject* pGraphicsObjectD(nullptr);
		GameObjectBasic* pGameObj2(nullptr);
		GraphicsObject* pGraphicsObjectE(nullptr);
		GraphicsObject* pGraphicsObjectF(nullptr);
		GraphicsObject* pGraphicsObjectG(nullptr);
		GameObjectBasic* pGameObj3(nullptr);
		GameObjectBasic* pGameObj4(nullptr);
		Vec3 LightColor(1.0f, 1.0f, 1.0f);
		Vec3 LightPos(1.0f, 30.0f, 100.0f);


		Mesh* pR2D2Mesh = new MeshProto("R2-D2.mt.proto.azul");
		MeshNodeMan::Add(Mesh::Name::R2D2, pR2D2Mesh);

		//TextureObject* pTexR2D2 = new TextureProto("R2-D2.mt.proto.azul");

	//	TexNodeMan::Add(TextureObject::Name::Test4, pTexR2D2);

		Vec3 Color(0.0f, 0.0f, 1.0f);

		pGraphicsObjectD = new GraphicsObject_Wireframe(Mesh::Name::R2D2,
			ShaderObject::Name::ConstColor,
			Color);
		pGameObj1 = new GameObjectBasic(pGraphicsObjectD);
		pGameObj1->SetScale(15, 15, 15);
		pGameObj1->SetTrans(1, 0.4f, 3);
		Quat q(Rot1::X, -MATH_PI2);
		pGameObj1->SetQuat(q);
		pGameObj1->delta_y = 0.01f;

		GameObjectMan::Add(pGameObj1, GameObjectMan::GetRoot());

		pGraphicsObjectE = new GraphicsObject_LightTexture(Mesh::Name::R2D2,
			ShaderObject::Name::LightTexture,
			TextureObject::Name::Test4,
			LightColor,
			LightPos);
		pGameObj2 = new GameObjectBasic(pGraphicsObjectE);
		pGameObj2->SetScale(15, 15, 15);
		pGameObj2->SetTrans(-1, 0.4f, 3);
		//Quat q(Rot1::X, -MATH_PI2);
		pGameObj2->SetQuat(q);
		pGameObj2->delta_y = 0.01f;

		GameObjectMan::Add(pGameObj2, GameObjectMan::GetRoot());

		//Crate
		Vec3 CrateColor(1.0f, 1.0f, 1.0f);
		pGraphicsObjectF = new GraphicsObject_LightTexture(Mesh::Name::CRATE,
			ShaderObject::Name::LightTexture,
			TextureObject::Name::Test5,
			CrateColor,
			LightPos);
		pGameObj3 = new GameObjectBasic(pGraphicsObjectF);
		pGameObj3->SetScale(0.2f, 0.2f, 0.2f);
		pGameObj3->SetTrans(-1, -0.5f, 3);
		//pGameObj3->SetQuat(q);
		pGameObj3->delta_y = 0.01f;
		pGameObj3->delta_z = 0.02f;

		GameObjectMan::Add(pGameObj3, GameObjectMan::GetRoot());

		//Space Frigate
		//Vec3 CrateColor(1.0f, 1.0f, 1.0f);
		pGraphicsObjectG = new GraphicsObject_LightTexture(Mesh::Name::SPACE_FRIGATE,
			ShaderObject::Name::LightTexture,
			TextureObject::Name::Test6,
			CrateColor,
			LightPos);
		pGameObj4 = new GameObjectBasic(pGraphicsObjectG);
		pGameObj4->SetScale(0.01f, 0.01f, 0.01f);
		pGameObj4->SetTrans(1, -0.5f, 3);
		//pGameObj3->SetQuat(q);
		pGameObj4->delta_x = 0.03f;
		pGameObj4->delta_y = 0.02f;

		GameObjectMan::Add(pGameObj4, GameObjectMan::GetRoot());

		// ---------------------------------
		// Create Animation
		// ---------------------------------


		//Walk - starting animation 
		//Die - secondary animation
		pClipA = new Clip("walk_mesh.clip", NUM_BONES, 39);

		// Anim controller
		AnimTime delta = 0.2f * AnimTime(AnimTime::Duration::FILM_24_FRAME);
		pAnimControllerA = new AnimController(delta, AnimController::Type::WALK);
		//assert(pAnimControllerC);

		//Run - starting animation
		//Side Step - secondary animation
		pClipB = new Clip("run+mesh.clip", NUM_BONES, 24);

		// Anim controller
		//AnimTime delta = 0.2f * AnimTime(AnimTime::Duration::FILM_24_FRAME);
		pAnimControllerB = new AnimController(delta, AnimController::Type::SIDE_STEP_RIGHT);
		//assert(pAnimControllerC);

		return true;
	}

	//-----------------------------------------------------------------------------
	// Game::Update()
	//      Called once per frame, update data, tranformations, etc
	//      Use this function to control process order
	//      Input, AI, Physics, Animation, and Graphics
	//-----------------------------------------------------------------------------
	void Game::Update(float)
	{
		this->intervalTimer.Toc();
		this->intervalTimer.Tic();

		static int count = 0;
		count++;

		char buff[20];
		memset(buff, 0x0, 20);
		sprintf_s(buff, 20, "Count %d", count);
		pFontSpriteC->ChangeMessage(buff, 310, 550);

		// -----------------------
		// Read inputs
		// -----------------------
		CameraDemoInput();

		// ------------------------------------
		// Camera update
		// ------------------------------------

		Camera *pCam3D = CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D);
		pCam3D->updateCamera();

		Camera *pCam2D = CameraMan::GetCurrent(Camera::Type::ORTHOGRAPHIC_2D);
		pCam2D->updateCamera();

		// ------------------------------------
		// Animate A
		// ------------------------------------
		if(pAnimControllerA)
		{
			pAnimControllerA->Update();
		}
		AnimDemoInput(pClipA, pAnimControllerA, pFontSpriteA);

		// ------------------------------------
		// Animate B
		// ------------------------------------
		if (pAnimControllerB)
		{
			pAnimControllerB->Update();
		}
		AnimDemoInput(pClipB, pAnimControllerB, pFontSpriteB);

		// ------------------------------------
		// Update GameObjects
		// ------------------------------------
		GameObjectMan::Update(this->globalTimer.Toc());

	}


	//-----------------------------------------------------------------------------
	// Game::Render()
	//		This function is called once per frame
	//	    Use this for draw graphics to the screen.
	//      Only do rendering here
	//-----------------------------------------------------------------------------
	void Game::Render()
	{
		this->SetDefaultTargetMode();

		GameObjectMan::Draw();
	}

	//-----------------------------------------------------------------------------
	// Game::UnLoadContent()
	//       unload content (resources loaded above)
	//       unload all content that was loaded before the Engine Loop started
	//-----------------------------------------------------------------------------
	void Game::UnloadContent()
	{
		GlyphMan::Destroy();
		ImageMan::Destroy();
		CameraMan::Destroy();
		ShaderObjectNodeMan::Destroy();
		MeshNodeMan::Destroy();
		GameObjectMan::Destroy();
		TexNodeMan::Destroy();
		DirectXDeviceMan::Destroy();

		delete pAnimControllerA;
		delete pAnimControllerB;
		delete pClipA;
		delete pClipB;


	}

	//------------------------------------------------------------------
	// Game::ClearBufferFunc()
	// Allows user to change the way the clear buffer function works
	// 
	// Clear the color and depth buffers.
	//------------------------------------------------------------------
	void Game::ClearDepthStencilBuffer()
	{
#ifdef _DEBUG
		const Vec4 ClearColor = Azul::Colors::LightGray;
#else
		const Vec4 ClearColor = Azul::Colors::Wheat;
#endif
		float clearDepth = 1.0f;
		uint8_t clearStencil = 0;
		g_d3dDeviceContext->ClearRenderTargetView(g_d3dRenderTargetView, (const float *)&ClearColor);
		g_d3dDeviceContext->ClearDepthStencilView(g_d3dDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, clearDepth, clearStencil);
	}

}

// --- End of File ---
