#include "ObservableObjectFactory.h"

#include "BaseObjectFactory.h"
#include "modules/observable/Observable.h"

Observable* ObservableObjectFactory::newObservable(unsigned int classId) {
    return static_cast<Observable*>(BaseObjectFactory::getInstance()->newBaseObject(
            classId));
}

Observable* ObservableObjectFactory::newObservable(
        const std::string& className) {
    return static_cast<Observable*>(BaseObjectFactory::getInstance()->newBaseObject(
            className));
}
