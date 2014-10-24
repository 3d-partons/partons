#include "CFFOutputData.h"

#include <map>
#include <utility>

#include "../../utils/stringUtils/Formatter.h"
#include "../gpd/GPDComputeType.h"
#include "CFFInputData.h"

CFFOutputData::CFFOutputData(CFFInputData* pCFFInputData) :
		m_cffInputData(pCFFInputData) {
}

CFFOutputData::CFFOutputData() :
		m_cffInputData(0) {

}

CFFOutputData::~CFFOutputData() {
	if (m_cffInputData != 0) {
		delete m_cffInputData;
		m_cffInputData = 0;
	}
}

std::string CFFOutputData::toString() {
	Formatter formatter;
	formatter << "[CFFOutputData]" << "\n";

	for (m_it = m_listCFFbyGPDComputeType.begin();
			m_it != m_listCFFbyGPDComputeType.end(); m_it++) {
		formatter << GPDComputeType(m_it->first).toString() << " = "
				<< (m_it->second).real() << ", " << (m_it->second).imag()
				<< "\n";
	}

	if (m_cffInputData != 0) {
		formatter << m_cffInputData->toString();
	}

	return formatter.str();
}

void CFFOutputData::add(GPDComputeType::Type gpdComputeType,
		std::complex<double> cff) {
	m_listCFFbyGPDComputeType.insert(std::make_pair(gpdComputeType, cff));
}
