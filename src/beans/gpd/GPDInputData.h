#ifndef GPD_INPUT_DATA_H
#define GPD_INPUT_DATA_H

/**
 * @file GPDInputData.h
 * @author: Bryan BERTHOU (CEA Saclay)
 * @date 24 juil. 2014
 * @version 1.0
 */

#include <string>

#include "../GenericData.h"

class GPDInputData: public GenericData {
public:

    GPDInputData(double _x, double _xi, double _t, double _MuF, double _MuR);

    std::string toString();

    double getMuF() const;
    double getMuR() const;
    double getT() const;
    double getX() const;
    double getXi() const;
    void setMuF(double muF);
    void setMuR(double muR);
    void setT(double t);
    void setX(double x);
    void setXi(double xi);

private:
    double m_x;       ///< Bjorken variable
    double m_xi;      ///< longitudinal momentum
    double m_t;       ///< momentum transfer (Mandelstam variable)
    double m_MuF;     ///< Factorisation
    double m_MuR;     ///< Re-normalisation
};

#endif /* GPD_INPUT_DATA_H */
