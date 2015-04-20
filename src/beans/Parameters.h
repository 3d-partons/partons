#ifndef PARAMETERS_H
#define PARAMETERS_H

/**
 * @file Parameters.h
 * @author: Bryan BERTHOU (CEA Saclay)
 * @date 08 December 2014
 * @version 1.0
 *
 * @class Parameters
 *
 * @brief
 */

#include <map>
#include <vector>

#include "GenericData.h"

class Parameters {
public:
    Parameters();
    Parameters(unsigned int key, GenericData* value);
    Parameters(unsigned int key, std::vector<GenericData*> values);
    virtual ~Parameters();

    void add(unsigned int key, GenericData* value);
    void add(unsigned int key, std::vector<GenericData*> values);

    std::vector<GenericData*> get(unsigned int key);
    GenericData* getUnique(unsigned int key);
    void clear();

private:
    std::map<unsigned int, std::vector<GenericData*> > m_parameters;
    std::map<unsigned int, std::vector<GenericData*> >::iterator m_it;
};

#endif /* PARAMETERS_H */
