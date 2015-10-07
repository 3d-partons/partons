#ifndef OBSERVABLE_OBJECT_FACTORY_H
#define OBSERVABLE_OBJECT_FACTORY_H

/**
 * @file ObservableObjectFactory.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 06 October 2015
 * @version 1.0
 *
 * @class ObservableObjectFactory
 *
 * @brief
 */

#include <string>

class Observable;

class ObservableObjectFactory {
public:
    static Observable* newObservable(unsigned int classId);
    static Observable* newObservable(const std::string & className);
};

#endif /* OBSERVABLE_OBJECT_FACTORY_H */
