#ifndef GPD_RESULT_DAO
#define GPD_RESULT_DAO

//#include <Qt/qsqltablemodel.h>

#include <QtSql/qsqltablemodel.h>
#include <string>

class GPDResultDao {
public:
    GPDResultDao();
    virtual ~GPDResultDao();

    //unsigned int get
    int insert(double x, double xi, double t, double MuF, double MuR,
            int gpdType, double gluonDistribution, int quarkFlavorType,
            double quarkDistribution, double quarkDistributionPlus,
            double quarkDistributionMinus);

    QSqlRecord getPlot2D(const std::string &abscissaName,
            const std::string &ordinateName);
private:
    QSqlTableModel m_QSqlTableModel;
};

#endif /* GPD_RESULT_DAO */
