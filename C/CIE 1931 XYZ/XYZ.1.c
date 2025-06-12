#include <math.h>
struct XYZ { double x, y, z };
struct xyY { double x, y, Y };

/*inline double gaussian(double x, double α, double μ, double σ1, double σ2) {
  double t = (x - μ) / (x < μ ? σ1 : σ2);
  return α * exp(-(t * t) / 2);
}*/

double gaussian(double x, double alpha, double mu, double sigma1, double sigma2) {
  double t = (x - mu) / (x < mu ? sigma1 : sigma2);
  return alpha * exp(-(t * t) / 2);
}

struct XYZ xyzFromWavelength(double wl) {
  struct XYZ color;
  color.x = gaussian(wl,  1.056, 5998, 379, 310) +
            gaussian(wl,  0.362, 4420, 160, 267) +
            gaussian(wl, -0.065, 5011, 204, 262);
  color.y = gaussian(wl,  0.821, 5688, 469, 405) +
            gaussian(wl,  0.286, 5309, 163, 311);
  color.z = gaussian(wl,  1.217, 4370, 118, 360) +
            gaussian(wl,  0.681, 4590, 260, 138);
  return color;
}

int main()
{
    double wl;
    printf("Wave Length(nm):");
    scanf("%lf",&wl);
    wl = wl*10;
    //printf("%f\n",wl);
    struct XYZ c1 = xyzFromWavelength(wl);
    struct xyY c2 = {c1.x/(c1.x+c1.y+c1.z), c1.y/(c1.x+c1.y+c1.z), c1.y};
    printf("CIE XYZ :\n%f,%f,%f\n",c1.x,c1.y,c1.z);
    printf("CIE xyY :\n%f,%f,%f\n",c2.x,c2.y,c2.Y);
}
