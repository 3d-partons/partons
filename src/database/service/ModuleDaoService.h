#ifndef MODULE_DAO_SERVICE
#define MODULE_DAO_SERVICE

/**
 * @file ModuleDaoService.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 05 May 2015
 * @version 1.0
 *
 * @class ModuleDaoService
 *
 * @brief
 */

#include <string>

class ModuleDaoService {
public:
    static int getModuleIdByClassName(const std::string &moduleClassName);
};

#endif /* MODULE_DAO_SERVICE */
