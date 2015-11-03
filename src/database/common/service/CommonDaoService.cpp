#include "CommonDaoService.h"

#include "../dao/CommonDao.h"

CommonDaoService::CommonDaoService() :
        BaseObject("CommonDaoService") {
}

CommonDaoService::~CommonDaoService() {
}

//TODO insert new date
int CommonDaoService::insertComputation(const time_t &dateTime) const {
    return CommonDao::insertComputation(dateTime);
}

int CommonDaoService::getComputationId(const time_t &dateTime) const {
    return CommonDao::getComputationIdByDateTime(dateTime);
}
