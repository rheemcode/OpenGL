#pragma once
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Camera.h"
#include "OpenGL/VertexArray.h"
#include "OpenGL/Shader.h"
#include "Renderer/Buffers/UniformBuffer.h" 
#include "Texture.h"

struct GLIB_API RenderCommand
{
	static void Init();
	static void DrawIndexed(const VertexArray& vertexArray);
	static void DrawIndexed(const uint32_t& p_indices);
	static void RenderLines(const VertexArray& vertexArray);
};


struct GLIB_API VertexAttribs
{
	Vector3 Position;
	Vector2 texCoord;
	float texIndex = 0;
};

struct GLIB_API Renderer2DData {

	static const uint32_t MaxQuads = 3;
	static const uint32_t MaxVertices = MaxQuads * 4;
	static const uint32_t MaxIndices = MaxQuads * 6;
	static const uint32_t MaxTextureSlots = 1;

	std::unique_ptr<VertexArray> m_vao;
	std::unique_ptr<VertexBuffer> m_vb;
	std::unique_ptr<Shader> shader;

	std::array<std::unique_ptr<Texture>, 32> textures;

	VertexAttribs* vertexAttribBase = nullptr;
	VertexAttribs* vertexAttribPtr = nullptr;

	Vector4 vertexPositions[4];

	uint32_t quadIndexCount = 0;
	uint32_t textureSlotIndex = 0;
	uint32_t textureSlotIndexCount = 0;

	struct CameraData
	{
		Matrix4x4 ViewProjection;
	};
};

struct GLIB_API CameraData
{
	std::shared_ptr<Matrix4x4> view;
	std::shared_ptr<Matrix4x4> proj;
};

struct RenderData
{
	class Mesh** meshes;
	int meshCount;
	std::shared_ptr<Shader> shader;
	std::shared_ptr<CameraData> cameraData;
	std::shared_ptr<class Framebuffer> framebuffer;
	std::shared_ptr<class UniformBuffer> uniformBuffer;
	std::shared_ptr<class UniformBuffer> materialsBuffer;
	std::shared_ptr<class GBuffer> gBuffer;
	std::shared_ptr<class PostProcess> postProcessEffect;
	std::shared_ptr<VertexArray> vertexArray;
	std::shared_ptr<VertexBuffer> vertexBuffer;
};


struct RenderPass
{
	enum { CASCADED, SINGLE} ShadowType;
	enum { GBUFFER_PASS, DEPTH_PASS, COLOR_PASS, SKYBOX, AABB, POSTPROCESS_PASS, DEFFERED_PASS} Pass;
	RenderData renderData;
};

struct RenderQueue
{
	std::vector<RenderPass> renderPasses;
};



class Renderer
{
	static RenderQueue renderQueue;
	static struct QuadData 
	{
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<VertexArray> m_VertexArray;

	} quadData;


public:

	enum PRIMITIVE_MODE
	{
		LINES,
		LINE_STRIP,
		TRIANGLES,
		TRIANGLE_FAN,
		TRIANGLE_STRIP,
	};

	static void Init();
	static void Clear();
	static void ShutDown();

	static void SetClearColor(float r, float g, float b, float a);
	static void PushPass(RenderPass&& renderPass);
	static void BeginScene();
	static void RenderSkybox(const RenderData& p_renderData);
	static void RenderDepth(const RenderData& p_renderData);
	static void RenderGBuffer(const RenderData& renderData);
	static void RenderDeffered(const RenderData& renderData);
	static void RenderMeshes(const RenderData& p_renderData);
	static void RenderSSAO(const RenderData& p_renderData);
	static void RenderAABB(const RenderData& p_renderData);
	static void FlushRenderQueue();
	static void SetViewport(int x, int y, int width, int height);
	static void EndScene();

};


class GLIB_API Renderer2D
{
	Renderer2DData m_Data;

	void Flush();

public:
	void Init();
	void BeginScene(const Camera& camera);
	void StartBatch();
	void EndScene();
	//void DrawQuad(const Matrix4x4& transform, const Texture& texture);
};