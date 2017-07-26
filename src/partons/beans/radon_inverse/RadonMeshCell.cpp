#include "../../../../include/partons/beans/radon_inverse/RadonMeshCell.h"

#include <ElementaryUtils/logger/CustomException.h>

namespace PARTONS {


RadonMeshCell::RadonMeshCell() :
        BaseObject("RadonMeshCell") {
}

void RadonMeshCell::test() {
    throw ElemUtils::CustomException(getClassName(), __func__, "mon texte");
}

} /* namespace PARTONS */
