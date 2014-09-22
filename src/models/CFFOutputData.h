#ifndef CFF_OUTPUT_DATA_H
#define CFF_OUTPUT_DATA_H

/**
 * @file CFFOutputData.h
 * @author: Bryan BERTHOU (CEA Saclay)
 * @date 28 juil. 2014
 * Last update : 15 September 2014
 * @version 1.0
 */

#include <complex>
#include <string>

#include "CFFInputData.h"

class CFFOutputData: public GenericData {
public:

    CFFOutputData(std::complex<double> cff, CFFInputData cffInputData);
    std::string toString();

private:
    std::complex<double> m_cff;
    CFFInputData m_cffInputData;
};

#endif /* CFF_OUTPUT_DATA_H */
