#include <math.h>
struct XYZ { double x, y, z; };

inline double gaussian(double x, double α, double μ, double σ1, double σ2) {
  double t = (x - μ) / (x < μ ? σ1 : σ2);
  return α * exp(-(t * t) / 2);
}

struct XYZ xyzFromWavelength(double λ) {
  struct XYZ color;
  color.x = gaussian(λ,  1.056, 5998, 379, 310) +
            gaussian(λ,  0.362, 4420, 160, 267) +
            gaussian(λ, -0.065, 5011, 204, 262);
  color.y = gaussian(λ,  0.821, 5688, 469, 405) +
            gaussian(λ,  0.286, 5309, 163, 311);
  color.z = gaussian(λ,  1.217, 4370, 118, 360) +
            gaussian(λ,  0.681, 4590, 260, 138);
  return color;
}
