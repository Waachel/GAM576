//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "GLTF.h"
#include "ConvertModel.h"
#include "CreateTexture.h"
#include "CubeMesh.h"
#include "SphereMesh.h"
#include "PyramidMesh.h"
#include "ConvertAnim.h"
#include "ChickenBot.h"
#include "SpriteMesh.h"
#include "ConvertXML.h"
#include "ConvertSkeleton.h"
#include "ConvertSkin.h"
#include "ConvertHierarchy.h"

using namespace Azul;

int main()
{
	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	// Sprite
	CreateSpriteMesh("Sprite");

	ConvertModel("R2-D2.glb", "R2-D2");

	CreateCubeMesh();

	CreateTexturePNG("TEST_PNG_RGB.png", "TEST_PNG_RGB");
	CreateTexturePNG("TEST_PNG_RGBA.png", "TEST_PNG_RGBA");
	CreateTexturePNG("StarCute.png", "StarCute");
	CreateTexturePNG("MudkipEmotes.png", "MudkipEmotes");
	CreateTextureTGA("TEST_TGA_BGR.tga", "TEST_TGA_BGR");
	CreateTextureTGA("TEST_TGA_BGRA.tga", "TEST_TGA_BGRA");

	// Font stuff
	CreateTexturePNG("FontArial36.png","Arial36");
	ConvertXML("MetricsArial36","Arial36");
	         
//	CreateTextureTGA("Rocks.tga");
//	CreateTextureTGA("stitch.tga");

	CreateTexturePNG("R2D2_BaseColor.png","R2D2_BaseColor");
	CreateTexturePNG("R2D2_ColorID.png","R2D2_ColorID");

	//crate
	ConvertModel("crate.glb", "crate");
	CreateTexturePNG("crate_BaseColor.png","crate_BaseColor");

	//frigate
	CreateTextureTGA("space_frigate.tga","SpaceFrigate");
	ConvertModel("space_frigate.glb","SpaceFrigate");

	// ChickenBot
	CreateTexturePNG("ChickenbotDiffuse.png","ChickenBot"); 
	ConvertSkin("walk_mesh", "Chickenbot");
	ConvertAnim("walk_mesh","ChickenBot_Walk");
	ConvertSkeleton("walk_mesh", "ChickenBot");
	ConvertHierarchy("walk_mesh", "ChickenBot");
	ConvertInvBind("walk_mesh", "ChickenBot");

	//Animation 2: Die Left
	ConvertAnim("die_left+mesh","ChickenBot_DieLeft");
	//Animation 3: Side Step Right
	ConvertAnim("sidestep_right+mesh","ChickenBot_SideStepRight");
	//Animation 4:Run
	ConvertAnim("run+mesh","ChickenBot_Run");

	// Mousey
	CreateTexturePNG("MouseyDiffuse.png", "Mousey");
	ConvertSkin("sillydancing+mesh", "Mousey");
	ConvertSkeleton("sillydancing+mesh", "Mousey");
	ConvertHierarchy("sillydancing+mesh", "Mousey");
	ConvertInvBind("sillydancing+mesh", "Mousey");

	ConvertAnim("sillydancing+mesh", "Mousey_SillyDance");
	ConvertAnim("zombiewalk+mesh", "Mousey_ZombieWalk");
	

	// clean up data directory
	system("del *.png");
	system("del *.tga");
	system("del *.glb");
	system("del *.xml");

	google::protobuf::ShutdownProtobufLibrary();

	return 0;
}

// ---  End of File ---------------
