#include "../../../include/partons/services/VizualisationService.h"

#include <Qt/qsqlquery.h>
#include <Qt/qvariant.h>
#include <QtCore/qstring.h>

#include "../../../include/partons/BaseObjectRegistry.h"
#include "../../../include/partons/utils/plot2D/Plot2D.h"
#include "../../../include/partons/utils/plot2D/Plot2DList.h"

// Initialise [class]::classId with a unique name.
const unsigned int VizualisationService::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new VizualisationService("VizualisationService"));

VizualisationService::VizualisationService(const std::string &className) :
        ServiceObject(className) {
}

VizualisationService::~VizualisationService() {
}

//TODO implement all function
void VizualisationService::computeTask(Task &task) {

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
