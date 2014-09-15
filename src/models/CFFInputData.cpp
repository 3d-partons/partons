#include "CFFInputData.h"

CFFInputData::CFFInputData(double _xi, double _t, double _Q2, double _MuF,
        double _MuR, QCDOrderType _qcdOrderType) :
        xi(_xi), t(_t), Q2(_Q2), MuF(_MuF), MuR(_MuR), qcdOrderType(
                _qcdOrderType) {
}

//CFFInputData::CFFInputData(GPDGenericModel* _pGPDGenericModel, double _Q2,
//        QCDOrderType _QCDOrderType) :
//        pGPDGenericModel(_pGPDGenericModel), Q2(_Q2), qcdOrderType(
//                _QCDOrderType) {
//
//}

CFFInputData::~CFFInputData() {
//    if (pGPDGenericModel != 0) {
//        delete pGPDGenericModel;
//        pGPDGenericModel = 0;
//    }
}

//GPDGenericModel* CFFInputData::getGpdGenericModel() const {
//    return pGPDGenericModel;
//}

double CFFInputData::getQ2() const {
    return Q2;
}

QCDOrderType CFFInputData::getQcdOrderType() const {
    return qcdOrderType;
}
