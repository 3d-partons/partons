#ifndef GPD_RESULT_DAO_SERVICE
#define GPD_RESULT_DAO_SERVICE

#include "../../BaseObject.h"

class GPDResult;
class GPDResultList;

class GPDResultDaoService: public BaseObject {
public:
    GPDResultDaoService();
    virtual ~ GPDResultDaoService();

    int insert(const GPDResult &gpdResult);

    int insert(const GPDResultList &gpdResultList);

};

#endif /* GPD_RESULT_DAO_SERVICE */
