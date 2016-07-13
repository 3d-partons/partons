#include "../../../../include/partons/utils/plot2D/Plot2DList.h"

#include <ElementaryUtils/string_utils/Formatter.h>
#include <vector>

Plot2DList::Plot2DList() :
        List<Plot2D>() {
}

Plot2DList::~Plot2DList() {
}

std::string Plot2DList::toStringPlotFile(const char &splitChar) const {
    ElemUtils::Formatter formatter;

    for (unsigned int i = 0; i != size(); i++) {
        formatter << m_data[i].getX() << splitChar << m_data[i].getY() << '\n';
    }

    return formatter.str();
}
