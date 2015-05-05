#include "Plot2DList.h"

Plot2DList::Plot2DList() {

}
Plot2DList::Plot2DList(const std::vector<Plot2D> &plots) {
    m_plots = plots;
}
Plot2DList::~Plot2DList() {

}

void Plot2DList::addPlot2D(const Plot2D &plot2D) {
    m_plots.push_back(plot2D);
}

const std::vector<Plot2D>& Plot2DList::getPlots() const {
    return m_plots;
}

void Plot2DList::setPlots(const std::vector<Plot2D>& plots) {
    m_plots = plots;
}
