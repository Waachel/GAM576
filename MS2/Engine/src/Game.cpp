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
#include "GameObjectAnimSkin.h"
#include "ShaderObject.h"
#include "ShaderObject_ColorByVertex.h"
#include "ShaderObject_FlatTexture.h"
#include "ShaderObject_SkinFlatTexture.h"
#include "ShaderObject_LightTexture.h"
#include "ShaderObject_ConstColor.h"
#include "ShaderObject_Sprite.h"
#include "ShaderObject_MixerCompute.h"
#include "ShaderObject_WorldCompute.h"
#include "Mesh.h"
#include "MeshProto.h"
#include "GraphicsObject_ColorByVertex.h"
#include "GraphicsObject_Wireframe.h"
#include "GraphicsObject_FlatTexture.h"
#include "GraphicsObject_SkinFlatTexture.h"
#include "GraphicsObject_LightTexture.h"
#include "GraphicsObject_ConstColor.h"
#include "GraphicsObject_Sprite.h"
#include "GraphicsObject_Null.h"
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
#include "Anim.h"
#include "GameObjectBasic.h"
#include "TimerController.h"
#include "AnimController.h"
#include "Clip.h"
#include "Image.h"
#include "ImageMan.h"
#include "Sprite.h"
#include "GlyphMan.h"
#include "HierarchyMan.h"
#include "InvBindMan.h"

#include "FontSprite.h"
#include "SkelMan.h"
#include "ClipMan.h"

#include "ComputeRSVBuffer.h"
#include "ComputeUAVBuffer.h"
#include "Mixer.h"
#include "WorldCompute.h"

#include "AnimDemoInput.h"

using namespace Azul;

namespace Azul
{
	// Todo: clean up
	AnimController* pAnimControllerA;
	AnimController* pAnimControllerB;
	Sprite* pSprite(nullptr);
	FontSprite* pFontSpriteA;
	FontSprite* pFontSpriteB;
	FontSprite* pFontSpriteC;

	//-----------------------------------------------------------------------------
	//  Game::Game()
	//		Game Engine Constructor
	//-----------------------------------------------------------------------------

	Game::Game(const char* const pName, int width, int height)
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
		SkelMan::Create();
		ClipMan::Create();
		HierarchyMan::Create();
		InvBindMan::Create();

		// --------------------------
		// Camera setup
		// --------------------------

		// Camera_0		

		//---------------------------------------------------------------------------------------------------------
		// Setup the current 3D perspective Camera
		//---------------------------------------------------------------------------------------------------------
		Camera* pCam0 = new Camera(Camera::Type::PERSPECTIVE_3D);

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
		Camera* pCam2D = new Camera(Camera::Type::ORTHOGRAPHIC_2D);

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

		Mesh* poSpriteMesh = new MeshProto("Sprite.m.proto.azul");
		MeshNodeMan::Add(Mesh::Name::SPRITE, poSpriteMesh);

		Mesh* pCubeMesh = new MeshProto("CubeMesh.m.proto.azul");
		MeshNodeMan::Add(Mesh::Name::CUBE, pCubeMesh);

		Mesh* pCrateMesh = new MeshProto("crate.m.proto.azul");
		MeshNodeMan::Add(Mesh::Name::CRATE, pCrateMesh);

		Mesh* pFrigateMesh = new MeshProto("spacefrigate.m.proto.azul");
		MeshNodeMan::Add(Mesh::Name::SPACE_FRIGATE, pFrigateMesh);

		Mesh* poChickenBot = new MeshProto("ChickenBot.m.proto.azul");
		MeshNodeMan::Add(Mesh::Name::ChickenBot, poChickenBot);

		Mesh* poMouseyMesh = new MeshProto("Mousey.m.proto.azul");
		MeshNodeMan::Add(Mesh::Name::MOUSEY, poMouseyMesh);

		// --------------------------------
		//  Shader
		// --------------------------------

		ShaderObject* poShaderC = new ShaderObject_Sprite(ShaderObject::Name::Sprite);
		ShaderObjectNodeMan::Add(poShaderC);

		ShaderObject* poShaderA0 = new ShaderObject_ColorByVertex(ShaderObject::Name::ColorByVertex);
		ShaderObject* poShaderA1 = new ShaderObject_FlatTexture(ShaderObject::Name::FlatTexture);
		ShaderObject* poShaderA2 = new ShaderObject_LightTexture(ShaderObject::Name::LightTexture);
		ShaderObject* poShaderA3 = new ShaderObject_ConstColor(ShaderObject::Name::ConstColor);
		ShaderObject* poShaderA4 = new ShaderObject_SkinFlatTexture(ShaderObject::Name::SkinFlatTexture);

		ShaderObject* poShaderA6 = new ShaderObject_MixerCompute(ShaderObject::Name::MixerCompute);
		ShaderObject* poShaderA7 = new ShaderObject_WorldCompute(ShaderObject::Name::WorldCompute);

		ShaderObjectNodeMan::Add(poShaderA0);
		ShaderObjectNodeMan::Add(poShaderA1);
		ShaderObjectNodeMan::Add(poShaderA2);
		ShaderObjectNodeMan::Add(poShaderA3);
		ShaderObjectNodeMan::Add(poShaderA4);

		ShaderObjectNodeMan::Add(poShaderA6);
		ShaderObjectNodeMan::Add(poShaderA7);

		// -----------------------------------
		//  Texture
		// -----------------------------------

		TextureObject* pTex0 = new TextureProto("TEST_PNG_RGB.t.proto.azul");
		TextureObject* pTex1 = new TextureProto("TEST_PNG_RGBA.t.proto.azul");
		TextureObject* pTex2 = new TextureProto("TEST_TGA_BGR.t.proto.azul");
		TextureObject* pTex3 = new TextureProto("TEST_TGA_BGRA.t.proto.azul");
		TextureObject* pTex4 = new TextureProto("R2D2_BaseColor.t.proto.azul");
		TextureObject* pTex5 = new TextureProto("crate_BaseColor.t.proto.azul");
		TextureObject* pTex6 = new TextureProto("spacefrigate.t.proto.azul");
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


		TextureObject* pTexChickenBot = new TextureProto("Chickenbot.t.proto.azul");
		TexNodeMan::Add(TextureObject::Name::ChickenBot, pTexChickenBot);

		TextureObject* pTexSpaceFrigate = new TextureProto("SpaceFrigate.t.proto.azul");
		TexNodeMan::Add(TextureObject::Name::SpaceFrigate, pTexSpaceFrigate);

		TextureObject* pTexMousey = new TextureProto("Mousey.t.proto.azul");
		TexNodeMan::Add(TextureObject::Name::Mousey, pTexMousey);

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
		TextureObject* pTmp = new TextureProto("Arial36.t.proto.azul");
		TexNodeMan::Add(TextureObject::Name::FontAriel36, pTmp);

		GlyphMan::Add("Arial36.xml.proto.azul", TextureObject::Name::FontAriel36);


		// ---------------------------------------------
		//  GraphicsObject -- needs model + shader
		// ---------------------------------------------

		GraphicsObject* pGraphicsObject(nullptr);

		// Star Sprite
		pGraphicsObject = new GraphicsObject_Sprite(Mesh::Name::SPRITE,
			ShaderObject::Name::Sprite,
			Image::Star,
			Rect(500, 150, 100, 100));
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

		pFontSpriteB->Set(FontSprite::Name::TestMessage, "Dance", Glyph::Name::Arial36pt, 260, 290);


		GraphicsObject* pGraphicsObjectC = new GraphicsObject_Sprite(Mesh::Name::SPRITE,
			ShaderObject::Name::Sprite,
			Image::GreenBird,  //anything
			Rect(100, 100, 100, 100));
		pFontSpriteC = new FontSprite(pGraphicsObjectC);
		GameObjectMan::Add(pFontSpriteC, GameObjectMan::GetRoot());
		Color FontColor(0.0f, 1.0f, 1.0f);
		pFontSpriteC->Set(FontSprite::Name::TestMessage, "Count", Glyph::Name::Arial36pt, 310, 550, FontColor);

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


		Mesh* pR2D2Mesh = new MeshProto("R2-D2.m.proto.azul");
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
		//Skeleton 0

		// Load resources
		HierarchyMan::Add("ChickenBot.h.proto.azul", Hierarchy::Name::ChickenBot);
		ClipMan::Add("ChickenBot_Walk.a.proto.azul", Clip::Name::Walk_ChickenBot, Skel::Name::ChickenBot1);
		InvBindMan::Add("ChickenBot.i.proto.azul", InvBind::Name::ChickenBot);
		//Walk - starting animation 
		//Die - secondary animation



		// Create Anim
		Clip* pClip = ClipMan::Find(Clip::Name::Walk_ChickenBot);

		// Setup Compute shaders data
		Mixer* ptMixer = new Mixer(pClip);
		WorldCompute* ptWorldCompute = new WorldCompute(ptMixer,
			Hierarchy::Name::ChickenBot,
			InvBind::Name::ChickenBot);
		assert(ptWorldCompute);

		// Anim needs: Clip, Skeleton, BoneResult
		Anim* ptAnim = new Anim(pClip, ptMixer);
		assert(ptAnim);

		// Skin Mesh
		GameObjectAnimSkin* pGameSkin;
		GraphicsObject_SkinFlatTexture* pGraphicsSkin;

		// add Mixer and WorldCompute to Graphics Object
		pGraphicsSkin = new GraphicsObject_SkinFlatTexture(Mesh::Name::ChickenBot,
			ShaderObject::Name::SkinFlatTexture,
			TextureObject::Name::ChickenBot,
			ptMixer,
			ptWorldCompute);

		// add Mixer and WorldCompute to Game Object
		pGameSkin = new GameObjectAnimSkin(pGraphicsSkin, ptMixer, ptWorldCompute);
		pGameSkin->SetName("ChickenSkinWalk");
		pGameSkin->delta_y = 0.003f;
		pGameSkin->SetScale(15,15,15);
		pGameSkin->SetTrans(0.3f, 0.1f, 3);

		GameObjectMan::Add(pGameSkin, GameObjectMan::GetRoot());

		pAnimControllerA = new AnimController(ptAnim, 0.2f * AnimTime(AnimTime::Duration::FILM_24_FRAME));

		
		//Skeleton 1

		// Load resources
		HierarchyMan::Add("Mousey.h.proto.azul", Hierarchy::Name::Mousey);
		ClipMan::Add("Mousey_SillyDance.a.proto.azul", Clip::Name::Dance_Mousey, Skel::Name::Mousey);
		InvBindMan::Add("Mousey.i.proto.azul", InvBind::Name::Mousey);

		// Create Anim
		Clip* pClip2 = ClipMan::Find(Clip::Name::Dance_Mousey);

		// Setup Compute shaders data
		Mixer* ptMixer2 = new Mixer(pClip2);
		WorldCompute* ptWorldCompute2 = new WorldCompute(ptMixer2,
			Hierarchy::Name::Mousey,
			InvBind::Name::Mousey);
		assert(ptWorldCompute2);

		// Anim needs: Clip, Skeleton, BoneResult
		Anim* ptAnim2 = new Anim(pClip2, ptMixer2);
		assert(ptAnim2);

		// Skin Mesh
		GameObjectAnimSkin* pGameSkin2;
		GraphicsObject_SkinFlatTexture* pGraphicsSkin2;

		// add Mixer and WorldCompute to Graphics Object
		pGraphicsSkin2 = new GraphicsObject_SkinFlatTexture(Mesh::Name::MOUSEY,
			ShaderObject::Name::SkinFlatTexture,
			TextureObject::Name::Mousey,
			ptMixer2,
			ptWorldCompute2);

		assert(pGraphicsSkin2);

		// add Mixer and WorldCompute to Game Object
		pGameSkin2 = new GameObjectAnimSkin(pGraphicsSkin2, ptMixer2, ptWorldCompute2);
		pGameSkin2->SetName("MouseyDance");
		pGameSkin2->delta_y = 0.009f;
		pGameSkin2->SetScale(0.5, 0.5, 0.5);
		pGameSkin2->SetTrans(-0.35f, 0.1f, 3);

		GameObjectMan::Add(pGameSkin2, GameObjectMan::GetRoot());

		pAnimControllerB = new AnimController(ptAnim2, 0.2f * AnimTime(AnimTime::Duration::FILM_24_FRAME));


		//chicken bot animations
		//ClipMan::Add("ChickenBot_Run.a.proto.azul", Clip::Name::Run_ChickenBot, Skel::Name::ChickenBot2);
		ClipMan::Add("ChickenBot_DieLeft.a.proto.azul", Clip::Name::DieLeft_ChickenBot, Skel::Name::ChickenBot1);
		ClipMan::Add("Mousey_ZombieWalk.a.proto.azul", Clip::Name::ZombieWalk_Mousey, Skel::Name::Mousey);
		//ClipMan::Add("ChickenBot_SideStepRight.a.proto.azul", Clip::Name::SideStepRight_ChickenBot, Skel::Name::ChickenBot2);


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

		Camera* pCam3D = CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D);
		pCam3D->updateCamera();

		Camera* pCam2D = CameraMan::GetCurrent(Camera::Type::ORTHOGRAPHIC_2D);
		pCam2D->updateCamera();

		// ------------------------------------
		// Animate A
		// ------------------------------------
		if (pAnimControllerA)
		{
			pAnimControllerA->Update();
		}
		AnimDemoInput(pAnimControllerA, pFontSpriteA);

		// ------------------------------------
		// Animate B
		// ------------------------------------
		if (pAnimControllerB)
		{
			pAnimControllerB->Update();
		}
		AnimDemoInput(pAnimControllerB, pFontSpriteB);

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
		InvBindMan::Destroy();
		HierarchyMan::Destroy();
		ClipMan::Destroy();
		SkelMan::Destroy();
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
		g_d3dDeviceContext->ClearRenderTargetView(g_d3dRenderTargetView, (const float*)&ClearColor);
		g_d3dDeviceContext->ClearDepthStencilView(g_d3dDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, clearDepth, clearStencil);
	}

}

// --- End of File ---