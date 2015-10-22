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
#include <string>
#include <vector>

#include "DVCSConvolCoeffFunctionResult.h"

class DVCSConvolCoeffFunctionResultList {
public:
    DVCSConvolCoeffFunctionResultList();
    virtual ~DVCSConvolCoeffFunctionResultList();

    void add(const DVCSConvolCoeffFunctionResult &result);
    const DVCSConvolCoeffFunctionResult& get(unsigned int index) const;

    size_t getSize() const;
    const DVCSConvolCoeffFunctionResult& getUniqueResult();

    virtual std::string toString();

private:
    std::vector<DVCSConvolCoeffFunctionResult> m_results;
};

#endif /* DVCS_CONVOL_COEFF_FUNCTION_RESULT_LIST_H */
