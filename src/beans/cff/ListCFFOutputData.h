#ifndef LIST_CFF_OUTPUT_DATA_H
#define LIST_CFF_OUTPUT_DATA_H

/**
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 20 november 2014
 * @version 1.0
 */

#include <stddef.h>
#include <map>
#include <string>

#include "CFFOutputData.h"

class GPDOutputData;

class ListCFFOutputData {
public:
    ListCFFOutputData();
    virtual ~ListCFFOutputData();

    void add(std::string _CFFModuleID, GPDOutputData _gpdOutputData);
    CFFOutputData* get(std::string _CFFModuleID);

    size_t getSize();
    CFFOutputData* getUniqueResult();

private:
    std::map<std::string, CFFOutputData> m_cffOutputDataList;
};

#endif /* LIST_CFF_OUTPUT_DATA_H */
