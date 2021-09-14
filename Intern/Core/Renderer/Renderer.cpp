#include <glpch.h>
#include "Renderer.h"
#include "RenderAPI.h"
#include "Debug.h"
#include "Renderer.h"
#include "Scene.h"
#include "Components/MeshRendererComponent.h"
#include "Input/Input.h"
#include "Thread.h"
#include "Buffers/FrameBuffer.h"
#include "stb_image_write.h"
#include "Profiler.h"

void RenderCommand::Init()
{
	GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glDepthFunc(GL_LESS));
	GLCall(glEnable(GL_CULL_FACE));
}

void RenderCommand::DrawIndexed(const VertexArray& vertexArray)
{
	GLCall(glDrawElements(GL_TRIANGLES, vertexArray.GetIndicies(), GL_UNSIGNED_INT, 0));
}

void RenderCommand::DrawIndexed(const uint32_t& p_indices)
{
	//PROFILE_FUNCTION
	GLCall(glDrawElements(GL_TRIANGLES, p_indices, GL_UNSIGNED_INT, 0));
}

void RenderCommand::RenderLines(const VertexArray& vertexArray)
{
	glDrawArrays(GL_LINES, 0, vertexArray.GetIndicies());
}

//
//
//RendererData Renderer::renderData;
TT Renderer::testRenderData;
//
//static Vector3 temp_aabbVertices[] =
//{
//	{-1.f, -1.f, -1.f}, // 0
//	{ 1.f, -1.f, -1.f}, // 1
//	{ 1.f, 1.f, -1.f},  // 2
//	{ -1.f, 1.f, -1.f}, // 3	
//
//	{-1.f, -1.f, 1.f}, // 4
//	{ 1.f, -1.f, 1.f}, // 5
//	{ 1.f, 1.f,  1.f},  // 6
//	{ -1.f, 1.f, 1.f}, // 7
//};
//
//static Vector3 aabVertices[24];
//
//static uint32_t indices[] = { 0, 1, 1, 2, 2, 3, 3, 0, 0, 4, 4, 5, 5, 1, 5, 6, 2, 6, 6, 7, 7, 3, 7, 4 };
//std::vector<Mesh> Renderer::s_renderMeshes;


RenderQueue Renderer::renderQueue;

void Renderer::Init()
{
	RenderCommand::Init();
	float testRender[] = {
	 -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
	 -0.5f,  0.5f, 0.0f, 0.0f, 1.0f };

	uint32_t quadIndices[] = { 0, 1, 2, 2, 3, 0 };

	testRenderData.m_VertexArray = std::make_unique<VertexArray>();
	testRenderData.m_VertexBuffer = std::make_unique<VertexBuffer>(testRender, sizeof(testRender));
	testRenderData.shader = std::make_unique<Shader>("Assets/Shaders/depthtest.glsl");
	testRenderData.m_VertexBuffer->SetLayout({ { AttribDataType::T_FLOAT, Attrib::VERTEXPOSITION, AttribCount::VEC3, false }, { AttribDataType::T_FLOAT, Attrib::UV, AttribCount::VEC2, false } });
	testRenderData.m_VertexArray->SetIndices(quadIndices, 6);
	testRenderData.m_VertexArray->AddBuffer(*testRenderData.m_VertexBuffer.get());
}

void Renderer::Clear()
{

}
void Renderer::SetClearColor(float r, float g, float b, float a)
{
	RenderAPI::SetClearColor(r, g, b, a);
}

void Renderer::PushPass(RenderPass&& renderPass)
{
	renderQueue.renderPasses.push_back(renderPass);
}

void Renderer::ShutDown()
{
	//testRenderData.m_aabbVertexArray.reset();
	//testRenderData.m_aabbVertexBuffer.reset();
	//testRenderData.shader.reset();
}

void Renderer::RenderSkybox(const RenderData& renderData)
{
	// this so wrong but it'll go away later
//	RenderAPI::ClearBuffers();
	RenderAPI::DisableCullFace();
	SkyBox* skybox = SkyBox::GetSingleton();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	Matrix4x4 viewMat = Matrix4x4(*renderData.cameraData->view);
	viewMat[3] = { 0, 0, 0, 1.f };

	skybox->BeginRender(*renderData.cameraData->proj, viewMat);
	RenderCommand::DrawIndexed(skybox->GetIndices());
	RenderAPI::EnableCullFace();
}

void Renderer::BeginScene(const RenderData& renderData)
{
}


void Renderer::RenderShadows(const RenderData& renderData)
{
	Scene* scene = Scene::GetActiveScene();
	const auto& shader = scene->shadowShader;
	std::shared_ptr<ShadowData> shadowData = renderData.shadowData;
	const auto& shadowBox = shadowData->shadowBounds;
	
	shader->Bind();
	shader->UploadUniformMat4("lightSpaceMatrix", shadowData->ProjView);

	scene->BindFBO(FrameBufferName::DEPTH);
	const Vector2& shadowMapWidth = shadowData->ShadowSize;
	RenderAPI::SetViewport(0, 0, shadowMapWidth.x, shadowMapWidth.y);
	RenderAPI::ClearDepthBuffer();
	//RenderAPI::CullFrontFace();

	const std::vector<Mesh>& meshes = renderData.meshes;
	for (const auto& mesh : meshes)
	{
		const auto& attribs = mesh.GetVertexAttribs();
		attribs.Bind();
		const auto& material = mesh.GetMaterial();
		scene->shadowShader->UploadUniformMat4("model", mesh.GetTransform().GetWorldMatrix());
		RenderCommand::DrawIndexed(attribs);
	//	RenderCommand::DrawIndexed(attribs);
	//	RenderCommand::DrawIndexed(attribs);
	}

	//RenderAPI::CullBackFace();
}

void Renderer::RenderAABB(const RenderData& renderData)
{

#ifdef RENDER_AABB

	RenderCommand::DrawIndexed(attribs);
	drawCalls++;

	Vector3 a;
	Vector3 b;

	int c = 0;
	int d = 1;
	for (int i = 0; i < 12; i++)
	{
		mesh.GetAABB().GetEdge(i, a, b);
		aabVertices[c] = a;
		aabVertices[d] = b;
		c += 2;
		d += 2;
	}

	renderData.shader->Bind();
	renderData.m_aabbVertexArray->Bind();
	renderData.m_aabbVertexBuffer->BufferSubData(aabVertices, 0, sizeof(aabVertices));
	renderData.shader->UploadUniformMat4("projView", renderData.proj * renderData.view);
	renderData.shader->UploadUniformMat4("model", mesh.GetTransform().GetWorldMatrix());
	RenderCommand::RenderLines(*renderData.m_aabbVertexArray);
	shader.Bind();

#endif // RENDER_AABB

}

void Renderer::RenderMeshes(const RenderData& renderData)
{
	RenderAPI::BindFrameBuffer(0);
	RenderAPI::ClearBuffers();
	{
		const Size2& fboSize = Display::GetSingleton()->GetWindowSize(MAIN_WINDOW_ID);
		RenderAPI::SetViewport(0, 0, fboSize.x, fboSize.y);
	}

	const auto& shader = renderData.shader;
	shader->Bind();
	shader->UploadUniformMat4("projView", *renderData.cameraData->proj * *renderData.cameraData->view);
	shader->UploadUniformMat4("shadowSpaceMatrix", renderData.shadowData->Bias * renderData.shadowData->ProjView);


#ifdef DRAW_DEPTH_MAP

	testRenderData.shader->Bind();
	testRenderData.shader->SetInt("depthMap", 0);
	testRenderData.m_VertexArray->Bind();
	RenderAPI::EnableVertexAttribArray(Attrib::UV);
	Scene::GetActiveScene()->BindFBOTex(FrameBufferTexture::SHADOWMAP);
	//glActiveTexture(GL_TEXTURE0);
	RenderCommand::DrawIndexed(*testRenderData.m_VertexArray);

#else // _DEBUG



	shader->UploadUniformInt("depthTexture", 0);
	Scene::GetActiveScene()->BindFBOTex(FrameBufferTexture::SHADOWMAP);
	const std::vector<Mesh>& meshes = renderData.meshes;

	for (const auto& mesh : meshes)
	{
		const auto& attribs = mesh.GetVertexAttribs();
		attribs.Bind();

		const auto& material = mesh.GetMaterial();
		if (mesh.GetMaterial().Diffuse != -1)
		{
			shader->UploadUniformInt("diffuseTexture", Model::TEX_DIFFUSE);
			mesh.GetModelInstance()->ActiveTexture(mesh.GetMaterial().Diffuse, Model::TEX_DIFFUSE);
			mesh.GetModelInstance()->BindTexture(mesh.GetMaterial().Diffuse, Model::TEX_DIFFUSE);
			shader->UploadUniformInt("currentTex", mesh.GetMaterial().Diffuse);
		}

		shader->UploadUniformMat4("model", mesh.GetTransform().GetWorldMatrix());

		RenderCommand::DrawIndexed(attribs);
	}
#endif
}

void Renderer::FlushRenderQueue()
{
	for (const auto& renderPass : renderQueue.renderPasses)
	{
		switch (renderPass.Pass)
		{
		case RenderPass::SKYBOX:
		{
			Renderer::RenderSkybox(renderPass.renderData);
			break;
		}
		case RenderPass::DEPTH_PASS:
		{
			Renderer::RenderShadows(renderPass.renderData);
			break;
		}
		case RenderPass::COLOR_PASS:
		{
			Renderer::RenderMeshes(renderPass.renderData);
			break;
		}
		default:
			break;
		}
	}

	Renderer::EndScene();
}

void Renderer::SetViewport(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
}

void Renderer::EndScene()
{
	renderQueue.renderPasses.clear();
	Display::GetSingleton()->SwapBuffer();
}


void Renderer2D::Init()
{

	//uint32_t* quadIndices = new uint32_t[m_Data.MaxIndices];

	//uint32_t offset = 0;
	//for (uint32_t i = 0; i < m_Data.MaxIndices; i += 6)
	//{
	//	quadIndices[i + 0] = offset + 0;
	//	quadIndices[i + 1] = offset + 1;
	//	quadIndices[i + 2] = offset + 2;

	//	quadIndices[i + 3] = offset + 2;
	//	quadIndices[i + 4] = offset + 3;
	//	quadIndices[i + 5] = offset + 0;

	//	offset += 4;
	//}

	//m_Data.m_vao = std::make_unique<VertexArray>();
	////m_Data.m_ib = std::make_unique<IndexBuffer>(quadIndices, m_Data.MaxIndices);
	//m_Data.m_vb = std::make_unique<VertexBuffer>(m_Data.MaxVertices * sizeof(VertexAttribs));
	//m_Data.m_vao->SetIndexBuffer(IndexBuffer(quadIndices, m_Data.MaxIndices));
	//m_Data.shader = std::make_unique<Shader>("src/Shaders/default.shader");


	//m_Data.vertexAttribBase = new VertexAttribs[m_Data.MaxVertices];
	//m_Data.vertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
	//m_Data.vertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
	//m_Data.vertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
	//m_Data.vertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	//m_Data.textureSlotIndex = 0;
	//m_Data.m_vb->SetLayout(
	//	{
	//		{AttribDataType::T_FLOAT, Attrib::VERTEXPOSITION,  AttribCount::VEC3, false},
	//		{GL_FLOAT, 1,  2, GL_FALSE},
	//		{GL_FLOAT, 2,  1, GL_FALSE},
	//	});

	//m_Data.m_vao->AddBuffer(*m_Data.m_vb);
	//delete[] quadIndices;

}


void Renderer2D::BeginScene(const Camera& camera)
{
	m_Data.shader->SetMat4("viewProjection", camera.GetViewProjectionMatrix());
	StartBatch();
}

void Renderer2D::EndScene()
{
	Flush();
}

void Renderer2D::Flush()
{
	if (m_Data.quadIndexCount == 0)
		return;

	uint32_t size = (uint32_t)((uint8_t*)m_Data.vertexAttribPtr - (uint8_t*)m_Data.vertexAttribBase);
	int val[] = { 0, 1, 2, 3, 4, 5 };
	m_Data.shader->SetIntArray("tex", val, 6);

	for (uint32_t i = 0; i < m_Data.textureSlotIndex; i++)
		m_Data.textures[i]->Bind(i);

	m_Data.m_vb->BufferSubData(m_Data.vertexAttribBase, 0, size);
	m_Data.m_vao->Bind();
	m_Data.shader->Bind();
	RenderCommand::DrawIndexed(*m_Data.m_vao);
}


void Renderer2D::StartBatch()
{
	m_Data.vertexAttribPtr = m_Data.vertexAttribBase;
}

static int ci = 0;
void Renderer2D::DrawQuad(const Matrix4x4& transform, const Texture& texture)
{

	constexpr int quadVertexCount = 4;
	const Vector2 texCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };


	float textureIndex = 0.0f;
	for (uint32_t i = 0; i < m_Data.textureSlotIndex; i++)
	{

		if (*m_Data.textures[i].get() == texture)
		{
			textureIndex = float(i + 1);
			break;
		}
	}

	if (textureIndex == 0.0f)
	{
		textureIndex = (float)m_Data.textureSlotIndex;
		m_Data.textures[m_Data.textureSlotIndex] = std::make_unique<Texture>(texture);
		m_Data.textureSlotIndex++;
	}

	for (int i = 0; i < quadVertexCount; i++)
	{
		m_Data.vertexAttribPtr->Position = transform * m_Data.vertexPositions[i];
		m_Data.vertexAttribPtr->texCoord = texCoords[i];
		m_Data.vertexAttribPtr->texIndex = textureIndex;
		m_Data.vertexAttribPtr++;
	}

	m_Data.quadIndexCount += 6;

}
