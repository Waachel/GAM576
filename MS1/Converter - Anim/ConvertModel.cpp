//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
#include "GLTF.h"
#include "GLTF_Helper.h"
#include "meshData.h"
#include "meshDataConverter.h"
#include "BoundingSphere.h"
#include "json.hpp"
#include "File.h"

using namespace Azul;
using namespace tinygltf;
using json = nlohmann::json;
bool firstMeshDone = false;

void ConvertModel(const char *const pFileName)
{
	bool status;
	tinygltf::Model gltfModel;

	// runtime model
	meshData  runModel;

	Trace::out("%12s ", pFileName);

	// Strip the extension .tga off the name
	unsigned int len = strlen(pFileName);
	char *pTmp = new char[len + 1]();
	memset(pTmp, 0, len);
	memcpy(pTmp, pFileName, len - strlen(".glb"));

	// base name
	std::string BaseName = pTmp;
	delete[] pTmp;

	// Load the gltfModel
	status = GLTF::LoadBinary(gltfModel, pFileName);
	assert(status);

	// Read glb into memory (raw buffer)
	char *poBuff = nullptr;
	unsigned int BuffSize(0);

	status = GLTF::GetGLBRawBuffer(poBuff, BuffSize, pFileName);
	assert(status);

	// Get GLB_Header
	GLB_header glbHeader;
	status = GLTF::GetGLBHeader(glbHeader, poBuff, BuffSize);
	assert(status);

	// Get Raw JSON
	char *poJSON = nullptr;
	unsigned int JsonSize(0);
	status = GLTF::GetRawJSON(poJSON, JsonSize, poBuff, BuffSize);
	assert(status);

	// Get the Binary Buffer Address
	char *pBinaryBuff = nullptr;
	unsigned int BinaryBuffSize = 0;
	status = GLTF::GetBinaryBuffPtr(pBinaryBuff, BinaryBuffSize, poBuff, BuffSize);
	assert(status);

	// Fill out the runtime Model data

	// Model Name
	memcpy_s(runModel.pMeshName, meshData::FILE_NAME_SIZE, gltfModel.materials[0].name.c_str(), gltfModel.materials[0].name.length());

	//Variables
	char* poNewBuff = nullptr;
	DWORD FileLength = 0;

	GLTF::SetVBO(gltfModel, "POSITION", runModel.vbo_vert, pBinaryBuff, 0);
	//runModel.vbo_vert.Print("vbo_vert");

	GLTF::SetVBO(gltfModel, "NORMAL", runModel.vbo_norm, pBinaryBuff, 0);
	//runModel.vbo_norm.Print("vbo_norm");

	GLTF::SetVBO(gltfModel, "TEXCOORD_0", runModel.vbo_uv, pBinaryBuff, 0);
	//runModel.vbo_uv.Print("vbo_uv");

	GLTF::SetVBO(gltfModel, "COLOR_0", runModel.vbo_color, pBinaryBuff, 0);
	//runModel.vbo_color.Print("vbo_color");

	GLTF::SetVBO_index(gltfModel, "INDICES", runModel.vbo_index, pBinaryBuff, 0);
	//runModel.vbo_index.Print("vbo_index");

	// PolyCount
	runModel.triCount = runModel.vbo_index.count / 3;
	runModel.vertCount = runModel.vbo_vert.count;

	// Bounding Sphere
	Sphere sphere;
	Vec3* pVerts = new Vec3[runModel.vertCount];

	for (unsigned int i = 0; i < runModel.vertCount; i++)
	{
		Vec3* pVec3 = (Vec3*) runModel.vbo_vert.poData;
		pVerts[i].set(pVec3[i].x(), pVec3[i].y(), pVec3[i].z());

	}
	RitterSphere(sphere, pVerts, runModel.vertCount);

	runModel.boundingSphereRadius = sphere.rad;
	runModel.boundingSphereCenter[0] = sphere.cntr[x];
	runModel.boundingSphereCenter[1] = sphere.cntr[y];
	runModel.boundingSphereCenter[2] = sphere.cntr[z];

	delete[] pVerts;

	// RenderMode
	runModel.mode = meshDataConverter::GetMode(gltfModel.meshes[0].primitives[0].mode);

	// Set texture from Image vector
	// Find index to color
	unsigned int colorIndex = 0;
	colorIndex = (unsigned int)gltfModel.materials[0].pbrMetallicRoughness.baseColorTexture.index;
	GLTF::SetTexture(gltfModel, "Color", runModel.text_color, pBinaryBuff);

	delete[] poBuff;

	meshData_proto mA_proto;
	runModel.Serialize(mA_proto);

	// -------------------------------
	//  Write to file
	//--------------------------------

	File::Handle fh;
	File::Error err;

	const char* pProtoFileType = nullptr;
	status = GLTF::GetAzulProtoType(pProtoFileType, runModel);
	assert(status);

	std::string OutputFileName = BaseName + pProtoFileType + ".proto.azul";
	Trace::out(" --> %22s\n", OutputFileName.c_str());

	err = File::Open(fh, OutputFileName.c_str(), File::Mode::WRITE);
	assert(err == File::Error::SUCCESS);

	std::string strOut;
	mA_proto.SerializeToString(&strOut);

	File::Write(fh, strOut.data(), strOut.length());
	assert(err == File::Error::SUCCESS);

	err = File::Close(fh);
	assert(err == File::Error::SUCCESS);

	// -------------------------------
	// Read and recreate model data
	// -------------------------------

	err = File::Open(fh, OutputFileName.c_str(), File::Mode::READ);
	assert(err == File::Error::SUCCESS);

	err = File::Seek(fh, File::Position::END, 0);
	assert(err == File::Error::SUCCESS);

	err = File::Tell(fh, FileLength);
	assert(err == File::Error::SUCCESS);

	poNewBuff = new char[FileLength]();
	assert(poNewBuff);

	err = File::Seek(fh, File::Position::BEGIN, 0);
	assert(err == File::Error::SUCCESS);

	err = File::Read(fh, poNewBuff, FileLength);
	assert(err == File::Error::SUCCESS);

	err = File::Close(fh);
	assert(err == File::Error::SUCCESS);

	std::string strIn(poNewBuff, FileLength);
	meshData_proto mB_proto;

	mB_proto.ParseFromString(strIn);

	meshData mB;
	mB.Deserialize(mB_proto);
	//mB.Print("mB");
	delete[] poJSON;

	delete[] poNewBuff;

	int meshSize = gltfModel.meshes.size();

	if (meshSize > 1)
	{
		for (int i = 1; i < meshSize; i++)
		{
			bool status2;
			tinygltf::Model gltfModel2;

			// runtime model
			meshData  runModel2;

			Trace::out("%12s ", pFileName);

			// Strip the extension .tga off the name
			unsigned int len2 = strlen(pFileName);
			char* pTmp2 = new char[len2 + 1]();
			memset(pTmp2, 0, len2);
			memcpy(pTmp2, pFileName, len2 - strlen(".glb"));

			// base name
			std::string BaseName2 = pTmp2;
			delete[] pTmp2;

			// Load the gltfModel2
			status2 = GLTF::LoadBinary(gltfModel2, pFileName);
			assert(status2);

			// Read glb into memory (raw buffer)
			char* poBuff2 = nullptr;
			unsigned int BuffSize2(0);

			status2 = GLTF::GetGLBRawBuffer(poBuff2, BuffSize2, pFileName);
			assert(status2);

			// Get GLB_Header
			GLB_header glbHeader2;
			status2 = GLTF::GetGLBHeader(glbHeader2, poBuff2, BuffSize2);
			assert(status2);

			// Get Raw JSON
			char* poJSON2 = nullptr;
			unsigned int JsonSize2(0);
			status2 = GLTF::GetRawJSON(poJSON2, JsonSize2, poBuff2, BuffSize2);
			assert(status2);

			// Get the Binary Buffer Address
			char* pBinaryBuff2 = nullptr;
			unsigned int BinaryBuffSize2 = 0;
			status2 = GLTF::GetBinaryBuffPtr(pBinaryBuff2, BinaryBuffSize2, poBuff2, BuffSize2);
			assert(status2);

			// Fill out the runtime Model data

			// Model Name
			memcpy_s(runModel2.pMeshName, meshData::FILE_NAME_SIZE, gltfModel2.materials[0].name.c_str(), gltfModel2.materials[0].name.length());

			//Variables
			char* poNewBuff2 = nullptr;
			DWORD FileLength2 = 0;

			GLTF::SetVBO(gltfModel2, "POSITION", runModel2.vbo_vert, pBinaryBuff2, i);
			//runModel2.vbo_vert.Print("vbo_vert");

			GLTF::SetVBO(gltfModel2, "NORMAL", runModel2.vbo_norm, pBinaryBuff2, i);
			//runModel2.vbo_norm.Print("vbo_norm");

			GLTF::SetVBO(gltfModel2, "TEXCOORD_0", runModel2.vbo_uv, pBinaryBuff2, i);
			//runModel2.vbo_uv.Print("vbo_uv");

			GLTF::SetVBO(gltfModel2, "COLOR_0", runModel2.vbo_color, pBinaryBuff2, i);
			//runModel2.vbo_color.Print("vbo_color");

			GLTF::SetVBO_index(gltfModel2, "INDICES", runModel2.vbo_index, pBinaryBuff2, i);
			//runModel2.vbo_index.Print("vbo_index");

			// PolyCount
			runModel2.triCount = runModel2.vbo_index.count / 3;
			runModel2.vertCount = runModel2.vbo_vert.count;

			// RenderMode
			runModel2.mode = meshDataConverter::GetMode(gltfModel2.meshes[i].primitives[0].mode);

			// Set texture from Image vector
			// Find index to color
			unsigned int colorIndex2 = 0;
			colorIndex2 = (unsigned int)gltfModel2.materials[0].pbrMetallicRoughness.baseColorTexture.index;
			GLTF::SetTexture(gltfModel2, "Color", runModel2.text_color, pBinaryBuff2);

			delete[] poBuff2;

			meshData_proto mA_proto2;
			runModel2.Serialize(mA_proto2);

			// -------------------------------
			//  Write to file
			//--------------------------------

			File::Handle fh2;
			File::Error err2;

			const char* pProtoFileType2 = nullptr;
			status2 = GLTF::GetAzulProtoType(pProtoFileType2, runModel2);
			assert(status2);

			std::string OutputFileName2 = BaseName2 + "1" + pProtoFileType2 + ".proto.azul";
			Trace::out(" --> %22s\n", OutputFileName2.c_str());

			err2 = File::Open(fh2, OutputFileName2.c_str(), File::Mode::WRITE);
			assert(err2 == File::Error::SUCCESS);

			std::string strOut2;
			mA_proto2.SerializeToString(&strOut2);

			File::Write(fh2, strOut2.data(), strOut2.length());
			assert(err2 == File::Error::SUCCESS);

			err2 = File::Close(fh2);
			assert(err2 == File::Error::SUCCESS);

			// -------------------------------
			// Read and recreate model data
			// -------------------------------

			err2 = File::Open(fh2, OutputFileName2.c_str(), File::Mode::READ);
			assert(err2 == File::Error::SUCCESS);

			err2 = File::Seek(fh2, File::Position::END, 0);
			assert(err2 == File::Error::SUCCESS);

			err2 = File::Tell(fh2, FileLength2);
			assert(err2 == File::Error::SUCCESS);

			poNewBuff2 = new char[FileLength2]();
			assert(poNewBuff2);

			err2 = File::Seek(fh2, File::Position::BEGIN, 0);
			assert(err2 == File::Error::SUCCESS);

			err2 = File::Read(fh2, poNewBuff2, FileLength2);
			assert(err2 == File::Error::SUCCESS);

			err2 = File::Close(fh2);
			assert(err2 == File::Error::SUCCESS);

			std::string strIn2(poNewBuff2, FileLength2);
			meshData_proto mB2_proto2;

			mB2_proto2.ParseFromString(strIn2);

			meshData mB2;
			mB2.Deserialize(mB2_proto2);
			//mB2.Print("mB2");
			delete[] poJSON2;

			delete[] poNewBuff2;
		}
	}

}

// --- End of File ---
