/**
 * @file DDGauge.h
 * @author Nabil Chouika (Irfu/SPhN, CEA Saclay)
 * @date 6 oct. 2016
 * @version 1.0
 */

#ifndef DDGAUGE_H_
#define DDGAUGE_H_

#include <string>

namespace PARTONS {

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
    DDGauge(const std::string& gaugeStr);
    virtual ~DDGauge();

    operator Type() const;

    std::string toString() const;

    static DDGauge::Type fromString(const std::string& gaugeStr);

private:
    DDGauge::Type m_type;
};


} /* namespace PARTONS */

#endif /* DDGAUGE_H_ */
