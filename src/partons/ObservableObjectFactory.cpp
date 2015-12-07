#include "../../include/partons/ObservableObjectFactory.h"

#include "../../include/partons/BaseObjectFactory.h"
#include "../../include/partons/modules/observable/Observable.h"

Observable* ObservableObjectFactory::newObservable(unsigned int classId) {
    return static_cast<Observable*>(BaseObjectFactory::getInstance()->newBaseObject(
            classId));
}

Observable* ObservableObjectFactory::newObservable(
        const std::string& className) {
    return static_cast<Observable*>(BaseObjectFactory::getInstance()->newBaseObject(
            className));
}
