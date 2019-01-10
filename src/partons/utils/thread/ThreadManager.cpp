#include "../../../../include/partons/utils/thread/ThreadManager.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>

#include "../../../../include/partons/ModuleObjectFactory.h"
#include "../../../../include/partons/Partons.h"

namespace PARTONS {

ThreadManager::ThreadManager() :
        BaseObject("ThreadManager") {
}

ThreadManager::~ThreadManager() {
}

void ThreadManager::newThread(const unsigned int numberOfThread,
        ModuleObject *pModuleObject) {

    for (unsigned int i = 0; i != numberOfThread; i++) {

        //info
        info(__func__, ElemUtils::Formatter() << "New thread number: " << i);

        //add
        m_listOfModuleObject.add(
                Partons::getInstance()->getModuleObjectFactory()->cloneModuleObject(
                        pModuleObject));
    }
}

//TODO gerer lors d'une exception l'arret et la cloture des thread en cours
void ThreadManager::launchAllAndWaitingFor() {

    if (m_listOfModuleObject.size() == 0) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                "List of instantiated thread(s) is empty");
    }

    // start all thread one by one
    for (unsigned int i = 0; i != m_listOfModuleObject.size(); i++) {

        m_listOfModuleObject[i]->launch();
        sf::sleep(sf::milliseconds(3));
    }

    // waiting end of each thread before continue
    for (unsigned int i = 0; i != m_listOfModuleObject.size(); i++) {
        m_listOfModuleObject[i]->wait();
    }
}

void ThreadManager::clearAllThread() {

    for (unsigned int i = 0; i != m_listOfModuleObject.size(); i++) {

        //info
        info(__func__, ElemUtils::Formatter() << "Delete thread number: " << i);

        //delete
        if (m_listOfModuleObject[i]) {

            Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
                    m_listOfModuleObject[i], 0);
            m_listOfModuleObject[i] = 0;
        }
    }

    //clear
    m_listOfModuleObject.clear();
}

} /* namespace PARTONS */
