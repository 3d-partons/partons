#include "VizualisationService.h"

#include <Qt/qsqlquery.h>
#include <Qt/qvariant.h>
#include <QtCore/qstring.h>

#include "../beans/Scenario.h"
#include "../ServiceRegistry.h"
#include "../utils/plot2D/Plot2D.h"
#include "../utils/plot2D/Plot2DList.h"

const std::string VizualisationService::ID =
        ServiceRegistry::getInstance()->registerNewService(getInstance());

// Global static pointer used to ensure a single instance of the class.
VizualisationService* VizualisationService::pInstance = 0;

VizualisationService::VizualisationService()
        : Service("VizualisationService") {
}

VizualisationService* VizualisationService::getInstance() {
    // Only allow one instance of class to be generated.
    if (!pInstance) {
        pInstance = new VizualisationService();
    }

    return pInstance;
}

VizualisationService::~VizualisationService() {
    if (pInstance != 0) {
        delete pInstance;
        pInstance = 0;
    }
}

//TODO implement all function
void VizualisationService::computeScenario(Scenario scenario) {

}

Plot2DList VizualisationService::getplot2DFromSQLQuery(
        const std::string &sqlQuery) {
    Plot2DList plot2DList;

    QSqlQuery query;

    query.prepare(QString(sqlQuery.c_str()));

    if (query.exec()) {
        while (query.next()) {
            plot2DList.addPlot2D(
                    Plot2D(query.value(0).toDouble(),
                            query.value(1).toDouble()));
        }
    }

    return plot2DList;
}
