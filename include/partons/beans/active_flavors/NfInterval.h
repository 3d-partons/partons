#ifndef NF_INTERVAL_H
#define NF_INTERVAL_H

/**
 * @file NfInterval.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 07 August 2015
 * @version 1.0
 *
 * @class NfInterval
 *
 * @brief
 */

#include <string>

class NfInterval {
public:
    NfInterval();
    NfInterval(unsigned short nf, double lowerBound, double upperBound,
            unsigned int index);
    virtual ~NfInterval();

    unsigned short getNf() const;
    void setNf(unsigned short nf);

    unsigned int getIndex() const;
    void setIndex(unsigned int index);

    /**
     * Return a pre-formatted characters string for output visualization of class member's values
     *
     * @return a pre-formatted characters string
     */
    virtual std::string toString();

    bool operator <(const NfInterval &other) const;
    double getLowerBound() const;
    void setLowerBound(double lowerBound);
    double getUpperBound() const;
    void setUpperBound(double upperBound);

private:
    unsigned short m_nf;
    double m_lowerBound;
    double m_upperBound;

    unsigned int m_index;
};

#endif /* NF_INTERVAL_H */
