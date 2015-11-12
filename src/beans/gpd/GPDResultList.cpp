#include "GPDResultList.h"

#include <sstream>

GPDResultList::GPDResultList() :
        m_computationDateTime(time(0)) {

}

GPDResultList::~GPDResultList() {

}

//TODO improve copy object
void GPDResultList::add(GPDResult gpdResult) {
    gpdResult.setComputationDateTime(m_computationDateTime);
    m_gpdResults.push_back(gpdResult);
}

const GPDResult& GPDResultList::get(unsigned int index) const {
    return m_gpdResults[index];
}

size_t GPDResultList::getSize() const {
    return m_gpdResults.size();
}

//TODO implement
const GPDResult& GPDResultList::getUniqueResult() {
    return m_gpdResults[0];
}

std::string GPDResultList::toString() {
    std::ostringstream os;

    for (unsigned int i = 0; i != m_gpdResults.size(); i++) {
        os << m_gpdResults[i].toString() << std::endl;
    }

    return os.str();
}
