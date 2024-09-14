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
#include <google/protobuf/stubs/common.h>


using namespace Azul;

int main()
{
	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	// Sprite
	CreateSpriteMesh("Sprite");

	// Font stuff
	CreateTexturePNG("FontArial36.png","Arial36");
	ConvertXML("MetricsArial36","Arial36");
	
	
	//Mousey
	CreateTexturePNG("MouseyDiffuse.png", "Mousey");
	ConvertSkin("MouseyT", "Mousey");
	//Convert Joint?
	ConvertSkeleton("MouseyT", "Mousey");
	ConvertHierarchy("MouseyT", "Mousey");
	ConvertInvBind("MouseyT", "Mousey");

	//Mousey Animations
	ConvertAnim("sillydancing+mesh", "Mousey_SillyDance");
	ConvertAnim("zombiewalk+mesh", "Mousey_ZombieWalk");
	ConvertAnim("Strut Walking", "Mousey_StrutWalk");
	ConvertAnim("Situp to Idle", "Mousey_SitupToIdle");
	ConvertAnim("Punching", "Mousey_Punching");
	ConvertAnim("Jump", "Mousey_Jump");
	ConvertAnim("Breakdance Freeze Var 2", "Mousey_Breakdance");
	ConvertAnim("Falling", "Mousey_Falling");

	// clean up data directory
	system("del *.png");
	system("del *.tga");
	system("del *.glb");
	system("del *.xml");

	google::protobuf::ShutdownProtobufLibrary();

	return 0;
}

// ---  End of File ---------------
