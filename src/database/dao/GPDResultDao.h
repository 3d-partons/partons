#ifndef GPD_RESULT_DAO
#define GPD_RESULT_DAO

//#include <Qt/qsqltablemodel.h>

//#include <QtSql/qsqltablemodel.h>

//#include <string>

class GPDResultDao {
public:

//    int insert(double x, double xi, double t, double MuF, double MuR,
//            int gpdType, double gluonDistribution, int quarkFlavorType,
//            double quarkDistribution, double quarkDistributionPlus,
//            double quarkDistributionMinus);

//    QSqlRecord getPlot2D(const std::string &abscissaName,
//            const std::string &ordinateName);

    static int insert(int moduleId, int gpdKinematicId);
};

#endif /* GPD_RESULT_DAO */
