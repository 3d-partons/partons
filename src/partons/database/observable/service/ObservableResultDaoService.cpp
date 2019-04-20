#include "../../../../../include/partons/database/observable/service/ObservableResultDaoService.h"

#include <ElementaryUtils/string_utils/StringUtils.h>

namespace PARTONS {

ObservableResultDaoService::ObservableResultDaoService(
        const std::string &className) :
        ResultDaoService(className), m_lastObservableKinematicId(-1), m_lastObservableResultId(
                -1), m_observableKinematicTableFile(
                ElemUtils::StringUtils::EMPTY), m_observableResultTableFile(
                ElemUtils::StringUtils::EMPTY) {
}

ObservableResultDaoService::~ObservableResultDaoService() {
}

} /* namespace PARTONS */
