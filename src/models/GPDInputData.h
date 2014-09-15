#ifndef GPD_INPUT_DATA_H
#define GPD_INPUT_DATA_H

/**
 * @file GPDInputData.h
 * @author: Bryan BERTHOU (CEA Saclay)
 * @date 24 juil. 2014
 * @version 1.0
 */

#include <string>

#include "GenericData.h"

class GPDInputData: public GenericData {
    double x;       ///< Bjorken variable
    double xi;      ///< longitudinal momentum
    double t;       ///< momentum transfer (Mandelstam variable)
    double MuF;     ///< Factorisation
    double MuR;     ///< Re-normalisation

public:

    GPDInputData(double _x, double _xi, double _t, double _MuF, double _MuR);

    std::string toString();

    double getMuF() const;
    double getMuR() const;
    double getT() const;
    double getX() const;
    double getXi() const;
};

#endif /* GPD_INPUT_DATA_H */
