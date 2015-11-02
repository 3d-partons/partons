#include "CommonDaoService.h"

#include "../dao/CommonDao.h"

CommonDaoService::CommonDaoService() :
        BaseObject("CommonDaoService") {
}

CommonDaoService::~CommonDaoService() {
}

//TODO insert new date
int CommonDaoService::insertComputation() const {
    return CommonDao::insertComputation("");
}
