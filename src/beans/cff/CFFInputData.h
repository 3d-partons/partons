#ifndef CFF_INPUT_DATA_H
#define CFF_INPUT_DATA_H

/**
 * @file GPDInputData.h
 * @author: Bryan BERTHOU (CEA Saclay)
 * @date 28 juil. 2014
 * Last update : 15 September 2014
 * @version 2.0
 */

#include <string>

class CFFInputData {
public:
    CFFInputData();
    CFFInputData(const double xB, const double t, const double Q2);
    CFFInputData(const unsigned int binId, const double xB, const double t,
            const double Q2);
    /**
     * Copy constructor
     *
     * @param other
     */
    CFFInputData(const CFFInputData &other);
    virtual ~CFFInputData();

    std::string toString();

    // ##### GETTERS & SETTERS #####

    double getQ2() const;
    void setQ2(double q2);
    double getT() const;
    void setT(double t);
    double getXB() const;
    void setXB(double xB);
    unsigned int getBinId() const;
    void setBinId(unsigned int binId);

private:
    unsigned int m_binId;
    double m_xB;
    double m_t;
    double m_Q2;

    //double xi;
    //double MuF;
    //double MuR;
    //QCDOrderType qcdOrderType;
};

#endif /* CFF_INPUT_DATA_H */
