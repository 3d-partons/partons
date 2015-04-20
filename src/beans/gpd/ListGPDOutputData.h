#ifndef LIST_GPD_OUTPUT_DATA_H
#define LIST_GPD_OUTPUT_DATA_H

/**
 * @file ListGPDOutputData.h
 * @author: Bryan BERTHOU (CEA Saclay)
 * @date 24 November 2014
 * @version 1.0
 *
 * @class ListGPDOutputData
 *
 * @brief
 */

#include <stddef.h>
#include <map>
#include <string>

#include "GPDOutputData.h"

class ListGPDOutputData {
public:
    ListGPDOutputData();
    virtual ~ListGPDOutputData();

    void add(std::string _GPDModuleID, GPDOutputData _gpdOutputData);
    GPDOutputData* get(std::string _GPDModuleID);

    size_t getSize();
    GPDOutputData* getUniqueResult();

private:
    std::map<std::string, GPDOutputData> m_gpdOutputDataList;
};

#endif /* LIST_GPD_OUTPUT_DATA_H */
