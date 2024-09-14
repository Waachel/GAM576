//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MESH_H
#define MESH_H

#include <d3d11.h>
#include "DLink.h"
#include "MathEngine.h"
#include "Camera.h"
#include "MeshLayout.h"

namespace Azul
{
	enum class VertexSlot : uint32_t
	{
		Position,  //0
		Color,     //1
		Norm,      //2
		TexCoord   //3
	};

	enum class ConstantBufferSlot : uint32_t
	{
		Projection,
		View,
		World,
		Color,
		LightPos,
		// w1(6)
		UV_Correction,
	};

	class Mesh
	{
	public:
		enum class Name
		{
			CUBE,
			PYRAMID,
			CRATE,
			SPHERE,
			R2D2,
			CAMERA,
			SPACE_FRIGATE,
			Bone,
			Bone_001,
			Bone_L,
			Bone_L_001,
			Bone_L_002,
			Bone_R,
			Bone_R_001,
			Bone_R_002,
			SPRITE,
			NULL_MESH,
			NOT_INITIALIZED
		};

	public:
		Mesh() = delete;
		Mesh(Mesh &copyMesh) = delete;
		Mesh &operator = (Mesh &copyMesh) = delete;
		virtual ~Mesh();

		Mesh(unsigned int numVerts, unsigned int numTriangles);

		ID3D11Buffer *CreateVertexBuffer(unsigned int NumBytes, void *pData);
		ID3D11Buffer *CreateIndexBuffer(unsigned int NumBytes, void *pData);
		ID3D11Buffer *CreateConstantBuffer(unsigned int NumBytes);

		void Wash();
		bool Compare(DLink *pTarget);
		char *GetName();
		void SetName(Mesh::Name _name);

		void ActivateMesh();
		void TransferConstantBuffer(Camera *pCam, Mat4 *pWorld);
		void RenderIndexBuffer();

		// w1(6)
		void TransferUVCorrection(Mat4 *pUVMatrix);

		// ---------------------------------------------
		// 	   Data:
		// ---------------------------------------------

		Name name;

		unsigned int numVerts;
		unsigned int numIndices;

		// Vertex buffer data
		ID3D11Buffer *poVertexBuffer_pos;
		ID3D11Buffer *poVertexBuffer_color;
		ID3D11Buffer *poVertexBuffer_norm;
		ID3D11Buffer *poVertexBuffer_texCoord;

		ID3D11Buffer *poIndexBuffer;

		// Shader resources - guaranteed every render
		ID3D11Buffer *poConstantBuff_Projection;
		ID3D11Buffer *poConstantBuff_World;
		ID3D11Buffer *poConstantBuff_View;

		// Shader resources - conditional depending on shader
		ID3D11Buffer *poConstantBuff_lightColor;
		ID3D11Buffer *poConstantBuff_lightPos;

		// w1(6)
		// for sprite texture
		ID3D11Buffer *poConstantBuff_uvMatrix;
	};
}

#endif

// --- End of File ---
