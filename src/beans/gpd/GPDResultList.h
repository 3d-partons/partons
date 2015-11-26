#ifndef GPD_RESULT_LIST_H
#define GPD_RESULT_LIST_H

/**
 * @file GPDResultList.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 28 April 2015
 * @version 1.0
 *
 * @class GPDResultList
 *
 * @brief
 */

#include <stddef.h>
#include <ctime>
#include <string>
#include <vector>

#include "GPDResult.h"

class GPDResultList {
public:
    GPDResultList();
    virtual ~GPDResultList();

    void add(GPDResult gpdResult);
    const GPDResult& get(unsigned int index) const;

    size_t size() const;
    const GPDResult& getUniqueResult();

    virtual std::string toString();

    GPDResult& operator[](size_t n);
    const GPDResult& operator[](size_t n) const;

private:
    std::vector<GPDResult> m_gpdResults;

    time_t m_computationDateTime;
};

#endif /* GPD_RESULT_LIST_H */
