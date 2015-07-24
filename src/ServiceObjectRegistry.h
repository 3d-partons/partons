/*
 * ServiceObjectRegistry.h
 *
 *  Created on: Jul 1, 2015
 *      Author: Bryan BERTHOU
 *
 *      Shared unique instance of a service class. Not a copied pointer !
 */

#ifndef SERVICE_OBJECT_REGISTRY_H
#define SERVICE_OBJECT_REGISTRY_H

#include <string>

class BaseObjectRegistry;
class DVCSConvolCoeffFunctionService;
class GPDService;
class ObservableService;
class ServiceObject;
class VizualisationService;

class ServiceObjectRegistry {
public:

    static ServiceObject* get(unsigned int classId);
    static ServiceObject* get(const std::string &className);

    static GPDService* getGPDService();
    static DVCSConvolCoeffFunctionService* getConvolCoeffFunctionService();
    static ObservableService* getObservableService();
    static VizualisationService* getVizualisationService();

private:

    static BaseObjectRegistry* m_pBaseObjectRegistry;

    static void checkBaseObjectRegistryNullPointer();
};

#endif /* SERVICE_OBJECT_REGISTRY_H */
