#include "Renderer.h"
#include "Renderer/Debug.h"
#include "Renderer.h"
#include <Renderer/Scene.h>

RendererData Renderer::renderData;

void Renderer::Init()
{
	RenderCommand::Init();
}

void Renderer::Clear()
{
	RenderCommand::Clear();
}


void Renderer::BeginScene(const Camera& camera )
{
	renderData.view = camera.GetViewMatrix();
	renderData.proj = camera.GetProjectionMatrix();
	
	Scene::sceneShader->UploadUniformMat4("view", renderData.view);
	Scene::sceneShader->UploadUniformMat4("proj", renderData.proj);
}


void Renderer::Render(const Primitive& primitive)
{
	//auto envLight = Scene::GetEnviromentLight();
	//const auto& attribs = primitive.GetVertexAttribs();
	//attribs.Bind();
	//const auto& shader = primitive.GetShader();
	//shader.UploadUniformMat4("view", view);
	//shader.UploadUniformMat4("proj", proj);
	//shader.UploadUniformMat4("model", primitive.GetTransform());

	//shader.UploadUniformVec3("LightPosition", envLight.lightPos);
	//shader.UploadUniformVec4("AmbientColor", envLight.ambientColor);
	//shader.UploadUniformVec4("LightColor", envLight.lightColor);
	//shader.UploadUniformFloat("AmbientStrength", envLight.ambientStrength);
	//shader.UploadUniformFloat("Shininess", .45f);
	//shader.UploadUniformVec3("ViewPosition", envLight.lightPos);
	//RenderCommand::DrawIndexed(attribs);

	
}

void Renderer::Render(const std::unique_ptr<Primitive>& primitive)
{
	const auto& envLight = Scene::GetEnviromentLight();
	const auto& light = Scene::GetLight();

	const auto& attribs = primitive->GetVertexAttribs();
	attribs.Bind();
	const auto& shader = *Scene::sceneShader;

	shader.UploadUniformMat4("model", primitive->GetTransform());
	shader.UploadUniformVec4("AmbientColor", envLight.Ambient);
	shader.UploadUniformFloat("AmbientStrength", envLight.Energy);
	//material
	shader.UploadUniformFloat("Shininess", 1.0f);
	shader.UploadUniformFloat("SpecularStrength", light.SpecularStrength);
	shader.UploadUniformVec4("ViewPosition", { renderData.view[3].x, renderData.view[3].y, renderData.view[3].z, 1.0f });

	RenderCommand::DrawIndexed(attribs);
	
}

void Renderer::EndScene()
{
}

void RenderCommand::Init()
{
	GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glDepthFunc(GL_LEQUAL));
}

void RenderCommand::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	glViewport(x, y, width, height);
}

void RenderCommand::SetClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void RenderCommand::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderCommand::DrawIndexed(const VertexArray& vertexArray)
{
	vertexArray.Bind();
	glDrawElements(GL_TRIANGLES, vertexArray.GetIndicies(), GL_UNSIGNED_INT, 0);
}

void Renderer2D::Init()
{

	uint32_t* quadIndices = new uint32_t[m_Data.MaxIndices];

	uint32_t offset = 0;
	for (uint32_t i = 0; i < m_Data.MaxIndices; i += 6)
	{
		quadIndices[i + 0] = offset + 0;
		quadIndices[i + 1] = offset + 1;
		quadIndices[i + 2] = offset + 2;

		quadIndices[i + 3] = offset + 2;
		quadIndices[i + 4] = offset + 3;
		quadIndices[i + 5] = offset + 0;

		offset += 4;
	}

	m_Data.m_vao = std::make_unique<VertexArray>();
	//m_Data.m_ib = std::make_unique<IndexBuffer>(quadIndices, m_Data.MaxIndices);
	m_Data.m_vb = std::make_unique<VertexBuffer>( m_Data.MaxVertices * sizeof(VertexAttribs));
	m_Data.m_vao->SetIndexBuffer(IndexBuffer(quadIndices, m_Data.MaxIndices));
	m_Data.shader = std::make_unique<Shader>("src/Shaders/default.shader");


	m_Data.vertexAttribBase = new VertexAttribs[m_Data.MaxVertices];
	m_Data.vertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
	m_Data.vertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
	m_Data.vertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
	m_Data.vertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	m_Data.textureSlotIndex = 0;
	m_Data.m_vb->SetLayout(
		{
			{GL_FLOAT, 0,  3, GL_FALSE},
			{GL_FLOAT, 1,  2, GL_FALSE},
			{GL_FLOAT, 2,  1, GL_FALSE},
		});

	m_Data.m_vao->AddBuffer(*m_Data.m_vb);
	delete[] quadIndices;

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
	int val[] = { 0, 1, 2, 3, 4, 5};
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
	for (uint32_t i = 0; i < m_Data.textureSlotIndex ; i++)
	{
		
		if (*m_Data.textures[i].get() == texture)
		{
			textureIndex = i + 1;
			break;
		}
	}

	if (textureIndex == 0.0f)
	{
		textureIndex = (float)m_Data.textureSlotIndex;
		m_Data.textures[m_Data.textureSlotIndex] = std::make_unique<Texture>(texture);
		m_Data.textureSlotIndex ++;
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

void Renderer2D::DrawSprite(const Sprite& sprite)
{
	DrawQuad(sprite.GetTransform(), sprite.GetTexture());
}


