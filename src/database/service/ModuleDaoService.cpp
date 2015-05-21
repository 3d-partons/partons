#include "ModuleDaoService.h"

#include "../dao/ModuleDao.h"

int ModuleDaoService::getModuleIdByClassName(
        const std::string &moduleClassName) {
    return ModuleDao::getModuleIdByClassName(moduleClassName);
}
