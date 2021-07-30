#include "Renderer.h"
#include "Renderer/Debug.h"
#include "Renderer.h"
#include "Renderer/Scene.h"
#include "Components/MeshRendererComponent.h"
#include "Input/Input.h"

void RenderCommand::Init()
{
	GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glDepthFunc(GL_LEQUAL));
	GLCall(glEnable(GL_CULL_FACE));
	
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
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
	//vertexArray.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, vertexArray.GetIndicies(), GL_UNSIGNED_INT, 0));
//	glDrawArrays(GL_TRIANGLES, 0, vertexArray.GetIndicies());
}

void RenderCommand::DrawIndexed(const uint32_t& p_indices)
{
	//vertexArray.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, p_indices, GL_UNSIGNED_INT, 0));
//	glDrawArrays(GL_TRIANGLES, 0, vertexArray.GetIndicies());
}

void RenderCommand::RenderLines(const VertexArray& vertexArray)
{
	//glEnable(GL_POLYGON_OFFSET_FILL);
	//glPolygonOffset(.1f, .1f); // move polygon backward

	vertexArray.Bind();
//	GLCall(glDrawElements(GL_LINES, vertexArray.GetIndicies(), GL_UNSIGNED_INT, 0));
	glDrawArrays(GL_LINES, 0, 24);
	//glDisable(GL_POLYGON_OFFSET_FILL);
}

RendererData Renderer::renderData;
static Vector3 temp_aabbVertices[] =
{
	{-1.f, -1.f, -1.f}, // 0
	{ 1.f, -1.f, -1.f}, // 1
	{ 1.f, 1.f, -1.f},  // 2
	{ -1.f, 1.f, -1.f}, // 3	
	
	{-1.f, -1.f, 1.f}, // 4
	{ 1.f, -1.f, 1.f}, // 5
	{ 1.f, 1.f,  1.f},  // 6
	{ -1.f, 1.f, 1.f}, // 7
};

static Vector3 aabVertices[24];

static uint32_t indices[] = { 0, 1, 1, 2, 2, 3, 3, 0, 0, 4, 4, 5, 5, 1, 5, 6, 2, 6, 6, 7, 7, 3, 7, 4};

void Renderer::Init()
{
	renderData.m_aabbVertexArray = std::make_unique<VertexArray>();
	renderData.m_aabbVertexBuffer = std::make_unique<VertexBuffer>(sizeof(aabVertices));
	renderData.shader = std::make_unique<Shader>("Assets/Shaders/aabb.glsl");
	renderData.m_aabbVertexBuffer->SetLayout({ { GL_FLOAT, 0, 3, 0 } });
	renderData.m_aabbVertexArray->SetIndices(indices, 24);
	renderData.m_aabbVertexArray->AddBuffer(*renderData.m_aabbVertexBuffer.get());
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
	
	Scene::sceneShader->UploadUniformMat4("projView", renderData.proj * renderData.view);
	SkyBox* skybox = SkyBox::GetSingleton();

	Matrix4x4 viewMat = renderData.view;
	viewMat[3] = { 0, 0, 0, 1.f };
	skybox->BeginRender(renderData.proj * viewMat);
	RenderCommand::DrawIndexed(skybox->GetIndices());


	Scene::sceneShader->UploadUniformMat4("proj", renderData.proj);
}

void Renderer::Render(const Primitive& primitive)
{	
}

static int drawCalls = 0;

void Renderer::Render(const AABB& p_aabb)
{

}

static bool shouldCull = false;

void Renderer::Render(std::shared_ptr<MeshRendererComponent> p_rendererComponent, const Frustum& p_frustum)
{
	const auto& envLight = Scene::GetEnviromentLight();
	const auto& lights = Scene::GetLight();
	const auto& shader = *Scene::sceneShader;
	shader.Bind();
	for (int i = 0; i < Scene::GetLightCount(); ++i)
	{
		auto& light = lights[i];
		std::stringstream ss;

		if (light->LightSource == light->DIRECTIONAL_LIGHT)
		{
			ss << "Lights[" << i << "]." << "LightType";
			shader.UploadUniformInt(ss.str(), light->LightSource);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Ambient";
			shader.UploadUniformVec3(ss.str(), envLight.Ambient);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Color";
			shader.UploadUniformVec3(ss.str(), light->LightColor);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Direction";
			shader.UploadUniformVec3(ss.str(), light->Direction);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Use";
			shader.UploadUniformInt(ss.str(), light->Use);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Energy";
			shader.UploadUniformFloat(ss.str(), light->Energy);
			ss.str(std::string());
		}
		else if (light->LightSource == light->POINT_LIGHT)
		{
			const auto pLight = &(PointLight&)*light;
			ss << "Lights[" << i << "]." << "LightType";
			shader.UploadUniformInt(ss.str(), pLight->LightSource);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Ambient";
			shader.UploadUniformVec3(ss.str(), envLight.Ambient);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Color";
			shader.UploadUniformVec3(ss.str(), pLight->LightColor);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Direction";
			shader.UploadUniformVec3(ss.str(), pLight->Direction);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Position";
			shader.UploadUniformVec3(ss.str(), pLight->Position);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Energy";
			shader.UploadUniformFloat(ss.str(), pLight->Energy);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Radius";
			shader.UploadUniformFloat(ss.str(), pLight->Radius);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "LightAttenuation";
			shader.UploadUniformVec2(ss.str(), pLight->LightAttenuation);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Use";
			shader.UploadUniformInt(ss.str(), pLight->Use);
			ss.str(std::string());
		}
		else
		{
			const auto spLight = &(SpotLight&)*light;
			ss << "Lights[" << i << "]." << "LightType";
			shader.UploadUniformInt(ss.str(), spLight->LightSource);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Ambient";
			shader.UploadUniformVec3(ss.str(), envLight.Ambient);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Color";
			shader.UploadUniformVec3(ss.str(), spLight->LightColor);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Direction";
			shader.UploadUniformVec3(ss.str(), spLight->Direction);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Position";
			shader.UploadUniformVec3(ss.str(), spLight->Position);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Energy";
			shader.UploadUniformFloat(ss.str(), spLight->Energy);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Radius";
			shader.UploadUniformFloat(ss.str(), spLight->Radius);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "LightAttenuation";
			shader.UploadUniformVec2(ss.str(), spLight->LightAttenuation);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Cutoff";
			shader.UploadUniformFloat(ss.str(), spLight->innerCutoff);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "OuterCutoff";
			shader.UploadUniformFloat(ss.str(), spLight->outerCutoff);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Use";
			shader.UploadUniformInt(ss.str(), spLight->Use);
		}
	}

	for (auto& mesh : p_rendererComponent->GetMeshes())
	{
		
		bool inside = mesh.GetInstanceBound().InFrustum(p_frustum);

			if (!inside)
			{
				continue;
			}

		const auto& material = mesh.GetMaterial();
		const auto& attribs = mesh.GetVertexAttribs();
		attribs.Bind();

		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		if (mesh.GetMaterial().Diffuse != -1)
		{
			p_rendererComponent->GetModel().BindTexture(mesh.GetMaterial().Diffuse);
			shader.UploadUniformInt("Material.diffuseMap", mesh.GetMaterial().Diffuse);
			shader.UploadUniformInt("diffuseTextures[" + std::to_string(mesh.GetMaterial().Diffuse) + "]", mesh.GetMaterial().Diffuse);
		}

		shader.UploadUniformVec4("Material.Color", material.Color);
		shader.UploadUniformFloat("Material.Shininess", material.Shininess);
		shader.UploadUniformFloat("Material.SpecularHighlights", material.SpecularHighlights);
		shader.UploadUniformMat4("model", mesh.GetTransform().GetWorldMatrix());
		shader.UploadUniformFloat("AmbientEnergy", envLight.Energy);
		shader.UploadUniformVec4("ViewPosition", { renderData.view[3].x, renderData.view[3].y, renderData.view[3].z, 1.0f });
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
			aabVertices[d] = b ;
			c += 2;
			d += 2;
		}


		renderData.shader->Bind();
		renderData.m_aabbVertexArray->Bind();
		renderData.m_aabbVertexBuffer->BufferSubData(aabVertices, 0, sizeof(aabVertices));
		renderData.shader->UploadUniformMat4("projView", renderData.proj* renderData.view);
		renderData.shader->UploadUniformMat4("model", mesh.GetTransform().GetWorldMatrix());
		RenderCommand::RenderLines(*renderData.m_aabbVertexArray);
		shader.Bind();
	}
	
	//Console::Log(std::to_string(drawCalls) + "\n");
	drawCalls = 0;
}

void Renderer::Render(const std::unique_ptr<Primitive>& primitive)
{
	const auto& envLight = Scene::GetEnviromentLight();
	const auto& lights = Scene::GetLight();
	const auto& material = primitive->GetMaterial();

	const auto& attribs = primitive->GetVertexAttribs();
	attribs.Bind();
	const auto& shader = *Scene::sceneShader;



	int i = 0;
	
	for (int i = 0; i < Scene::GetLightCount(); ++i)
	{
		auto& light = lights[i];
		std::stringstream ss;

		if (light->LightSource == light->DIRECTIONAL_LIGHT)
		{
			ss << "Lights[" << i << "]." << "LightType";
			shader.UploadUniformInt(ss.str(), light->LightSource);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Ambient";
			shader.UploadUniformVec3(ss.str(), envLight.Ambient);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Color";
			shader.UploadUniformVec3(ss.str(), light->LightColor);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Direction";
			shader.UploadUniformVec3(ss.str(), light->Direction);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Use";
			shader.UploadUniformInt(ss.str(), light->Use);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Energy";
			shader.UploadUniformFloat(ss.str(), light->Energy);
			ss.str(std::string());
		}
		else if (light->LightSource == light->POINT_LIGHT)
		{
			const auto pLight = &(PointLight&)*light;
			ss << "Lights[" << i << "]." << "LightType";
			shader.UploadUniformInt(ss.str(), pLight->LightSource);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Ambient";
			shader.UploadUniformVec3(ss.str(), envLight.Ambient);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Color";
			shader.UploadUniformVec3(ss.str(), pLight->LightColor);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Direction";
			shader.UploadUniformVec3(ss.str(), pLight->Direction);			
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Position";
			shader.UploadUniformVec3(ss.str(), pLight->Position);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Energy";
			shader.UploadUniformFloat(ss.str(), pLight->Energy);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Radius";
			shader.UploadUniformFloat(ss.str(), pLight->Radius);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "LightAttenuation";
			shader.UploadUniformVec2(ss.str(), pLight->LightAttenuation);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Use";
			shader.UploadUniformInt(ss.str(), pLight->Use);
			ss.str(std::string());
		}
		else
		{
			const auto spLight = &(SpotLight&)*light;
			ss << "Lights[" << i << "]." << "LightType";
			shader.UploadUniformInt(ss.str(), spLight->LightSource);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Ambient";
			shader.UploadUniformVec3(ss.str(), envLight.Ambient);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Color";
			shader.UploadUniformVec3(ss.str(), spLight->LightColor);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Direction";
			shader.UploadUniformVec3(ss.str(), spLight->Direction);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Position";
			shader.UploadUniformVec3(ss.str(), spLight->Position);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Energy";
			shader.UploadUniformFloat(ss.str(), spLight->Energy);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Radius";
			shader.UploadUniformFloat(ss.str(), spLight->Radius);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "LightAttenuation";
			shader.UploadUniformVec2(ss.str(), spLight->LightAttenuation);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Cutoff";
			shader.UploadUniformFloat(ss.str(), spLight->innerCutoff);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "OuterCutoff";
			shader.UploadUniformFloat(ss.str(), spLight->outerCutoff);
			ss.str(std::string());
			ss << "Lights[" << i << "]." << "Use";
			shader.UploadUniformInt(ss.str(), spLight->Use);
		}
	}
	//glBindTexture(GL_TEXTURE_2D, 0);
	//if (primitive->GetMaterial().Diffuse != -1)
		

	shader.UploadUniformVec4("Material.Color", material.Color);
	shader.UploadUniformFloat("Material.Shininess", material.Shininess);
	shader.UploadUniformFloat("Material.SpecularHighlights", material.SpecularHighlights);

	shader.UploadUniformMat4("model", primitive->GetTransform());
	shader.UploadUniformFloat("AmbientEnergy", envLight.Energy);
	//material
	
	shader.UploadUniformVec4("ViewPosition", { renderData.view[3].x, renderData.view[3].y, renderData.view[3].z, 1.0f });

	RenderCommand::DrawIndexed(attribs);


	
}

void Renderer::SetViewport(float x, float y, float width, float height)
{
	glViewport(x, y, width, height);
}

void Renderer::EndScene()
{
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
			textureIndex = float(i + 1);
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


