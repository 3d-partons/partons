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

namespace PARTONS {

/**
 * @class Plot2DList
 *
 * @brief List<Plot2D> with a member to get a string representation to be written in a file.
 */
class Plot2DList: public List<Plot2D> {

public:

    /**
     * Constructor.
     */
    Plot2DList();

    /**
     * Destructor.
     */
    ~Plot2DList();

    /**
     * Get string representation to be written in a file.
     */
    virtual std::string toStringPlotFile(const char &splitChar) const;
};

} /* namespace PARTONS */

#endif /* PLOT_2D_LIST_H */
