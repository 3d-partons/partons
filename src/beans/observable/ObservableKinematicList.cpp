#include "ObservableKinematicList.h"

#include "../../utils/fileUtils/FileUtils.h"
#include "../../utils/stringUtils/Formatter.h"
#include "../../utils/stringUtils/StringUtils.h"

ObservableKinematicList::ObservableKinematicList() :
        BaseObject("ObservableKinematicList") {
    // TODO Auto-generated constructor stub

}

ObservableKinematicList::ObservableKinematicList(
        const std::string &kinematicFilePath) :
        BaseObject("ObservableKinematicList") {
    std::vector<std::string> kinematicString = FileUtils::readByLine(
            kinematicFilePath);

    if (kinematicString.empty()) {
        throwException(__func__,
                Formatter() << "Empty kinematic input file : "
                        << kinematicFilePath);
    }

    for (size_t i = 0; i != kinematicString.size(); i++) {
        std::vector<std::string> kinematicValues = StringUtils::split(
                kinematicString[i], '|');
        if (kinematicValues.size() < 4) {
            throwException(__func__,
                    Formatter()
                            << "Missing column value in your kinematic input file : "
                            << kinematicFilePath
                            << " ; You must provided 4 column");
        }

        m_kinematicList.push_back(
                ObservableKinematic(kinematicValues[0], kinematicValues[1],
                        kinematicValues[2], kinematicValues[3]));

    }
}

ObservableKinematicList::~ObservableKinematicList() {
    // TODO Auto-generated destructor stub
}

size_t ObservableKinematicList::size() const {
    return m_kinematicList.size();
}

ObservableKinematic& ObservableKinematicList::operator [](size_t n) {
    return m_kinematicList[n];
}

const ObservableKinematic& ObservableKinematicList::operator [](
        size_t n) const {
    return m_kinematicList[n];
}
