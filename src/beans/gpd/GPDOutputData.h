#ifndef GPD_OUTPUT_DATA_H
#define GPD_OUTPUT_DATA_H

/**
 * @file GPDOutputData.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 24/07/2014
 * @version 1.0
 *
 * Last update : 24 September 2014
 *
 * @class GPDOutputData
 * @brief
 */

#include <map>
#include <string>
#include <vector>

#include "GPDComputeType.h"
#include "GPDResultData.h"

class GPDOutputData: public GenericData {
    /**
     * A list that stores each results of quark flavor GPD identified by the type of GPD computed (H, Ht, E, ...)
     */
    std::map<GPDComputeType::Type, GPDResultData> gpdResults;
    std::map<GPDComputeType::Type, GPDResultData>::iterator m_it;

public:
    /**
     * Default constructor
     */
    GPDOutputData();
    /**
     * Default destructor
     */
    virtual ~GPDOutputData();

    std::vector<GPDComputeType> listGPDTypeComputed();
    void addGPDResultData(GPDResultData gpdResultData);
    GPDResultData* getGPDResultData(GPDComputeType::Type _gpdComputeType);

    std::string toString();
};

#endif /* GPD_OUTPUT_DATA_H */
