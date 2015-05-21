#ifndef GPD_RESULT_DAO_SERVICE
#define GPD_RESULT_DAO_SERVICE
class GPDResultList;

//#include <string>
class GPDResult;

//#include "../dao/GPDResultDao.h"

class GPDResultDaoService {
public:
//    GPDResultDaoService();
//    virtual ~ GPDResultDaoService();

// void insert(const GPDResult &gpdResult);
    static int insert(const GPDResult &gpdResult);

    static int insert(const GPDResultList &gpdResultList);

//    Plot2DList getplot2D(const std::string &abscissaName,
//            const std::string &ordinateName);
//
//private:
//    GPDResultDao m_gpdResultDao;
};

#endif /* GPD_RESULT_DAO_SERVICE */
