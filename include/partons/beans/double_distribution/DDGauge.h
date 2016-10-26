/**
 * @file DDGauge.h
 * @author Nabil Chouika (Irfu/SPhN, CEA Saclay)
 * @date 6 oct. 2016
 * @version 1.0
 */

#ifndef DDGAUGE_H_
#define DDGAUGE_H_

#include <string>

/**
 * @class DDGauge
 * @brief Gauge used for the Double Distribution representation: BMKS, Pobylitsa, etc.
 */

class DDGauge {
public:
    enum Type {
        UNDEFINED, BMKS, Pobylitsa
    };

    DDGauge();
    DDGauge(DDGauge::Type type);
    virtual ~DDGauge();

    operator Type() const;

    std::string toString();

private:
    DDGauge::Type m_type;
};


#endif /* DDGAUGE_H_ */
