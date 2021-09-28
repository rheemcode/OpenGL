#version 430 core

uniform sampler2D sourceBuffer;
uniform vec2 sourceBufferTexelSize;

uniform vec4 curveThreshold;
uniform float clampIntensity;

uniform sampler2D baseBuffer;
uniform vec2 baseBufferTexels;
uniform float sampleScale;

uniform vec3 bloomColor;
uniform bool bloomAddBase;

in vec4 uvcoordsvar;

out vec4 FragColor;


vec3 safe_color(vec3 c)
{
	return clamp(c, vec3(0.0), vec3(1e20));
}

vec3 median(vec3 a, vec3 b, vec3 c)
{
	return a + b + c - min(min(a, b), c) - max(max(a, b), c);
}

float max_v3(vec3 a)
{
	return max(a.z, max(a.x, a.y));
}

/* -------------- Filters ------------- */
vec3 downsample_filer_high(sampler2D tex, vec2 uv, vec2 texelSize)
{
	vec4 d = texelSize.xyxy * vec4(-1, -1, +1, +1);
	vec3 s1 = textureLod(tex, uv + d.xy, 0.0).rgb;
	vec3 s2 = textureLod(tex, uv + d.zy, 0.0).rgb;
	vec3 s3 = textureLod(tex, uv + d.xw, 0.0).rgb;
	vec3 s4 = textureLod(tex, uv + d.w, 0.0).rgb;
	float s1w = 1.0 / (max_v3(s1) + 1.0);
	float s2w = 1.0 / (max_v3(s2) + 1.0);
	float s3w = 1.0 / (max_v3(s3) + 1.0);
	float s4w = 1.0 / (max_v3(s4) + 1.0);
	float one_div_wsum = 1.0 / (s1w + s2w + s3w + s4w);

	return (s1 * s1w + s2 * s2w + s3 * s3w + s4 * s4w) * one_div_wsum;

}

vec3 downsample_filter(sampler2D tex, vec2 uv, vec2 texelSize)
{
	/* Downsample with a 4x4 box filter */
	vec4 d = texelSize.xyxy * vec4(-1, -1, 1, 1);
	vec3 s;

	s = textureLod(tex, uv + d.xy, 0.0).rgb;
	s += textureLod(tex, uv + d.zy, 0.0).rgb;
	s += textureLod(tex, uv + d.xw, 0.0).rgb;
	s += textureLod(tex, uv + d.zw, 0.0).rgb;
	
	return s * (1.0 / 4);
}

vec3 upsample_filter_high(sampler2D tex, vec2 uv, vec2 texelSize)
{
	vec4 d = texelSize.xyxy * vec4(1, 1, -1, 0);
	vec3 s;

	s = textureLod(tex, uv - d.xy, 0.0).rgb;
	s += textureLod(tex, uv - d.wy, 0.0).rgb * 2;
	s += textureLod(tex, uv - d.zy, 0.0).rgb;

	s += textureLod(tex, uv + d.zw, 0.0).rgb * 2;
	s += textureLod(tex, uv, 0.0).rgb * 4;
	s += textureLod(tex, uv + d.xw, 0.0).rgb * 2;

	s += textureLod(tex, uv + d.zy, 0.0).rgb;
	s += textureLod(tex, uv + d.wy, 0.0).rgb * 2;
	s += textureLod(tex, uv + d.xy, 0.0).rgb;

	return s * (1.0 / 16.0);
}

vec3 upsample_filter(sampler2D tex, vec2 uv, vec2 texelSize)
{
  /* 4-tap bilinear upsampler */
  vec4 d = texelSize.xyxy * vec4(-1, -1, +1, +1) * (sampleScale * 0.5);

  vec3 s;
  s = textureLod(tex, uv + d.xy, 0.0).rgb;
  s += textureLod(tex, uv + d.zy, 0.0).rgb;
  s += textureLod(tex, uv + d.xw, 0.0).rgb;
  s += textureLod(tex, uv + d.zw, 0.0).rgb;

  return s * (1.0 / 4.0);
}


/* ------------- Steps -------------- */

vec4 step_blit()
{
	vec2 uv = uvcoordsvar.xy + sourceBufferTexelSize.xy * 0.5;
	
#ifdef HIGH_QUALITY /* Anti flicker */
	vec3 d = sourceBufferTexelSize.xyx * vec3(1, 1, 0);
	vec3 s0 = safe_color(textureLod(sourceBuffer, uvcoordsvar.xy, 0.0).rgb);
	vec3 s1 = safe_color(textureLod(sourceBuffer, uvcoordsvar.xy - d.xz, 0.0).rgb);
	vec3 s2 = safe_color(textureLod(sourceBuffer, uvcoordsvar.xy + d.xz, 0.0).rgb);
	vec3 s3 = safe_color(textureLod(sourceBuffer, uvcoordsvar.xy - d.zy, 0.0).rgb);
	vec3 s4 = safe_color(textureLod(sourceBuffer, uvcoordsvar.xy + d.zy, 0.0).rgb);
	vec3 m = median(median(s0.rgb, s1, s2), s3, s4);
#else
	vec3 s0 = safe_color(textureLod(sourceBuffer, uvcoordsvar.xy, 0.0).rgb);
	vec3 m = s0.rgb;
#endif
	
	/* Pixel Brightness */
	float br = max_v3(m);

	/* Under-threshold: quadratic curve */
	float rq = clamp(br - curveThreshold.x, 0, curveThreshold.y);
	rq = curveThreshold.z * rq * rq;

	/* Combine and apply the brighnes response curve */
	m *= max(rq, br - curveThreshold.w) / max(1e-5, br);
	
	/* Clamp pixel intensity if clamping is enabled */
	if (clampIntensity > 0.0)
	{
		br = max(1e-5, max_v3(m));
		m *= 1.0 - max(0.0, br - clampIntensity) / br;
	}
	
	return vec4(m, 1.0);
}

vec4 step_downsample()
{

#ifdef HIGH_QUALITY
	vec3 samp = downsample_filter_high(sourceBuffer, uvcoordsvar.xy, suourceBufferTexelSize);
#else
	vec3 samp = downsample_filter(sourceBuffer, uvcoordsvar.xy, sourceBufferTexelSize);
#endif
	return vec4(samp, 1.0);
}

vec4 step_upsample()
{
#ifdef HIGH_QUALITY
	vec3 blur = upsample_filter_high(sourceBuffer, uvcoordsvar.xy, sourceBufferTexelSize);
#else 
	vec3 blur = upsample_filter(sourceBuffer, uvcoordsvar.xy, sourceBufferTexelSize);
#endif
	vec3 base = textureLod(baseBuffer, uvcoordsvar.xy, 0.0).rgb;
	return vec4(base + blur, 1.0);
}

vec4 step_resolve()
{
#ifdef HIGH_QUALITY
	vec3 blur = upsample_filter_high(sourceBuffer, uvcoordsvar.xy, sourceBufferTexelSize);
#else
	vec3 blur = upsample_filter(sourceBuffer, uvcoordsvar.xy, sourceBufferTexelSize);
#endif
	vec4 base = bloomAddBase ? textureLod(baseBuffer, uvcoordsvar.xy, 0.0) : vec4(0.0);;
	vec3 cout = base.rgb + blur * bloomColor;
	return vec4(cout, base.a);
}

void main()
{
#if defined(STEP_BLIT)
	FragColor = step_blit();
#elif defined(STEP_DOWNSAMPLE)
	FragColor = step_downsample();
#elif defined(STEP_UPSAMPLE)
	FragColor = step_upsample();
#elif defined(STEP_RESOLVE)
	FragColor = step_resolve();
#endif
}
