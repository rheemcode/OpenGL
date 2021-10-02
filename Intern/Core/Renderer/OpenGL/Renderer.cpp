#include <glpch.h>
#include "Renderer.h"
#include "RenderAPI.h"
#include "Debug.h"
#include "Renderer.h"
#include "Scene.h"
#include "SkyBox.h"
#include "Components/MeshRendererComponent.h"
#include "Input/Input.h"
#include "Thread.h"
#include "Buffers/FrameBuffer.h"
#include "Buffers/GBuffer.h"
#include "Effects/SSAO.h"
#include "stb_image_write.h"
#include "Profiler.h"

void RenderCommand::Init()
{
	GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glDepthFunc(GL_LESS));
	//GLCall(glEnable(GL_MULTISAMPLE));
	GLCall(glEnable(GL_CULL_FACE));

	int enabled = 0;
	glGetIntegerv(GL_SAMPLE_BUFFERS, &enabled);
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

static Vector3 aabbVertices[24];

RenderQueue Renderer::renderQueue;

Renderer::QuadData Renderer::quadData;

void Renderer::Init()
{
	RenderCommand::Init();
	Texture::CreateDefaultTexture();

	float quad[] = {
	 -1.f, -1.f, 0.0f,  0.0f, 0.0f,
	 1.f, -1.f, 0.0f, 1.0f, 0.0f,
	 1.f,  1.f, 0.0f,  1.0f, 1.0f,
	 -1.f,  1.f, 0.0f, 0.0f, 1.0f };

	uint32_t quadIndices[] = { 0, 1, 2, 2, 3, 0 };

	quadData.m_VertexArray = std::make_unique<VertexArray>();
	quadData.m_VertexBuffer = std::make_unique<VertexBuffer>(quad, sizeof(quad));
	quadData.m_VertexBuffer->SetLayout({
		{ AttribDataType::T_FLOAT, Attrib::VERTEXPOSITION, AttribCount::VEC3, false },
		{ AttribDataType::T_FLOAT, Attrib::UV, AttribCount::VEC2, false },
		});
	quadData.m_VertexArray->SetIndices(quadIndices, 6);
	quadData.m_VertexArray->AddBuffer(*quadData.m_VertexBuffer.get());
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


void Renderer::RenderDepth(const RenderData& renderData)
{
	//PROFILE_FUNCTION
	const auto& shader = renderData.shader;
	std::shared_ptr<ShadowData> shadowData = renderData.shadowData;
	const auto& shadowBox = shadowData->shadowBounds;
	
	shader->Bind();
	//shader->UploadUniformMat4("lightSpaceMatrix", shadowData->ProjView);

	renderData.framebuffer->Bind(FramebufferName::DEPTH);
	const Vector2& shadowMapWidth = shadowData->ShadowSize;
	RenderAPI::SetViewport(0, 0, (uint32_t)shadowMapWidth.x, (uint32_t)shadowMapWidth.y);
	//RenderAPI::CullFrontFace();

	

	//for (int i = 0; i < shadowData->splitCount; i++)
	//{
	//	shader->UploadUniformMat4("lightSpaceMatrix", shadowData->Proj[i] * shadowData->View[i]);
		//renderData.framebuffer->BindTexture(FramebufferTexture::SHADOWMAPARRAY, i);
		RenderAPI::ClearDepthBuffer();
		for (int i = 0; i < renderData.meshCount; ++i)
		{
			const auto& mesh = renderData.meshes[i];
			const auto& attribs = mesh.GetVertexAttribs();
			attribs.Bind();
			//RenderAPI::DisableVertexAttribArray(Attrib::UV);
			//RenderAPI::DisableVertexAttribArray(Attrib::NORMAL);
			const auto& material = mesh.GetMaterial();
			shader->UploadUniformMat4("model", mesh.GetTransform().GetWorldMatrix());
			RenderCommand::DrawIndexed(attribs);
			//RenderAPI::EnableVertexAttribArray(Attrib::UV);
			//RenderAPI::EnableVertexAttribArray(Attrib::NORMAL);
		}
	//}

	//RenderAPI::CullBackFace();
}

void Renderer::RenderGBuffer(const RenderData& renderData)
{
	const auto* shader = renderData.gBuffer->m_bufferGenShader.get();
	renderData.gBuffer->BindFramebuffer();
	{
		const Size2& fboSize = Display::GetSingleton()->GetScreenSize(0);
		RenderAPI::SetViewport(0, 0, (uint32_t)fboSize.x, (uint32_t)fboSize.y);
	}

	RenderAPI::ClearBuffers();
	shader->Bind();
	for (int i = 0; i < renderData.meshCount; ++i)
	{
		const auto& mesh = renderData.meshes[i];
		const auto& attribs = mesh.GetVertexAttribs();
		attribs.Bind();

		if (mesh.GetMaterial().Diffuse != -1)
		{
			glActiveTexture(GL_TEXTURE0);
			mesh.GetModelInstance()->BindTexture(mesh.GetMaterial().Diffuse, Model::TEX_DIFFUSE);
		}

		else
		{
			glActiveTexture(GL_TEXTURE0);
			BIND_DEFAULT_TEXTURE();
		}

		renderData.uniformBuffer->UploadData(mesh.GetTransform().GetWorldMatrix(), 128);
		renderData.uniformBuffer->FlushBuffer();
		RenderCommand::DrawIndexed(attribs);
	}
}

void Renderer::RenderDeffered(const RenderData& renderData)
{
	// Render Framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
	{
		const Size2& fboSize = Display::GetSingleton()->GetWindowSize(MAIN_WINDOW_ID);
		RenderAPI::SetViewport(0, 0, (uint32_t)fboSize.x, (uint32_t)fboSize.y);
	}
	RenderAPI::ClearBuffers();
	
	renderData.gBuffer->BindShader(GBuffer::USE);
	quadData.m_VertexArray->Bind();
	renderData.gBuffer->BindAllTextures();
	auto* ssaoEffect = (SSAO*)renderData.postProcessEffect.get();
	Texture::ActiveTexture(Texture::TEXTURE3);
	ssaoEffect->BindFrambufferTexture();

	RenderCommand::DrawIndexed(6);

	// Copy Framebuffer's Depth Bit to default Framebuffer's Depth Bit
//	renderData.gBuffer->BindFramebuffer(Framebuffer::READ);
//	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
//	const Size2& fboSize = Display::GetSingleton()->GetWindowSize(MAIN_WINDOW_ID);
	//const Size2& scrSize = Display::GetSingleton()->GetScreenSize(0);
	//glBlitFramebuffer(0, 0, scrSize.x, scrSize.y, 0, 0, fboSize.x, fboSize.y, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	//glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
}

void Renderer::RenderSSAO(const RenderData& renderData)
{
	// RenderSSAO first;
	//PROFILE_FUNCTION
	auto* ssaoEffect = (SSAO*)renderData.postProcessEffect.get();
	ssaoEffect->BindFramebuffer();
	RenderAPI::ClearBuffers();
	const Size2& fboSize = Display::GetSingleton()->GetScreenSize(0);
	RenderAPI::SetViewport(0, 0, (uint32_t)fboSize.x, (uint32_t)fboSize.y);

	ComputeShader* shader = (ComputeShader*)ssaoEffect->GetShader();
	shader->Bind();

	Texture::ActiveTexture(Texture::TEXTURE0);
	renderData.gBuffer->BindTexture(FramebufferTexture::POSITION);
	Texture::ActiveTexture(Texture::TEXTURE1);
	renderData.gBuffer->BindTexture(FramebufferTexture::NORMAL);
	Texture::ActiveTexture(Texture::TEXTURE2);
	ssaoEffect->BindTexture();

	//shader->SetVec2("screenSize", fboSize);
	ssaoEffect->BindFramebufferImageTexture();
	Vector2 dispatchSize = Vector2(Math::Round(fboSize.x /8 ), Math::Round(fboSize.y / 8));
	shader->Dispatch(dispatchSize.x, dispatchSize.y, 1);
	//GLCall(glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT));


	//quadData.m_VertexArray->Bind();
	
	//RenderCommand::DrawIndexed(6);
}

void Renderer::RenderAABB(const RenderData& renderData)
{
	//RenderAPI::ClearBuffers();
	
	for (int i = 0; i < renderData.meshCount; ++i)
	{
		const auto& mesh = renderData.meshes[i];
		Vector3 a;
		Vector3 b;

		int c = 0;
		int d = 1;
		for (int i = 0; i < 12; i++)
		{
			mesh.GetAABB().GetEdge(i, a, b);
			aabbVertices[c] = a;
			aabbVertices[d] = b;
			c += 2;
			d += 2;
		}

		renderData.shader->Bind();
		renderData.vertexArray->Bind();
		renderData.vertexBuffer->BufferSubData(aabbVertices, 0, sizeof(aabbVertices));
		renderData.shader->UploadUniformMat4("projView", *renderData.cameraData->proj * *renderData.cameraData->view);
		renderData.shader->UploadUniformMat4("model", mesh.GetTransform().GetWorldMatrix());
		RenderCommand::RenderLines(*renderData.vertexArray.get());
	}
}

void Renderer::RenderMeshes(const RenderData& renderData)
{
	RenderAPI::BindFrameBuffer(0);
	RenderAPI::ClearBuffers();
	{
		const Size2& fboSize = Display::GetSingleton()->GetWindowSize(MAIN_WINDOW_ID);
		RenderAPI::SetViewport(0, 0, (uint32_t)fboSize.x, (uint32_t)fboSize.y);
	}

	const auto& shader = renderData.shader;
	shader->Bind();

	
	renderData.shadowData->UpdateFarBounds(*renderData.cameraData->proj);
	shader->UploadUniformVec4("farDistance", renderData.shadowData->farBound);
	const std::shared_ptr<UniformBuffer>& matricesBuffer = renderData.uniformBuffer;
	shader->UploadUniformInt("depthTexture", 0);
	//shader->UploadUniformInt("diffuseTexture", 1);
	renderData.framebuffer->BindTexture(FramebufferTexture::SHADOWMAP);
	for (int i = 0; i < renderData.meshCount; ++i)
	{
		const auto& mesh = renderData.meshes[i];
		const auto& attribs = mesh.GetVertexAttribs();
		attribs.Bind();

		const auto& material = mesh.GetMaterial();
		glActiveTexture(GL_TEXTURE0);
		//	BIND_DEFAULT_TEXTURE();
			//shader->UploadUniformInt("texture_diffuse1", 0);
		if (mesh.GetMaterial().Diffuse != -1)
		{
			mesh.GetModelInstance()->BindTexture(mesh.GetMaterial().Diffuse, Model::TEX_DIFFUSE);
			//shader->UploadUniformInt("diffuseTexture", Model::TEX_DIFFUSE);
			//mesh.GetModelInstance()->ActiveTexture(mesh.GetMaterial().Diffuse, Model::TEX_DIFFUSE);
			//mesh.GetModelInstance()->BindTexture(mesh.GetMaterial().Diffuse, Model::TEX_DIFFUSE);
			//s/hader->UploadUniformInt("currentTex", mesh.GetMaterial().Diffuse);
		}
		matricesBuffer->UploadData(mesh.GetTransform().GetWorldMatrix(), 128);
		matricesBuffer->FlushBuffer();
		RenderCommand::DrawIndexed(attribs);
	}
}

void Renderer::FlushRenderQueue()
{
	for (const auto& renderPass : renderQueue.renderPasses)
	{
		switch (renderPass.Pass)
		{
		case RenderPass::SKYBOX:
		{
			//Renderer::RenderSkybox(renderPass.renderData);
			break;
		}
		case RenderPass::DEPTH_PASS:
		{
			Renderer::RenderDepth(renderPass.renderData);
			break;
		}
		case RenderPass::COLOR_PASS:
		{
			Renderer::RenderMeshes(renderPass.renderData);
			break;
		}
		case RenderPass::DEFFERED_PASS:
		{
			Renderer::RenderDeffered(renderPass.renderData);
			break;
		}
		case RenderPass::GBUFFER_PASS:
		{
			Renderer::RenderGBuffer(renderPass.renderData);
			break;
		}
		case RenderPass::AABB:
		{
			Renderer::RenderAABB(renderPass.renderData);
			break;
		}
		case RenderPass::POSTPROCESS_PASS:
		{
			Renderer::RenderSSAO(renderPass.renderData);
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
//void Renderer2D::DrawQuad(const Matrix4x4& transform, const Texture& texture)
//{
//
//	constexpr int quadVertexCount = 4;
//	const Vector2 texCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
//
//
//	float textureIndex = 0.0f;
//	for (uint32_t i = 0; i < m_Data.textureSlotIndex; i++)
//	{
//
//	/*	if (*m_Data.textures[i].get() == texture)
//		{
//			textureIndex = float(i + 1);
//			break;
//		}*/
//	}
//
//	if (textureIndex == 0.0f)
//	{
//		textureIndex = (float)m_Data.textureSlotIndex;
//	//	m_Data.textures[m_Data.textureSlotIndex] = std::make_unique<Texture>(texture);
//		m_Data.textureSlotIndex++;
//	}
//
//	for (int i = 0; i < quadVertexCount; i++)
//	{
//		m_Data.vertexAttribPtr->Position = transform * m_Data.vertexPositions[i];
//		m_Data.vertexAttribPtr->texCoord = texCoords[i];
//		m_Data.vertexAttribPtr->texIndex = textureIndex;
//		m_Data.vertexAttribPtr++;
//	}
//
//	m_Data.quadIndexCount += 6;
//
//}
