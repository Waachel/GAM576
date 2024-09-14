//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "GLTF.h"
#include "meshData.h"
#include "meshDataConverter.h"

#include "json.hpp"
#include "File.h"
#include "MathEngine.h"
#include "ConvertAnim.h"
#include "Skeleton.h"
#include "Anim.h"

using namespace Azul;
using namespace tinygltf;
using json = nlohmann::json;

namespace Azul
{
	void ConvertAnim(const char *const pName, const char *const pTargetName)
	{
		assert(pName);

		char sBuff[40];
		sprintf_s(sBuff, 40, "Anim(%s)", pName);
		Trace::out("%-25s", sBuff);

	bool status;
	tinygltf::Model gltfModel;

	// runtime model
	meshData  runModel;

		// glb file name defined here:
		const char *pGLB_Name = pName;
		const char *pGLB_extension = ".glb";

		size_t FileNameSize = strlen(pGLB_Name) + strlen(pGLB_extension) + 1;

		char *poFileName = new char[FileNameSize]();
		strcat_s(poFileName, FileNameSize, pGLB_Name);
		strcat_s(poFileName, FileNameSize, pGLB_extension);

	// Load the gltfModel
		status = GLTF::LoadBinary(gltfModel, poFileName);
	assert(status);

	// Read glb into memory (raw buffer)
	char *poBuff = nullptr;
	unsigned int BuffSize(0);

		status = GLTF::GetGLBRawBuffer(poBuff, BuffSize, poFileName);
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





	//-----------------------------------
	// Time
	//-----------------------------------
	{
		//Trace::out("Time: \n");
		unsigned char *pBuff = (unsigned char *)&gltfModel.buffers[0].data[0];
		auto TimeAccessor = gltfModel.accessors[0];
		auto TimeBuffView = gltfModel.bufferViews[(size_t)TimeAccessor.bufferView];

		pBuff = pBuff + TimeBuffView.byteOffset;
		assert(TimeAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
		assert(TimeAccessor.type == TINYGLTF_TYPE_SCALAR);
		//float *pFloatData = (float *)pBuff;
		//Trace::out("start: %p\n", pBuff);
		for(size_t i = 0; i < TimeAccessor.count; i++)
		{
			//Trace::out("\t %d: %f \n", i, *pFloatData++);
		}
		//size_t len = ((size_t)pFloatData - (size_t)pBuff);
		//Trace::out("end: %p  len:%d\n", pFloatData, len);
//		assert(len == TimeBuffView.byteLength);
		//Trace::out("\n");
	}

	size_t FrameIndex = 20;

	GLTF::OutputTrans(gltfModel, 1, 4, FrameIndex);

	GLTF::OutputQuat(gltfModel, 2, 4, FrameIndex);
	GLTF::OutputQuat(gltfModel, 3, 5, FrameIndex);
	GLTF::OutputQuat(gltfModel, 4, 6, FrameIndex);
	GLTF::OutputQuat(gltfModel, 5, 7, FrameIndex);
	GLTF::OutputQuat(gltfModel, 6, 8, FrameIndex);
	GLTF::OutputQuat(gltfModel, 7, 9, FrameIndex);
	GLTF::OutputQuat(gltfModel, 8, 10, FrameIndex);


	//-----------------------------------
	// Nodes
	//-----------------------------------

	for(size_t i = 0; i < gltfModel.nodes.size(); i++)
	{
		auto pNode = gltfModel.nodes[i];
		//Trace::out("%d: %s \n", i, pNode.name.c_str());
		for(size_t j = 0; j < pNode.children.size(); j++)
		{
			//Trace::out("\t child: (%d) %s \n", pNode.children[j], gltfModel.nodes[ (size_t)pNode.children[j]].name.c_str());
		}

		//Trace::out("\n");
		//Trace::out("\t Vec3(%ff,%ff,%ff);\n", pNode.translation[0], pNode.translation[1], pNode.translation[2]);
		//Trace::out("\t Quat(%ff,%ff,%ff,%ff);\n", pNode.rotation[0], pNode.rotation[1], pNode.rotation[2],pNode.rotation[3]);
		//Trace::out("\t Vec3(%ff,%ff,%ff);\n", pNode.scale[0], pNode.scale[1], pNode.scale[2]);
		//Trace::out("\n");
	}

	//Trace::out("\n");

	
	//-----------------------------------
	// Nodes
	//-----------------------------------

	for(size_t i = 0; i < gltfModel.nodes.size(); i++)
	{
		auto pNode = gltfModel.nodes[i];
		//Trace::out("%d: %s \n", i, pNode.name.c_str());
		for(size_t j = 0; j < pNode.children.size(); j++)
		{
			//Trace::out("\t child: (%d) %s \n", pNode.children[j], gltfModel.nodes[ (size_t)pNode.children[j]].name.c_str());
		}
	}

	//Trace::out("\n");



	File::Handle fh;
	File::Error err;
	const char* pClip_Name = pName;
	const char* pClip_extension = ".clip";

	FileNameSize = strlen(pClip_Name) + strlen(pClip_extension) + 1;

	char* poFileClipName = new char[FileNameSize]();
	strcat_s(poFileClipName, FileNameSize, pClip_Name);
	strcat_s(poFileClipName, FileNameSize, pClip_extension);


	std::string OutputFileName = poFileClipName;
	Trace::out(" --> %22s\n", OutputFileName.c_str());

	err = File::Open(fh, OutputFileName.c_str(), File::Mode::WRITE);
	assert(err == File::Error::SUCCESS);

	auto Accessor = gltfModel.accessors[1];


	//Trans 4
	unsigned char* pBuffT = (unsigned char*)&gltfModel.buffers[0].data[0];
	auto TransAccessor = gltfModel.accessors[1];
	auto TransBuffView = gltfModel.bufferViews[(size_t)TransAccessor.bufferView];

	pBuffT = pBuffT + TransBuffView.byteOffset;
	assert(TransAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
	assert(TransAccessor.type == TINYGLTF_TYPE_VEC3);
	Vec3f* pVect3 = (Vec3f*)pBuffT;

	//File::Write(fh, pVect3, sizeof(Vec3));
	//assert(err == File::Error::SUCCESS);
	pVect3++;

	Trace::out("\t pTmp->poBone[0].T = Vec3(%ff, %ff, %f);  \n",
		pVect3->x, pVect3->y, pVect3->z);

	//Rotation - Quat 4
	unsigned char* pBuffQ = (unsigned char*)&gltfModel.buffers[0].data[0];
	auto QuatAAccessor = gltfModel.accessors[2];
	auto QuatABuffView = gltfModel.bufferViews[(size_t)QuatAAccessor.bufferView];

	pBuffQ = pBuffQ + QuatABuffView.byteOffset;
	assert(QuatAAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
	assert(QuatAAccessor.type == TINYGLTF_TYPE_VEC4);
	Vec4f* pVect4Q4 = (Vec4f*)pBuffQ;

	//Rotation - Quat 5
	pBuffQ = (unsigned char*)&gltfModel.buffers[0].data[0];
	QuatAAccessor = gltfModel.accessors[3];
	QuatABuffView = gltfModel.bufferViews[(size_t)QuatAAccessor.bufferView];

	pBuffQ = pBuffQ + QuatABuffView.byteOffset;
	assert(QuatAAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
	assert(QuatAAccessor.type == TINYGLTF_TYPE_VEC4);
	Vec4f* pVect4Q5 = (Vec4f*)pBuffQ;

	//Rotation - Quat 6
	pBuffQ = (unsigned char*)&gltfModel.buffers[0].data[0];
	QuatAAccessor = gltfModel.accessors[4];
	QuatABuffView = gltfModel.bufferViews[(size_t)QuatAAccessor.bufferView];

	pBuffQ = pBuffQ + QuatABuffView.byteOffset;
	assert(QuatAAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
	assert(QuatAAccessor.type == TINYGLTF_TYPE_VEC4);
	Vec4f* pVect4Q6 = (Vec4f*)pBuffQ;


	//Rotation - Quat 7
	pBuffQ = (unsigned char*)&gltfModel.buffers[0].data[0];
	QuatAAccessor = gltfModel.accessors[5];
	QuatABuffView = gltfModel.bufferViews[(size_t)QuatAAccessor.bufferView];

	pBuffQ = pBuffQ + QuatABuffView.byteOffset;
	assert(QuatAAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
	assert(QuatAAccessor.type == TINYGLTF_TYPE_VEC4);
	Vec4f* pVect4Q7 = (Vec4f*)pBuffQ;

	//Rotation - Quat 8
	pBuffQ = (unsigned char*)&gltfModel.buffers[0].data[0];
	QuatAAccessor = gltfModel.accessors[6];
	QuatABuffView = gltfModel.bufferViews[(size_t)QuatAAccessor.bufferView];

	pBuffQ = pBuffQ + QuatABuffView.byteOffset;
	assert(QuatAAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
	assert(QuatAAccessor.type == TINYGLTF_TYPE_VEC4);
	Vec4f* pVect4Q8 = (Vec4f*)pBuffQ;

	//Rotation - Quat 9
	pBuffQ = (unsigned char*)&gltfModel.buffers[0].data[0];
	QuatAAccessor = gltfModel.accessors[7];
	QuatABuffView = gltfModel.bufferViews[(size_t)QuatAAccessor.bufferView];

	pBuffQ = pBuffQ + QuatABuffView.byteOffset;
	assert(QuatAAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
	assert(QuatAAccessor.type == TINYGLTF_TYPE_VEC4);
	Vec4f* pVect4Q9 = (Vec4f*)pBuffQ;

	//Rotation - Quat 10
	pBuffQ = (unsigned char*)&gltfModel.buffers[0].data[0];
	QuatAAccessor = gltfModel.accessors[8];
	QuatABuffView = gltfModel.bufferViews[(size_t)QuatAAccessor.bufferView];

	pBuffQ = pBuffQ + QuatABuffView.byteOffset;
	assert(QuatAAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT);
	assert(QuatAAccessor.type == TINYGLTF_TYPE_VEC4);
	Vec4f* pVect4Q10 = (Vec4f*)pBuffQ;

	//which frame we are on
	for (size_t i = 0; i < Accessor.count; i++)
	{

		//4
		File::Write(fh, pVect4Q4, sizeof(Vec4));
		assert(err == File::Error::SUCCESS);

		Trace::out("\t 4.Q = Quat(%ff, %ff, %ff, %ff);\n",
			pVect4Q4->x, pVect4Q4->y, pVect4Q4->z, pVect4Q4->w);

		pVect4Q4++;

		//5
		File::Write(fh, pVect4Q5, sizeof(Vec4));
		assert(err == File::Error::SUCCESS);

		Trace::out("\t 5.Q = Quat(%ff, %ff, %ff, %ff);\n",
			pVect4Q5->x, pVect4Q5->y, pVect4Q5->z, pVect4Q5->w);

		pVect4Q5++;

		//6
		File::Write(fh, pVect4Q6, sizeof(Vec4));
		assert(err == File::Error::SUCCESS);

		Trace::out("\t 6.Q = Quat(%ff, %ff, %ff, %ff);\n",
			pVect4Q6->x, pVect4Q6->y, pVect4Q6->z, pVect4Q6->w);

		pVect4Q6++;


		//7
		File::Write(fh, pVect4Q7, sizeof(Vec4));
		assert(err == File::Error::SUCCESS);

		Trace::out("\t 7.Q = Quat(%ff, %ff, %ff, %ff);\n",
			pVect4Q7->x, pVect4Q7->y, pVect4Q7->z, pVect4Q7->w);

		pVect4Q7++;


		//8
		File::Write(fh, pVect4Q8, sizeof(Vec4));
		assert(err == File::Error::SUCCESS);

		Trace::out("\t 8.Q = Quat(%ff, %ff, %ff, %ff);\n",
			pVect4Q8->x, pVect4Q8->y, pVect4Q8->z, pVect4Q8->w);

		pVect4Q8++;

		//9
		File::Write(fh, pVect4Q9, sizeof(Vec4));
		assert(err == File::Error::SUCCESS);

		Trace::out("\t 9.Q = Quat(%ff, %ff, %ff, %ff);\n",
			pVect4Q9->x, pVect4Q9->y, pVect4Q9->z, pVect4Q9->w);

		pVect4Q9++;

		//10
		File::Write(fh, pVect4Q10, sizeof(Vec4));
		assert(err == File::Error::SUCCESS);

		Trace::out("\t 10.Q = Quat(%ff, %ff, %ff, %ff);\n",
			pVect4Q10->x, pVect4Q10->y, pVect4Q10->z, pVect4Q10->w);

		pVect4Q10++;
	}


	// -------------------------------
	//  Write to file
	//--------------------------------
	
		err = File::Close(fh);
		assert(err == File::Error::SUCCESS);
	


	//-------------------------------------------------
	// Fill out the runtime Model data
	//-------------------------------------------------
	
	
		// Fill out the runtime Model data
	
		// Model Name
		memcpy_s(runModel.pMeshName, meshData::FILE_NAME_SIZE, gltfModel.materials[0].name.c_str(), gltfModel.materials[0].name.length());

		delete[] poBuff;

		meshData_proto mA_proto;
		runModel.Serialize(mA_proto);

		// ANIMATION
		ANIM animData;
		animData.CreateAnim(gltfModel, pTargetName);

		delete[] poFileName;
		delete[] poFileClipName;
		delete[] poJSON;
	
	}
}

// --- End of File ---
