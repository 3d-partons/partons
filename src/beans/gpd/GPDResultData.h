#ifndef GPD_RESULT_DATA_H
#define GPD_RESULT_DATA_H

/**
 * @file GPDResultData.h
 * @brief
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 24/07/2014
 * Last update : 17 September 2014
 * @version 1.0
 */

#include <stddef.h>
#include <map>
#include <string>
#include <vector>

#include "../QuarkFlavor.h"
#include "GPDComputeType.h"
#include "GPDQuarkFlavorData.h"

//TODO PartonDistributionResultData
class GPDResultData {
public:

    GPDResultData();
    GPDResultData(const GPDResultData & other);
    GPDResultData(GPDComputeType::Type _gpdComputeType);
    ~GPDResultData();

    void addGPDQuarkFlavorData(GPDQuarkFlavorData &_gpdQuarkFlavorData);
    GPDQuarkFlavorData* getGPDQuarkFlavorData(
            QuarkFlavor::Type quarkFlavorType);

    std::vector<QuarkFlavor> listQuarkTypeComputed();

    std::string toString();

    GPDComputeType* getGpdComputeType();
    void setGpdComputeType(GPDComputeType &_gpdComputeType);
    void setGluon(double gluon);
    double getSinglet() const;
    void setSinglet(double singlet);
    double getGluon() const;
//    double getSquareChargeAveraged() const;
//    void setSquareChargeAveraged(double squareChargeAveraged);
    std::vector<GPDQuarkFlavorData> getListOfQuarkFlavorData();
    size_t sizeOfListOfQuarkFlavorData();

private:
    std::map<QuarkFlavor::Type, GPDQuarkFlavorData> m_gpdQuarkFlavorData;
    std::map<QuarkFlavor::Type, GPDQuarkFlavorData>::iterator m_it;

    GPDComputeType m_gpdComputeType;
// double squareChargeAveraged;
    double m_gluon;
    double m_singlet;
};

#endif /* GPD_RESULT_DATA_H */
