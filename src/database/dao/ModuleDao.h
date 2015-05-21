#ifndef MODULE_DAO
#define MODULE_DAO

/**
 * @file ModuleDao.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 05 May 2015
 * @version 1.0
 *
 * @class ModuleDao
 *
 * @brief
 */

#include <string>

class ModuleDao {
public:
    static int getModuleIdByClassName(const std::string &className);

private:

};

#endif /* MODULE_DAO */
