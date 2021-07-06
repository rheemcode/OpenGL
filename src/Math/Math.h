#pragma once
#include <math.h>
#include <cstdint>
#ifndef NO_FDECL
#include "fdecl.h"
#endif
namespace Math
{
	#define EPSILON 0.00001
	#define EPSILON2 (CMP_EPSILON * CMP_EPSILON)


	#define SQRT12 0.7071067811865475244008443621048490
	#define SQRT2 1.4142135623730950488016887242
	#define TAU 6.2831853071795864769252867666
	#define PI 3.1415926535897932384626433833
	#define Math_INF INFINITY
	#define Math_NAN NAN

	template<typename T>
	static T Cos(T p_x) {}
	
	template<>
	static float Cos(float p_x) { return cosf(p_x); }
	
	template<>
	static double Cos(double p_x) { return cos(p_x); }
	
	template<typename T>
	static T Tan(T p_x) { return static_cast<T>(tan(p_x)); }
	template<typename T>
	static T ASin(T p_x) { return static_cast<T>(asin(p_x)); }
	template<typename T>
	static T ACos(T p_x) { return static_cast<T>(acos(p_x)); }
	template<typename T>
	static T ATan(T p_x) { return static_cast<T>(atan(p_x)); }
	template<typename T>
	static T ATan2(T p_y, T p_x) { return static_cast<T>(atan2(p_y, p_x)); }
	
	template<typename T>
	static T Sin(T p_x) {}
	template<>
	static float Sin(float p_x) { return sinf(p_x); }
	template<>
	static double Sin(double p_x) { return sin(p_x); }
	template<typename T>
	static T SinH(T p_x) { return static_cast<T>(sinh(p_x)); }
	template<typename T>
	static T CosH(T p_x) { return static_cast<T>(cosh(p_x)); }
	template<typename T>
	static T TanH(T p_x) { return static_cast<T>(tanh(p_x)); }
	template<typename T>
	static T Sqrt(T p_x) { return static_cast<T>(sqrt(p_x)); }
	template<typename T>
	static T RSqrt(T p_x) { return static_cast<T>(1) / static_cast<T>(sqrt(p_x)); }
	template<typename T>
	static T FMod(T p_x, T p_y) { return static_cast<T>(fmod(p_x, p_y)); }
	template<typename T>
	static T Floor(T p_x) { return static_cast<T>(floor(p_x)); }
	template<typename T>
	static T Ceil(T p_x) { return static_cast<T>(ceil(p_x)); }
	template<typename T>
	static T Pow(T p_x, T p_y) { return static_cast<T>(pow(p_x, p_y)); }
	template<typename T>
	static T Log(T p_x) { return static_cast<T>(log(p_x)); }
	template<typename T>
	static T Log2(T p_x) { return static_cast<T>(log2(p_x)); }
	template<typename T>
	static T Exp(T p_x) { return static_cast<T>(exp(p_x)); }
	
	static int Abs(int g) { return ::abs(g); }
	static float Abs(float g) { return ::fabs(g); }
	template<typename T>
	static T Sign(T p_val)
	{
		if (signbit(p_val)) return static_cast<T>(-1);
		return static_cast<T>(1);

	}
	template<typename T>
	static T FPosMod(T p_x, T p_y) 
	 {
		T value = FMod(p_x, p_y);
		if ((value < 0 && p_y > 0) || (value > 0 && p_y < 0)) {
			value += p_y;
		}
		value += 0.0;
		return value;
	}
	
	template<typename T>
	static T FPosModp(T p_x, T p_y)
	 {
		T value = FMod(p_x, p_y);
		if (value < 0) {
			value += p_y;
		}
		value += 0.0;
		return value;
	}
	 
	 static int64_t PosMod(int64_t p_x, int64_t p_y) 
	 {
		int64_t value = p_x % p_y;
		if ((value < 0 && p_y > 0) || (value > 0 && p_y < 0)) {
			value += p_y;
		}
		return value;
	}

	template<typename T>
	static T Deg2Rad(T p_y) { return static_cast<T>(p_y * (PI / 180.0)); }
	template<typename T>
	static T Rad2deg(T p_y) { return static_cast<T>(p_y * (180.0 / PI)); }
	template<typename T>
	static T Lerp(T p_from, T p_to, T p_t) { return p_from + (p_to - p_from) * p_t; }
	//static float Lerp(float p_from, float p_to, float p_t) { return ( 1.f - p_t ) * p_from + p_t * p_to; }
	template<typename T>
	static T LerpAngle(T p_from, T p_to, T p_weight) {
		T difference = static_cast<T>(fmod(p_to - p_from, TAU));
		T distance = static_cast<T>(fmod(2.0 * difference, TAU) - difference);
		return p_from + distance * p_weight;
	}
	 
	static  double InverseLerp(double p_from, double p_to, double p_value) { return (p_value - p_from) / (p_to - p_from); }
	static  float InverseLerp(float p_from, float p_to, float p_value) { return (p_value - p_from) / (p_to - p_from); }

	static  double RangeLerp(double p_value, double p_istart, double p_istop, double p_ostart, double p_ostop) { return Lerp(p_ostart, p_ostop, InverseLerp(p_istart, p_istop, p_value)); }
	static  float RangeLerp(float p_value, float p_istart, float p_istop, float p_ostart, float p_ostop) { return Lerp(p_ostart, p_ostop,  InverseLerp(p_istart, p_istop, p_value)); }

	static  float MoveTowards(float p_current, float p_target, float p_maxdelta)
	 {
		 if (Abs(p_target - p_current) <= p_maxdelta)
			 return p_target;

		 return p_current + Sign(p_target - p_current) * p_maxdelta;
	 }


	static  double Round(double p_val) { return (p_val >= 0) ? Floor(p_val + 0.5) : -Floor(-p_val + 0.5); }
	static  float Round(float p_val) { return (p_val >= 0.f) ? Floor(p_val + 0.5f) : -Floor(-p_val + 0.5f); }



	static  float Clamp(float p_val, float p_min, float p_max)
	 {
		 if (p_val < p_min)
			 p_val = p_min;
		 if (p_val > p_max)
			 p_val = p_max;
			 return p_val;
	 }

	static  float ClampInt(int p_val, int p_min, int p_max)
	 {
		 if (p_val < p_min)
			 p_val = p_min;
		 if (p_val > p_max)
			 p_val = p_max;
		 return float(p_val);
	 }

	static float ClampNeg1to1(float value) {
		 if (value < -1.f) value = -1.f;
		 if (value > 1.f) value = 1.f;
		 return float(value);
	 }

	static  uint32_t HalfbitsToFloatbits(uint16_t h) {
		uint16_t h_exp, h_sig;
		uint32_t f_sgn, f_exp, f_sig;

		h_exp = (h & 0x7c00u);
		f_sgn = ((uint32_t)h & 0x8000u) << 16;
		switch (h_exp) {
		case 0x0000u: /* 0 or subnormal */
			h_sig = (h & 0x03ffu);
			/* Signed zero */
			if (h_sig == 0) {
				return f_sgn;
			}
			/* Subnormal */
			h_sig <<= 1;
			while ((h_sig & 0x0400u) == 0) {
				h_sig <<= 1;
				h_exp++;
			}
			f_exp = ((uint32_t)(127 - 15 - h_exp)) << 23;
			f_sig = ((uint32_t)(h_sig & 0x03ffu)) << 13;
			return f_sgn + f_exp + f_sig;
		case 0x7c00u: /* inf or NaN */
			/* All-ones exponent and a copy of the significand */
			return f_sgn + 0x7f800000u + (((uint32_t)(h & 0x03ffu)) << 13);
		default: /* normalized */
			/* Just need to adjust the exponent and shift */
			return f_sgn + (((uint32_t)(h & 0x7fffu) + 0x1c000u) << 13);
		}
	}

	static  float HalfPtrToFloat(const uint16_t* h) {
		union {
			uint32_t u32;
			float f32;
		} u;

		u.u32 = HalfbitsToFloatbits(*h);
		return u.f32;
	}

	 static float HalfToFloat(const uint16_t h) {
		return HalfPtrToFloat(&h);
	}

	static  uint16_t MakeHalfFloat(float f) {
		union {
			float fv;
			uint32_t ui;
		} ci;
		ci.fv = f;

		uint32_t x = ci.ui;
		uint32_t sign = (unsigned short)(x >> 31);
		uint32_t mantissa;
		uint32_t exp;
		uint16_t hf;

		// get mantissa
		mantissa = x & ((1 << 23) - 1);
		// get exponent bits
		exp = x & (0xFF << 23);
		if (exp >= 0x47800000) {
			// check if the original single precision float number is a NaN
			if (mantissa && (exp == (0xFF << 23))) {
				// we have a single precision NaN
				mantissa = (1 << 23) - 1;
			}
			else {
				// 16-bit half-float representation stores number as Inf
				mantissa = 0;
			}
			hf = (((uint16_t)sign) << 15) | (uint16_t)((0x1F << 10)) |
				(uint16_t)(mantissa >> 13);
		}
		// check if exponent is <= -15
		else if (exp <= 0x38000000) {
			/*// store a denorm half-float value or zero
		exp = (0x38000000 - exp) >> 23;
		mantissa >>= (14 + exp);

		hf = (((uint16_t)sign) << 15) | (uint16_t)(mantissa);
		*/
			hf = 0; //denormals do not work for 3D, convert to zero
		}
		else {
			hf = (((uint16_t)sign) << 15) |
				(uint16_t)((exp - 0x38000000) >> 13) |
				(uint16_t)(mantissa >> 13);
		}

		return hf;
	}


};