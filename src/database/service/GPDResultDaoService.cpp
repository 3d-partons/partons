#include "GPDResultDaoService.h"

#include <QtCore/qstring.h>
#include <QtCore/qvariant.h>
#include <QtSql/qsqlrecord.h>
#include <QtSql/qsqltablemodel.h>
#include <map>
#include <utility>

#include "../../beans/gpd/GPDResult.h"
#include "../../beans/gpd/GPDType.h"
#include "../../beans/kinematic/GPDKinematic.h"
#include "../../beans/parton_distribution/GluonDistribution.h"
#include "../../beans/parton_distribution/PartonDistribution.h"
#include "../../beans/parton_distribution/QuarkDistribution.h"
#include "../../beans/QuarkFlavor.h"
#include "../../utils/plot2D/Plot2D.h"
#include "../../utils/plot2D/Plot2DList.h"

GPDResultDaoService::GPDResultDaoService() {

}
GPDResultDaoService::~GPDResultDaoService() {

}

void GPDResultDaoService::insert(const GPDResult &gpdResult) {

    std::map<GPDType::Type, PartonDistribution> partonDistributions =
            gpdResult.getPartonDistributions();
    std::map<GPDType::Type, PartonDistribution>::iterator itPartonDistributions;

    for (itPartonDistributions = partonDistributions.begin();
            itPartonDistributions != partonDistributions.end();
            itPartonDistributions++) {
        std::map<QuarkFlavor::Type, QuarkDistribution> quarkDistributions =
                (itPartonDistributions->second).getQuarkDistributions();
        std::map<QuarkFlavor::Type, QuarkDistribution>::iterator it_quarkDistribution;

        for (it_quarkDistribution = quarkDistributions.begin();
                it_quarkDistribution != quarkDistributions.end();
                it_quarkDistribution++) {
            m_gpdResultDao.insert(gpdResult.getGpdKinematic().getX(),
                    gpdResult.getGpdKinematic().getXi(),
                    gpdResult.getGpdKinematic().getT(),
                    gpdResult.getGpdKinematic().getMuF(),
                    gpdResult.getGpdKinematic().getMuR(),
                    itPartonDistributions->first,
                    (itPartonDistributions->second).getGluonDistribution().getGluonDistribution(),
                    it_quarkDistribution->first,
                    (it_quarkDistribution->second).getQuarkDistribution(),
                    (it_quarkDistribution->second).getQuarkDistributionPlus(),
                    (it_quarkDistribution->second).getQuarkDistributionMinus());
        }
    }
}

Plot2DList GPDResultDaoService::getplot2D(const std::string &abscissaName,
        const std::string &ordinateName) {
    Plot2DList plot2DList;

    QSqlTableModel model;
    model.setTable(QString::fromStdString(GPDResult::GPD_RESULT_DB_TABLE_NAME));
    model.select();

    int xIndex = model.record().indexOf(QString::fromStdString(abscissaName));
    int yIndex = model.record().indexOf(QString::fromStdString(ordinateName));

    for (int i = 0; i < model.rowCount(); i++) {
        QSqlRecord record = model.record(i);
        plot2DList.addPlot2D(
                Plot2D(record.value(xIndex).toDouble(),
                        record.value(yIndex).toDouble()));
    }

    return plot2DList;
}
