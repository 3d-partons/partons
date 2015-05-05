#include "GPDResultList.h"

#include <sstream>
//#include <utility>
#include <vector>

class GPDResult;

GPDResultList::GPDResultList() {

}

GPDResultList::~GPDResultList() {

}

void GPDResultList::add(const GPDResult &gpdResult) {
    m_gpdResults.push_back(gpdResult);
}

unsigned int GPDResultList::getSize() {
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
