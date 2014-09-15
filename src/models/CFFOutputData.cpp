#include "CFFOutputData.h"

#include <sstream>

std::string CFFOutputData::toString() {
    std::ostringstream os;

    os << "#[CFFOutputData]" << std::endl;
    os << std::endl;

    return os.str();
}
