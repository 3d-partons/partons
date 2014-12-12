#ifndef CFF_OUTPUT_DATA_H
#define CFF_OUTPUT_DATA_H

/**
 * @file CFFOutputData.h
 * @author: Bryan BERTHOU (CEA Saclay)
 * @date 28 juil. 2014
 * @version 1.0
 */

#include <complex>
#include <map>
#include <string>

#include "../gpd/GPDComputeType.h"
#include "CFFInputData.h"

class CFFOutputData {
public:
    CFFOutputData();
    CFFOutputData(CFFInputData cffInputData);

    virtual ~CFFOutputData();

    void add(GPDComputeType::Type gpdComputeType, std::complex<double> cff);
    std::complex<double> getCFF(GPDComputeType::Type gpdComputeType);

    std::string toString();

    // ##### GETTERS & SETTERS #####

    const CFFInputData& getCffInputData() const;
    void setCffInputData(const CFFInputData& cffInputData);

private:
    std::map<GPDComputeType::Type, std::complex<double> > m_listCFFbyGPDComputeType;
    std::map<GPDComputeType::Type, std::complex<double> >::iterator m_it;

    CFFInputData m_cffInputData;
};

#endif /* CFF_OUTPUT_DATA_H */
