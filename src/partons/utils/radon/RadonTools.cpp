#include "../../../../include/partons/utils/radon/RadonTools.h"

#include <algorithm>
#include <cmath>

namespace PARTONS {


double RadonTools::GPDOfSquareCell(double x, double xi, double u1, double u2,
        double v1, double v2) {
    if (xi == 1.) {
        return (x / sqrt(2.) >= u1 && x / sqrt(2.) <= u2) ?
                (v2 - v1) / sqrt(2.) : 0.;
    } else if (xi == -1.) {
        return (-x / sqrt(2.) >= v1 && -x / sqrt(2.) <= v2) ?
                (u2 - u1) / sqrt(2.) : 0.;
    } else {
        double v_min = ((1. - xi) * v1 + x * sqrt(2.)) / (1. + xi);
        double v_max = ((1. - xi) * v2 + x * sqrt(2.)) / (1. + xi);
        double u_min = std::max(u1, (fabs(xi) <= 1.) ? v_min : v_max);
        double u_max = std::min(u2, (fabs(xi) <= 1.) ? v_max : v_min);
        return sqrt(2.) / fabs(1. - xi) * std::max(0., u_max - u_min);
    }
}

double RadonTools::GPDOfLowerTriangleCell(double x, double xi, double u1,
        double u2, double v1, double v2) {
    if (xi == 0.) {
        return (x * sqrt(2.) + v1 >= u1 && x * sqrt(2.) + v1 <= u2) ?
                (u2 - sqrt(2.) * x - v1) * sqrt(2.) : 0.;
    } else if (xi == 1.) {
        return (x / sqrt(2.) >= u1 && x / sqrt(2.) <= u2) ?
                (x / sqrt(2.) - u1) / sqrt(2.) : 0.;
    } else if (xi == -1.) {
        return (-x / sqrt(2.) >= v1 && -x / sqrt(2.) <= v2) ?
                (v2 + x / sqrt(2.)) / sqrt(2.) : 0.;
    } else {
        double v_min = ((1. - xi) * v1 + x * sqrt(2.)) / (1. + xi);
        double v_max = ((1. - xi) * (v1 - u1) + x * sqrt(2.)) / (2 * xi);
        double u_min, u_max;
        if (xi > 1.) {
            u_max = std::min(u2, v_min);
            u_min = std::max(u1, v_max);
        } else if (xi > 0.) {
            u_max = std::min(u2, v_max);
            u_min = std::max(u1, v_min);
        } else if (xi > -1.) {
            u_max = u2;
            u_min = std::max(u1, std::max(v_min, v_max));
        } else {
            u_max = std::min(u2, v_min);
            u_min = std::max(u1, v_max);
        }
        return sqrt(2.) / fabs(1. - xi) * std::max(0., u_max - u_min);
    }
}

} /* namespace PARTONS */
