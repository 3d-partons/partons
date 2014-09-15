#include "ListGPDOutputData.h"

#include <utility>

ListGPDOutputData::ListGPDOutputData() {
}

ListGPDOutputData::~ListGPDOutputData() {

}

size_t ListGPDOutputData::getSize() {
    return gpdOutputDataList.size();
}

GPDOutputData* ListGPDOutputData::getUniqueResult() {
    if (getSize() != 0) {
        return gpdOutputDataList.begin()->second;
    } else {
        return 0;
    }
}

void ListGPDOutputData::add(std::string _GPDModuleID,
        GPDOutputData* _pGPDOutputData) {
    gpdOutputDataList.insert(
            std::pair<std::string, GPDOutputData*>(_GPDModuleID,
                    _pGPDOutputData));
}

GPDOutputData* ListGPDOutputData::get(std::string _GPDModuleID) {
    return gpdOutputDataList[_GPDModuleID];
}
