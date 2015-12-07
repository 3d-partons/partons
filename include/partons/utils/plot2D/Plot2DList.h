#ifndef PLOT_2D_LIST_H
#define PLOT_2D_LIST_H

/**
 * @file Plot2DList.h
 * @author: Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 29 April 2015
 * @version 1.0
 *
 * @class Plot2DList
 *
 * @brief
 */

#include <vector>

#include "Plot2D.h"

class Plot2DList {
public:
    Plot2DList();
    Plot2DList(const std::vector<Plot2D> &plots);
    ~Plot2DList();

    void addPlot2D(const Plot2D &plot2D);

    // ##### GETTERS & SETTERS #####

    const std::vector<Plot2D>& getPlots() const;
    void setPlots(const std::vector<Plot2D>& plots);

private:
    std::vector<Plot2D> m_plots;
};

#endif /* PLOT_2D_LIST_H */
