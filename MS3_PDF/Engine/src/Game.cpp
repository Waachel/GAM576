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
	AnimController* pAnimControllerC;
	AnimController* pAnimControllerD;
	AnimController* pAnimControllerE;
	AnimController* pAnimControllerF;
	AnimController* pAnimControllerG;
	AnimController* pAnimControllerH;
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

		TextureObject* pTexMousey = new TextureProto("Mousey.t.proto.azul");
		TexNodeMan::Add(TextureObject::Name::Mousey, pTexMousey);

		// -----------------------------------
		//  Image
		// -----------------------------------

		ImageMan::Add(Image::Name::GreenBird,
			TextureObject::Name::Test1,
			Rect(244.0f, 134.0f, 102.0f, 75.0f));


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

		// FontSprite
		pGraphicsObject = new GraphicsObject_Sprite(Mesh::Name::SPRITE,
			ShaderObject::Name::Sprite,
			Image::GreenBird,  //anything
			Rect(100, 100, 100, 100));
		pFontSpriteA = new FontSprite(pGraphicsObject);
		GameObjectMan::Add(pFontSpriteA, GameObjectMan::GetRoot());

		pFontSpriteA->Set(FontSprite::Name::TestMessage, "", Glyph::Name::Arial36pt, 425, 290);


		

		// ---------------------------------
		// Create Animation
		// ---------------------------------

		// Load resources
		HierarchyMan::Add("Mousey.h.proto.azul", Hierarchy::Name::Mousey);
		ClipMan::Add("Mousey_SillyDance.a.proto.azul", Clip::Name::Dance_Mousey, Skel::Name::Mousey);
		ClipMan::Add("Mousey_ZombieWalk.a.proto.azul", Clip::Name::ZombieWalk_Mousey, Skel::Name::Mousey);
		ClipMan::Add("Mousey_Breakdance.a.proto.azul", Clip::Name::Breakdance_Mousey, Skel::Name::Mousey);
		ClipMan::Add("Mousey_Falling.a.proto.azul", Clip::Name::Falling_Mousey, Skel::Name::Mousey);
		ClipMan::Add("Mousey_Jump.a.proto.azul", Clip::Name::Jump_Mousey, Skel::Name::Mousey);
		ClipMan::Add("Mousey_Punching.a.proto.azul", Clip::Name::Punching_Mousey, Skel::Name::Mousey);
		ClipMan::Add("Mousey_SitupToIdle.a.proto.azul", Clip::Name::Situp_Mousey, Skel::Name::Mousey);
		ClipMan::Add("Mousey_StrutWalk.a.proto.azul", Clip::Name::StrutWalk_Mousey, Skel::Name::Mousey);
		InvBindMan::Add("Mousey.i.proto.azul", InvBind::Name::Mousey);
		
		//Skeleton 1

		// Create Anim
		Clip* pClip = ClipMan::Find(Clip::Name::ZombieWalk_Mousey);

		// Setup Compute shaders data
		Mixer* ptMixer = new Mixer(pClip);
		WorldCompute* ptWorldCompute = new WorldCompute(ptMixer,
			Hierarchy::Name::Mousey,
			InvBind::Name::Mousey);
		assert(ptWorldCompute);

		// Anim needs: Clip, Skeleton, BoneResult
		Anim* ptAnim = new Anim(pClip, ptMixer);
		assert(ptAnim);

		// Skin Mesh
		GameObjectAnimSkin* pGameSkin;
		GraphicsObject_SkinFlatTexture* pGraphicsSkin;

		// add Mixer and WorldCompute to Graphics Object
		pGraphicsSkin = new GraphicsObject_SkinFlatTexture(Mesh::Name::MOUSEY,
			ShaderObject::Name::SkinFlatTexture,
			TextureObject::Name::Mousey,
			ptMixer,
			ptWorldCompute);

		// add Mixer and WorldCompute to Game Object
		pGameSkin = new GameObjectAnimSkin(pGraphicsSkin, ptMixer, ptWorldCompute);
		pGameSkin->SetName("MouseyZombieWalk");
		//pGameSkin->delta_y = 0.003f;
		pGameSkin->SetScale(0.5, 0.5, 0.5);
		pGameSkin->SetTrans(-0.9f, 0.1f, 3);

		GameObjectMan::Add(pGameSkin, GameObjectMan::GetRoot());

		pAnimControllerA = new AnimController(ptAnim, 0.3f * AnimTime(AnimTime::Duration::FILM_24_FRAME));
		
		//Skeleton 2

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
		//pGameSkin2->delta_y = 0.009f;
		pGameSkin2->SetScale(0.5, 0.5, 0.5);
		pGameSkin2->SetTrans(-0.35f, 0.1f, 3);

		GameObjectMan::Add(pGameSkin2, GameObjectMan::GetRoot());

		pAnimControllerB = new AnimController(ptAnim2, 0.3f * AnimTime(AnimTime::Duration::FILM_24_FRAME));

		//Skeleton 3

		// Create Anim
		Clip* pClip3 = ClipMan::Find(Clip::Name::Breakdance_Mousey);

		// Setup Compute shaders data
		Mixer* ptMixer3 = new Mixer(pClip3);
		WorldCompute* ptWorldCompute3 = new WorldCompute(ptMixer3,
			Hierarchy::Name::Mousey,
			InvBind::Name::Mousey);
		assert(ptWorldCompute3);

		// Anim needs: Clip, Skeleton, BoneResult
		Anim* ptAnim3 = new Anim(pClip3, ptMixer3);
		assert(ptAnim3);

		// Skin Mesh
		GameObjectAnimSkin* pGameSkin3;
		GraphicsObject_SkinFlatTexture* pGraphicsSkin3;

		// add Mixer and WorldCompute to Graphics Object
		pGraphicsSkin3 = new GraphicsObject_SkinFlatTexture(Mesh::Name::MOUSEY,
			ShaderObject::Name::SkinFlatTexture,
			TextureObject::Name::Mousey,
			ptMixer3,
			ptWorldCompute3);

		assert(pGraphicsSkin3);

		// add Mixer and WorldCompute to Game Object
		pGameSkin3 = new GameObjectAnimSkin(pGraphicsSkin3, ptMixer3, ptWorldCompute3);
		pGameSkin3->SetName("MouseyBreakdance");
		//pGameSkin3->delta_y = 0.009f;
		pGameSkin3->SetScale(0.5, 0.5, 0.5);
		pGameSkin3->SetTrans(0.15f, 0.1f, 3);

		GameObjectMan::Add(pGameSkin3, GameObjectMan::GetRoot());

		pAnimControllerC = new AnimController(ptAnim3, 0.3f * AnimTime(AnimTime::Duration::FILM_24_FRAME));

		//Skeleton 4

		// Create Anim
		Clip* pClip4 = ClipMan::Find(Clip::Name::Falling_Mousey);

		// Setup Compute shaders data
		Mixer* ptMixer4 = new Mixer(pClip4);
		WorldCompute* ptWorldCompute4 = new WorldCompute(ptMixer4,
			Hierarchy::Name::Mousey,
			InvBind::Name::Mousey);
		assert(ptWorldCompute4);

		// Anim needs: Clip, Skeleton, BoneResult
		Anim* ptAnim4 = new Anim(pClip4, ptMixer4);
		assert(ptAnim4);

		// Skin Mesh
		GameObjectAnimSkin* pGameSkin4;
		GraphicsObject_SkinFlatTexture* pGraphicsSkin4;

		// add Mixer and WorldCompute to Graphics Object
		pGraphicsSkin4 = new GraphicsObject_SkinFlatTexture(Mesh::Name::MOUSEY,
			ShaderObject::Name::SkinFlatTexture,
			TextureObject::Name::Mousey,
			ptMixer4,
			ptWorldCompute4);

		assert(pGraphicsSkin4);

		// add Mixer and WorldCompute to Game Object
		pGameSkin4 = new GameObjectAnimSkin(pGraphicsSkin4, ptMixer4, ptWorldCompute4);
		pGameSkin4->SetName("MouseyFalling");
		//pGameSkin4->delta_y = 0.009f;
		pGameSkin4->SetScale(0.5, 0.5, 0.5);
		pGameSkin4->SetTrans(0.8f, 0.09f, 3);

		GameObjectMan::Add(pGameSkin4, GameObjectMan::GetRoot());

		pAnimControllerD = new AnimController(ptAnim4, 0.3f * AnimTime(AnimTime::Duration::FILM_24_FRAME));

		//Skeleton 5

		// Create Anim
		Clip* pClip5 = ClipMan::Find(Clip::Name::Jump_Mousey);

		// Setup Compute shaders data
		Mixer* ptMixer5 = new Mixer(pClip5);
		WorldCompute* ptWorldCompute5 = new WorldCompute(ptMixer5,
			Hierarchy::Name::Mousey,
			InvBind::Name::Mousey);
		assert(ptWorldCompute5);

		// Anim needs: Clip, Skeleton, BoneResult
		Anim* ptAnim5 = new Anim(pClip5, ptMixer5);
		assert(ptAnim5);

		// Skin Mesh
		GameObjectAnimSkin* pGameSkin5;
		GraphicsObject_SkinFlatTexture* pGraphicsSkin5;

		// add Mixer and WorldCompute to Graphics Object
		pGraphicsSkin5 = new GraphicsObject_SkinFlatTexture(Mesh::Name::MOUSEY,
			ShaderObject::Name::SkinFlatTexture,
			TextureObject::Name::Mousey,
			ptMixer5,
			ptWorldCompute5);

		assert(pGraphicsSkin5);

		// add Mixer and WorldCompute to Game Object
		pGameSkin5 = new GameObjectAnimSkin(pGraphicsSkin5, ptMixer5, ptWorldCompute5);
		pGameSkin5->SetName("MouseyJump");
		//pGameSkin5->delta_y = 0.009f;
		pGameSkin5->SetScale(0.5, 0.5, 0.5);
		pGameSkin5->SetTrans(-0.9f, -0.8f, 3);

		GameObjectMan::Add(pGameSkin5, GameObjectMan::GetRoot());

		pAnimControllerE = new AnimController(ptAnim5, 0.3f * AnimTime(AnimTime::Duration::FILM_24_FRAME));

		//Skeleton 6

		// Create Anim
		Clip* pClip6 = ClipMan::Find(Clip::Name::Punching_Mousey);

		// Setup Compute shaders data
		Mixer* ptMixer6 = new Mixer(pClip6);
		WorldCompute* ptWorldCompute6 = new WorldCompute(ptMixer6,
			Hierarchy::Name::Mousey,
			InvBind::Name::Mousey);
		assert(ptWorldCompute6);

		// Anim needs: Clip, Skeleton, BoneResult
		Anim* ptAnim6 = new Anim(pClip6, ptMixer6);
		assert(ptAnim6);

		// Skin Mesh
		GameObjectAnimSkin* pGameSkin6;
		GraphicsObject_SkinFlatTexture* pGraphicsSkin6;

		// add Mixer and WorldCompute to Graphics Object
		pGraphicsSkin6 = new GraphicsObject_SkinFlatTexture(Mesh::Name::MOUSEY,
			ShaderObject::Name::SkinFlatTexture,
			TextureObject::Name::Mousey,
			ptMixer6,
			ptWorldCompute6);

		assert(pGraphicsSkin6);

		// add Mixer and WorldCompute to Game Object
		pGameSkin6 = new GameObjectAnimSkin(pGraphicsSkin6, ptMixer6, ptWorldCompute6);
		pGameSkin6->SetName("MouseyPunching");
		//pGameSkin6->delta_y = 0.009f;
		pGameSkin6->SetScale(0.5, 0.5, 0.5);
		pGameSkin6->SetTrans(-0.35f, -0.8f, 3);

		GameObjectMan::Add(pGameSkin6, GameObjectMan::GetRoot());

		pAnimControllerF = new AnimController(ptAnim6, 0.3f * AnimTime(AnimTime::Duration::FILM_24_FRAME));

		//Skeleton 7

		// Create Anim
		Clip* pClip7 = ClipMan::Find(Clip::Name::Situp_Mousey);

		// Setup Compute shaders data
		Mixer* ptMixer7 = new Mixer(pClip7);
		WorldCompute* ptWorldCompute7 = new WorldCompute(ptMixer7,
			Hierarchy::Name::Mousey,
			InvBind::Name::Mousey);
		assert(ptWorldCompute7);

		// Anim needs: Clip, Skeleton, BoneResult
		Anim* ptAnim7 = new Anim(pClip7, ptMixer7);
		assert(ptAnim7);

		// Skin Mesh
		GameObjectAnimSkin* pGameSkin7;
		GraphicsObject_SkinFlatTexture* pGraphicsSkin7;

		// add Mixer and WorldCompute to Graphics Object
		pGraphicsSkin7 = new GraphicsObject_SkinFlatTexture(Mesh::Name::MOUSEY,
			ShaderObject::Name::SkinFlatTexture,
			TextureObject::Name::Mousey,
			ptMixer7,
			ptWorldCompute7);

		assert(pGraphicsSkin7);

		// add Mixer and WorldCompute to Game Object
		pGameSkin7 = new GameObjectAnimSkin(pGraphicsSkin7, ptMixer7, ptWorldCompute7);
		pGameSkin7->SetName("MouseySitup");
		//pGameSkin7->delta_y = 0.009f;
		pGameSkin7->SetScale(0.5, 0.5, 0.5);
		pGameSkin7->SetTrans(0.25f, -0.8f, 3);

		GameObjectMan::Add(pGameSkin7, GameObjectMan::GetRoot());

		pAnimControllerG = new AnimController(ptAnim7, 0.3f * AnimTime(AnimTime::Duration::FILM_24_FRAME));

		//Skeleton 8

		// Create Anim
		Clip* pClip8 = ClipMan::Find(Clip::Name::StrutWalk_Mousey);

		// Setup Compute shaders data
		Mixer* ptMixer8 = new Mixer(pClip8);
		WorldCompute* ptWorldCompute8 = new WorldCompute(ptMixer8,
			Hierarchy::Name::Mousey,
			InvBind::Name::Mousey);
		assert(ptWorldCompute8);

		// Anim needs: Clip, Skeleton, BoneResult
		Anim* ptAnim8 = new Anim(pClip8, ptMixer8);
		assert(ptAnim8);

		// Skin Mesh
		GameObjectAnimSkin* pGameSkin8;
		GraphicsObject_SkinFlatTexture* pGraphicsSkin8;

		// add Mixer and WorldCompute to Graphics Object
		pGraphicsSkin8 = new GraphicsObject_SkinFlatTexture(Mesh::Name::MOUSEY,
			ShaderObject::Name::SkinFlatTexture,
			TextureObject::Name::Mousey,
			ptMixer8,
			ptWorldCompute8);

		assert(pGraphicsSkin8);

		// add Mixer and WorldCompute to Game Object
		pGameSkin8 = new GameObjectAnimSkin(pGraphicsSkin8, ptMixer8, ptWorldCompute8);
		pGameSkin8->SetName("MouseyStrut");
		//pGameSkin8->delta_y = 0.009f;
		pGameSkin8->SetScale(0.5, 0.5, 0.5);
		pGameSkin8->SetTrans(0.8f, -0.8f, 3);

		GameObjectMan::Add(pGameSkin8, GameObjectMan::GetRoot());

		pAnimControllerH = new AnimController(ptAnim8, 0.3f * AnimTime(AnimTime::Duration::FILM_24_FRAME));

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
		AnimDemoInput(pAnimControllerB, pFontSpriteA);

		// ------------------------------------
		// Animate C
		// ------------------------------------
		if (pAnimControllerC)
		{
			pAnimControllerC->Update();
		}
		AnimDemoInput(pAnimControllerC, pFontSpriteA);

		// ------------------------------------
		// Animate D
		// ------------------------------------
		if (pAnimControllerD)
		{
			pAnimControllerD->Update();
		}
		AnimDemoInput(pAnimControllerD, pFontSpriteA);


		// ------------------------------------
		// Animate E
		// ------------------------------------
		if (pAnimControllerE)
		{
			pAnimControllerE->Update();
		}
		AnimDemoInput(pAnimControllerE, pFontSpriteA);

		// ------------------------------------
		// Animate F
		// ------------------------------------
		if (pAnimControllerF)
		{
			pAnimControllerF->Update();
		}
		AnimDemoInput(pAnimControllerF, pFontSpriteA);

		// ------------------------------------
		// Animate G
		// ------------------------------------
		if (pAnimControllerG)
		{
			pAnimControllerG->Update();
		}
		AnimDemoInput(pAnimControllerG, pFontSpriteA);

		// ------------------------------------
		// Animate H
		// ------------------------------------
		if (pAnimControllerH)
		{
			pAnimControllerH->Update();
		}
		AnimDemoInput(pAnimControllerH, pFontSpriteA);

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
		delete pAnimControllerC;
		delete pAnimControllerD;
		delete pAnimControllerE;
		delete pAnimControllerF;
		delete pAnimControllerG;
		delete pAnimControllerH;

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