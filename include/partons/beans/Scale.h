#ifndef SCALE_H
#define SCALE_H

/**
 * @file Scale.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 07 October 2015
 * @version 1.0
 *
 * @class Scale
 *
 * @brief
 */

#include "../BaseObject.h"

class Scale: public BaseObject {
public:
    Scale();
    Scale(double MuF2, double MuR2);
    virtual ~Scale();

    double getMuF2() const;
    void setMuF2(double muF2);
    double getMuR2() const;
    void setMuR2(double muR2);

private:
    double m_MuF2;
    double m_MuR2;
};

#endif /* SCALE_H */
