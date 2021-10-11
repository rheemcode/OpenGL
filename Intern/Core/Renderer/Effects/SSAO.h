#pragma once
#include "Shader.h"
#include "Texture.h"
#include "Buffers/FrameBuffer.h"
#include "Buffers/UniformBuffer.h"
#include <random>

#define MAX_SSAO_SAMPLES 32

class PostProcess
{

};

class SSAO : public PostProcess
{
	friend class Scene;
	int m_samples = 32;
	float m_radius = 0.5f;
	float m_bias = 0.025f;

	std::array<Vector3, MAX_SSAO_SAMPLES> m_kernel;
	std::array<Vector3, 1366 * 768> ssaoNoise;

	std::shared_ptr<Texture> m_noiseTexture;
	std::shared_ptr<Framebuffer> m_ssaoFbo;
	std::shared_ptr<Shader> m_shader;
	std::shared_ptr<ComputeShader> m_computeShader;
	std::shared_ptr<UniformBuffer> m_uniformBuffer;

public:
	void GenerateKernel()
	{
		std::uniform_real_distribution<float> randomFloats(0.0, 1.0);
		std::random_device rd;
		std::mt19937 gen(rd());
		int offset = 0;
		for (uint32_t i = 0; i < m_samples; i++)
		{
			Vector3 sample
			(
				randomFloats(gen) * 2.0f - 1.0f,
				randomFloats(gen) * 2.0f - 1.0f,
				randomFloats(gen) * 2.0f - 1.0f
			);

			sample = Vector3::Normalize(sample);
			sample *= randomFloats(gen);

			float scale = (float)i / (float)m_samples;
			scale = Math::Lerp(0.1f, 1.0f, scale * scale);
			sample *= scale;
			m_uniformBuffer->UploadData(sample, offset);
			offset += 16;
		}

		m_uniformBuffer->FlushBuffer();

		for (uint32_t i = 0; i < 1366 * 768; i++)
		{
			Vector3 noise
			(
				randomFloats(gen) * 2.0f - 1.0f,
				randomFloats(gen) * 2.0f - 1.0f,
				randomFloats(gen)
			);
			ssaoNoise[i] = noise;
		}
	}

	void GenerateNoiseTexture()
	{
		m_noiseTexture = std::make_shared<Texture2D>();
		TextureParameters texParam;
		texParam.internalFormat = TextureFormat::RGBA16F;
		texParam.dataFormat = TextureFormat::RGB;
		texParam.magFilter = TextureFilter::NEAREST;
		texParam.minFilter = TextureFilter::NEAREST;
		texParam.textureDataType = TextureDataType::FLOAT;
		texParam.wrap = TextureWrap::REPEAT;
		m_noiseTexture->SetHeight(1366);
		m_noiseTexture->SetWidth(768);
		m_noiseTexture->Create(ssaoNoise.data(), texParam);
	}

	void CreateFramebuffer()
	{
		m_ssaoFbo = std::make_shared<Framebuffer>();
		m_ssaoFbo->CreateTexture(FramebufferTexture::COLOR);
		TextureParameters texParam;
		texParam.internalFormat = TextureFormat::RED;
		texParam.dataFormat = TextureFormat::RED;
		texParam.magFilter = TextureFilter::NEAREST;
		texParam.minFilter = TextureFilter::NEAREST;
		texParam.textureDataType = TextureDataType::FLOAT;
		texParam.wrap = TextureWrap::NONE;
		m_ssaoFbo->AttachColorTexture(texParam);

		m_shader = std::make_shared<Shader>("./Assets/Shaders/ssao.glsl");
		m_computeShader = std::make_shared<ComputeShader>("./Assets/Shaders/ssao_compute.comp.glsl");
		m_shader->Bind();
		m_shader->SetInt("gPosition", 0);
		m_shader->SetInt("gNormal", 1);
		m_shader->SetInt("texNoise", 2);

		m_computeShader->Bind();
		m_computeShader->SetInt("gPosition", 0);
		m_computeShader->SetInt("gNormal", 1);
		m_computeShader->SetInt("texNoise", 2);

	}

	void BindTexture()
	{
		m_noiseTexture->Bind();
	}

	void FlushUniforms()
	{
		m_uniformBuffer->FlushBuffer();
	}

	void BindFramebuffer()
	{
		m_ssaoFbo->Bind(FramebufferName::COLORBUFFER);
	}

	void BindFrambufferTexture()
	{
		m_ssaoFbo->BindTexture(FramebufferTexture::COLOR);
	}

	void BindFramebufferImageTexture()
	{
		m_ssaoFbo->BindImageTexture(FramebufferTexture::COLOR, 0);
	}

	Shader* GetShader() { return m_computeShader.get(); }

	void SetSamples(int samples) 
	{ 
		m_samples = samples;
		GenerateKernel();
	}

	SSAO() {};
};

