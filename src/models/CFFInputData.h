#ifndef CFF_INPUT_DATA_H
#define CFF_INPUT_DATA_H

/**
 * @file GPDInputData.h
 * @author: Bryan BERTHOU (CEA Saclay)
 * @date 28 juil. 2014
 * @version 1.0
 */

#include "GenericData.h"
#include "QCDOrderType.h"

class GK11Model;
class GPDGenericModel;

class CFFInputData: public GenericData {
    //GPDGenericModel* pGPDGenericModel;

    double beanId;
    double xi;
    double t;
    double Q2;
    double MuF;
    double MuR;
    QCDOrderType qcdOrderType;

public:
    CFFInputData(double _xi, double _t, double _Q2, double _MuF, double _MuR,
            QCDOrderType _qcdOrderType);
//    CFFInputData(GPDGenericModel* _pGPDGenericModel, double _Q2,
//            QCDOrderType _QCDOrderType);

    virtual ~CFFInputData();

    //GPDGenericModel* getGpdGenericModel() const;
    double getQ2() const;
    QCDOrderType getQcdOrderType() const;
};

#endif /* CFF_INPUT_DATA_H */
