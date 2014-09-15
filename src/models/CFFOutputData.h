#ifndef CFF_OUTPUT_DATA_H
#define CFF_OUTPUT_DATA_H

/**
 * @file CFFOutputData.h
 * @author: Bryan BERTHOU (CEA Saclay)
 * @date 28 juil. 2014
 * @version 1.0
 */

#include <string>

#include "GenericData.h"

class CFFOutputData: public GenericData {

public:

    std::string toString();
};

#endif /* CFF_OUTPUT_DATA_H */
