#ifndef GPD_RESULT_DAO_SERVICE
#define GPD_RESULT_DAO_SERVICE

#include <string>

#include "../dao/GPDResultDao.h"

class Plot2DList;

class GPDResult;

class GPDResultDaoService {
public:
    GPDResultDaoService();
    virtual ~ GPDResultDaoService();

    void insert(const GPDResult &gpdResult);

    Plot2DList getplot2D(const std::string &abscissaName,
            const std::string &ordinateName);

private:
    GPDResultDao m_gpdResultDao;
};

#endif /* GPD_RESULT_DAO_SERVICE */
