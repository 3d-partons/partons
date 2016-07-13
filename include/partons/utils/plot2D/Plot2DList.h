#ifndef PLOT_2D_LIST_H
#define PLOT_2D_LIST_H

/**
 * @file Plot2DList.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date April 29, 2015
 * @version 1.0
 */

#include <string>

#include "../../beans/List.h"
#include "Plot2D.h"

/**
 * @class Plot2DList
 *
 * @brief
 */
class Plot2DList: public List<Plot2D> {
public:
    Plot2DList();
    ~Plot2DList();

    virtual std::string toStringPlotFile(const char &splitChar) const;
};

#endif /* PLOT_2D_LIST_H */
