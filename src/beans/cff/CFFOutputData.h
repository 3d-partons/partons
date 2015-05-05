#ifndef CFF_OUTPUT_DATA_H
#define CFF_OUTPUT_DATA_H

/**
 * @file CFFOutputData.h
 * @author: Bryan BERTHOU (CEA Saclay)
 * @date 28 July 2014
 * @version 1.0
 *
 * @class CFFOutputData
 *
 * @brief
 */

#include <complex>
#include <map>
#include <string>

#include "../gpd/GPDType.h"
#include "CFFInputData.h"

class CFFOutputData {
public:
    CFFOutputData();
    CFFOutputData(CFFInputData cffInputData);

    virtual ~CFFOutputData();

    void add(GPDType::Type gpdComputeType, std::complex<double> cff);
    std::complex<double> getCFF(GPDType::Type gpdComputeType);

    std::string toString();

    // ##### GETTERS & SETTERS #####

    const CFFInputData& getCffInputData() const;
    void setCffInputData(const CFFInputData& cffInputData);

private:
    std::map<GPDType::Type, std::complex<double> > m_listCFFbyGPDComputeType;
    std::map<GPDType::Type, std::complex<double> >::iterator m_it;

    CFFInputData m_cffInputData;
};

#endif /* CFF_OUTPUT_DATA_H */
