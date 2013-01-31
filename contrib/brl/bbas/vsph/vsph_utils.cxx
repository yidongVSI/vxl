#include "vsph_utils.h"
#include <vnl/vnl_math.h>

// compute b-a on the circle
double vsph_utils::azimuth_diff(double phi_a, double phi_b,
                                bool in_radians) {
  double a = phi_a, b = phi_b, pi = vnl_math::pi,
         two_pi = vnl_math::twopi;
  double diff = b-a;
  if (!in_radians) {
    if (diff > 180)  diff -= 360;
    if (diff < -180) diff += 360;
  }
  else {
    if (diff > pi)  diff -= two_pi;
    if (diff < -pi) diff += two_pi;
  }
  return diff;
}

double vsph_utils::distance_on_usphere(vsph_sph_point_2d const& a,
				       vsph_sph_point_2d const& b){
  double dist = vcl_fabs(vsph_utils::azimuth_diff(a.phi_, b.phi_));
  dist += vcl_fabs(a.theta_-b.theta_);
  return dist;
}

bool vsph_utils::a_eq_b(double phi_a, double phi_b, bool in_radians){
  double a = phi_a, b = phi_b, pi = vnl_math::pi;
  if (!in_radians) {
    if (((a == -180)&&(b == 180))||((a == 180)&&(b == -180))) return true;
  }
  if (((a == -pi)&&(b == pi))||((a == pi)&&(b == -pi))) return true;
  return a == b;
}

bool vsph_utils::a_lt_b(double phi_a, double phi_b, bool in_radians)
{
  double diff = vsph_utils::azimuth_diff(phi_a, phi_b, in_radians);
  return diff > 0; // a < b if (b-a) > 0
}

vsph_sph_box_2d vsph_utils::intersection(vsph_sph_box_2d const& b1,
                                         vsph_sph_box_2d const& b2) {
  vsph_sph_box_2d rbox;

  double phi_min =
    vsph_utils::a_lt_b(b1.min_phi(),b2.min_phi()) ? b2.min_phi() : b1.min_phi();

  double phi_max =
    vsph_utils::a_lt_b(b1.max_phi(),b2.max_phi()) ? b1.max_phi() : b2.max_phi();

  double theta_min =
    b1.min_theta() < b2.min_theta() ? b2.min_theta() : b1.min_theta();

  double theta_max =
    b1.max_theta() < b2.max_theta() ? b1.max_theta() : b2.max_theta();

  //arbitrarily set units according to b1
  rbox.add(theta_min, phi_min, b1.in_radians());
  rbox.add(theta_max, phi_max, b1.in_radians());

  return rbox;
}
