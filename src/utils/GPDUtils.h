#ifndef GPD_UTILS_H
#define GPD_UTILS_H

/**
 * @file GPDUtils.h
 * @brief
 * @author Bryan BERTHOU (CEA Saclay)
 * @date 25 Juil 2014
 * @version 1.0
 */

class GPDUtils {
public:
    static double convertXBToXi(double xB) {
        return xB / (2. - xB);
    }
};

#endif /* GPD_UTILS_H */
