#include "Math.h"

double Math::Ease(double p_x, double p_c)
{
	if (p_x < 0) {
		p_x = 0;
	}
	else if (p_x > 1.0) {
		p_x = 1.0;
	}
	if (p_c > 0) {
		if (p_c < 1.0) {
			return 1.0 - Math::Pow(1.0 - p_x, 1.0 / p_c);
		}
		else {
			return Math::Pow(p_x, p_c);
		}
	}
	else if (p_c < 0) {
		//inout ease

		if (p_x < 0.5) {
			return Math::Pow(p_x * 2.0, -p_c) * 0.5;
		}
		else {
			return (1.0 - Math::Pow(1.0 - (p_x - 0.5) * 2.0, -p_c)) * 0.5 + 0.5;
		}
	}
	else {
		return 0; // no ease (raw)
	}
}

double Math::Stepify(double p_value, double p_step)
{
	if (p_step == 0) {
		return p_value;
	}
	return Math::Floor(p_value / p_step + 0.5) * p_step;
}
