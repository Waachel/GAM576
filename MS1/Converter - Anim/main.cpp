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

using namespace Azul;

int main()
{
	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;


	ConvertModel("R2-D2.glb");

	CreateCubeMesh();
	CreateSpriteMesh();

	CreateTexturePNG("TEST_PNG_RGB.png");
	CreateTexturePNG("TEST_PNG_RGBA.png");
	CreateTexturePNG("StarCute.png");
	CreateTexturePNG("MudkipEmotes.png");
	CreateTextureTGA("TEST_TGA_BGR.tga");
	CreateTextureTGA("TEST_TGA_BGRA.tga");

	// Font stuff
	CreateTexturePNG("FontArial36.png");
	ConvertXML("MetricsArial36");
	         
//	CreateTextureTGA("Rocks.tga");
//	CreateTextureTGA("stitch.tga");

	CreateTexturePNG("R2D2_BaseColor.png");
	CreateTexturePNG("R2D2_ColorID.png");

	//Animation 1: Walk
	ConvertAnim("walk_mesh.glb");
	// 
	//Animation 2: Die Left
	ConvertAnim("die_left+mesh.glb");
	// 
	//Animation 3: Side Step Right
	ConvertAnim("sidestep_right+mesh.glb");
	// 
	//Animation 4:Run
	ConvertAnim("run+mesh.glb");

	//crate
	ConvertModel("crate.glb");
	CreateTexturePNG("crate_BaseColor.png");

	//frigate
	ConvertModel("space_frigate.glb");
	CreateTextureTGA("space_frigate.tga");


	//CreateCubeMesh();
	//CreateSphereMesh();	
	CreatePyramidMesh();

	//CreateTextureTGA("Duckweed.tga");
	//CreateTextureTGA("RedBrick.tga");
	//CreateTextureTGA("Rocks.tga");
	//CreateTextureTGA("Stone.tga");
	CreateTexturePNG("ChickenbotDiffuse.png");  //<--- use this for the model
	CreateChickenBotModels();


	// clean up data directory
	system("del *.png");
	system("del *.tga");
	system("del *.glb");
	system("del *.xml");

	google::protobuf::ShutdownProtobufLibrary();

	return 0;
}

// ---  End of File ---------------
