#include "ObservableResultList.h"

#include <sstream>

#include "../../utils/fileUtils/FileUtils.h"
#include "../../utils/stringUtils/Formatter.h"

ObservableResultList::ObservableResultList() :
        Computation() {
}

ObservableResultList::~ObservableResultList() {
}

// Copy object before add it to m_observableResultList ; we need to do it to set proper DateTime
void ObservableResultList::add(ObservableResult observableResult) {
    observableResult.setDateTime(getDateTime());
    m_observableResultList.push_back(observableResult);
}

std::string ObservableResultList::toString() {
    std::ostringstream os;

    for (unsigned int i = 0; i != m_observableResultList.size(); i++) {
        os << m_observableResultList[i].toString() << std::endl;
    }

    return os.str();
}

void ObservableResultList::add(ObservableResultList observableResultList) {
    for (size_t i = 0; i != observableResultList.size(); i++) {
        add(observableResultList[i]);
    }
}

size_t ObservableResultList::size() const {
    return m_observableResultList.size();
}

ObservableResult& ObservableResultList::operator [](size_t n) {
    return m_observableResultList[n];
}

const ObservableResult& ObservableResultList::operator [](size_t n) const {
    return m_observableResultList[n];
}

void ObservableResultList::printTofileAsList(const std::string &filePath) {
    Formatter formatter;

    for (size_t i = 0; i != m_observableResultList.size(); i++) {
        formatter << m_observableResultList[i].getValue() << ";";
    }

    FileUtils::writef(filePath, formatter.str());
}
