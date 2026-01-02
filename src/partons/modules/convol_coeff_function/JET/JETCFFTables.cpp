#include "../../../../../include/partons/modules/convol_coeff_function/JET/JETCFFTables.h"

#include <boost/multi_array/multi_array_ref.hpp>
#include <boost/multi_array.hpp>
#include "../../../../../include/partons/beans/convol_coeff_function/ConvolCoeffFunctionResult.h"
#include "../../../../../include/partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h"
#include "../../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../../include/partons/modules/convol_coeff_function/ConvolCoeffFunctionModule.h"
#include <TFile.h>
#include <TNtuple.h>

namespace PARTONS {

const std::string JETCFFTables::PARAMETER_NAME_SET_FILE = "cff_set_file";

const unsigned int JETCFFTables::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new JETCFFTables("JETCFFTables"));

JETCFFTables::JETCFFTables(const std::string &className) :
        JETConvolCoeffFunctionModule(className), m_printOutsideGridWarnings(
                true) {

    //not depend on GPD
    setIsGPDModuleDependent(false);

    //assign function
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::H,
                    &JETConvolCoeffFunctionModule::computeCFF));

    //paths
    m_cffSetFile = "data/JETCFFTables/tables_GK.root";

    m_tablesLoaded = false;
}

JETCFFTables::JETCFFTables(const JETCFFTables &other) :
        JETConvolCoeffFunctionModule(other), m_printOutsideGridWarnings(true) {

    //paths
    m_cffSetFile = other.m_cffSetFile;

    m_tablesLoaded = false;
}

JETCFFTables* JETCFFTables::clone() const {
    return new JETCFFTables(*this);
}

JETCFFTables::~JETCFFTables() {
}

void JETCFFTables::initModule() {
    JETConvolCoeffFunctionModule::initModule();
}

void JETCFFTables::isModuleWellConfigured() {
    JETConvolCoeffFunctionModule::isModuleWellConfigured();
}

std::complex<double> JETCFFTables::computeCFF() {

    //check if loaded
    if (!m_tablesLoaded) {
        loadGrids();
    }

    //check range
    if (! checkRange(m_xi, m_ranges.at(0), std::make_pair(false, false)))
        return std::complex<double>(0., 0.);
    if (! checkRange(m_t, m_ranges.at(1), std::make_pair(false, false)))
        return std::complex<double>(0., 0.);
    if (! checkRange(m_z, m_ranges.at(2), std::make_pair(false, false)))
        return std::complex<double>(0., 0.);
    if (! checkRange(m_qPerp2, m_ranges.at(3), std::make_pair(false, false)))
        return std::complex<double>(0., 0.);
    if (! checkRange(m_Q2, m_ranges.at(4), std::make_pair(false, false)))
        return std::complex<double>(0., 0.);

    //arguments
    array<double, 5> args;

    args[0] = log(m_xi);
    args[1] = m_t;
    args[2] = m_z;
    args[3] = log(m_qPerp2);
    args[4] = log(m_Q2);

    //evaluate
    auto it = m_map.find({m_currentGPDComputeType, m_jetType, m_jetCFFType});

    if (it == m_map.end()) {
        throw ElemUtils::CustomException(getClassName(), __func__, ElemUtils::Formatter() << "Unable to find data for GPD " << GPDType(m_currentGPDComputeType).toString() << " jet type " <<
            JetType(m_jetType).toString() << " and CFF type " << JetCFFType(m_jetCFFType).toString());
    }

    return std::complex<double>(it->second.first->interp(args.begin()), it->second.second->interp(args.begin()));
}

void JETCFFTables::configure(const ElemUtils::Parameters &parameters) {

    //run for mother
    JETConvolCoeffFunctionModule::configure(parameters);

    if (parameters.isAvailable(JETCFFTables::PARAMETER_NAME_SET_FILE)) {

        // get name
        m_cffSetFile = parameters.getLastAvailable().getString();

        //load for the next evaluation
        m_tablesLoaded = false;
    }
}

void JETCFFTables::loadGrids() {

    //clear
    m_map.clear();

    //ranges
    m_ranges = std::vector<std::pair<double, double> > (5, std::make_pair(0., 0.));

    //H
    m_map[{GPDType::H, JetType::UP, JetCFFType::LL}] = loadGrid("H_u_LL");
    m_map[{GPDType::H, JetType::UP, JetCFFType::TL}] = loadGrid("H_u_TL");
    m_map[{GPDType::H, JetType::UP, JetCFFType::TT1}] = loadGrid("H_u_TT1");
    m_map[{GPDType::H, JetType::UP, JetCFFType::TT2}] = loadGrid("H_u_TT2");

    //mark as true
    m_tablesLoaded = true;
}

std::pair<std::shared_ptr<NDInterpolator_5_ML>,
        std::shared_ptr<NDInterpolator_5_ML> > JETCFFTables::loadGrid(
        const std::string& type) {

    //read grid
    std::vector < std::vector<double> > grid = readGrid(type);

    //grid nodes
    std::vector<double> nodesXi;
    std::vector<double> nodesT;
    std::vector<double> nodesZ;
    std::vector<double> nodesQPerp2;
    std::vector<double> nodesQ2;

    //iterator
    std::vector<std::vector<double> >::const_iterator it;

    //loop
    for (it = grid.begin(); it != grid.end(); it++) {

        //set nodes
        if (checkIfUnique(nodesXi, log(it->at(0))))
            nodesXi.push_back(log(it->at(0)));
        if (checkIfUnique(nodesT, it->at(1)))
            nodesT.push_back(it->at(1));
        if (checkIfUnique(nodesZ, it->at(2)))
            nodesZ.push_back(it->at(2));
        if (checkIfUnique(nodesQPerp2, log(it->at(3))))
            nodesQPerp2.push_back(log(it->at(3)));
        if (checkIfUnique(nodesQ2, log(it->at(4))))
            nodesQ2.push_back(log(it->at(4)));
    }

    //sort
    std::sort(nodesXi.begin(), nodesXi.end());
    std::sort(nodesT.begin(), nodesT.end());
    std::sort(nodesZ.begin(), nodesZ.end());
    std::sort(nodesQPerp2.begin(), nodesQPerp2.end());
    std::sort(nodesQ2.begin(), nodesQ2.end());

    //range
    std::pair<double, double> rangeXb = std::make_pair(exp(nodesXi.at(0)),
        exp(nodesXi.at(nodesXi.size() - 1)));
    std::pair<double, double> rangeT = std::make_pair(nodesT.at(0),
        nodesT.at(nodesT.size() - 1));
    std::pair<double, double> rangeZ = std::make_pair(nodesZ.at(0),
        nodesZ.at(nodesZ.size() - 1));
    std::pair<double, double> rangeQPerp2 = std::make_pair(exp(nodesQPerp2.at(0)),
        exp(nodesQPerp2.at(nodesQPerp2.size() - 1)));
    std::pair<double, double> rangeQ2 = std::make_pair(exp(nodesQ2.at(0)),
        exp(nodesQ2.at(nodesQ2.size() - 1)));

    checkAndSetRange(m_ranges.at(0), rangeXb);
    checkAndSetRange(m_ranges.at(1), rangeT);
    checkAndSetRange(m_ranges.at(2), rangeZ);
    checkAndSetRange(m_ranges.at(3), rangeQPerp2);
    checkAndSetRange(m_ranges.at(4), rangeQ2);

    //store
    std::vector < std::vector<double>::iterator > grid_iter_list;

    grid_iter_list.push_back(nodesXi.begin());
    grid_iter_list.push_back(nodesT.begin());
    grid_iter_list.push_back(nodesZ.begin());
    grid_iter_list.push_back(nodesQPerp2.begin());
    grid_iter_list.push_back(nodesQ2.begin());

    array<int, 5> grid_sizes;

    grid_sizes[0] = nodesXi.size();
    grid_sizes[1] = nodesT.size();
    grid_sizes[2] = nodesZ.size();
    grid_sizes[3] = nodesQPerp2.size();
    grid_sizes[4] = nodesQ2.size();

    //values
    int num_elements = grid_sizes[0] * grid_sizes[1] * grid_sizes[2]
            * grid_sizes[3] * grid_sizes[4];

    boost::multi_array<double, 5> f_values_re(grid_sizes);
    boost::multi_array<double, 5> f_values_im(grid_sizes);
    array<int, 5> index;

    for (it = grid.begin(); it != grid.end(); it++) {

        index[0] = findIndex(nodesXi, log(it->at(0)));
        index[1] = findIndex(nodesT, it->at(1));
        index[2] = findIndex(nodesZ, it->at(2));
        index[3] = findIndex(nodesQPerp2, log(it->at(3)));
        index[4] = findIndex(nodesQ2, log(it->at(4)));

        if (index[0] == -1 || index[1] == -1 || index[2] == -1
                || index[3] == -1 || index[4] == -1) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter() << "Node not found for xi: "
                            << log(it->at(0)) << ", t: " << it->at(1)
                            << ", z: " << it->at(2) << ", qPerp2: " << log(it->at(3)) << ", Q2: " << log(it->at(4)));
        }

        f_values_re(index) = it->at(4);
        f_values_im(index) = it->at(5);
    }

    //status
    info(__func__,
            ElemUtils::Formatter() << "CFF " << type
                    << " processed successfully:" << " xi (min, max, nNodes): ("
                    << rangeXb.first << ", " << rangeXb.second << ", "
                    << nodesXi.size() << ")," << " t (min, max, nNodes): ("
                    << rangeT.first << ", " << rangeT.second << ", "
                    << nodesT.size() << ")," << " z (min, max, nNodes): ("
                    << rangeZ.first << ", " << rangeZ.second << ", "
                    << nodesZ.size() << ")," << " qPerp2 (min, max, nNodes): ("
                    << rangeQPerp2.first << ", " << rangeQPerp2.second << ", "
                    << nodesQPerp2.size() << ")," << " Q2 (min, max, nNodes): ("
                    << rangeQ2.first << ", " << rangeQ2.second << ", "
                    << nodesQ2.size() << ")");

    //initialize
    return std::make_pair(
            std::make_shared < NDInterpolator_5_ML
                    > (grid_iter_list.begin(), grid_sizes.begin(), f_values_re.data(), f_values_re.data()
                            + num_elements),
            std::make_shared < NDInterpolator_5_ML
                    > (grid_iter_list.begin(), grid_sizes.begin(), f_values_im.data(), f_values_im.data()
                            + num_elements));
}

void JETCFFTables::checkAndSetRange(std::pair<double, double>& target,
        const std::pair<double, double>& source) {

    if (target.first != target.second) {
        if ((target.first != source.first)
                || (target.second != source.second)) {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter() << "Ranges not compatible: A: ("
                            << target.first << ", " << target.second << "), B: "
                            << source.first << ", " << source.second << ")");
        }
    } else {
        target = source;
    }
}

bool JETCFFTables::checkRange(double& value,
        const std::pair<double, double>& range,
        const std::pair<bool, bool>& freeze) {

    if (value < range.first) {

        if (m_printOutsideGridWarnings) {

            warn(__func__,
                    "Value outside grid, freeze or return zero (more of these warning will be suppressed)");
            m_printOutsideGridWarnings = false;
        }

        if (freeze.first) {
            value = range.first;
        } else {
            return false;
        }
    }

    if (value > range.second) {

        if (m_printOutsideGridWarnings) {

            warn(__func__,
                    "Value outside grid, freeze or return zero (more of these warning will be suppressed)");
            m_printOutsideGridWarnings = false;
        }

        if (freeze.second) {
            value = range.second;
        } else {
            return false;
        }
    }

    return true;
}

std::vector<std::vector<double> > JETCFFTables::readGrid(
        const std::string& type) const {

    //open file
    TFile rootFile(m_cffSetFile.c_str(), "READ");

    if (rootFile.IsZombie()) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Unable to open file: "
                        << m_cffSetFile);
    }

    //result
    std::vector < std::vector<double> > result;

    //tree name
    std::string id = type;

    //variables
    Float_t xi, t, z, qPerp2, Q2, Re, Im;

    //get
    TNtuple* tree = (TNtuple*) rootFile.Get(id.c_str());

    tree->SetBranchAddress("xi", &xi);
    tree->SetBranchAddress("t", &t);
    tree->SetBranchAddress("z", &z);
    tree->SetBranchAddress("qPerp2", &qPerp2);
    tree->SetBranchAddress("Q2", &Q2);
    tree->SetBranchAddress("Re", &Re);
    tree->SetBranchAddress("Im", &Im);

    //loop
    for (Int_t i = 0; i < (Int_t) tree->GetEntries(); i++) {

        tree->GetEntry(i);

        std::vector<double> resultThis(7);

        resultThis.at(0) = xi;
        resultThis.at(1) = t;
        resultThis.at(2) = z;
        resultThis.at(3) = qPerp2;
        resultThis.at(4) = Q2;
        resultThis.at(5) = Re;
        resultThis.at(6) = Im;

        result.push_back(resultThis);
    }

    //close
    rootFile.Close();

    //return
    return result;
}

int JETCFFTables::findIndex(const std::vector<double>& vec,
        double val) const {

    //iterator
    std::vector<double>::const_iterator it;

    //loop
    for (it = vec.begin(); it != vec.end(); it++) {
        if (val == *it)
            return (it - vec.begin());
    }

    return -1;
}

bool JETCFFTables::checkIfUnique(const std::vector<double>& vec,
        double val) const {

    //get index
    int index = findIndex(vec, val);

    //return
    return (index == -1);
}

const std::string& JETCFFTables::getCFFSetFile() const {
    return m_cffSetFile;
}

void JETCFFTables::setCFFSetFile(const std::string& cffSetFile) {
    m_cffSetFile = cffSetFile;
}

} /* namespace PARTONS */
