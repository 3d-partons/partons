#ifndef DVCS_CONVOL_COEFF_FUNCTION_RESULT_LIST_H
#define DVCS_CONVOL_COEFF_FUNCTION_RESULT_LIST_H

/**
 * @file DVCSConvolCoeffFunctionResultList.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 22 October 2015
 * @version 1.0
 *
 * @class DVCSConvolCoeffFunctionResultList
 *
 * @brief
 */

#include <stddef.h>
#include <ctime>
#include <string>
#include <vector>

#include "DVCSConvolCoeffFunctionResult.h"

class DVCSConvolCoeffFunctionResultList {
public:
    DVCSConvolCoeffFunctionResultList();
    virtual ~DVCSConvolCoeffFunctionResultList();

    void add(DVCSConvolCoeffFunctionResult result);
    const DVCSConvolCoeffFunctionResult& get(unsigned int index) const;

    size_t size() const;
    const DVCSConvolCoeffFunctionResult& getUniqueResult();

    virtual std::string toString();

    DVCSConvolCoeffFunctionResult& operator[](size_t n);
    const DVCSConvolCoeffFunctionResult& operator[](size_t n) const;

private:
    std::vector<DVCSConvolCoeffFunctionResult> m_results;

    time_t m_computationDateTime;
};

#endif /* DVCS_CONVOL_COEFF_FUNCTION_RESULT_LIST_H */
