#include "ListGPDOutputData.h"

#include <utility>

ListGPDOutputData::ListGPDOutputData() {
}

ListGPDOutputData::~ListGPDOutputData() {

}

size_t ListGPDOutputData::getSize() {
	return m_gpdOutputDataList.size();
}

GPDOutputData* ListGPDOutputData::getUniqueResult() {
	if (getSize() != 0) {
		return &(m_gpdOutputDataList.begin()->second);
	} else {
		return 0;
	}
}

void ListGPDOutputData::add(std::string _GPDModuleID,
		GPDOutputData _gpdOutputData) {
	m_gpdOutputDataList.insert(
			std::pair<std::string, GPDOutputData>(_GPDModuleID,
					_gpdOutputData));
}

GPDOutputData* ListGPDOutputData::get(std::string _GPDModuleID) {
	return &(m_gpdOutputDataList[_GPDModuleID]);
}
