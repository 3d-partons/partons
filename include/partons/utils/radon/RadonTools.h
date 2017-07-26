/**
 * @file RadonTools.h
 * @author nchouika
 * @date 11 oct. 2016
 * @version 1.0
 */

#ifndef RADONTOOLS_H_
#define RADONTOOLS_H_

namespace PARTONS {

/**
 * @class RadonTools
 * @brief Tools for Radon Transform, such as the transform of square cells, etc.
 */

class RadonTools {
public:
    static double GPDOfSquareCell(double x, double xi, double u1, double u2,
            double v1, double v2);
    static double GPDOfLowerTriangleCell(double x, double xi, double u1, double u2,
            double v1, double v2);
};

} /* namespace PARTONS */

#endif /* RADONTOOLS_H_ */
