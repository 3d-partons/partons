#include "CommonDaoService.h"

CommonDaoService::CommonDaoService() :
        BaseObject("CommonDaoService") {
}

CommonDaoService::~CommonDaoService() {
}

int CommonDaoService::insertComputation(const time_t &dateTime) const {
    return m_commonDao.insertComputation(dateTime);
}

int CommonDaoService::getComputationId(const time_t &dateTime) const {
    return m_commonDao.getComputationIdByDateTime(dateTime);
}
