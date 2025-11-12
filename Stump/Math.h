#ifndef ST_MATH_H
#define ST_MATH_H

#include <cmath>
#include <limits>
#include "Typedefs.h"
class Math
{
public:
	static constexpr float PI = 3.14159265358979323846f;
	static constexpr float E = 2.71828182845904523536f;
	static constexpr float Math_TAU = 6.28318530717958647692f; // 2*PI
	static constexpr float EPSILON = 1e-6f;
#ifdef PRECISE_MATH_CHECK
	static constexpr float UNIT_EPSILON = 0.00001;
#else
	static constexpr float UNIT_EPSILON = 0.001;
#endif
	static constexpr float SQRT12 = 0.7071067811865475244008443621048490;
	static constexpr float SQRT2 = 1.4142135623730950488016887242;
	static constexpr long float INF = std::numeric_limits<float>::infinity();
	

	static double Sin(double radians) { return std::sin(radians); }
	static float Sinf(float radians) { return std::sinf(radians); }

	static double Cos(double radians) { return std::cos(radians); }
	static float Cosf(float radians) { return std::cosf(radians); }

	static double Tan(double radians) { return std::tan(radians); }
	static float Tanf(float radians) { return std::tanf(radians); }

	static double Ctan(double radians) { return 1.0 / std::tan(radians); }
	static float Ctanf(float radians) { return 1.0f / std::tanf(radians); }

	static double Asin(double radians) { return std::asin(radians); }
	static float Asinf(float radians) { return std::asinf(radians); }

	static double Acos(double radians) { return std::acos(radians); }
	static float Acosf(float radians) { return std::acosf(radians); }

	static double Atan(double radians) { return std::atan(radians); }
	static float Atanf(float radians) { return std::atanf(radians); }

	static double Atan2(double p_y, double p_x) { return ::atan2(p_y, p_x); }
	static float Atan2(float p_y, float p_x) { return ::atan2f(p_y, p_x); }

	static double ToRadians(double degrees) { return degrees * (PI / 180.0); }
	static float ToRadiansf(float degrees) { return degrees * (PI / 180.0f); }

	static double ToDegrees(double radians) { return radians * (180.0 / PI); }
	static float ToDegreesf(float radians) { return radians * (180.0f / PI); }

	static double Sqrt(double value) { return std::sqrt(value); }
	static float Sqrtf(float value) { return std::sqrtf(value); }

	static double Abs(double value) { return std::fabs(value); }
	static float Absf(float value) { return std::fabsf(value); }
	static int Absi(int value) { return value < 0 ? -value : value; }

	static float Clampf(float value, float min, float max) {
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}
	static double Clamp(double value, double min, double max) {
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}

	static float Powf(float base, float exponent) { return std::powf(base, exponent); }
	static double Pow(double base, double exponent) { return std::pow(base, exponent); }

	static float Logf(float value) { return std::logf(value); }
	static double Log(double value) { return std::log(value); }

	static float Log10f(float value) { return std::log10f(value); }
	static double Log10(double value) { return std::log10(value); }

	static float Log2f(float value) { return std::log2f(value); }
	static double Log2(double value) { return std::log2(value); }

	static float Expf(float value) { return std::expf(value); }
	static double Exp(double value) { return std::exp(value); }

	static float Floorf(float value) { return std::floorf(value); }
	static double Floor(double value) { return std::floor(value); }

	static float Ceilf(float value) { return std::ceilf(value); }
	static double Ceil(double value) { return std::ceil(value); }

	static float Roundf(float value) { return std::roundf(value); }
	static double Round(double value) { return std::round(value); }

	static float Fmodf(float x, float y) { return std::fmodf(x, y); }
	static double Fmod(double x, double y) { return std::fmod(x, y); }

	static double Fposmod(double p_x, double p_y) {
		double value = Math::Fmod(p_x, p_y);
		if (((value < 0) && (p_y > 0)) || ((value > 0) && (p_y < 0))) {
			value += p_y;
		}
		value += 0.0;
		return value;
	}

	static float Fposmod(float x, float y) {
		float value = Math::Fmodf(x, y);
		if (((value < 0) && (y > 0)) || ((value > 0) && (y < 0))) {
			value += y;
		}
		value += 0.0f;
		return value;
	}

	static int Posmod(int x, int y) {
		int value = x % y;
		if (((value < 0) && (y > 0)) || ((value > 0) && (y < 0))) {
			value += y;
		}
		return value;
	}

	static double Lerp(double x, double y, double t) {
		return x + (y - x) * t;
	}
	static float Lerpf(float x, float y, float t) {
		return x + (y - x) * t;
	}

	static double Lerp_angle(double p_from, double p_to, double p_weight) {
		double difference = fmod(p_to - p_from, Math_TAU);
		double distance = fmod(2.0 * difference, Math_TAU) - difference;
		return p_from + distance * p_weight;
	}
	static float Lerp_angle(float p_from, float p_to, float p_weight) {
		float difference = fmod(p_to - p_from, (float)Math_TAU);
		float distance = fmod(2.0f * difference, (float)Math_TAU) - difference;
		return p_from + distance * p_weight;
	}

	static bool IsEqualApproxRatio(num_fd a, num_fd b, num_fd epsilon = EPSILON, num_fd min_epsilon = EPSILON) {
		// this is an approximate way to check that numbers are close, as a ratio of their average size
		// helps compare approximate numbers that may be very big or very small
		num_fd diff = abs(a - b);
		if (diff == 0 || diff < min_epsilon) {
			return true;
		}
		num_fd avg_size = (abs(a) + abs(b)) / 2;
		diff /= avg_size;
		return diff < epsilon;
	}

	static bool IsEqualApprox(double a, double b, double tolerance) {
		if (a == b) return true;

		return Math::Abs(a - b) <= tolerance;
	}
	static bool IsEqualApprox(double a, double b) {
		
		if (a == b) {
			return true;
		}
		
		double tolerance = EPSILON * abs(a);
		if (tolerance < EPSILON) {
			tolerance = EPSILON;
		}
		return abs(a - b) < tolerance;
	}

	static bool IsEqualApprox(float a, float b, float tolerance) {
		if (a == b) return true;
		return Math::Absf(a - b) <= tolerance;
	}

	static bool IsZeroApprox(float x) {
		return Abs(x) < (float)EPSILON;
	}

	static double Ease(double p_x, double p_c);
	static int StepDecimals(double p_step);
	static int RangeStepDecimals(double p_step);
	static double Stepify(double p_value, double p_step);
	static double Dectime(double p_value, double p_amount, double p_step);

	static int LargerPrime(int p_val);

	static void Seed(int x);
	static void Randomize();
	static int RandFromSeed(int* seed);
	static int Rand();

};

#endif // !MATH_H

