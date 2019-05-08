#include "../../../../include/partons/utils/plot2D/Plot2DList.h"

#include <ElementaryUtils/string_utils/Formatter.h>

#include "../../../../include/partons/utils/type/PhysicalType.h"
#include "../../../../include/partons/utils/type/PhysicalUnit.h"

namespace PARTONS {

Plot2DList::Plot2DList() :
        List<Plot2D>() {
}

Plot2DList::~Plot2DList() {
}

std::string Plot2DList::toStringPlotFile(const char &splitChar) const {
    ElemUtils::Formatter formatter;

    for (unsigned int i = 0; i != size(); i++) {
        formatter << m_data[i].getX().getValue() << splitChar
                << PhysicalUnit(m_data[i].getX().getUnit()).getShortName()
                << splitChar << m_data[i].getY().getValue() << splitChar
                << PhysicalUnit(m_data[i].getY().getUnit()).getShortName()
                << '\n';
    }

    return formatter.str();
}

} /* namespace PARTONS */
