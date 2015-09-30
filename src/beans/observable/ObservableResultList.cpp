#include "ObservableResultList.h"

#include <sstream>

ObservableResultList::ObservableResultList() {
    // TODO Auto-generated constructor stub

}

ObservableResultList::~ObservableResultList() {
    // TODO Auto-generated destructor stub
}

void ObservableResultList::add(const ObservableResult& observableResult) {
    m_observableResultList.push_back(observableResult);
}

std::string ObservableResultList::toString() {
    std::ostringstream os;

    for (unsigned int i = 0; i != m_observableResultList.size(); i++) {
        os << m_observableResultList[i].toString() << std::endl;
    }

    return os.str();
}
