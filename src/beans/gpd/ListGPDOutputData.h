#ifndef LIST_GPD_OUTPUT_DATA_H
#define LIST_GPD_OUTPUT_DATA_H

#include <stddef.h>
#include <map>
#include <string>

#include "GPDOutputData.h"

class ListGPDOutputData {
    std::map<std::string, GPDOutputData> m_gpdOutputDataList;

public:
    ListGPDOutputData();
    virtual ~ListGPDOutputData();

    void add(std::string _GPDModuleID, GPDOutputData _gpdOutputData);
    GPDOutputData* get(std::string _GPDModuleID);

    size_t getSize();
    GPDOutputData* getUniqueResult();
};

#endif /* LIST_GPD_OUTPUT_DATA_H */
