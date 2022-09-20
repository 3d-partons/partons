#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/parameters/Parameter.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/logger/LoggerManager.h>
#include <NumA/integration/one_dimension/ChebyshevAIntegrator1D.h>
#include <NumA/integration/one_dimension/ChebyshevBIntegrator1D.h>
#include <NumA/integration/one_dimension/DExpIntegrator1D.h>
#include <NumA/integration/one_dimension/GaussLegendreIntegrator1D.h>
#include <NumA/integration/one_dimension/GaussLegendreSeStIntegrator1D.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <NumA/integration/one_dimension/QuadratureIntegrator1D.h>
#include <NumA/integration/one_dimension/TrapezoidalIntegrator1D.h>
#include <NumA/integration/one_dimension/TrapezoidalLogIntegrator1D.h>
#include <NumA/integration/one_dimension/gauss_kronrod_adaptive/GaussKronrodAdaptive.h>
#include <NumA/integration/one_dimension/gauss_kronrod_adaptive/Workspace.h>
#include <NumA/utils/Tolerances.h>
#include <NumA/utils/Errors.h>
#include "../partons/include/partons/FundamentalPhysicalConstants.h"
#include "../partons/include/partons/utils/type/UnitCategory.h"
#include "../partons/include/partons/utils/type/PhysicalUnit.h"
#include "../partons/include/partons/BaseObjectRegistry.h"
#include "../partons/include/partons/Partons.h"
#include "../partons/include/partons/services/automation/XMLParserI.h"
#include "../partons/include/partons/services/automation/AutomationService.h"
#include "../partons/include/partons/beans/automation/Scenario.h"
#include "../partons/include/partons/ServiceObjectRegistry.h"
#include "../partons/include/partons/ModuleObject.h"
#include "../partons/include/partons/ModuleObjectFactory.h"
#include "../partons/include/partons/beans/channel/ChannelType.h"
#include "../partons/include/partons/services/GPDService.h"
#include "../partons/include/partons/beans/automation/Task.h"
#include "../partons/include/partons/beans/automation/BaseObjectData.h"
#include "../partons/include/partons/beans/List.h"
#include "../partons/include/partons/modules/gpd/GPDModule.h"
#include "../partons/include/partons/modules/evolution/gpd/GPDEvolutionModule.h"
#include "../partons/include/partons/beans/gpd/GPDKinematic.h"
#include "../partons/include/partons/beans/gpd/GPDResult.h"
#include "../partons/include/partons/beans/gpd/GPDType.h"
#include "../partons/include/partons/modules/gpd/GPDGK16.h"
#include "../partons/include/partons/modules/gpd/GPDGK11.h"
#include "../partons/include/partons/modules/gpd/GPDGK16Numerical.h"
#include "../partons/include/partons/modules/gpd/GPDHM18.h"
#include "../partons/include/partons/modules/gpd/GPDMMS13.h"
#include "../partons/include/partons/modules/gpd/GPDMPSSW13.h"
#include "../partons/include/partons/modules/gpd/GPDSelectOnePartonType.h"
#include "../partons/include/partons/modules/gpd/GPDVGG99.h"
#include "../partons/include/partons/modules/gpd/GPDVinnikov06.h"
#include "../partons/include/partons/services/DVCSConvolCoeffFunctionService.h"
#include "../partons/include/partons/beans/KinematicUtils.h"
#include "../partons/include/partons/BaseObject.h"
#include "../partons/include/partons/beans/Kinematic.h"
#include "../partons/include/partons/modules/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionModule.h"
#include "../partons/include/partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h"
#include "../partons/include/partons/modules/convol_coeff_function/DVCS/DVCSCFFStandard.h"
#include "../partons/include/partons/modules/convol_coeff_function/DVCS/DVCSCFFConstant.h"
#include "../partons/include/partons/modules/convol_coeff_function/DVCS/DVCSCFFDispersionRelation.h"
#include "../partons/include/partons/modules/convol_coeff_function/DVCS/DVCSCFFHeavyQuark.h"
#include "../partons/include/partons/modules/convol_coeff_function/DVCS/DVCSCFFNN.h"
#include "../partons/include/partons/beans/PerturbativeQCDOrderType.h"
#include "../partons/include/partons/modules/convol_coeff_function/ConvolCoeffFunctionModule.h"
#include "../partons/include/partons/services/ConvolCoeffFunctionService.h"
#include "../partons/include/partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h"
#include "../partons/include/partons/services/DVCSObservableService.h"
#include "../partons/include/partons/modules/scales/DVCS/DVCSScalesModule.h"
#include "../partons/include/partons/modules/scales/DVCS/DVCSScalesQ2Multiplier.h"
#include "../partons/include/partons/modules/observable/DVCS/DVCSObservable.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAc.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAcCos0Phi.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAcCos1Phi.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAcCos2Phi.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAcCos3Phi.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAllMinus.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAllMinusCos0Phi.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAllMinusCos1Phi.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAllMinusCos2Phi.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAllPlus.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAllPlusCos0Phi.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAllPlusCos1Phi.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAllPlusCos2Phi.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAltDVCSCosPhiMPhis.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAltDVCSCosPhiMPhisCos0Phi.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAltDVCSCosPhiMPhisCos1Phi.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAltDVCSSinPhiMPhis.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAltDVCSSinPhiMPhisSin1Phi.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAltIntCosPhiMPhis.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAltIntCosPhiMPhisCos0Phi.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAltIntCosPhiMPhisCos1Phi.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAltIntCosPhiMPhisCos2Phi.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAltIntSinPhiMPhis.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAltIntSinPhiMPhisSin1Phi.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAltIntSinPhiMPhisSin2Phi.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAluDVCS.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAluDVCSSin1Phi.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAluInt.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAluIntSin1Phi.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAluIntSin2Phi.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAluMinus.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAluMinusSin1Phi.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAluPlus.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAulMinus.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAulMinusSin1Phi.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAulMinusSin2Phi.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAulMinusSin3Phi.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAulPlus.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAulPlusSin1Phi.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAulPlusSin2Phi.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAulPlusSin3Phi.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAutDVCSSinPhiMPhis.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAutDVCSSinPhiMPhisCos0Phi.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAutIntSinPhiMPhis.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAutDVCSSinPhiMPhis.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAutIntSinPhiMPhisCos0Phi.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAutIntSinPhiMPhisCos1Phi.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAutIntSinPhiMPhisSin1Phi.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAutMinusSinPhiMPhis.h"
#include "../partons/include/partons/modules/observable/DVCS/asymmetry/DVCSAutMinusSinPhiMPhisCos0Phi.h"
#include "../partons/include/partons/modules/observable/DVCS/cross_section/DVCSCrossSectionDifferenceLUMinus.h"
#include "../partons/include/partons/modules/observable/DVCS/cross_section/DVCSCrossSectionUUBHSubProc.h"
#include "../partons/include/partons/modules/observable/DVCS/cross_section/DVCSCrossSectionUUDVCSSubProc.h"
#include "../partons/include/partons/modules/observable/DVCS/cross_section/DVCSCrossSectionUUDVCSSubProcPhiIntegrated.h"
#include "../partons/include/partons/modules/observable/DVCS/cross_section/DVCSCrossSectionUUMinus.h"
#include "../partons/include/partons/modules/observable/DVCS/cross_section/DVCSCrossSectionUUMinusPhiIntegrated.h"
#include "../partons/include/partons/modules/observable/DVCS/cross_section/DVCSCrossSectionUUVirtualPhotoProduction.h"
#include "../partons/include/partons/modules/observable/DVCS/cross_section/DVCSCrossSectionUUVirtualPhotoProductionPhiIntegrated.h"
#include "../partons/include/partons/modules/observable/DVCS/cross_section/DVCSCrossSectionTotal.h"
#include "../partons/include/partons/modules/xi_converter/DVCS/DVCSXiConverterModule.h"
#include "../partons/include/partons/modules/xi_converter/DVCS/DVCSXiConverterXBToXi.h"
#include "../partons/include/partons/modules/process/DVCS/DVCSProcessModule.h"
#include "../partons/include/partons/modules/process/DVCS/DVCSProcessBMJ12.h"
#include "../partons/include/partons/modules/process/DVCS/DVCSProcessGV08.h"
#include "../partons/include/partons/modules/process/DVCS/DVCSProcessVGG99.h"
#include "../partons/include/partons/beans/observable/ObservableKinematic.h"
#include "../partons/include/partons/beans/observable/DVCS/DVCSObservableKinematic.h"
#include "../partons/include/partons/beans/observable/DVCS/DVCSObservableResult.h"
#include "../partons/include/partons/beans/kinematic/KinematicType.h"
#include "../partons/include/partons/modules/evolution/gpd/GPDEvolutionVinnikov.h"
#include "../partons/include/partons/modules/running_alpha_strong/RunningAlphaStrongModule.h"
#include "../partons/include/partons/modules/running_alpha_strong/RunningAlphaStrongStandard.h"
#include "../partons/include/partons/modules/running_alpha_strong/RunningAlphaStrongVinnikov.h"
#include "../partons/include/partons/modules/active_flavors_thresholds/ActiveFlavorsThresholdsModule.h"
#include "../partons/include/partons/modules/active_flavors_thresholds/ActiveFlavorsThresholdsConstant.h"
#include "../partons/include/partons/modules/active_flavors_thresholds/ActiveFlavorsThresholdsQuarkMasses.h"
#include "../partons/include/partons/modules/MathIntegratorModule.h"
#include "../partons/include/partons/utils/type/PhysicalType.h"



void init() {
    char** t;
    PARTONS::Partons::getInstance()->init(0, t);
}

namespace py = pybind11;

#ifndef PYBIND11_GIL_SCOPED_ACQUIRE
#define PYBIND11_GIL_SCOPED_ACQUIRE ::pybind11::gil_scoped_acquire
#endif

//Methode permettant d'ajouter un attribut de List au module.
template<typename T> void declare_list(py::module &m, const char* typestr) {
    using Class = PARTONS::List<T>;
    std::string pyclass_name = std::string("List") + typestr;
    py::class_<Class>(m, pyclass_name.c_str(), py::buffer_protocol(), py::dynamic_attr())
        .def(py::init<>(), "Constructor")
        .def("add", py::overload_cast<const T&>(&Class::add),(std::string("  Add new ")+typestr).c_str(), py::arg("data"))
        .def("add", py::overload_cast<const Class&>(&Class::add), (std::string("  Add list of new ") +typestr+std::string("s.\n\n    list : List of ")+typestr+std::string("s to be added.")).c_str(), py::arg("list"))
        .def("get", py::overload_cast<size_t>(&Class::get, py::const_), (std::string("  Get reference to element of given index.\n\n    n : Index of requested element.\n\n    return : Reference to requested element.")).c_str(), py::arg("n"))
        .def("size", py::overload_cast<>(&Class::size, py::const_), (std::string("  Get size of this list.\n\n    return : Size of the list")).c_str())
        .def("toString", &Class::toString, (std::string("  Return a pre-formatted characters string for output visualization of ")+typestr+std::string("s values.\n\n    return : A pre-formatted characters string.")).c_str())
        .def("sort", &Class::sort, (std::string("  Sort elements of this list.").c_str()))
        .def("isEmpty", py::overload_cast<>(&Class::isEmpty, py::const_), "  Check if this list is empty.\n\n    return : True if list is empty, otherwise false.")
        .def("clear", &Class::clear, "  Clear list.")
        .def("resize", &Class::resize, "  Resize list to given size.\n\n    n : Size to be set.", py::arg("n"))
        .def("removeLast", &Class::removeLast, (std::string("  Remove the last ")+typestr+std::string(" from this list")).c_str())
        .def("getLast", py::overload_cast<>(&Class::getLast, py::const_), (std::string("  Get the last ")+typestr+std::string(" of this list.\n\n    return : Requested element.")).c_str());
}

template<typename T> void declare_PhysicalType(py::module &m, const char* typestr) {
    using Class = PARTONS::PhysicalType<T>;
    std::string pyclass_name = std::string("PhysicalType") + typestr;
    py::class_<Class, PARTONS::BaseObject>(m, pyclass_name.c_str(), py::buffer_protocol(), py::dynamic_attr())
        .def(py::init<>(), "Default constructor.")
        .def(py::init<PARTONS::PhysicalUnit::Type>(), "Assignement constructor.", py::arg("unit"))
        .def(py::init<T, PARTONS::PhysicalUnit::Type>(), "Assignement constructor.", py::arg("value"), py::arg("unit"))
        .def(py::init<const std::string&, PARTONS::PhysicalUnit::Type>(), "Assignement constructor.", py::arg("stringValue"), py::arg("unit"))
        .def(py::init<const ElemUtils::GenericType&, PARTONS::PhysicalUnit::Type>(), "Assignement constructor.", py::arg("value"), py::arg("unit"))
        .def(py::init<const Class&>(), "Assignement constructor.", py::arg("other"))
        .def("toString", py::overload_cast<>(&Class::toString, py::const_), "  Return string representing the whole object.")
        .def("getValue", py::overload_cast<>(&Class::getValue, py::const_), "  Get value.")
        .def("setValue", &Class::setValue, "  Set value. This function makes the object marked as initialized.", py::arg("value"))
        .def("getUnit", py::overload_cast<>(&Class::getUnit, py::const_), "  Get unit.")
        .def("setUnit", &Class::setUnit, "  Set unit.", py::arg("unit"))
        .def("isInitialized", py::overload_cast<>(&Class::isInitialized, py::const_), "  Check if initialized.")
        .def("setInitialized", &Class::setInitialized, "  Set as initialized.", py::arg("initialized"))
        .def("makeSameUnitAs", py::overload_cast<PARTONS::PhysicalUnit::Type>(&Class::makeSameUnitAs, py::const_), "  Check if the same unit category. If units are different make the conversion.", py::arg("other"))
        .def("makeSameUnitAs", py::overload_cast<const Class&>(&Class::makeSameUnitAs, py::const_), "  Check if the same unit category. If units are different make the conversion.", py::arg("other"))
        .def("checkIfSameUnitAs", py::overload_cast<PARTONS::PhysicalUnit::Type>(&Class::checkIfSameUnitAs, py::const_), "  Check if the same unit. If units are different throw exception.", py::arg("other"))
        .def("checkIfSameUnitAs", py::overload_cast<const Class&>(&Class::checkIfSameUnitAs, py::const_), "  Check if the same unit. If units are different throw exception.", py::arg("other"))
        .def("checkIfSameUnitCategoryAs", py::overload_cast<PARTONS::PhysicalUnit::Type>(&Class::checkIfSameUnitCategoryAs, py::const_), "  Check if same unit category. If units are different throw exception.", py::arg("other"))
        .def("checkIfSameUnitCategoryAs", py::overload_cast<const Class&>(&Class::checkIfSameUnitCategoryAs, py::const_), "  Check if same unit category. If units are different throw exception.", py::arg("other"));
}



//Méthode permettant d'ajouter un modèle GPD GK au module
template<typename T> void declare_gpd_module_gk(py::module &m, const char* typestr) {
    std::string pyclass_name = typestr;
    py::class_<T, PARTONS::GPDModule>(m, pyclass_name.c_str())
        .def(py::init<const std::string&>())
        .def("clone", py::overload_cast<>(&T::clone, py::const_))
        .def("toString", py::overload_cast<>(&T::toString, py::const_))
        .def("configure", &T::configure, py::arg("parameters"))
        .def("getB0", py::overload_cast<>(&T::getB0, py::const_))
        .def("getC1", py::overload_cast<>(&T::getC1, py::const_))
        .def("getC2", py::overload_cast<>(&T::getC2, py::const_))
        .def("getC3", py::overload_cast<>(&T::getC3, py::const_))
        .def("getC4", py::overload_cast<>(&T::getC4, py::const_))
        .def("getC5", py::overload_cast<>(&T::getC5, py::const_))
        .def("getC6", py::overload_cast<>(&T::getC6, py::const_))
        .def("getC7", py::overload_cast<>(&T::getC7, py::const_))
        .def("getC8", py::overload_cast<>(&T::getC8, py::const_))
        .def("getEdval1mtab", py::overload_cast<>(&T::getEdval1mtab, py::const_))
        .def("getEdval1tab", py::overload_cast<>(&T::getEdval1tab, py::const_))
        .def("getEi1tab", py::overload_cast<>(&T::getEi1tab, py::const_))
        .def("getEs1tab", py::overload_cast<>(&T::getEs1tab, py::const_))
        .def("getEtdval1mtab", py::overload_cast<>(&T::getEtdval1mtab, py::const_))
        .def("getEtdval1tab", py::overload_cast<>(&T::getEtdval1tab, py::const_))
        .def("getEtuval1mtab", py::overload_cast<>(&T::getEtuval1mtab, py::const_))
        .def("getEtuval1tab", py::overload_cast<>(&T::getEtuval1tab, py::const_))
        .def("getEuval1mtab", py::overload_cast<>(&T::getEuval1mtab, py::const_))
        .def("getEuval1tab", py::overload_cast<>(&T::getEuval1tab, py::const_))
        .def("getEdValMx", py::overload_cast<>(&T::getEdValMx, py::const_))
        .def("getEtdValMx", py::overload_cast<>(&T::getEtdValMx, py::const_))
        .def("getEtuValMx", py::overload_cast<>(&T::getEtuValMx, py::const_))
        .def("getEuValMx", py::overload_cast<>(&T::getEuValMx, py::const_))
        .def("getHdValMx", py::overload_cast<>(&T::getHdValMx, py::const_))
        .def("getHtdValMx", py::overload_cast<>(&T::getHtdValMx, py::const_))
        .def("getHtuValMx", py::overload_cast<>(&T::getHtuValMx, py::const_))
        .def("getHuValMx", py::overload_cast<>(&T::getHuValMx, py::const_))
        .def("getL", py::overload_cast<>(&T::getL, py::const_))
        .def("getHdval1mtab", py::overload_cast<>(&T::getHdval1mtab, py::const_))
        .def("getHdval1tab", py::overload_cast<>(&T::getHdval1tab, py::const_))
        .def("getHi1tab", py::overload_cast<>(&T::getHi1tab, py::const_))
        .def("getHs1tab", py::overload_cast<>(&T::getHs1tab, py::const_))
        .def("getHtdval1mtab", py::overload_cast<>(&T::getHtdval1mtab, py::const_))
        .def("getHtdval1tab", py::overload_cast<>(&T::getHtdval1tab, py::const_))
        .def("getHti1tab", py::overload_cast<>(&T::getHti1tab, py::const_))
        .def("getHtuval1mtab", py::overload_cast<>(&T::getHtuval1mtab, py::const_))
        .def("getHtuval1tab", py::overload_cast<>(&T::getHtuval1tab, py::const_))
        .def("getHuval1mtab", py::overload_cast<>(&T::getHuval1mtab, py::const_))
        .def("getHuval1tab", py::overload_cast<>(&T::getHuval1tab, py::const_))
        .def("getKappaS", py::overload_cast<>(&T::getKappaS, py::const_))
        .def("getEdval", py::overload_cast<>(&T::getEdval, py::const_))
        .def("getEgluon", py::overload_cast<>(&T::getEgluon, py::const_))
        .def("getEsea", py::overload_cast<>(&T::getEsea, py::const_))
        .def("getEtdval", py::overload_cast<>(&T::getEtdval, py::const_))
        .def("getEtgluon", py::overload_cast<>(&T::getEtgluon, py::const_))
        .def("getEtsea", py::overload_cast<>(&T::getEtsea, py::const_))
        .def("getEtuval", py::overload_cast<>(&T::getEtuval, py::const_))
        .def("getEuval", py::overload_cast<>(&T::getEuval, py::const_))
        .def("getHdval", py::overload_cast<>(&T::getHdval, py::const_))
        .def("getHgluon", py::overload_cast<>(&T::getHgluon, py::const_))
        .def("getHsea", py::overload_cast<>(&T::getHsea, py::const_))
        .def("getHtdval", py::overload_cast<>(&T::getHtdval, py::const_))
        .def("getHtgluon", py::overload_cast<>(&T::getHtgluon, py::const_))
        .def("getHtsea", py::overload_cast<>(&T::getHtsea, py::const_))
        .def("getHtuval", py::overload_cast<>(&T::getHtuval, py::const_))
        .def("getHuval", py::overload_cast<>(&T::getHuval, py::const_))
        .attr("classId") = T::classId;
}

//Méthode permettant d'ajouter un modèle GPD classique au module
template<typename T> void declare_gpd_module_classique(py::module &m, const char* typestr) {
    std::string pyclass_name = typestr;
    py::class_<T, PARTONS::GPDModule>(m, pyclass_name.c_str())
        .def(py::init<std::string&>())
        .def("configure", &T::configure, py::arg("parameters"))
        .attr("classId") = T::classId;
}

template<typename T> void declare_gpd_module_MPSS(py::module &m, const char* typestr) {
    std::string pyclass_name = typestr;
    py::class_<T, PARTONS::GPDModule>(m, pyclass_name.c_str())
        .def(py::init<std::string&>())
        .def("getNbOfQuarkFlavor", py::overload_cast<>(&T::getNbOfQuarkFlavor, py::const_))
        .def("getNbOfColor", py::overload_cast<>(&T::getNbOfColor, py::const_))
        .def("getMx", py::overload_cast<>(&T::getMx, py::const_))
        .def("getCA", py::overload_cast<>(&T::getCA, py::const_))
        .def("getCF", py::overload_cast<>(&T::getCF, py::const_))
        .def("getTF", py::overload_cast<>(&T::getF1d, py::const_))
        .def("getF1d", py::overload_cast<>(&T::getF1d, py::const_))
        .def("getF1u", py::overload_cast<>(&T::getF1u, py::const_))
        .def("getFD", py::overload_cast<>(&T::getFD, py::const_))
        .def("getProfileShapeVal", py::overload_cast<>(&T::getProfileShapeVal, py::const_))
        .def("getProfileShapeSea", py::overload_cast<>(&T::getProfileShapeSea, py::const_))
        .def("getProfileShapeGlue", py::overload_cast<>(&T::getProfileShapeGlue, py::const_))
        .def("getQuarkDTerm", py::overload_cast<>(&T::getQuarkDTerm, py::const_))
        .def("getGluonDTerm", py::overload_cast<>(&T::getGluonDTerm, py::const_))
        .attr("classId") = T::classId;
}

template<typename T> void declare_gpd_module_select(py::module &m, const char* typestr) {
    std::string pyclass_name = typestr;
    py::class_<T, PARTONS::GPDModule> classT(m, pyclass_name.c_str());
    classT.def(py::init<std::string&>())
        .def("getGPDModule", py::overload_cast<>(&T::getGPDModule, py::const_))
        .def("setGPDModule", &T::setGPDModule)
        .def("getPartonType", py::overload_cast<>(&T::getPartonType, py::const_))
        .def("setPartonType", &T::setPartonType);
    classT.attr("classId") = T::classId;
    classT.attr("PARAMETER_NAME_SELECTONEPARTONTYPE_PARTONTYPE") = T::PARAMETER_NAME_SELECTONEPARTONTYPE_PARTONTYPE;
}

template<typename T> void declare_dvcs_module(py::module &m, const char* typestr) {
  py::class_<T, PARTONS::DVCSConvolCoeffFunctionModule> dvcsCFF(m, typestr);
  dvcsCFF.def(py::init<const std::string&>(), "  Constructor\n  See BaseObject.BaseObject and ModuleObject.ModuleObject for more details.\n\n    className : Name of the last child class.")
      .def("clone", py::overload_cast<>(&T::clone, py::const_))
      .attr("classId") = T::classId;
}

template<typename T1, typename T2> void declare_observable(py::module &m, const char* typestr, const char* description) {
    py::class_<T1, T2>(m, typestr, description)
      .def(py::init<const std::string&>(), "  Constructor\n\n    className : Na of the class.")
      .def("clone", py::overload_cast<>(&T1::clone, py::const_))
      .def("configure", &T1::configure)
      .def("setProcessModule", &T1::setProcessModule)
      .attr("classId") = T1::classId;
}

template<typename T1, typename T2> void declare_process(py::module &m, const char* typestr, const char* description) {
    py::class_<T1, T2>(m, typestr, description)
      .def(py::init<const std::string&>(), "  Constructor\n\n    className : Na of the class.")
      .def("clone", py::overload_cast<>(&T1::clone, py::const_))
      .def("configure", &T1::configure)
      .attr("classId") = T1::classId;
}

template<typename T> void declare_quadrature_integrator(py::module&m, const char* typestr,unsigned int valInit, const char* description) {
  py::class_<T, NumA::QuadratureIntegrator1D>(m, typestr, description)
      .def(py::init<unsigned int>(), "  Constructor.\n\n    N : Order of quadrature (number of nodes).", py::arg("N") = valInit)
      .def("clone", py::overload_cast<>(&T::clone, py::const_))
      .def("setN", &T::setN, py::arg("n"));
}



PYBIND11_MODULE(pypartons, m) {

    m.doc()="Partons library";
    auto mPARTONS = m.def_submodule("PARTONS");
    auto mElemUtils = m.def_submodule("ElemUtils");
    auto mNumA = m.def_submodule("NumA");
    auto mPARTONSConstant = mPARTONS.def_submodule("Constant");
      // Wrapping of partons/include/partons/FundamentalPhysicalConstants.h
    mPARTONSConstant.attr("PI") = PyFloat_FromDouble(PARTONS::Constant::PI);
    mPARTONSConstant.attr("ZETA_3") = PyFloat_FromDouble(PARTONS::Constant::ZETA_3);

    mPARTONSConstant.attr("SPEED_OF_LIGHT") = PyFloat_FromDouble(PARTONS::Constant::SPEED_OF_LIGHT);
    mPARTONSConstant.attr("PLANCK_CONSTANT") = PyFloat_FromDouble(PARTONS::Constant::PLANCK_CONSTANT);
    mPARTONSConstant.attr("PLANCK_CONSTANT_REDUCED") = PyFloat_FromDouble(PARTONS::Constant::PLANCK_CONSTANT_REDUCED);
    mPARTONSConstant.attr("FINE_STRUCTURE_CONSTANT") = PyFloat_FromDouble(PARTONS::Constant::FINE_STRUCTURE_CONSTANT);

    mPARTONSConstant.attr("HBarC") = PyFloat_FromDouble(PARTONS::Constant::HBarC);
    mPARTONSConstant.attr("HBarC2") = PyFloat_FromDouble(PARTONS::Constant::HBarC2);
    mPARTONSConstant.attr("CONV_GEVm1_TO_FM") = PyFloat_FromDouble(PARTONS::Constant::CONV_GEVm1_TO_FM);
    mPARTONSConstant.attr("CONV_GEVm2_TO_NBARN") = PyFloat_FromDouble(PARTONS::Constant::CONV_GEVm2_TO_NBARN);
    mPARTONSConstant.attr("CONV_GEVm2_TO_PBARN") = PyFloat_FromDouble(PARTONS::Constant::CONV_GEVm2_TO_PBARN);

    mPARTONSConstant.attr("PROTON_MASS") = PyFloat_FromDouble(PARTONS::Constant::PROTON_MASS);

    mPARTONSConstant.attr("ELECTRON_MASS") = PyFloat_FromDouble(PARTONS::Constant::ELECTRON_MASS);
    mPARTONSConstant.attr("MUON_MASS") = PyFloat_FromDouble(PARTONS::Constant::MUON_MASS);
    mPARTONSConstant.attr("TAU_MASS") = PyFloat_FromDouble(PARTONS::Constant::TAU_MASS);

    mPARTONSConstant.attr("QUARK_DOWN_MASS") = PyFloat_FromDouble(PARTONS::Constant::QUARK_DOWN_MASS);
    mPARTONSConstant.attr("QUARK_UP_MASS") = PyFloat_FromDouble(PARTONS::Constant::QUARK_UP_MASS);
    mPARTONSConstant.attr("QUARK_STRANGE_MASS") = PyFloat_FromDouble(PARTONS::Constant::QUARK_STRANGE_MASS);
    mPARTONSConstant.attr("QUARK_CHARM_MASS") = PyFloat_FromDouble(PARTONS::Constant::QUARK_CHARM_MASS);
    mPARTONSConstant.attr("QUARK_BOTTOM_MASS") = PyFloat_FromDouble(PARTONS::Constant::QUARK_BOTTOM_MASS);
    mPARTONSConstant.attr("QUARK_TOP_MASS") = PyFloat_FromDouble(PARTONS::Constant::QUARK_TOP_MASS);

    mPARTONSConstant.attr("PI_ZERO_MASS") = PyFloat_FromDouble(PARTONS::Constant::PI_ZERO_MASS);

    mPARTONSConstant.attr("Z_BOSON_MASS") = PyFloat_FromDouble(PARTONS::Constant::Z_BOSON_MASS);

    mPARTONSConstant.attr("D_ELEC_CHARGE") = PyFloat_FromDouble(PARTONS::Constant::D_ELEC_CHARGE);
    mPARTONSConstant.attr("U_ELEC_CHARGE") = PyFloat_FromDouble(PARTONS::Constant::U_ELEC_CHARGE);
    mPARTONSConstant.attr("S_ELEC_CHARGE") = PyFloat_FromDouble(PARTONS::Constant::S_ELEC_CHARGE);
    mPARTONSConstant.attr("C_ELEC_CHARGE") = PyFloat_FromDouble(PARTONS::Constant::C_ELEC_CHARGE);
    mPARTONSConstant.attr("B_ELEC_CHARGE") = PyFloat_FromDouble(PARTONS::Constant::B_ELEC_CHARGE);
    mPARTONSConstant.attr("T_ELEC_CHARGE") = PyFloat_FromDouble(PARTONS::Constant::T_ELEC_CHARGE);

    mPARTONSConstant.attr("D2_ELEC_CHARGE") = PyFloat_FromDouble(PARTONS::Constant::D2_ELEC_CHARGE);
    mPARTONSConstant.attr("U2_ELEC_CHARGE") = PyFloat_FromDouble(PARTONS::Constant::U2_ELEC_CHARGE);
    mPARTONSConstant.attr("S2_ELEC_CHARGE") = PyFloat_FromDouble(PARTONS::Constant::S2_ELEC_CHARGE);
    mPARTONSConstant.attr("C2_ELEC_CHARGE") = PyFloat_FromDouble(PARTONS::Constant::C2_ELEC_CHARGE);
    mPARTONSConstant.attr("B2_ELEC_CHARGE") = PyFloat_FromDouble(PARTONS::Constant::B2_ELEC_CHARGE);
    mPARTONSConstant.attr("T2_ELEC_CHARGE") = PyFloat_FromDouble(PARTONS::Constant::T2_ELEC_CHARGE);

    mPARTONSConstant.attr("POSITRON_CHARGE") = PyFloat_FromDouble(PARTONS::Constant::POSITRON_CHARGE);

    // Wrapping of partons/include/partons/utils/type/UnitCategory.h

    py::class_<PARTONS::UnitCategory> unitCategory(mPARTONS, "UnitCategory");

    py::enum_<PARTONS::UnitCategory::Type>(unitCategory, "Type")
      .value("UNDEFINED", PARTONS::UnitCategory::UNDEFINED)
      .value("NONE", PARTONS::UnitCategory::NONE)
      .value("EMP", PARTONS::UnitCategory::EMP)
      .value("EMP2", PARTONS::UnitCategory::EMP2)
      .value("DISTANCE_TIME", PARTONS::UnitCategory::DISTANCE_TIME)
      .value("CROSS_SECTION", PARTONS::UnitCategory::CROSS_SECTION)
      .value("ANGLE", PARTONS::UnitCategory::ANGLE)
      .export_values();

    py::class_<ElemUtils::Packet> packet(mElemUtils, "Packet");

    unitCategory.def(py::init<>())
      .def(py::init<PARTONS::UnitCategory::Type>())
      .def(py::init<const PARTONS::UnitCategory>())
      .def("toString", &PARTONS::UnitCategory::toString)
      .def("getShortName", &PARTONS::UnitCategory::getShortName)
      .def("toString", py::overload_cast<>(&PARTONS::UnitCategory::toString, py::const_))
      .def("serialize", py::overload_cast<ElemUtils::Packet&>(&PARTONS::UnitCategory::serialize, py::const_))
      .def("unserialize", &PARTONS::UnitCategory::unserialize)
      .def("getType", &PARTONS::UnitCategory::getType)
      .def("setType", &PARTONS::UnitCategory::setType);

      //Wrapping of ../partons/partons/include/partons/utils/type/PhysicalUnit.h

    py::class_<PARTONS::PhysicalUnit> physicalUnit(mPARTONS, "PhysicalUnit");

    physicalUnit.def(py::init<>())
      .def(py::init<PARTONS::PhysicalUnit::Type>())
      .def(py::init<const std::string&>())
      .def(py::init<const PARTONS::PhysicalUnit>())
      .def("toString", &PARTONS::PhysicalUnit::toString)
      .def("getShortName", &PARTONS::PhysicalUnit::getShortName)
      .def("serialize", py::overload_cast<ElemUtils::Packet&>(&PARTONS::PhysicalUnit::serialize, py::const_))
      .def("unserialize", &PARTONS::PhysicalUnit::unserialize)
      .def("getUnitCategory", &PARTONS::PhysicalUnit::getUnitCategory)
      .def("getConversionFactor", &PARTONS::PhysicalUnit::getConversionFactor)
      .def("getType", &PARTONS::PhysicalUnit::getType)
      .def("setType", &PARTONS::PhysicalUnit::setType);


    py::enum_<PARTONS::PhysicalUnit::Type>(physicalUnit, "Type")
      .value("UNDEFINED", PARTONS::PhysicalUnit::UNDEFINED)
      .value("NONE", PARTONS::PhysicalUnit::NONE)
      .value("EV", PARTONS::PhysicalUnit::EV)
      .value("KEV", PARTONS::PhysicalUnit::KEV)
      .value("MEV", PARTONS::PhysicalUnit::MEV)
      .value("GEV", PARTONS::PhysicalUnit::GEV)
      .value("TEV", PARTONS::PhysicalUnit::TEV)
      .value("EV2", PARTONS::PhysicalUnit::EV2)
      .value("KEV2", PARTONS::PhysicalUnit::KEV2)
      .value("MEV2", PARTONS::PhysicalUnit::MEV2)
      .value("GEV2", PARTONS::PhysicalUnit::GEV2)
      .value("TEV2", PARTONS::PhysicalUnit::TEV2)
      .value("FM", PARTONS::PhysicalUnit::FM)
      .value("PM", PARTONS::PhysicalUnit::PM)
      .value("NM", PARTONS::PhysicalUnit::NM)
      .value("UM", PARTONS::PhysicalUnit::UM)
      .value("MM", PARTONS::PhysicalUnit::MM)
      .value("M", PARTONS::PhysicalUnit::M)
      .value("FS", PARTONS::PhysicalUnit::FS)
      .value("PS", PARTONS::PhysicalUnit::PS)
      .value("NS", PARTONS::PhysicalUnit::NS)
      .value("US", PARTONS::PhysicalUnit::US)
      .value("MS", PARTONS::PhysicalUnit::MS)
      .value("S", PARTONS::PhysicalUnit::S)
      .value("GEVm2", PARTONS::PhysicalUnit::GEVm2)
      .value("FM2", PARTONS::PhysicalUnit::FM2)
      .value("FB", PARTONS::PhysicalUnit::FB)
      .value("PB", PARTONS::PhysicalUnit::PB)
      .value("NB", PARTONS::PhysicalUnit::NB)
      .value("UB", PARTONS::PhysicalUnit::UB)
      .value("MB", PARTONS::PhysicalUnit::MB)
      .value("B", PARTONS::PhysicalUnit::B)
      .value("DEG", PARTONS::PhysicalUnit::DEG)
      .value("RAD", PARTONS::PhysicalUnit::RAD)
      .value("MRAD", PARTONS::PhysicalUnit::MRAD)
      .export_values();

      //Wrapping of partons/partons/include/partons/BaseObject.h

    py::class_<PARTONS::BaseObject> baseObject(mPARTONS, "BaseObject");

    baseObject.def(py::init<std::string&>(), "  Constructor\n\n    className : Class's name of child class.", py::arg("className"))
        .def("toString", py::overload_cast<>(&PARTONS::BaseObject::toString, py::const_), "  Return a pre-formatted characters string for output visualization of class member's values.\n\n    return : a pre-formatted characters string.")
        .def("serialize", py::overload_cast<ElemUtils::Packet&>(&PARTONS::BaseObject::serialize, py::const_), "  Used to split a complex C++ object into a concatenation of simple type.\n\n    packet", py::arg("packet"))
        .def("unserialize", &PARTONS::BaseObject::unserialize, "  Used to rebuild a complex C++ object from a concatenation of simple type.\n\n    packet", py::arg("packet"))
        .def("getClassName", py::overload_cast<>(&PARTONS::BaseObject::getClassName, py::const_), "    return : Name of the last child class.")
        .def("getObjectId", py::overload_cast<>(&PARTONS::BaseObject::getObjectId, py::const_), "    return : Unique id identifying each object.")
        .def("getIndexId", py::overload_cast<>(&PARTONS::BaseObject::getIndexId, py::const_), "    return : Integer used when object is created from database. It is related to the id column value in the right database table.")
        .def("setIndexId", &PARTONS::BaseObject::setIndexId, "    indexId : Integer used when object is created from database. It related to the id Column value in the right databse table.");

      //Wrapping of partons/partons/include/partons/BaseObjectRegistry.h

    py::class_<PARTONS::BaseObjectRegistry> baseObjectRegistry(mPARTONS, "BaseObjectRegistry", "    The Registry is the analog of a phonebook, whitch lists all available objects (modules or services most of the time) identified by a unique integer identifier or by a unique string (class name) for translation. And only one species of each.\n\n  From the point of view of software enginnering, the reistry corresponds to the singleton design pattern whitch ensures taht it is unique.\n  When a new module or services are created, the first thing ti di is to call this unique instance, and to register the new module or serviceswith a name provided by the developer of the class\n  In turn the Registry gives a unique identifier encoded ina integer varaible for performance purpose.\n  Registry stores pointers to all objects in a generic way, i.e. whatever their nature are: pointers to GPDModule, to RunningAlphaStrongModule, to AutomationService, etc.\n  This is achieved by requiring all objects to derive from a single parent class named BaseObject.");

    baseObjectRegistry.def("getInstance", &PARTONS::BaseObjectRegistry::getInstance, "  Static function to be able to retrieve a unique instance pointer of this class anywhere in the code.\n\n    return : A unique instance of this class.")
        .def("registerBaseObject", &PARTONS::BaseObjectRegistry::registerBaseObject, "  Store a unique instance of a module identified by a unique string character key.\n\n    BaseObject : An instance of the module built by its default constructor.\n    return : A unique identifier by class.", py::arg("BaseObject"))
        .def("resolveBaseObjectDependencies", &PARTONS::BaseObjectRegistry::resolveBaseObjectDependencies, "  Some objects like modules depend on each other.\n  But it is like impossible to guarantee the order in whitch they are added to the registry will be made upon the resolution of statics variables (mechanism of self-registration)\n  So we need to perform this task just before the use of PARTONS software to resolve dependencies between all kind of registered objects")
        .def("toString", &PARTONS::BaseObjectRegistry::toString, "  Print information about the current state of registry (size of the registry, whitch objects are stored, ...).\n\n    return : string.")
        .def("get", py::overload_cast<unsigned int>(&PARTONS::BaseObjectRegistry::get, py::const_), "  Try to find stored object identified by its unique class identifier\n\n    classId\n    return : reference to object in memory or throw an exception", py::arg("classId"))
        .def("get", py::overload_cast<const std::string&>(&PARTONS::BaseObjectRegistry::get, py::const_), "  Try to find stored object identified by its class name.\n\n    className\n    return : reference to object in memory ot throw an exception.")
        .def("size", &PARTONS::BaseObjectRegistry::size, "  Return the size of the registry.\n  How many objects are stored.\n\n    return : Registry size.")
        .def("getObjectClassIdByClassName", &PARTONS::BaseObjectRegistry::getObjectClassIdByClassName);

      //Wrapping of partons/partons/include/partons/beans/Kinematic.h

    py::class_<PARTONS::Kinematic, PARTONS::BaseObject> kinematic(mPARTONS, "Kinematic");

    kinematic.def("toString", py::overload_cast<>(&PARTONS::Kinematic::toString, py::const_))
        .def("serialize", py::overload_cast<ElemUtils::Packet&>(&PARTONS::Kinematic::serialize, py::const_), "  Serialize into given Packet.\n\n    packet : Target Packet.", py::arg("packet"))
        .def("unserialize", &PARTONS::Kinematic::unserialize, "  Retrieve data from given Packet\n\n    packet : Input Packet.")
        .def("serializeIntoStdVector", py::overload_cast<std::vector<double>&>(&PARTONS::Kinematic::serializeIntoStdVector, py::const_), "  Serialize into vector<double>.")
        .def("unserializeFromStdVector", &PARTONS::Kinematic::unserializeFromStdVector, "  Unserialize from vector<double>.")
        .def("getChannelType", py::overload_cast<>(&PARTONS::Kinematic::getChannelType, py::const_), "  Get channel type.")
        .def("getHashSum", py::overload_cast<>(&PARTONS::Kinematic::getHashSum, py::const_), "  Get hash sum of class content.")
        .def("setHashSum", py::overload_cast<const std::string&>(&PARTONS::Kinematic::setHashSum, py::const_), "  Set hash sum of class content.\n\n    hashSum", py::arg("hashSum"));


      //Wrapping of partons/partons/include/partons/Partons.h

    py::class_<PARTONS::Partons> partons(mPARTONS, "Partons");

    partons.def("getInstance", py::overload_cast<>(&PARTONS::Partons::getInstance), py::return_value_policy::reference, "  Share a unique pointer of this class.")
        .def("init", &init, "  Must be called before any other thing.\n  It initializeseach mandatory bloc in right order.")
        .def("close", &PARTONS::Partons::close, "  Called at the very end of the code.\n  Send close to the Logger Manager and wait to the end of job (flush the logger buffer) before free memory.")
        .def("getBaseObjectRegistry", &PARTONS::Partons::getBaseObjectRegistry, py::return_value_policy::reference)
        .def("getServiceObjectRegistry", &PARTONS::Partons::getServiceObjectRegistry, py::return_value_policy::reference)
        //.def("getBaseObjectFactory", &PARTONS::Partons::getBaseObjectFactory, py::return_value_policy::reference);
        .def("getModuleObjectFactory", &PARTONS::Partons::getModuleObjectFactory, py::return_value_policy::reference)
        .def("getLoggerManager", &PARTONS::Partons::getLoggerManager, py::return_value_policy::reference);
        //.def("getEnvironmentConfiguration", &PARTONS::Partons::getEnvironmentConfiguration, py::return_value_policy::reference);

      //Wrapping of partons/partons/include/partons/services/automation/XMLParserI.h
/*
    py::class_<PARTONS::XMLParserI> xmlParserI(mPARTONS, "XMLParserI");

    xmlParserI.def(py::init<const std::string&>())
        //.def("parseScenario", &PARTONS::XMLParserI::parseScenario);
        .def("parseScenario", py::overload_cast<PARTONS::Scenario*>(&PARTONS::XMLParserI::parseScenario));

      //Wrapping of partons/partons/include/partons/services/automation/AutomationService.h
*/
    py::class_<PARTONS::AutomationService, PARTONS::BaseObject> automationService(mPARTONS, "AutomationService", "    Automation service to dynamically run complex tasks (by calling service object methods) or to create some complex C++ objects, all described by a XML file.\n\n  It allows users to perform complex tasks in a simple way without know C++ programming language ans without rebuild their project.\n  Automation service provides some methods to retrieve Scenario or ComputationConfiguration objects from a specific XML file.\n  Scenario can be used to automates the whole computation process by calling many other servicess to performs tasks previously defined in XML file.\n  ComputeConfiguration is only used to configure a specific module, then you can do whatever you want this new pre-configured module.");

    automationService.def(py::init<const std::string&>())
        .def("parseXMLFile", &PARTONS::AutomationService::parseXMLFile)
        .def("playScenario", &PARTONS::AutomationService::playScenario);

      // Wrapping of partons/partons/include/partons/beans/automation/Scenario.h

    py::class_<PARTONS::Scenario> scenario(mPARTONS, "Scenario");

    scenario.def(py::init<>());
        //.def(py::init<const std::string&>())
        //.def(py::init<const int, const std::string&, const time_t, const std::string&, const std::string&, const std::string&>())
/*        .def("getTask", &PARTONS::Scenario::getTask)
        .def("add", &PARTONS::Scenario::add)
        .def("size", &PARTONS::Scenario::size)
        .def("getDescription", &PARTONS::Scenario::getDescription)
        .def("getTasks", &PARTONS::Scenario::getTasks)
        .def("setTasks", &PARTONS::Scenario::setTasks);*/

      //wrapping of partons/partons/include/partons/ServiceObjectRegistry.h

    py::class_<PARTONS::ServiceObjectRegistry> serviceObjectRegistry(mPARTONS, "ServiceObjectRegistry");

    serviceObjectRegistry//.def("get", &PARTONS::ServiceObjectRegistry::get)
        .def("getDVCSConvolCoeffFunctionService", py::overload_cast<>(&PARTONS::ServiceObjectRegistry::getDVCSConvolCoeffFunctionService, py::const_), py::return_value_policy::reference)
        .def("getAutomationService", py::overload_cast<>(&PARTONS::ServiceObjectRegistry::getAutomationService, py::const_), py::return_value_policy::reference)
        .def("getGPDService", py::overload_cast<>(&PARTONS::ServiceObjectRegistry::getGPDService, py::const_), py::return_value_policy::reference)
        .def("getDVCSObservableService", py::overload_cast<>(&PARTONS::ServiceObjectRegistry::getDVCSObservableService, py::const_), py::return_value_policy::reference);

      //Wrapping of partons/partons/include/partons/ModuleObject

    py::class_<PARTONS::ModuleObject, PARTONS::BaseObject> moduleObject(mPARTONS, "ModuleObject");

    moduleObject.def("getReferenceModuleId", &PARTONS::ModuleObject::getReferenceModuleId)
        .def("setReferenceModuleId", &PARTONS::ModuleObject::setReferenceModuleId)
        .def("getChannelType", &PARTONS::ModuleObject::getChannelType)
        .attr("CLASS_NAME") = py::cast(PARTONS::ModuleObject::CLASS_NAME);

      //Wrapping of partons/partons/include/partons/beans/channel/ChannelType.h

    py::class_<PARTONS::ChannelType> channelType(mPARTONS, "ChannelType");

    py::enum_<PARTONS::ChannelType::Type>(channelType, "Type")
        .value("UNDEFINED", PARTONS::ChannelType::UNDEFINED)
        .value("DVCS", PARTONS::ChannelType::DVCS)
        .value("DVMP", PARTONS::ChannelType::DVMP)
        .value("TCS", PARTONS::ChannelType::TCS)
        .export_values();

    channelType.def(py::init<>())
        .def(py::init<PARTONS::ChannelType::Type>())
        .def("toString", &PARTONS::ChannelType::toString)
        .def("getShortName", &PARTONS::ChannelType::getShortName)
        .def("getType", &PARTONS::ChannelType::getType)
        .def("setType", &PARTONS::ChannelType::setType);

      //warpping of partons/partons/include/partons/beans/List.h

        //declare_list<PARTONS::GPDType::Type>(mPARTONS, "GPDType");
        declare_list<PARTONS::GPDResult>(mPARTONS, "GPDResult");
        //declare_list<PARTONS::GPDModule>(mPARTONS, "GPDModule");
        declare_list<PARTONS::DVCSConvolCoeffFunctionKinematic>(mPARTONS, "DVCSConvolCoeffFunctionKinematic");
        declare_list<PARTONS::GPDKinematic>(mPARTONS, "GPDKinematic");
        declare_list<PARTONS::DVCSConvolCoeffFunctionResult>(mPARTONS, "DVCSConvolCoeffFunctionResult");
        declare_list<PARTONS::DVCSObservableKinematic>(mPARTONS, "DVCSObservableKinematic");
        declare_list<PARTONS::KinematicType>(mPARTONS, "KinematicType");
        declare_list<PARTONS::DVCSObservableResult>(mPARTONS, "DVCSObservableResult");

    py::class_<PARTONS::List<PARTONS::GPDType>>(mPARTONS, "ListGPDType")
        .def(py::init<>(), "Constructor")
        .def("add", ([](PARTONS::List<PARTONS::GPDType>* l, const PARTONS::GPDType::Type t){l->add(t);}), py::arg("data"))
        .def("add", py::overload_cast<const PARTONS::List<PARTONS::GPDType>&>(&PARTONS::List<PARTONS::GPDType>::add), (std::string("  Add list of new GPDType.\n\n    list : List of GPDTypes to be added.")).c_str(), py::arg("list"))
        .def("get", py::overload_cast<size_t>(&PARTONS::List<PARTONS::GPDType>::get, py::const_), (std::string("  Get reference to element of given index.\n\n    n : Index of requested element.\n\n    return : Reference to requested element.")).c_str(), py::arg("n"))
        .def("size", py::overload_cast<>(&PARTONS::List<PARTONS::GPDType>::size, py::const_), (std::string("  Get size of this list.\n\n    return : Size of the list")).c_str())
        .def("sort", &PARTONS::List<PARTONS::GPDType>::sort, (std::string("  Sort elements of this list.").c_str()))
        .def("toString", &PARTONS::List<PARTONS::GPDType>::toString, (std::string("  Return a pre-formatted characters string for output visualization of GPDTypes values.\n\n    return : A pre-formatted characters string.")).c_str())
        .def("isEmpty", py::overload_cast<>(&PARTONS::List<PARTONS::GPDType>::isEmpty, py::const_), "  Check if this list is empty.\n\n    return : True if list is empty, otherwise false.")
        .def("clear", &PARTONS::List<PARTONS::GPDType>::clear, "  Clear list.")
        .def("resize", &PARTONS::List<PARTONS::GPDType>::resize, "  Resize list to given size.\n\n    n : Size to be set.", py::arg("n"))
        .def("removeLast", &PARTONS::List<PARTONS::GPDType>::removeLast, (std::string("  Remove the last GPDType.Type from this list")).c_str())
        .def("getLast", py::overload_cast<>(&PARTONS::List<PARTONS::GPDType>::getLast, py::const_), (std::string("  Get the last GPDType.Type of this list.\n\n    return : Requested element.")).c_str());

      //Wrapping of partons/partons/include/partons/services/GPDService.h

    py::class_<PARTONS::GPDService> gpdService(mPARTONS, "GPDService");

    gpdService.def("computeSingleKinematic", py::overload_cast<const PARTONS::GPDKinematic&, PARTONS::GPDModule*, const PARTONS::List<PARTONS::GPDType>&>(&PARTONS::GPDService::computeSingleKinematic, py::const_), "  Computes GPD model at specific Kinematics.\n\n    gpdKinematic : GPD Kinematic.\n    gpdModule : GPDModule to use.\n    gpdType : List of GPDType to compute. Default : all the GPDTypes available with the GPDModule.\n    return : GPDResult", py::arg("gpdKinematic"), py::arg("gpdModule"), py::arg_v("gpdType", PARTONS::List<PARTONS::GPDType>(), "ListGPDType()"))
                    //&PARTONS::GPDService::computeSingleKinematic, "Calculate a single Kinematic"/*, py::arg("gpdKinematic"), py::arg("gpdModule"), py::arg("gpdType")=PARTONS::List<PARTONS::GPDType>()*/)
        .def("computeManyKinematic", &PARTONS::GPDService::computeManyKinematic, "  Computes a GPD model for a list of kinematics.\n\n    gpdKinematicList : List of gpdKinematics.\n    gpdModule : GPDModule to use for the computation.\n    gpdTypeList : List of GPD to compute. Default : all the GPDTypes available with the GPDModule.\n    storeInDB : Boolean to store the results and kinematics on the database. Default : false.\n    retrun : List of GPDResult", py::arg("gpdKinematicList"), py::arg("gpdModule"), py::arg_v("gpdTypeList", PARTONS::List<PARTONS::GPDType>(), "ListGPDType()"))
        .def("newGPDModuleFromTask", py::overload_cast<const PARTONS::Task&>(&PARTONS::GPDService::newGPDModuleFromTask, py::const_), "  Devises the GPDModule from an automation task.\n\n    task : Automation task.\n    return : GPDModule pointer.", py::arg("task"))
        //.def("newKinematicFromTask", py::overload_cast<const PARTONS::Task&>(&PARTONS::GPDService::newKinematicFromTask, py::const_))
        //.def("newListOfKinematicFromTask", py::overload_cast<const PARTONS::Task&>(&PARTONS::GPDService::newListOfKinematicFromTask, py::const_));
        ;

      //Wrapping of partons/partons/include/partons/beans/automation/Task.h

    py::class_<PARTONS::Task, PARTONS::BaseObject> task(mPARTONS, "Task");

    task.def(py::init<>())
        .def("toString", py::overload_cast<>(&PARTONS::Task::toString, py::const_))
        .def("getScenario", &PARTONS::Task::getScenario, "  Get scenario that hold this task.")
        .def("setScenario", &PARTONS::Task::setScenario, "  Set the scenario that hold this task.", py::arg("scenario"))
        .def("getServiceName", py::overload_cast<>(&PARTONS::Task::getServiceName, py::const_), "  Get name of service responsible for running this scenario.")
        .def("setServiceName", &PARTONS::Task::setServiceName, "  Set name of service responsable for running this scenario.", py::arg("serviceName"))
        .def("getFunctionName", py::overload_cast<>(&PARTONS::Task::getFunctionName, py::const_), "  Get name of function to be evaluated by this task.")
        .def("setFunctionName", &PARTONS::Task::setFunctionName, "  Set name of function to be evaluated by this task.", py::arg("functionName"))
        .def("isStoreInDB", &PARTONS::Task::isStoreInDB, "  Check if the result of this task is going to be store in the database.")
        .def("setStoreInDB", &PARTONS::Task::setStoreInDB, "  Set if the result of this task is going to be store in the database.", py::arg("storeInDB"))
        .def("getScenarioTaskIndexNumber", py::overload_cast<>(&PARTONS::Task::getScenarioTaskIndexNumber, py::const_), "  Get index of this task in a given scenario holding it.")
        .def("setScenarioTaskIndexNumber", &PARTONS::Task::setScenarioTaskIndexNumber, "  Set index of this task in a given scenario holding it.", py::arg("scenarioTaskIndexNumber"))
        .def("getModuleComputationConfiguration", py::overload_cast<>(&PARTONS::Task::getModuleComputationConfiguration, py::const_), "  Get base object data stored in xml block given by XMLParserI.COMPUTATION_CONFIGURATION_NODE_NAME tag.")
        .def("setModuleComputationConfiguration", &PARTONS::Task::setModuleComputationConfiguration, "  Set base object data stored in XMLParserI.COMPUTATION_CONFIGURATION_NODE_NAME", py::arg("moduleComputationConfiguration"))
        .def("getKinematicsData", py::overload_cast<>(&PARTONS::Task::getKinematicsData, py::const_), "  Get base object data stored in xml block given by xmlParserI.KINEMATICS_NODE_NAME")
        .def("setKinematicsData", &PARTONS::Task::setKinematicsData, "  Set base object data stored in xml block given by xmlParserI.KINEMATICS_NODE_NAME", py::arg("kinematicsData"))
        .def("getTaskParameters", py::overload_cast<>(&PARTONS::Task::getTaskParameters, py::const_), "  Get base object data stored in xml block given by XMLParserI.TASK_NODE_NAME tag")
        .def("setTaskParameters", &PARTONS::Task::setTaskParameters, "  Get base object data stored in xml block given by XMLParserI.TASK_NODE_NAME tag", py::arg("taskParameters"));

      //Wrapping of partons/partons/include/partons/beans/automation/BaseObjectData.h

    py::class_<PARTONS::BaseObjectData, PARTONS::BaseObject> baseObjectData(mPARTONS, "BaseObjectData");

    baseObjectData.def(py::init<>())
        .def(py::init<const std::string&, const std::string&>())
        .def("addSubModule", &PARTONS::BaseObjectData::addSubModule)
        .def("addParameter", &PARTONS::BaseObjectData::addParameter)
        .def("addParameters", &PARTONS::BaseObjectData::addParameters)
        //.def("isAvailableSubModule", py::overload_cast<const std::string&>(&PARTONS::BaseObjectData::isAvailableSubMobule, py::const_))
        .def("getLastAvailable", py::overload_cast<>(&PARTONS::BaseObjectData::getLastAvailable, py::const_))
        .def("toString", py::overload_cast<>(&PARTONS::BaseObjectData::toString, py::const_))
        .def("getModuleType", py::overload_cast<>(&PARTONS::BaseObjectData::getModuleType, py::const_))
        .def("setModuleType", &PARTONS::BaseObjectData::setModuleType)
        .def("getModuleClassName", py::overload_cast<>(&PARTONS::BaseObjectData::getModuleClassName, py::const_))
        .def("setModuleClassName", &PARTONS::BaseObjectData::setModuleClassName)
        .def("getParameters", py::overload_cast<>(&PARTONS::BaseObjectData::getParameters, py::const_))
        .def("setParameters", &PARTONS::BaseObjectData::setParameters)
        .def("getSubModules", py::overload_cast<>(&PARTONS::BaseObjectData::getSubModules,py::const_));


      //Wrapping of partons/partons/include/partons/modules/gpd/GPDModule.h

    py::class_<PARTONS::GPDModule, PARTONS::ModuleObject> gpdModule(mPARTONS, "GPDModule");

    gpdModule.def("getListOfAvailableGPDTypeForComputation", py::overload_cast<>(&PARTONS::GPDModule::getListOfAvailableGPDTypeForComputation, py::const_))
        .def("getMuF2Ref", py::overload_cast<>(&PARTONS::GPDModule::getMuF2Ref, py::const_))
        .def("getEvolQcdModule", py::overload_cast<>(&PARTONS::GPDModule::getEvolQcdModule, py::const_))
        .def("setEvolQcdModule", &PARTONS::GPDModule::setEvolQcdModule, py::arg("evolQcdModule"));


      //Wrapping of partons/partons/include/partons/ModuleObjectFactory.h

    py::class_<PARTONS::ModuleObjectFactory, PARTONS::BaseObject> moduleObjectFactory(mPARTONS, "ModuleObjectFactory");

    moduleObjectFactory.def("newGPDModule", py::overload_cast<const std::string&>(&PARTONS::ModuleObjectFactory::newGPDModule), py::return_value_policy::reference, "  Specialization of BaseObjectFactory.newBaseObject into ModuleObject.\n\n    className : Name of the last child class.\n    return : ModuleObject.", py::arg("className"))
        .def("newGPDModule", py::overload_cast<unsigned int>(&PARTONS::ModuleObjectFactory::newGPDModule), py::return_value_policy::reference, "  Specialization of BaseObjectFactory.newBaseObject into ModuleObject.\n\n    moduleObjectOrig : Module object to be cloned.\n    return : Cloned object.", py::arg("classId"))
        .def("newDVCSConvolCoeffFunctionModule", py::overload_cast<unsigned int>(&PARTONS::ModuleObjectFactory::newDVCSConvolCoeffFunctionModule), py::return_value_policy::reference, "  Specialization of ModuleObjectFactory.newModuleObject into dvcsConvolCoeffFunctionModule.\n\n    classId : Unique identifier of last child class.\n    return : DVCSConvolCoeffFunctionModule.", py::arg("classId"))
        .def("newDVCSConvolCoeffFunctionModule", py::overload_cast<const std::string&>(&PARTONS::ModuleObjectFactory::newDVCSConvolCoeffFunctionModule), py::return_value_policy::reference, "  Specialization of ModuleObjectFactory.newModuleObject into dvcsConvolCoeffFunctionModule.\n\n    classname : Name of last child class.\n    return : dvcsConvolCoeffFunctionModule.", py::arg("className"))
        .def("newDVCSScalesModule", py::overload_cast<unsigned int>(&PARTONS::ModuleObjectFactory::newDVCSScalesModule), py::return_value_policy::reference, "  Specialization of ModuleObjectFactory.newModuleObject into a DVCSScalesModule.\n\n    classId : Unique identifier of last child.\n    return : DVCSScalesModule.", py::arg("classId"))
        .def("newDVCSScalesModule", py::overload_cast<const std::string&>(&PARTONS::ModuleObjectFactory::newDVCSScalesModule), py::return_value_policy::reference, "  Specialization of ModuleObjectFactory.newModuleObject into a DVCSScalesModule.\n\n    className : Name of last child class.\n    return : DVCSScalesModule.", py::arg("className"))
        .def("newDVCSXiConverterModule", py::overload_cast<unsigned int>(&PARTONS::ModuleObjectFactory::newDVCSXiConverterModule), py::return_value_policy::reference, "  Specialization of ModuleObjectFactory.newModuleObject into a DVCSXiConverterModule.\n\n    classId : Unique identifier of last child.\n    return : DVCSXiConverterModule.", py::arg("classId"))
        .def("newDVCSXiConverterModule", py::overload_cast<const std::string&>(&PARTONS::ModuleObjectFactory::newDVCSXiConverterModule), py::return_value_policy::reference, "  Specialization of ModuleObjectFactory.newModuleObject into a DVCSXiConverterModule.\n\n    className : Name of last child class.\n    return : DVCSXiConverterModule.", py::arg("className"))
        .def("newDVCSObservable", py::overload_cast<unsigned int>(&PARTONS::ModuleObjectFactory::newDVCSObservable), py::return_value_policy::reference, "  Specialization of ModuleObjectFactory.newModuleObject into a DVCSObservable.\n\n    classId : Unique identifier of a last child class.\n    return : Observable.", py::arg("classId"))
        .def("newDVCSObservable", py::overload_cast<const std::string&>(&PARTONS::ModuleObjectFactory::newDVCSObservable), py::return_value_policy::reference, "  Specialization of ModuleObjectFactory.newModuleObject into a DVCSObservable.\n\n    className : Name of a last child class.\n    return : Observable.", py::arg("className"))
        .def("newDVCSProcessModule", py::overload_cast<unsigned int>(&PARTONS::ModuleObjectFactory::newDVCSProcessModule), py::return_value_policy::reference, "  Specialization of ModuleObjectFactory.newModuleObject into a DVCSProcessModule.\n\n    classId : Unique identifier of a last child class.\n    return : ProcessModule.", py::arg("classId"))
        .def("newDVCSProcessModule", py::overload_cast<const std::string&>(&PARTONS::ModuleObjectFactory::newDVCSProcessModule), py::return_value_policy::reference, "  Specialization of ModuleObjectFactory.newModuleObject into a DVCSProcessModule.\n\n    className : Name of a last child class.\n    return : ProcessModule.", py::arg("className"))
        .def("newGPDEvolutionModule", py::overload_cast<unsigned int>(&PARTONS::ModuleObjectFactory::newGPDEvolutionModule), py::return_value_policy::reference, "  Specialization of ModuleObjectFactory.newModuleObject into GPDEvolutionModule.\n\n    classId : Unique identifier of last child class.\n    return : GPDEvolutionModule.", py::arg("classId"))
        .def("newGPDEvolutionModule", py::overload_cast<const std::string&>(&PARTONS::ModuleObjectFactory::newGPDEvolutionModule), py::return_value_policy::reference, "  Specialization of ModuleObjectFactory.newModuleObject into GPDEvolutionModule.\n\n    className : Name of last child class.\n    return : GPDEvolutionModule.", py::arg("className"))
        .def("newRunningAlphaStrongModule", py::overload_cast<unsigned int>(&PARTONS::ModuleObjectFactory::newRunningAlphaStrongModule), py::return_value_policy::reference, "  Specialization of ModuleObjectFactory.newModuleObject into RunningAlphaStrongModule.\n\n    classId : Unique identifier of last child class.\n    return : RunningAlphaStrongModule.", py::arg("classId"))
        .def("newRunningAlphaStrongModule", py::overload_cast<const std::string&>(&PARTONS::ModuleObjectFactory::newRunningAlphaStrongModule), py::return_value_policy::reference, "  Specialization of ModuleObjectFactory.newModuleObject into RunningAlphaStrongModule.\n\n    className : Name of last child class.\n    return : RunningAlphaStrongModule.", py::arg("className"))
        .def("newActiveFlavorsThresholdsModule", py::overload_cast<unsigned int>(&PARTONS::ModuleObjectFactory::newActiveFlavorsThresholdsModule), py::return_value_policy::reference, "  Specialization of ModuleObjectFactory.newModuleObject into ActiveFlavorsThresholdsModule.\n\n    classId : Unique identifier of last child class.\n    return : ActiveFlavorsThresholdsModule.", py::arg("classId"))
        .def("newActiveFlavorsThresholdsModule", py::overload_cast<const std::string&>(&PARTONS::ModuleObjectFactory::newActiveFlavorsThresholdsModule), py::return_value_policy::reference, "  Specialization of ModuleObjectFactory.newModuleObject into ActiveFlavorsThresholdsModule.\n\n    className : Name of last child class.\n    return : ActiveFlavorsThresholdsModule.", py::arg("className"))
        .def("toString", py::overload_cast<>(&PARTONS::ModuleObjectFactory::toString, py::const_))
        .def("updateModulePointerReference", &PARTONS::ModuleObjectFactory::updateModulePointerReference,
            "Method to update a pointer. Used to keep track of the modules and remove them when they become orphans (i.e. no pointer points to them).", py::arg("ModuleObjectTarget"), py::arg("ModuleObjectSource").none(true) = nullptr);


      //Wrapping of partons/partons/include/partons/modules/gpd/GPDGK16.h
      //            partons/partons/include/partons/modules/gpd/GPDGK11.h

    declare_gpd_module_gk<PARTONS::GPDGK16>(mPARTONS, "GPDGK16");
    declare_gpd_module_gk<PARTONS::GPDGK11>(mPARTONS, "GPDGK11");
    declare_gpd_module_classique<PARTONS::GPDHM18>(mPARTONS, "GPDHM18");
    declare_gpd_module_classique<PARTONS::GPDMMS13>(mPARTONS, "GPDMSS13");
    declare_gpd_module_classique<PARTONS::GPDVGG99>(mPARTONS, "GPDVGG99");
    declare_gpd_module_classique<PARTONS::GPDVinnikov06>(mPARTONS, "GPDVinnikov06");
    declare_gpd_module_classique<PARTONS::GPDGK16Numerical>(mPARTONS, "GPDGK16Numerical");
    declare_gpd_module_MPSS<PARTONS::GPDMPSSW13>(mPARTONS, "GPDMPSSW13");
    declare_gpd_module_select<PARTONS::GPDSelectOnePartonType>(mPARTONS, "GPDSelectOnePartonType");


      //Wrapping of partons/partons/include/partons/beans/gpd/GPDResult.h

    py::class_<PARTONS::GPDResult> gpdResult(mPARTONS, "GPDResult");

    gpdResult.def(py::init<>())
        .def(py::init<const PARTONS::GPDKinematic>())
        .def(py::init<const PARTONS::GPDResult>())
        .def("addPartonDistribution", &PARTONS::GPDResult::addPartonDistribution)
        .def("getPartonDistribution", py::overload_cast<PARTONS::GPDType::Type>(&PARTONS::GPDResult::getPartonDistribution, py::const_))
        .def("isAvailable", &PARTONS::GPDResult::isAvailable)
        .def("getLastAvailable", py::overload_cast<>(&PARTONS::GPDResult::getLastAvailable, py::const_))
        .def("toString", py::overload_cast<>(&PARTONS::GPDResult::toString, py::const_))
        //.def("compare",)
        .def("listGPDTypeComputed", py::overload_cast<>(&PARTONS::GPDResult::listGPDTypeComputed, py::const_))
        .def("getPartonDistributions", py::overload_cast<>(&PARTONS::GPDResult::getPartonDistributions, py::const_))
        .def("setPartonDistributions", &PARTONS::GPDResult::setPartonDistributions);

      //Wrapping of partons/partons/include/partons/beans/gpd/GPDType.h

    py::class_<PARTONS::GPDType> gpdType(mPARTONS, "GPDType");

    py::enum_<PARTONS::GPDType::Type>(gpdType, "Type")
        .value("UNDEFINED", PARTONS::GPDType::UNDEFINED)
        .value("ALL", PARTONS::GPDType::ALL)
        .value("H", PARTONS::GPDType::H)
        .value("E", PARTONS::GPDType::E)
        .value("Ht", PARTONS::GPDType::Ht)
        .value("Et", PARTONS::GPDType::Et)
        .value("HTrans", PARTONS::GPDType::HTrans)
        .value("ETrans", PARTONS::GPDType::ETrans)
        .value("HtTrans", PARTONS::GPDType::HtTrans)
        .value("EtTrans", PARTONS::GPDType::EtTrans)
        .value("H3p", PARTONS::GPDType::H3p)
        .value("E3p", PARTONS::GPDType::E3p)
        .value("Ht3p", PARTONS::GPDType::Ht3p)
        .value("Et3p", PARTONS::GPDType::Et3p)
        .value("H3m", PARTONS::GPDType::H3m)
        .value("E3m", PARTONS::GPDType::E3m)
        .value("Ht3m", PARTONS::GPDType::Ht3m)
        .value("Et3m", PARTONS::GPDType::Et3m)
        .value("END", PARTONS::GPDType::END)
        .export_values();

    //gpdType.attr("GPD_TYPE_DB_COLUMN_NAME") = PARTONS::GPDType::GPD_TYPE_DB_COLUMN_NAME;
    gpdType.def(py::init<>())
        //.def(py::init<PARTONS::Type>())
        .def(py::init<const PARTONS::GPDType::Type&>())
        .def("toString", &PARTONS::GPDType::toString)
        .def("fromString", &PARTONS::GPDType::fromString)
        .def("getListOfGPDTypeFromString", &PARTONS::GPDType::getListOfGPDTypeFromString)
        .def("getType", py::overload_cast<>(&PARTONS::GPDType::getType, py::const_))
        .def("setType", &PARTONS::GPDType::setType);

      //Wrapping of partons/partons/include/partons/beans/gpd/GPDKinematic.h

    py::class_<PARTONS::GPDKinematic> gpdKinematic(mPARTONS, "GPDKinematic");

    gpdKinematic.attr("GPD_KINEMATIC_CLASS_NAME") = PARTONS::GPDKinematic::GPD_KINEMATIC_CLASS_NAME;
    gpdKinematic.attr("KINEMATIC_PARAMETER_NAME_X") = PARTONS::GPDKinematic::KINEMATIC_PARAMETER_NAME_X;
    gpdKinematic.attr("KINEMATIC_PARAMETER_NAME_XI") = PARTONS::GPDKinematic::KINEMATIC_PARAMETER_NAME_XI;
    gpdKinematic.attr("KINEMATIC_PARAMETER_NAME_T") = PARTONS::GPDKinematic::KINEMATIC_PARAMETER_NAME_T;
    gpdKinematic.attr("KINEMATIC_PARAMETER_NAME_MUF2") = PARTONS::GPDKinematic::KINEMATIC_PARAMETER_NAME_MUF2;
    gpdKinematic.attr("KINEMATIC_PARAMETER_NAME_MUR2") = PARTONS::GPDKinematic::KINEMATIC_PARAMETER_NAME_MUR2;
    gpdKinematic.attr("KINEMATIC_PARAMETER_NAME_X_UNIT") = PARTONS::GPDKinematic::KINEMATIC_PARAMETER_NAME_X_UNIT;
    gpdKinematic.attr("KINEMATIC_PARAMETER_NAME_XI_UNIT") = PARTONS::GPDKinematic::KINEMATIC_PARAMETER_NAME_XI_UNIT;
    gpdKinematic.attr("KINEMATIC_PARAMETER_NAME_T_UNIT") = PARTONS::GPDKinematic::KINEMATIC_PARAMETER_NAME_T_UNIT;
    gpdKinematic.attr("KINEMATIC_PARAMETER_NAME_MUF2_UNIT") = PARTONS::GPDKinematic::KINEMATIC_PARAMETER_NAME_MUF2_UNIT;
    gpdKinematic.attr("KINEMATIC_PARAMETER_NAME_MUR2_UNIT") = PARTONS::GPDKinematic::KINEMATIC_PARAMETER_NAME_MUR2_UNIT;
    gpdKinematic.def(py::init<>())
        .def(py::init<double, double, double, double, double>())
        .def(py::init<PARTONS::PhysicalType<double>, PARTONS::PhysicalType<double>, PARTONS::PhysicalType<double>, PARTONS::PhysicalType<double>, PARTONS::PhysicalType<double>>())
        .def(py::init<PARTONS::GPDKinematic&>());


      //Wrapping of partons/partons/include/partons/services/DVCSConvolCoeffFunctionService.h

    py::class_<PARTONS::DVCSConvolCoeffFunctionService> dvcsConvolCoeffFunctionService(mPARTONS, "DVCSConvolCoeffFunctionService");

    dvcsConvolCoeffFunctionService.attr("classId") = PARTONS::DVCSConvolCoeffFunctionService::classId;
    dvcsConvolCoeffFunctionService.def("newDVCSConvolCoeffFunctionModuleFromTask", py::overload_cast<const PARTONS::Task&>(&PARTONS::DVCSConvolCoeffFunctionService::newDVCSConvolCoeffFunctionModuleFromTask, py::const_), "  Uses an automation (XML file) to configure a DVCSConvolCoeffFunctionModule.\n\n    task : Automation task.\n    return : Pre-configured DVCSConvolCoeffFunctionModule.", py::arg("task"))
        .def("computeTask", &PARTONS::DVCSConvolCoeffFunctionService::computeTask, py::arg("task"))
        .def("computeSingleKinematic", py::overload_cast<const PARTONS::DVCSConvolCoeffFunctionKinematic&, PARTONS::ConvolCoeffFunctionModule<PARTONS::DVCSConvolCoeffFunctionKinematic, PARTONS::DVCSConvolCoeffFunctionResult>* , const PARTONS::List<PARTONS::GPDType>&>(&PARTONS::DVCSConvolCoeffFunctionService::computeSingleKinematic, py::const_), py::return_value_policy::reference, "  Computes a dvcsConvolCoeffFunctionModule at specific kinematics.\n\n    kinematic : CFF Kinematics.\n    ConvolCoeffFunctionModule : CFF model to use for the computation.\n    gpdTypeList : List of GPDType to compute. Default : all the GPDTypes availables with (both) the ConvolCoeffFunctionModule (AND the underlying GPDModule, if any).\n    return : Result.", py::arg("kinematic"), py::arg("convolCoeffFunctionModule"), py::arg_v("gpdTypeList", PARTONS::List<PARTONS::GPDType>(), "ListGPDType()"))
        //.def("computeManyKinematic", py::overload_cast<const PARTONS::List<PARTONS::DVCSConvolCoeffFunctionKinematic>&, PARTONS::ConvolCoeffFunctionModule<PARTONS::DVCSConvolCoeffFunctionKinematic, PARTONS::DVCSConvolCoeffFunctionResult>* , const PARTONS::List<PARTONS::GPDType>&>(&PARTONS::DVCSConvolCoeffFunctionService::computeManyKinematic), "  Computes a CFF Model for a list of kinematics.\n\n    kinematics : List of kinematics.\n    convolCoeffFunctionModule = CFF model to use for computation.\n    gpdTypeList : List of GPD to compute. Default : all the GPDTypes available with (both) the ConvolCoeffFunctionModule (AND the underlying GPDModule, if any).\n    storeInDN : Boolean to store the results and kinematics on the database. Default : false.\n    return : List of result.", py::arg("kinematics"), py::arg("convolCoeffFunctionModule"), py::arg_v("gpdTypeList", PARTONS::List<PARTONS::GPDType>(), "ListGPDType()"));
        .def("computeManyKinematic", &PARTONS::DVCSConvolCoeffFunctionService::computeManyKinematic, "  Computes a CFF Model for a list of kinematics.\n\n    kinematics : List of kinematics.\n    convolCoeffFunctionModule = CFF model to use for computation.\n    gpdTypeList : List of GPD to compute. Default : all the GPDTypes available with (both) the ConvolCoeffFunctionModule (AND the underlying GPDModule, if any).\n    storeInDN : Boolean to store the results and kinematics on the database. Default : false.\n    return : List of result.", py::arg("kinematics"), py::arg("convolCoeffFunctionModule"), py::arg_v("gpdTypeList", PARTONS::List<PARTONS::GPDType>(), "ListGPDType()"));

      py::class_<PARTONS::ConvolCoeffFunctionModule<PARTONS::DVCSConvolCoeffFunctionKinematic, PARTONS::DVCSConvolCoeffFunctionResult>, std::unique_ptr<PARTONS::ConvolCoeffFunctionModule<PARTONS::DVCSConvolCoeffFunctionKinematic, PARTONS::DVCSConvolCoeffFunctionResult>, py::nodelete>>(mPARTONS, "ConvolCoeffFunctionModule");

      //Wrapping of partons/partons/include/partons/modules/convol_coeff_function/DVCSConvolCoeffFunctionModule.h

    py::class_<PARTONS::DVCSConvolCoeffFunctionModule, PARTONS::ConvolCoeffFunctionModule<PARTONS::DVCSConvolCoeffFunctionKinematic, PARTONS::DVCSConvolCoeffFunctionResult>> dvcsConvolCoeffFunctionModule(mPARTONS, "DVCSConvolCoeffFunctionModule");

    dvcsConvolCoeffFunctionModule.attr("DVCS_CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME") = PARTONS::DVCSConvolCoeffFunctionModule::DVCS_CONVOL_COEFF_FUNCTION_MODULE_CLASS_NAME;
    dvcsConvolCoeffFunctionModule.def("resolveObjectDependencies", &PARTONS::DVCSConvolCoeffFunctionModule::resolveObjectDependencies)
        .def("run", &PARTONS::DVCSConvolCoeffFunctionModule::run)
        .def("configure", py::overload_cast<const ElemUtils::Parameters&>(&PARTONS::DVCSConvolCoeffFunctionModule::configure))
        .def("toString", py::overload_cast<>(&PARTONS::DVCSConvolCoeffFunctionModule::toString, py::const_))
        .def("computeUnpolarized", &PARTONS::DVCSConvolCoeffFunctionModule::computeUnpolarized, "  Method to compute some CFFs.\n  Can be implemented and used in child class if needed\n\n    return : Complex result.")
        .def("computePolarized", &PARTONS::DVCSConvolCoeffFunctionModule::computePolarized, "  Method to compute some CFFs.\n  Can be implemented ans used in child class, if needed.\n\n    return : Complex result.")
        .def("computeCFF", &PARTONS::DVCSConvolCoeffFunctionModule::computeCFF, "  Method to compute some CFFs.\n  Can be implemented and used in child class, if needed.\n\n    return : Complex result.")
        .def("setGPDModule", &PARTONS::DVCSConvolCoeffFunctionModule::setGPDModule, "  Set pointer to the underlying GPD module.", py::arg("gpdModule"));

      //Wrapping of /partons/partons/include/partons/beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h

    py::class_<PARTONS::DVCSConvolCoeffFunctionKinematic> dvcsConvolCoeffFunctionKinematic(mPARTONS, "DVCSConvolCoeffFunctionKinematic");

    dvcsConvolCoeffFunctionKinematic.attr("DVCS_CONVOL_COEFF_FUNCTION_KINEMATIC_CLASS_NAME") = PARTONS::DVCSConvolCoeffFunctionKinematic::DVCS_CONVOL_COEFF_FUNCTION_KINEMATIC_CLASS_NAME;
    dvcsConvolCoeffFunctionKinematic.def(py::init<>(), "  Default constructor.")
        .def(py::init<double, double, double, double, double>(), "  Assignement constructor.\n\n    xi : Skewness variable.\n    t : Four-momentum transfer squared of hadron target (in GeV^2).\n    Q2 : Virtual-photon virtuality (in GeV^2)\n    MuF2 : Factorization scale squared (in GeV^2).\n    MuR2 : Renormalization scale squared (in GeV^2).", py::arg("xi"), py::arg("t"), py::arg("Q2"), py::arg("MuF2"), py::arg("MuR2"))
        .def(py::init<const ElemUtils::GenericType& , const ElemUtils::GenericType&, const ElemUtils::GenericType&, const ElemUtils::GenericType&, const ElemUtils::GenericType&>(), "  Assignement constructor.\n\n    xi : Skewness variable.\n    t : Four-momentum transfer squared of hadron target (in GeV^2).\n    Q2 : Virtual-photon virtuality (in GeV^2)\n    MuF2 : Factorization scale squared (in GeV^2).\n    MuR2 : Renormalization scale squared (in GeV^2).", py::arg("xi"), py::arg("t"), py::arg("Q2"), py::arg("MuF2"), py::arg("MuR2"))
        .def(py::init<PARTONS::DVCSConvolCoeffFunctionKinematic&>(), "  Copy constructor.\n\n    other : Object to be copied.", py::arg("other"))
        .def("configure", &PARTONS::DVCSConvolCoeffFunctionKinematic::configure)
        .def("toString", &PARTONS::DVCSConvolCoeffFunctionKinematic::toString)
        .def("serialize", py::overload_cast<ElemUtils::Packet&>(&PARTONS::DVCSConvolCoeffFunctionKinematic::serialize, py::const_), "  Serialize into given Packet.\n\n    packet : Target Packet.", py::arg("packet"))
        .def("unserialize", &PARTONS::DVCSConvolCoeffFunctionKinematic::unserialize, "  Retrive data from given Packet.\n\n    packet : Input Packet", py::arg("packet"))
        .def("getQ2", py::overload_cast<>(&PARTONS::DVCSConvolCoeffFunctionKinematic::getQ2, py::const_), "  Get virtual-photon virtuality.");


      //Wrapping of partons/elementary-utils/include/ElementaryUtils/parameters/GenericType.h

    py::class_<ElemUtils::GenericType> genericType(mElemUtils, "GenericType");

    genericType.def(py::init<const int>(), "  Construct a GenericTypeObject from any simple type.\n\n    value.", py::arg("value"));

      //Wrapping of partons/elementary-utils/include/ElementaryUtils/parameters/Parameters.h

    py::class_<ElemUtils::Parameters> parameters(mElemUtils, "Parameters", "    Obejct defining a list of pieces of data (of common Parameter type).\n\n    Each member of the list is uniquely identifiedby its name.");

    parameters.def(py::init<>())
        .def(py::init<const std::string&, const ElemUtils::GenericType>(), py::arg("key"), py::arg("value"))
        .def(py::init<const std::string&, const PARTONS::PerturbativeQCDOrderType::Type&>(), py::arg("key"), py::arg("value"))
        .def(py::init<const ElemUtils::Parameter&>(), py::arg("parameter"))
        .def(py::init<const std::string&, const int>(), py::arg("key"), py::arg("value"))
        .def(py::init<const std::string&, const double>(), py::arg("key"), py::arg("value"))
        .def(py::init<const std::string&, const std::string&>(), py::arg("key"), py::arg("value"))
        .def("add", py::overload_cast<const std::string&, const ElemUtils::GenericType&>(&ElemUtils::Parameters::add), py::arg("key"), py::arg("value"))
        .def("add", py::overload_cast<const ElemUtils::Parameter&>(&ElemUtils::Parameters::add), py::arg("parameter"))
        .def("add", py::overload_cast<const ElemUtils::Parameters&>(&ElemUtils::Parameters::add), py::arg("parameters"))
        .def("add", ([](ElemUtils::Parameters* p, const std::string& s, int i){p->add(s, ElemUtils::GenericType(i));}), py::arg("key"), py::arg("value"))
        .def("add", ([](ElemUtils::Parameters* p, const std::string& s, double d){p->add(s, ElemUtils::GenericType(d));}), py::arg("key"), py::arg("value"))
        .def("add", ([](ElemUtils::Parameters* p, const std::string& s, std::string& s2){p->add(s, ElemUtils::GenericType(s2));}), py::arg("key"), py::arg("value"))
        .def("isAvailable", py::overload_cast<const std::string&>(&ElemUtils::Parameters::isAvailable, py::const_), py::arg("parameterName"))
        .def("size", py::overload_cast<>(&ElemUtils::Parameters::size, py::const_))
        .def("clear", &ElemUtils::Parameters::clear)
        .def("toString", py::overload_cast<>(&ElemUtils::Parameters::toString, py::const_))
        .def("update", py::overload_cast<const std::string&, const ElemUtils::GenericType&>(&ElemUtils::Parameters::update), py::arg("key"), py::arg("value"))
        //.def("update", py::overload_cast<const std::string&, const ElemUtils::Parameters&>(&ElemUtils::Parameters::update), py::arg("key"), py::arg("parameter"))
        .def("remove", &ElemUtils::Parameters::remove)
        .def("get", py::overload_cast<const std::string&>(&ElemUtils::Parameters::get, py::const_), py::arg("key"))
        .def("getLastAvailable", py::overload_cast<>(&ElemUtils::Parameters::getLastAvailable, py::const_), "  Care ! Use it after isAvailable() to ensure that the iterator is in the map range.")
        .def("key", py::overload_cast<size_t>(&ElemUtils::Parameters::key, py::const_), py::arg("index"))
        .def("stringValue", py::overload_cast<size_t>(&ElemUtils::Parameters::stringValue, py::const_), py::arg("index"));


      //Wrapping of partons/elementary-utils/include/ElementaryUtils/parameters/Parameter.h

    py::class_<ElemUtils::Parameter, ElemUtils::GenericType> parameter(mElemUtils, "Parameter", "    A piece of data of generic type whitch is given a simply readable name (a string variable).\n\n  As it is the case for GenericType, a parameter can be defined from any class.");

    parameter.def(py::init<const std::string&, const int>())
        .def(py::init<const std::string&, const double>())
        .def(py::init<const std::string, const std::string&>())
        .def("toString", py::overload_cast<>(&ElemUtils::Parameter::toString, py::const_))
        .def("getName", py::overload_cast<>(&ElemUtils::Parameter::getName, py::const_));

      //Wrapping of ElementaryUtils/logger/LoggerManager.h

    py::class_<ElemUtils::LoggerManager> loggerManager(mElemUtils, "LoggerManager");

    loggerManager.def("getInstance", py::overload_cast<>(&ElemUtils::LoggerManager::getInstance), py::return_value_policy::reference, "  Share a unique pointer of this class.")
        .def("init", &ElemUtils::LoggerManager::init)
        .def("close", &ElemUtils::LoggerManager::close)
        .def("run", &ElemUtils::LoggerManager::run)
        .def("debug", &ElemUtils::LoggerManager::debug, "  Use flushBuffer() after this function to print the text.", py::arg("className"), py::arg("functionName")="debug", py::arg("message"))
        .def("info", &ElemUtils::LoggerManager::info, "  Use flushBuffer() after this function to print the text.", py::arg("className"), py::arg("functionName")="info", py::arg("message"))
        .def("warn", &ElemUtils::LoggerManager::warn, "  Use flushBuffer() after this function to print the text.", py::arg("className"), py::arg("functionName")="warn", py::arg("message"))
        .def("error", py::overload_cast<const std::string&, const std::string&, const std::string&>(&ElemUtils::LoggerManager::error), "  Use flushBuffer() after this function to print the text.", py::arg("className"), py::arg("functionName")="error", py::arg("message"))
        .def("toString", &ElemUtils::LoggerManager::toString)
        .def("flushBuffer", &ElemUtils::LoggerManager::flushBuffer);

      //Wrapping of partons/partons/include/partons/modules/convol_coeff_function/DVCS/DVCSCFFStandard.h
      //Wrapping of partons/partons/include/partons/modules/convol_coeff_function/DVCS/DVCSCFFDispersionRelation.h
      //Wrapping of partons/partons/include/partons/modules/convol_coeff_function/DVCS/DVCSCFFHeavyQuark.h
      //Wrapping of partons/partons/include/partons/modules/convol_coeff_function/DVCS/DVCSCFFNN.h

    declare_dvcs_module<PARTONS::DVCSCFFStandard>(mPARTONS, "DVCSCFFStandard");
    declare_dvcs_module<PARTONS::DVCSCFFDispersionRelation>(mPARTONS, "DVCSCFFDispersionRelation");
    declare_dvcs_module<PARTONS::DVCSCFFHeavyQuark>(mPARTONS, "DVCSCFFHeavyQuark");
    declare_dvcs_module<PARTONS::DVCSCFFNN>(mPARTONS, "DVCSCFFNN");

        //Wrapping of partons/partons/include/partons/modules/convol_coeff_function/DVCS/DVCSCFFStandard.h

      py::class_<PARTONS::DVCSCFFConstant, PARTONS::DVCSConvolCoeffFunctionModule> dvcsCFFConstant(mPARTONS, "DVCSCFFConstant");

      dvcsCFFConstant.def(py::init<const std::string&>(), "  Constructor\n  See BaseObject.BaseObject and ModuleObject.ModuleObject for more details.\n\n    className : Name of the last child class.")
          .def("clone", py::overload_cast<>(&PARTONS::DVCSCFFConstant::clone, py::const_))
          .def("configure", &PARTONS::DVCSCFFConstant::configure, py::arg("parameters"))
          .def("computeCFF", &PARTONS::DVCSCFFConstant::computeCFF)
          .def("getCFFs", py::overload_cast<>(&PARTONS::DVCSCFFConstant::getCFFs, py::const_), "  Get map containing CFFs.")
          .def("setCFFs", &PARTONS::DVCSCFFConstant::setCFFs, "  Set map containing CFFs.\n\n    cffs : Map to be set.", py::arg("cffs"))
          .def("getCFF", py::overload_cast<PARTONS::GPDType::Type>(&PARTONS::DVCSCFFConstant::getCFF, py::const_), "  Get specific CFF.\n\n    gppdType : Type of CFF to be get.", py::arg("gpdType"))
          .def("setCFF", &PARTONS::DVCSCFFConstant::setCFF, "  Set specific CFF.\n\n    gpdType : Type of CFF to be set.\n    cff : CFF to be set.", py::arg("gpdType"), py::arg("cff"))
          .attr("classId") = PARTONS::DVCSCFFConstant::classId;
      dvcsCFFConstant.attr("PARAMETER_NAME_CFF_H_Re") = PARTONS::DVCSCFFConstant::PARAMETER_NAME_CFF_H_Re;
      dvcsCFFConstant.attr("PARAMETER_NAME_CFF_H_Im") = PARTONS::DVCSCFFConstant::PARAMETER_NAME_CFF_H_Im;
      dvcsCFFConstant.attr("PARAMETER_NAME_CFF_E_Re") = PARTONS::DVCSCFFConstant::PARAMETER_NAME_CFF_E_Re;
      dvcsCFFConstant.attr("PARAMETER_NAME_CFF_E_Im") = PARTONS::DVCSCFFConstant::PARAMETER_NAME_CFF_E_Im;
      dvcsCFFConstant.attr("PARAMETER_NAME_CFF_Ht_Re") = PARTONS::DVCSCFFConstant::PARAMETER_NAME_CFF_Ht_Re;
      dvcsCFFConstant.attr("PARAMETER_NAME_CFF_Ht_Im") = PARTONS::DVCSCFFConstant::PARAMETER_NAME_CFF_Ht_Im;
      dvcsCFFConstant.attr("PARAMETER_NAME_CFF_Et_Re") = PARTONS::DVCSCFFConstant::PARAMETER_NAME_CFF_Et_Re;
      dvcsCFFConstant.attr("PARAMETER_NAME_CFF_Et_Im") = PARTONS::DVCSCFFConstant::PARAMETER_NAME_CFF_Et_Im;

        //Wrapping of partons/partons/include/partons/beans/PerturbativeQCDOrderType.h

      py::class_<PARTONS::PerturbativeQCDOrderType, PARTONS::BaseObject> perturbativeQCDOrderType(mPARTONS, "PerturbativeQCDOrderType", "    Definition of enumeration values for QCD orders of calculation.\n\n  This class defines a set  of enumeration values that are used to distinguish between QCD orders of calculation. In addition, a declared object of this class is always associated to one QCD order (see PerturbativeQCDOrderType.m_Type), so member functions can act on it. E.g.");

      py::enum_<PARTONS::PerturbativeQCDOrderType::Type>(perturbativeQCDOrderType, "Type")
          .value("UNDEFINED", PARTONS::PerturbativeQCDOrderType::UNDEFINED)
          .value("LO", PARTONS::PerturbativeQCDOrderType::LO)
          .value("NLO", PARTONS::PerturbativeQCDOrderType::NLO)
          .value("NNLO", PARTONS::PerturbativeQCDOrderType::NNLO)
          .value("LL", PARTONS::PerturbativeQCDOrderType::LL)
          .value("NLL", PARTONS::PerturbativeQCDOrderType::NLL)
          .export_values();

      perturbativeQCDOrderType.def(py::init<>(), "  Default constructor.")
          .def(py::init<PARTONS::PerturbativeQCDOrderType::Type>(), "  Assignement constructor.\n\n    type : Type to be assigned.", py::arg("type"))
          .def(py::init<std::string&>(), "  Assignement contructor trying to match QCD order type from given string. If unable to match set PerturbativeQCDOrderType.UNDEFINED.\n\n    perturbativeQCDOrderTypeString : String to be matched.", py::arg("perturbativeQCDOrderTypeString"))
          .def("toString", py::overload_cast<>(&PARTONS::PerturbativeQCDOrderType::toString, py::const_), "  Get string representation of type being assigned to a declared object of this class.\n\n    return : String representation of assigned type, like \"LO\" for PerturbativeQCDOrderType.LO")
          .def("getType", py::overload_cast<>(&PARTONS::PerturbativeQCDOrderType::getType, py::const_), "  Get type being assigned to a declared object of this class.")
          .def("setType", &PARTONS::PerturbativeQCDOrderType::setType, "  Assign type to a declared object of this class.", py::arg("type"))
          .attr("PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE") = PARTONS::PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE;

      //Wrapping of partons/partons/include/partons/beasn/convol_coeff_function/DVCS/dvcsConvolCoeffFunctionResult

    py::class_<PARTONS::DVCSConvolCoeffFunctionResult> dvcsConvolCoeffFunctionResult(mPARTONS, "DVCSConvolCoeffFunctionResult");

    dvcsConvolCoeffFunctionResult.def(py::init<>(), "  Default Constructor")
        .def(py::init<const PARTONS::DVCSConvolCoeffFunctionResult&>(), "  Assignement constructor.\n\n    kinematic : DVCS CFF kinematics to be assigned.", py::arg("kinematic"))
        .def(py::init<const PARTONS::DVCSConvolCoeffFunctionResult&>(), "  Copy constructor\n\n    other : Object to be copied.", py::arg("other"))
        .def("toString", &PARTONS::DVCSConvolCoeffFunctionResult::toString);

      //Wrapping of partons/partons/include/partons/services/DVCSObservableService.h

    py::class_<PARTONS::DVCSObservableService> dvcsObservableService(mPARTONS, "DVCSObservableService", "    Singleton to handle and compute some pre-configured observable modules.");

    dvcsObservableService.attr("classId") = PARTONS::DVCSObservableService::classId;
    dvcsObservableService.def("storeResultListInDatabase", &PARTONS::DVCSObservableService::storeResultListInDatabase, py::arg("retults"))
    .def("computeManyKinematic", py::overload_cast<const PARTONS::List<PARTONS::DVCSObservableKinematic> &, PARTONS::Observable<PARTONS::DVCSObservableKinematic, PARTONS::DVCSObservableResult>*, const PARTONS::List<PARTONS::GPDType>&>(&PARTONS::ObservableService<PARTONS::DVCSObservableKinematic, PARTONS::DVCSObservableResult>::computeManyKinematic), "  Computes an Observable for a list of kinematics.\n\n    listOfKinematic : List of kinematics.\n    observable : Observable to compute.\n    listOfGPDType, List of GPDType to compute. Default : all the GPDTypes available with (both) the underlying ConvolCoeffFunctionModule (AND the underlying GPDModule, if any).\n    return : List of ObservableResult.", py::arg("listOfKinematic"), py::arg("observable"), py::arg_v("gpdTypeList", PARTONS::List<PARTONS::GPDType>(), "ListGPDType()"))
    .def("computeSingleKinematic", py::overload_cast<const PARTONS::DVCSObservableKinematic&, PARTONS::Observable<PARTONS::DVCSObservableKinematic, PARTONS::DVCSObservableResult>*, const PARTONS::List<PARTONS::GPDType>&>(&PARTONS::ObservableService<PARTONS::DVCSObservableKinematic, PARTONS::DVCSObservableResult>::computeSingleKinematic, py::const_), "  Computes an Observable at specific kinematics.\n\n    observableKinematic : Kinematics.\n    observable : Observable to compute.\n    listOfGPDType, List of GPDType to compute. Default : all the GPDTypes available with (both) the underlying ConvolCoeffFunctionModule (AND the underlying GPDModule, if any).\n    return : ObservableResult.", py::arg("observableKinematic"), py::arg("observable"), py::arg_v("gpdTypeList", PARTONS::List<PARTONS::GPDType>(), "ListGPDType()"));


      //Wrapping of partons/partons/include/partons/modules/scales/DVCS/DVCSScalesModule.h

    py::class_<PARTONS::DVCSScalesModule, PARTONS::ModuleObject> dvcsScalesModule(mPARTONS, "ScalesModule", "    Abetract class for modules evaluating factorization and renormalization scales (DVCS case).");

    dvcsScalesModule.attr("DVCS_SCALES_MODULE_CLASS_NAME") = PARTONS::DVCSScalesModule::DVCS_SCALES_MODULE_CLASS_NAME;
    dvcsScalesModule.def("clone", py::overload_cast<>(&PARTONS::DVCSScalesModule::clone, py::const_));

      //Wrapping of partons/partons/include/partons/modules/scales/DVCS/DVCSScalesQ2Multiplier.h

    py::class_<PARTONS::DVCSScalesQ2Multiplier, PARTONS::DVCSScalesModule> dvcsScalesQ2Multiplier(mPARTONS, "DVCSScalesQ2Multiplier", "    Evaluation of factorization and renormalization scales as a linear of Q^2\n\n  This model evaluates factorization and renormalization scales as a linear function of Q^2, i.e. muF2 = muR2 = lambda Q^2.\n  The value of lambda cans be changed by DVCSScalesQ2Multiplier.configure() function using DVCSScalesQ2Multiplier.PARAM_NAME_LAMBDA parameter key.");

    dvcsScalesQ2Multiplier.attr("PARAMETER_NAME_LAMBDA") = PARTONS::DVCSScalesQ2Multiplier::PARAMETER_NAME_LAMBDA;
    dvcsScalesQ2Multiplier.attr("classId") = PARTONS::DVCSScalesQ2Multiplier::classId;
    dvcsScalesQ2Multiplier.def(py::init<const std::string&>(), "  Constructor.\n\n    className : Name of the class.", py::arg("className"))
    .def(py::init<const std::string&>(), "  Copy constructor.\n\n    other : Object to be copied", py::arg("other"))
    .def("clone", py::overload_cast<>(&PARTONS::DVCSScalesQ2Multiplier::clone, py::const_))
    .def("configure", &PARTONS::DVCSScalesQ2Multiplier::configure);



      //Wrapping of partons/include/partons/modules/observalbe/DVCS/DVCSObservable.h

    py::class_<PARTONS::Observable<PARTONS::DVCSObservableKinematic, PARTONS::DVCSObservableResult>, PARTONS::ModuleObject> observable(mPARTONS, "Observable");

    py::class_<PARTONS::DVCSObservable, PARTONS::Observable<PARTONS::DVCSObservableKinematic, PARTONS::DVCSObservableResult>> dvcsObservable(mPARTONS, "DVCSObservable");

    dvcsObservable.attr("DVCS_OBSERVABLE_MODULE_CLASS_NAME") = PARTONS::DVCSObservable::DVCS_OBSERVABLE_MODULE_CLASS_NAME;

    dvcsObservable.def("toString", py::overload_cast<>(&PARTONS::DVCSObservable::toString, py::const_))
        .def("resolveObjectDependencies", &PARTONS::DVCSObservable::resolveObjectDependencies)
        .def("run", &PARTONS::DVCSObservable::run)
        .def("configure", &PARTONS::DVCSObservable::configure)
        .def("getListOfAvailableGPDTypeForComputation", py::overload_cast<>(&PARTONS::DVCSObservable::getListOfAvailableGPDTypeForComputation, py::const_));

      //Wrapping of partons/partons/include/partons/modules/observable/DVCS/asymmetry

    declare_observable<PARTONS::DVCSAc, PARTONS::DVCSObservable>(mPARTONS, "DVCSAc", "    Beam charge asymmetry.");
    declare_observable<PARTONS::DVCSAcCos0Phi, PARTONS::DVCSAc>(mPARTONS, "DVCSAcCos0Phi", "    0th Fourier moment of beam charge asymmetry.");
    declare_observable<PARTONS::DVCSAcCos1Phi, PARTONS::DVCSAc>(mPARTONS, "DVCSAcCos1Phi", "    1th Fourier moment of beam charge asymmetry.");
    declare_observable<PARTONS::DVCSAcCos2Phi, PARTONS::DVCSAc>(mPARTONS, "DVCSAcCos2Phi", "    2th Fourier moment of beam charge asymmetry.");
    declare_observable<PARTONS::DVCSAcCos3Phi, PARTONS::DVCSAc>(mPARTONS, "DVCSAcCos3Phi", "    3th Fourier moment of beam charge asymmetry.");
    declare_observable<PARTONS::DVCSAllMinus, PARTONS::DVCSObservable>(mPARTONS, "DVCSAllMinus", "    Longitudinally polarized beam and target asymmetry for negative beam charge.");
    declare_observable<PARTONS::DVCSAllMinusCos0Phi, PARTONS::DVCSAllMinus>(mPARTONS, "DVCSAllMinusCos0Phi", "    0th Fourier moment of longitudinally polarized beam and target asumetry for negative beam charge.");
    declare_observable<PARTONS::DVCSAllMinusCos1Phi, PARTONS::DVCSAllMinus>(mPARTONS, "DVCSAllMinusCos1Phi", "    1th Fourier moment of longitudinally polarized beam and target asumetry for negative beam charge.");
    declare_observable<PARTONS::DVCSAllMinusCos2Phi, PARTONS::DVCSAllMinus>(mPARTONS, "DVCSAllMinusCos2Phi", "    2th Fourier moment of longitudinally polarized beam and target asumetry for negative beam charge.");
    declare_observable<PARTONS::DVCSAllPlus, PARTONS::DVCSObservable>(mPARTONS, "DVCSAllPlus", "    Longitudinally polarized beam and target asymmetry for positive beam charge.");
    declare_observable<PARTONS::DVCSAllPlusCos0Phi, PARTONS::DVCSAllPlus>(mPARTONS, "DVCSAllPlusCos0Phi", "    0th Fourier moment of longitudinally polarized beam and target asumetry for positive beam charge.");
    declare_observable<PARTONS::DVCSAllPlusCos1Phi, PARTONS::DVCSAllPlus>(mPARTONS, "DVCSAllPlusCos1Phi", "    1th Fourier moment of longitudinally polarized beam and target asumetry for positive beam charge.");
    declare_observable<PARTONS::DVCSAllPlusCos2Phi, PARTONS::DVCSAllPlus>(mPARTONS, "DVCSAllPlusCos2Phi", "    2th Fourier moment of longitudinally polarized beam and target asumetry for positive beam charge.");
    declare_observable<PARTONS::DVCSAltDVCSCosPhiMPhis, PARTONS::DVCSObservable>(mPARTONS, "DVCSAltDVCSCosPhiMPhis", "    Longitudinally beam and transverse target asymetry (DVCS part).");
    declare_observable<PARTONS::DVCSAltDVCSCosPhiMPhisCos0Phi, PARTONS::DVCSAltDVCSCosPhiMPhis>(mPARTONS, "DVCSAltDVCSCosPhiMPhisCos0Phi", "    0th Fourier moment of longitudinally beqm and transverse target asymetry (DVCS part).");
    declare_observable<PARTONS::DVCSAltDVCSCosPhiMPhisCos1Phi, PARTONS::DVCSAltDVCSCosPhiMPhis>(mPARTONS, "DVCSAltDVCSCosPhiMPhisCos1Phi", "    1th Fourier moment of longitudinally beqm and transverse target asymetry (DVCS part).");
    declare_observable<PARTONS::DVCSAltDVCSSinPhiMPhis, PARTONS::DVCSObservable>(mPARTONS, "DVCSAltDVCSSinPhiMPhis", "    Longitudinally beam and transverse target asymetry (DVCS part).");
    declare_observable<PARTONS::DVCSAltDVCSSinPhiMPhisSin1Phi, PARTONS::DVCSAltDVCSSinPhiMPhis>(mPARTONS, "DVCSAltDVCSSinPhiMPhisSin1Phi", "    1th Fourier moment of longitudinally beqm and transverse target asymetry (DVCS part).");
    declare_observable<PARTONS::DVCSAltIntCosPhiMPhis, PARTONS::DVCSObservable>(mPARTONS, "DVCSAltIntCosPhiMPhis", "    Longitudinally beam and transverse target asymetry (interference part).");
    declare_observable<PARTONS::DVCSAltIntCosPhiMPhisCos0Phi, PARTONS::DVCSAltIntCosPhiMPhis>(mPARTONS, "DVCSAltIntCosPhiMPhisCos0Phi", "    0th Fourier moment of longitudinally beqm and transverse target asymetry (interference part).");
    declare_observable<PARTONS::DVCSAltIntCosPhiMPhisCos1Phi, PARTONS::DVCSAltIntCosPhiMPhis>(mPARTONS, "DVCSAltIntCosPhiMPhisCos1Phi", "    1th Fourier moment of longitudinally beqm and transverse target asymetry (interference part).");
    declare_observable<PARTONS::DVCSAltIntCosPhiMPhisCos2Phi, PARTONS::DVCSAltIntCosPhiMPhis>(mPARTONS, "DVCSAltIntCosPhiMPhisCos2Phi", "    2th Fourier moment of longitudinally beqm and transverse target asymetry (interference part).");
    declare_observable<PARTONS::DVCSAltIntSinPhiMPhis, PARTONS::DVCSObservable>(mPARTONS, "DVCSAltIntSinPhiMPhis", "    Longitudinally beam and transverse target asymetry (interference part).");
    declare_observable<PARTONS::DVCSAltIntSinPhiMPhisSin1Phi, PARTONS::DVCSAltIntSinPhiMPhis>(mPARTONS, "DVCSAltIntSinPhiMPhisSin1Phi", "    1th Fourier moment of longitudinally beqm and transverse target asymetry (interference part).");
    declare_observable<PARTONS::DVCSAltIntSinPhiMPhisSin2Phi, PARTONS::DVCSAltIntSinPhiMPhis>(mPARTONS, "DVCSAltIntSinPhiMPhisSin2Phi", "    2th Fourier moment of longitudinally beqm and transverse target asymetry (interference part).");
    declare_observable<PARTONS::DVCSAluDVCS, PARTONS::DVCSObservable>(mPARTONS, "DVCSAluDVCS", "    Longitudinally polarized beam asymmetry for sum over beam charges (DVCS part).");
    declare_observable<PARTONS::DVCSAluDVCSSin1Phi, PARTONS::DVCSAluDVCS>(mPARTONS, "DVCSAluDVCSSin1Phi", "    1th Fourier moment of longitudinally polarized beam asymmetry for sum over beam charges (DVCS part).");
    declare_observable<PARTONS::DVCSAluInt, PARTONS::DVCSObservable>(mPARTONS, "DVCSAluInt", "    Longitudinally polarized beam asymmetry for difference over beam charges (DVCS part).");
    declare_observable<PARTONS::DVCSAluIntSin1Phi, PARTONS::DVCSAluInt>(mPARTONS, "DVCSAluIntSin1Phi", "    1th Fourier moment of longitudinally polarized beam asymmetry for difference over beam charges (DVCS part).");
    declare_observable<PARTONS::DVCSAluIntSin2Phi, PARTONS::DVCSAluInt>(mPARTONS, "DVCSAluIntSin2Phi", "    2th Fourier moment of longitudinally polarized beam asymmetry for difference over beam charges (DVCS part).");
    declare_observable<PARTONS::DVCSAluMinus, PARTONS::DVCSObservable>(mPARTONS, "DVCSAluMinus", "    Longitudinally polarized beam asymmetry for negative beam charges.");
    declare_observable<PARTONS::DVCSAluMinusSin1Phi, PARTONS::DVCSAluMinus>(mPARTONS, "DVCSAluMinusSin1Phi", "    Ath Fourier moment of longitudinally polarized beam asymmetry for negative beam charges.");
    declare_observable<PARTONS::DVCSAluPlus, PARTONS::DVCSObservable>(mPARTONS, "DVCSAluPlus", "    Longitudinally polarized beam asymmetry for positive beam charges.");
    declare_observable<PARTONS::DVCSAulMinus, PARTONS::DVCSObservable>(mPARTONS, "DVCSAulMinus", "    Longitudinally polarized target asymmetry for negative beam charges.");
    declare_observable<PARTONS::DVCSAulMinusSin1Phi, PARTONS::DVCSAulMinus>(mPARTONS, "DVCSAulMinusSin1Phi", "    1th Fourier moment of longitudinally polarized target asymmetry for negative beam charges.");
    declare_observable<PARTONS::DVCSAulMinusSin2Phi, PARTONS::DVCSAulMinus>(mPARTONS, "DVCSAulMinusSin2Phi", "    2th Fourier moment of longitudinally polarized target asymmetry for negative beam charges.");
    declare_observable<PARTONS::DVCSAulMinusSin3Phi, PARTONS::DVCSAulMinus>(mPARTONS, "DVCSAulMinusSin3Phi", "    3th Fourier moment of longitudinally polarized target asymmetry for negative beam charges.");
    declare_observable<PARTONS::DVCSAulPlus, PARTONS::DVCSObservable>(mPARTONS, "DVCSAulPlus", "    Longitudinally polarized target asymmetry for positive beam charges.");
    declare_observable<PARTONS::DVCSAulPlusSin1Phi, PARTONS::DVCSAulPlus>(mPARTONS, "DVCSAulPlusSin1Phi", "    1th Fourier moment of longitudinally polarized target asymmetry for positive beam charges.");
    declare_observable<PARTONS::DVCSAulPlusSin2Phi, PARTONS::DVCSAulPlus>(mPARTONS, "DVCSAulPlusSin2Phi", "    2th Fourier moment of longitudinally polarized target asymmetry for positive beam charges.");
    declare_observable<PARTONS::DVCSAulPlusSin3Phi, PARTONS::DVCSAulPlus>(mPARTONS, "DVCSAulPlusSin3Phi", "    3th Fourier moment of longitudinally polarized target asymmetry for positive beam charges.");
    declare_observable<PARTONS::DVCSAutDVCSSinPhiMPhis, PARTONS::DVCSObservable>(mPARTONS, "DVCSAutDVCSSinPhiMPhis", "    Transverse target asymmetry (DVCS part).");
    declare_observable<PARTONS::DVCSAutDVCSSinPhiMPhisCos0Phi, PARTONS::DVCSAutDVCSSinPhiMPhis>(mPARTONS, "DVCSAutDVCSSinPhiMPhisCos0Phi", "    0th Fourier moment of transverse target asymmetry (DVCS part).");
    declare_observable<PARTONS::DVCSAutIntSinPhiMPhis, PARTONS::DVCSObservable>(mPARTONS, "DVCSAutIntSinPhiMPhis", "    Transverse target asymmetry (interferenxe part).");
    declare_observable<PARTONS::DVCSAutIntSinPhiMPhisCos0Phi, PARTONS::DVCSAutIntSinPhiMPhis>(mPARTONS, "DVCSAutIntSinPhiMPhisCos0Phi", "    0th Fourier moment of transverse target asymmetry (interferenxe part).");
    declare_observable<PARTONS::DVCSAutIntSinPhiMPhisCos1Phi, PARTONS::DVCSAutIntSinPhiMPhis>(mPARTONS, "DVCSAutIntSinPhiMPhisCos1Phi", "    1th Fourier moment of transverse target asymmetry (interferenxe part).");
    declare_observable<PARTONS::DVCSAutIntSinPhiMPhisSin1Phi, PARTONS::DVCSAutIntSinPhiMPhis>(mPARTONS, "DVCSAutIntSinPhiMPhisSin1Phi", "    1th Fourier moment of transverse target asymmetry (interferenxe part).");
    declare_observable<PARTONS::DVCSAutMinusSinPhiMPhis, PARTONS::DVCSObservable>(mPARTONS, "DVCSAutMinusSinPhiMPhis", "    Transverse target asymmetry for negative beam charge.");
    declare_observable<PARTONS::DVCSAutMinusSinPhiMPhisCos0Phi, PARTONS::DVCSAutMinusSinPhiMPhis>(mPARTONS, "DVCSAutMinusSinPhiMPhisCos0Phi", "    0th Fourier moment of transverse target asymmetry for negative beam charge.");

      //Wrapping of partons/partons/include/partons/modules/observable/DVCS/cross_section

    declare_observable<PARTONS::DVCSCrossSectionDifferenceLUMinus, PARTONS::DVCSObservable>(mPARTONS, "DVCSCrossSectionDifferenceLUMinus", "    Unpolarized cross-section difference for longitudinally polarized beam and negative beam charge.");
    declare_observable<PARTONS::DVCSCrossSectionUUBHSubProc, PARTONS::DVCSObservable>(mPARTONS, "DVCSCrossSectionUUBHSubProc", "    Unpolarized cross-section for BH only");
    declare_observable<PARTONS::DVCSCrossSectionUUDVCSSubProc, PARTONS::DVCSObservable>(mPARTONS, "DVCSCrossSectionUUDVCSSubProc", "    Unpolarized cross-section for DVCS only");
    declare_observable<PARTONS::DVCSCrossSectionUUDVCSSubProcPhiIntegrated, PARTONS::DVCSCrossSectionUUDVCSSubProc>(mPARTONS, "DVCSCrossSectionUUDVCSSubProcPhiIntegrated", "    Unpolarized cross-section for electro-production integrated over phi.");
    declare_observable<PARTONS::DVCSCrossSectionUUMinus, PARTONS::DVCSObservable>(mPARTONS, "DVCSCrossSectionUUMinus", "    Unpolarized cross-section for negative beam charge.");
    declare_observable<PARTONS::DVCSCrossSectionUUMinusPhiIntegrated, PARTONS::DVCSCrossSectionUUMinus>(mPARTONS, "DVCSCrossSectionUUMinusPhiIntegrated", "    Unpolarized cross-section for electro-production integrated over phi.");
    declare_observable<PARTONS::DVCSCrossSectionUUVirtualPhotoProduction, PARTONS::DVCSObservable>(mPARTONS, "DVCSCrossSectionUUVirtualPhotoProduction", "    Unpolarized cross-section for virtual photo-production.");
    declare_observable<PARTONS::DVCSCrossSectionUUVirtualPhotoProductionPhiIntegrated, PARTONS::DVCSCrossSectionUUVirtualPhotoProduction>(mPARTONS, "DVCSCrossSectionUUVirtualPhotoProductionPhiIntegrated", "    Unpolarized cross-section for virtual photo-production integrated over phi.");

      //Wrapping of partons/partons/include/partons/modules/xi_converter/DVCSXiConverterModule.h

    py::class_<PARTONS::DVCSXiConverterModule, PARTONS::ModuleObject> dvcsXiConverterModule(mPARTONS, "DVCSXiConverterModule", "    Abstract class for modules evaluating GPD variales xi (DVCS case).");

    dvcsXiConverterModule.attr("DVCS_XI_CONVERTER_MODULE_CLASS_NAME") = PARTONS::DVCSXiConverterModule::DVCS_XI_CONVERTER_MODULE_CLASS_NAME;
    dvcsXiConverterModule.def("clone", py::overload_cast<>(&PARTONS::DVCSXiConverterModule::clone, py::const_));

      //Wrapping of partons/partons/include/partons/modules/xi_converter/DVCSXiConverterXBToXi.h

    py::class_<PARTONS::DVCSXiConverterXBToXi, PARTONS::DVCSXiConverterModule> dvcsXiConverterXBToXi(mPARTONS, "DVCSXiConverterXBToXi", "    Evaluation of GPD varaible xi from xB for DVCS.");

    dvcsXiConverterXBToXi.def(py::init<const std::string&>(), "  Constructor.\n\n    className : Name of the class.", py::arg("className"))
        .def(py::init<const PARTONS::DVCSXiConverterXBToXi&>(), "  Copy constructor.\n\n    other : Object to be copied.", py::arg("other"))
        .def("clone", py::overload_cast<>(&PARTONS::DVCSXiConverterXBToXi::clone, py::const_))
        .attr("classId") = PARTONS::DVCSXiConverterXBToXi::classId;


      //Wrapping of partons/partons/include/partons/modules/process

    py::class_<PARTONS::DVCSProcessModule, PARTONS::ModuleObject> dvcsProcessModule(mPARTONS, "DVCSProcessModule", "    Abstract class for computing the differentials cross section of photon electroproduction process (also called DVCS; Deeply Virtual Compton Scattering).");

    dvcsProcessModule.def("clone", py::overload_cast<>(&PARTONS::DVCSProcessModule::clone, py::const_))
        .def("toString", py::overload_cast<>(&PARTONS::DVCSProcessModule::toString, py::const_))
        .def("run", &PARTONS::DVCSProcessModule::run)
        .def("configure", &PARTONS::DVCSProcessModule::configure)
        .def("getListOfAvailableGPDTypeForComputation", py::overload_cast<>(&PARTONS::DVCSProcessModule::getListOfAvailableGPDTypeForComputation, py::const_))
        .def("resetPreviousKinematic", &PARTONS::DVCSProcessModule::resetPreviousKinematic, "  Reset previous kinematics.")
        .def("isPreviousCCFKinematicDifferent", py::overload_cast<const PARTONS::DVCSConvolCoeffFunctionKinematic&>(&PARTONS::DVCSProcessModule::isPreviousCCFKinematicDifferent, py::const_), "  Check if this kinmematics is different than the previous one.")
        .def("getScaleModule", py::overload_cast<>(&PARTONS::DVCSProcessModule::getScaleModule, py::const_), "  Get scale module.")
        .def("setScaleModule", &PARTONS::DVCSProcessModule::setScaleModule, "  Set scale module.", py::arg("scaleModule"))
        .def("getXiConverterModule", &PARTONS::DVCSProcessModule::getXiConverterModule, "  Get xi converter module.")
        .def("setXiConverterModule", &PARTONS::DVCSProcessModule::setXiConverterModule, "  Set xi converter module.", py::arg("xiConverterModule"))
        .def("getConvolCoeffFunctionModule", py::overload_cast<>(&PARTONS::DVCSProcessModule::getConvolCoeffFunctionModule, py::const_), "  Get CFF module.")
        .def("setConvolCoeffFunctionModule", &PARTONS::DVCSProcessModule::setConvolCoeffFunctionModule, "  Set CFF module.")
        .attr("DVCS_PROCESS_MODULE_CLASS_NAME") = PARTONS::DVCSProcessModule::DVCS_PROCESS_MODULE_CLASS_NAME;

    declare_process<PARTONS::DVCSProcessBMJ12, PARTONS::DVCSProcessModule>(mPARTONS, "DVCSProcessBMJ12", "    Module for DVCS process using the Belitsky-Müller set of formulas.\n\n  Code based on the published papers :\n    - arxiv:hep-ph/0112108 @cite Belitsky2001ns for the BH amplitude ;\n    - arxiv:1212.6674 @cite Belitsky2012ch for the DVCS amplitude and interference.");
    declare_process<PARTONS::DVCSProcessGV08, PARTONS::DVCSProcessModule>(mPARTONS, "DVCSProcessGV08", "    Module for the DVCS process using the unpublished Guichon-Vanderhaeghen set of formulas.");
    declare_process<PARTONS::DVCSProcessVGG99, PARTONS::DVCSProcessModule>(mPARTONS, "DVCSProcessVGG99", "    FGG process model for DVCS\n\n  For references see :\n    - Prog. Part. Nucl. Phys. 47, 401 (2001)\n    - Phys. Rev. Lett. 80 5064 (1998).\n    - Phys. Rev. D 60, 094017 (1999).\n    - Phys. Rev. D 72, 054013 (2005).");

      //Wrapping of partons/partons/include/partons/beans/observable/ObservableKinematic.h

    py::class_<PARTONS::ObservableKinematic, PARTONS::Kinematic> observableKinematic(mPARTONS, "ObservableKinematic", "  Abstract class representing single observable kinematics.\n  This class represents a single observable kinematics (none).");

    observableKinematic.def("configure", &PARTONS::ObservableKinematic::configure, py::arg("parameters"))
        .def("toString", py::overload_cast<>(&PARTONS::ObservableKinematic::toString, py::const_))
        .def("serialize", py::overload_cast<ElemUtils::Packet&>(&PARTONS::ObservableKinematic::serialize, py::const_), "  Serialize into given Packet.\n\n    packet : Target Packet.", py::arg("packet"))
        .def("unserialize", &PARTONS::ObservableKinematic::unserialize, "  Retrieve data from given Packet.\n\n    packet : Input Packet.", py::arg("packet"))
        .def("serializeIntoStdVector", py::overload_cast<std::vector<double>&>(&PARTONS::ObservableKinematic::serializeIntoStdVector, py::const_), "  Serialize to std::vector<double>.", py::arg("vec"));

      //Wrapping of partons/partons/include/partons/beans/observable/DVCS/DVCSObservableKinematic.h

    py::class_<PARTONS::DVCSObservableKinematic, PARTONS::ObservableKinematic> dvcsObservableKinematic(mPARTONS, "DVCSObservableKinematic", "    Class representing single observable kinematics for DVCS process.\n\n  This class represents a single observable kinematics for DVCS process (xB, t, Q2, Eb, phi).");

      dvcsObservableKinematic.attr("DVCS_OBSERVABLE_KINEMATIC_CLASS_NAME") = PARTONS::DVCSObservableKinematic::DVCS_OBSERVABLE_KINEMATIC_CLASS_NAME;
      dvcsObservableKinematic.attr("KINEMATIC_PARAMETER_NAME_XB") = PARTONS::DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_XB;
      dvcsObservableKinematic.attr("KINEMATIC_PARAMETER_NAME_Q2") = PARTONS::DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2;
      dvcsObservableKinematic.attr("KINEMATIC_PARAMETER_NAME_BEAM_ENERGY") = PARTONS::DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY;
      dvcsObservableKinematic.attr("KINEMATIC_PARAMETER_NAME_PHI") = PARTONS::DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI;
      dvcsObservableKinematic.attr("KINEMATIC_PARAMETER_NAME_XB_UNIT") = PARTONS::DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_XB_UNIT;
      dvcsObservableKinematic.attr("KINEMATIC_PARAMETER_NAME_Q2_UNIT") = PARTONS::DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_Q2_UNIT;
      dvcsObservableKinematic.attr("KINEMATIC_PARAMETER_NAME_BEAM_ENERGY_UNIT") = PARTONS::DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_BEAM_ENERGY_UNIT;
      dvcsObservableKinematic.attr("KINEMATIC_PARAMETER_NAME_PHI_UNIT") = PARTONS::DVCSObservableKinematic::KINEMATIC_PARAMETER_NAME_PHI_UNIT;
      dvcsObservableKinematic.def(py::init<>(), "  Default constructor.")
          .def(py::init<double, double, double, double, double>(), "  Assignement constructor.\n\n    xB : Bjorken variable.\n    t : Four-momentum transfer squared of hadron target (in GeV2).\n    Q2 : Virtual-photon virtuality (in GeV2).\n    E = Beam energy (in GeV).\n    phi : Angle between leptonic  and hadronic planes (in radians, Trento convention).", py::arg("xB"), py::arg("t"), py::arg("Q2"), py::arg("E"), py::arg("phi"))
          .def(py::init<const PARTONS::PhysicalType<double>, const PARTONS::PhysicalType<double>, const PARTONS::PhysicalType<double>, const PARTONS::PhysicalType<double>, const PARTONS::PhysicalType<double>>(), "  Assignement constructor.\n\n    xB : Bjorken variable.\n    t : Four-momentum transfer squared of hadron target (in GeV2).\n    Q2 : Virtual-photon virtuality (in GeV2).\n    E = Beam energy (in GeV).\n    phi : Angle between leptonic  and hadronic planes (in radians, Trento convention).", py::arg("xB"), py::arg("t"), py::arg("Q2"), py::arg("E"), py::arg("phi"))
          .def(py::init<const ElemUtils::GenericType&, const ElemUtils::GenericType&, const ElemUtils::GenericType&, const ElemUtils::GenericType&, const ElemUtils::GenericType&>(), "  Assignement constructor.\n\n    xB : Bjorken variable.\n    t : Four-momentum transfer squared of hadron target (in GeV2).\n    Q2 : Virtual-photon virtuality (in GeV2).\n    E = Beam energy (in GeV).\n    phi : Angle between leptonic  and hadronic planes (in radians, Trento convention).", py::arg("xB"), py::arg("t"), py::arg("Q2"), py::arg("E"), py::arg("phi"))
          .def(py::init<const PARTONS::DVCSObservableKinematic&>(), "  Copy constructor.\n\n    other : Object to be copied.", py::arg("other"))
          .def("configure", &PARTONS::DVCSObservableKinematic::configure, py::arg("parameters"))
          .def("toString", py::overload_cast<>(&PARTONS::DVCSObservableKinematic::toString, py::const_))
          .def("serialize", py::overload_cast<ElemUtils::Packet&>(&PARTONS::DVCSObservableKinematic::serialize, py::const_), "  Serialize into given Packet.nn    packet : Target Packet.", py::arg("packet"))
          .def("unserialize", &PARTONS::DVCSObservableKinematic::unserialize, "  Retrieve data from give, Packet.\n\n    packet : Input Packet.", py::arg("packet"))
          .def("getXB", py::overload_cast<>(&PARTONS::DVCSObservableKinematic::getXB, py::const_), "  Get Bjorken variable.")
          .def("setXB", py::overload_cast<const PARTONS::PhysicalType<double>&>(&PARTONS::DVCSObservableKinematic::setXB), "  Set Bjorken vairable.", py::arg("xB"))
          .def("setXB", py::overload_cast<double, const PARTONS::PhysicalUnit::Type>(&PARTONS::DVCSObservableKinematic::setXB), "  Set Bjorken vairable.", py::arg("xB"), py::arg_v("unit", PARTONS::PhysicalUnit::NONE, "PhysicalUnit.NONE"))
          .def("getT", py::overload_cast<>(&PARTONS::DVCSObservableKinematic::getT, py::const_), "  Get four-momentum transfer squared of hadron target.")
          .def("setT", py::overload_cast<const PARTONS::PhysicalType<double>&>(&PARTONS::DVCSObservableKinematic::setT), "  Set four-momentum transfer squared of hadron target.", py::arg("t"))
          .def("setT", py::overload_cast<double, const PARTONS::PhysicalUnit::Type>(&PARTONS::DVCSObservableKinematic::setT), "  Set four-momentum transfer squared of hadron target.", py::arg("t"), py::arg_v("unit", PARTONS::PhysicalUnit::GEV2, "PhysicalUnit.GEV2"))
          .def("getQ2", py::overload_cast<>(&PARTONS::DVCSObservableKinematic::getQ2, py::const_), "  Get virtual-photon virtuality.")
          .def("setQ2", py::overload_cast<const PARTONS::PhysicalType<double>&>(&PARTONS::DVCSObservableKinematic::setQ2), "  Set virtual-photon virtuality.", py::arg("Q2"))
          .def("setQ2", py::overload_cast<double, const PARTONS::PhysicalUnit::Type>(&PARTONS::DVCSObservableKinematic::setQ2), "  Set virtual-photon virtuality.", py::arg("Q2"), py::arg_v("unit", PARTONS::PhysicalUnit::GEV2, "PhysicalUnit.GEV2"))
          .def("getE", py::overload_cast<>(&PARTONS::DVCSObservableKinematic::getE, py::const_), "  Get beam energy.")
          .def("setE", py::overload_cast<const PARTONS::PhysicalType<double>&>(&PARTONS::DVCSObservableKinematic::setE), "  Set beam energy.", py::arg("E"))
          .def("setE", py::overload_cast<double, const PARTONS::PhysicalUnit::Type>(&PARTONS::DVCSObservableKinematic::setE), "  Set beam energy.", py::arg("E"), py::arg_v("unit", PARTONS::PhysicalUnit::GEV, "PhysicalUnit.GEV"))
          .def("getPhi", py::overload_cast<>(&PARTONS::DVCSObservableKinematic::getPhi, py::const_), "  Get angle between leptonic a,d hadronic planes (in radians).")
          .def("setPhi", py::overload_cast<const PARTONS::PhysicalType<double>&>(&PARTONS::DVCSObservableKinematic::setPhi), "  Set angle between leptonic a,d hadronic planes (in radians).", py::arg("phi"))
          .def("setPhi", py::overload_cast<double, const PARTONS::PhysicalUnit::Type>(&PARTONS::DVCSObservableKinematic::setPhi), "  Set angle between leptonic a,d hadronic planes (in radians).", py::arg("phi"), py::arg_v("unit", PARTONS::PhysicalUnit::RAD, "PhysicalUnit.RAD"));





        //Wrapping of partons/parons/include/partons/beans/observable/DVCS/DVCSObservableResult.h

      py::class_<PARTONS::DVCSObservableResult> dvcsObservableResult(mPARTONS, "DVCSObservableResult", "    Class representing single result of DVCS computaion.\n\n  This class is used to store a result of a single DVCS observable computation.");

      dvcsObservableResult.def(py::init<>(), "  Default constructor.")
          .def(py::init<const PARTONS::PhysicalType<double>&>(), "  Assignement constructor.\n\n    value : Value to be assigned.", py::arg("value"))
          .def(py::init<const PARTONS::DVCSObservableKinematic&>(), "  Assignement constructor.\n\n    kinematic : DVCS observable kinematic to be assigned.", py::arg("kinematic"))
          .def(py::init<const PARTONS::PhysicalType<double>&, const PARTONS::DVCSObservableKinematic&>(), "  Assignement constructor.\n\n    value : Value to be assigned.\n    kinematic : DVCS observable kinematic to be assigned.", py::arg("value"), py::arg("kinematic"))
          .def(py::init<const PARTONS::DVCSObservableResult&>(), "  Copy constructor.\n\n    other : Object to be copied.", py::arg("other"))
          .def("toString", py::overload_cast<>(&PARTONS::DVCSObservableResult::toString, py::const_));


        //Wrapping of partons/partons/include/partons/beans/KinematicUtils.h

      py::class_<PARTONS::KinematicUtils, PARTONS::BaseObject> kinematicUtils(mPARTONS, "KinematicUtils", "    Set of utilization tools to handle kinematic-like classes.\n\n  This chass is used as a container for utilization tools to handle kinematic-like classes.\n  It provides in particular tools to parse text files in order to create a set of kinematic-like objects.");

      kinematicUtils.def(py::init<>(), "  Default constructor.")
          .def("getGPDKinematicFromFile", &PARTONS::KinematicUtils::getGPDKinematicFromFile, "  Parse a text file in order to retrieve a list of GPDKinematic objects.\n  The parsed file should display seperate lines of the form \"x | xi | t | MuF2 | MuR2\".\n\n    filePath : Path to file to be parsed.\n    return : List of extracted GPDKinematic objects.", py::arg("filePath"))
          .def("getDVCSCCFKinematicFromFile", &PARTONS::KinematicUtils::getDVCSCCFKinematicFromFile, "  Parse a text file in order to retrieve a list of DVCSConvolCoeffFunctionKinematic objects.\n  The parsed file should display seperate lines of the form \"x | xi | t | MuF2 | MuR2\".\n\n    filePath : Path to file to be parsed.\n    return : List of extracted DVCSConvolCoeffFunctionKinematic objects.", py::arg("filePath"))
          //.def("getTCSCCFKinematicFromFile", &PARTONS::KinematicUtils::getTCSCCFKinematicFromFile, "  Parse a text file in order to retrieve a list of TVSConvolCoeffFunctionKinematic objects.\n  The parsed file should display seperate lines of the form \"x | xi | t | MuF2 | MuR2\".\n\n    filePath : Path to file to be parsed.\n    return : List of extracted DTCSConvolCoeffFunctionKinematic objects.", py::arg("filePath"))
          .def("getDVCSObservableKinematicFromFile", &PARTONS::KinematicUtils::getDVCSObservableKinematicFromFile, "  Parse a text file in order to retrieve a list of ObservableKinematic objects.\n  The parsed file should display seperate lines of the form \"x | xi | t | MuF2 | MuR2\".\n\n    filePath : Path to file to be parsed.\n    return : List of extracted ObservableKinematic objects.", py::arg("filePath"));
          //.def("getTCSObservableKinematicFromFile", &PARTONS::KinematicUtils::getTCSObservableKinematicFromFile, "  Parse a text file in order to retrieve a list of TCSObservableKinematic objects.\n  The parsed file should display seperate lines of the form \"x | xi | t | MuF2 | MuR2\".\n\n    filePath : Path to file to be parsed.\n    return : List of extracted TCSObservableKinematic objects.", py::arg("filePath"))

        //Wrapping of NumA/utils/Tolerances.h

      py::class_<NumA::Tolerances> tolerances(mNumA, "Tolerances", "    Define absolute and relative tolerances for comparision of real numbers (double) and check if they are positive.");

      tolerances.def(py::init<>(), "  Default constructot (tolerances equal to 0).")
          .def(py::init<double, double>(), "  Constructor.\n\n    absolute Tolerance : Absolute tolerance.\n    relativeTolerance : Relative tolerance.", py::arg("absoluteTolerance"), py::arg("relativeTolerance"))
          .def("toSring", py::overload_cast<>(&NumA::Tolerances::toString, py::const_), "    return : Pre-formatted string with the tolerances.")
          .def("getAbsoluteTolerance", py::overload_cast<>(&NumA::Tolerances::getAbsoluteTolerance, py::const_), "    return : Absolute tolerance.")
          .def("getRelativeTolerance", py::overload_cast<>(&NumA::Tolerances::getRelativeTolerance, py::const_), "    return : Relative tolerance.")
          .def("setAbsuluteTolerance", &NumA::Tolerances::setAbsoluteTolerance, "    absoluteTolerance : Absolute tolerance.", py::arg("absoluteTolerance"))
          .def("setRelativeTolerance", &NumA::Tolerances::setRelativeTolerance, "    relativeTolerance : Relative tolerance.", py::arg("relativeTolerance"));

        //Wrapping of NumA/utils/Errors.h

      py::class_<NumA::Errors> errors(mNumA, "Errors", "    Class for defining estimations of absolute and relative errors.");

      errors.def(py::init<>(), "  Default constructor (errors equal to 0).")
          .def(py::init<const double, const double>(), "  Constructor.\n\n    absolute : Absolute error.\n    relative : Relative error.", py::arg("absolute"), py::arg("relative"))
          .def("getAbsolute", py::overload_cast<>(&NumA::Errors::getAbsolute, py::const_), "    return : Absolute error.")
          .def("setAbsolute", &NumA::Errors::setAbsolute, "    absolute : Absolute error.", py::arg("absolute"))
          .def("getRelative", py::overload_cast<>(&NumA::Errors::getRelative, py::const_), "    return : Relative error.")
          .def("setRelative", &NumA::Errors::setRelative, "    relative : Relative error.", py::arg("relative"));

        //Wrapping of NumA/integratoi/one_dimension/IntegratorType1D.h

      py::class_<NumA::IntegratorType1D> integratorType1D(mNumA, "IntegratorType1D", "    Type of one-dimensional integrations (wrapper for enum).\n\n    Se Integrator1D documentation for an example.");

      py::enum_<NumA::IntegratorType1D::Type>(integratorType1D, "Type")
          .value("UNDEFINED", NumA::IntegratorType1D::UNDEFINED)
          .value("TRAPEZOIDAL", NumA::IntegratorType1D::TRAPEZOIDAL)
          .value("TRAPEZOIDALLOG", NumA::IntegratorType1D::TRAPEZOIDALLOG)
          .value("GL", NumA::IntegratorType1D::GL)
          .value("DEXP", NumA::IntegratorType1D::DEXP)
          .value("GK21_ADAPTIVE", NumA::IntegratorType1D::GK21_ADAPTIVE)
          .export_values();

      integratorType1D.def(py::init<>(), "  Default constructor.")
          .def(py::init<NumA::IntegratorType1D::Type>(), "  Assignement constructor.\n\n    type : Type to be assigned.", py::arg("type"))
          .def(py::init<const std::string&>(), "  Assignement constructor trying to match integration type from given string. If unable to match set IntegratorType1D::UNDEFINED.\n\n    integratorType1DString : String to be matched.")
          .def("toString", py::overload_cast<>(&NumA::IntegratorType1D::toString, py::const_), "  Returns a formatted string displaying the type.\n\n    return : string.")
          .def("getShortName", &NumA::IntegratorType1D::getShortName, "  Returns a short name for the type.\n\n    return : string.")
          .def("getType", py::overload_cast<>(&NumA::IntegratorType1D::getType, py::const_), "  Returns the wrapper enum.\n\n    return : IntegratorType1D.Type.")
          .def("setType", &NumA::IntegratorType1D::setType, "  <set the type of integration.\n\n    type : integratorType1D.type.", py::arg("type"));

        //Wrapping of NumA/integrator/Integrator1D.h

      py::class_<NumA::Integrator1D> integrator1D(mNumA, "Integrator1D", "  Abstract class for all integration routines.");

      integrator1D.def("clone", py::overload_cast<>(&NumA::Integrator1D::clone, py::const_))
          //.def("integrate", &NumA::Integrator1D::integrate, "  Integration routine.\n\n    function : Function representing the one-dimensional function to integrate.\n    a : Lower bound.\n    b : Upper bound.\n    parameters : Parameters tha can be passed to the function.\n    return : Integral.", py::arg("function"), py::arg("a"), py::arg("b"), py::arg("parameters"))
          .def("configure", &NumA::Integrator1D::configure, "  Provides a generic method to configure all types of integrations by passing a Parameters object.\n  Parameters class represents a list of couples key/values (see Parameters class documentation for more info).\n\n    parameters : ElemUtils.Parameters object.", py::arg("parameters"))
          .def("getTolerances", py::overload_cast<>(&NumA::Integrator1D::getTolerances, py::const_), "    return : Absolute ans relative tolerances.")
          .def("setTolerances", &NumA::Integrator1D::setTolerances, "    tolerances : Absolute ans relative tolerances.", py::arg("tolerances"))
          .def("getErrors", py::overload_cast<>(&NumA::Integrator1D::getErrors, py::const_), "    return : Absolute and relative errors estimations.")
          .def("setErrors", &NumA::Integrator1D::setErrors, "    errors : Absolute and relative errors estimations.", py::arg("errors"));
      integrator1D.attr("PARAM_NAME_ABSOLUTE_TOLERANCE") = NumA::Integrator1D::PARAM_NAME_ABSOLUTE_TOLERANCE;
      integrator1D.attr("PARAM_NAME_RELATIVE_TOLERANCE") = NumA::Integrator1D::PARAM_NAME_RELATIVE_TOLERANCE;

      // Wrapping of NumA/integrator/QuadratureIntegrator1D.h

    py::class_<NumA::QuadratureIntegrator1D, NumA::Integrator1D> quadratureIntegrator1D(mNumA, "QuadratureIntegrator1D", "    Abstract class (for fixed quadrature rules).\n\n  Child classes must define a way to compute the nodes xi and weights wi.");

    quadratureIntegrator1D.attr("PARAM_NAME_N") = NumA::QuadratureIntegrator1D::PARAM_NAME_N;
    quadratureIntegrator1D.def("clone", py::overload_cast<>(&NumA::QuadratureIntegrator1D::clone, py::const_))
        .def("configure", &NumA::QuadratureIntegrator1D::configure)
        .def("getN", py::overload_cast<>(&NumA::QuadratureIntegrator1D::getN, py::const_), "    return : Order of the quadrature (number of nodes).")
        .def("setN", &NumA::QuadratureIntegrator1D::setN, "    n : Order of the quadrature (number of nodes).", py::arg("n") = 0)
        .def("getNodes", py::overload_cast<>(&NumA::QuadratureIntegrator1D::getNodes, py::const_), "    return : Nodes of the quadrature.")
        .def("getWeights", py::overload_cast<>(&NumA::QuadratureIntegrator1D::getWeights, py::const_), "    return : Weights of the quadrature.");

    declare_quadrature_integrator<NumA::TrapezoidalIntegrator1D>(mNumA, "TrapezoidalIntegrator1D", 40, "    Trapezoidal integration.");
    declare_quadrature_integrator<NumA::TrapezoidalLogIntegrator1D>(mNumA, "TrapezoidalLogIntegrator1D", 40, "    Trapezoidal integration with logarithmic step.");
    declare_quadrature_integrator<NumA::ChebyshevAIntegrator1D>(mNumA, "ChebyshevAIntegrator1D", 48, "    Chebyshev quadrature for computing the integral int from -1 to 1 of dx f(x) sqrt(1-x^2).");
    declare_quadrature_integrator<NumA::ChebyshevBIntegrator1D>(mNumA, "ChebyshevBIntegrator1D", 48, "    Chebyshev quadrature for computing the integral int from -1 to 1 of dx f(x)/sqrt(1-x^2).");
    declare_quadrature_integrator<NumA::GaussLegendreIntegrator1D>(mNumA, "GaussLegendreIntegrator1D", 50, "    Gauss-Legengre quadrature.");
    declare_quadrature_integrator<NumA::GaussLegendreSeStIntegrator1D>(mNumA, "GaussLegendreSeStIntegrator1D", 50, "    Gauss-Legendre quadrature.");

    py::class_<NumA::DExpIntegrator1D, NumA::Integrator1D>(mNumA, "DExpIntegrator1D", "    This is an implementation of the double exponential rule.\n\n  The functions being integrated are required to be smooth (no discontinuities in the function or any of its derivatives), with no infinities in the middle of the integration interval (but it does allow functions that become infinite at the ends of the integration interval)\n  The method is based on the observation that the trapezoid rule converges extremely fast for functions that go to zero like exp(-exp(t)). In practice, it implements a change of variables to implement this property in the function to be integrated. \n The change of variable is x = tanh( pi sinh(t) /2) which transforms an integral over [-1, 1] into an integral with integrand suited to the double exponential rule. The transformed integral is infinite, but one can truncate the domain of integration to [-3, 3].\n  The limit '3' is chosen for two reasons:\n    1. The transformed x values are equal to 1 for 12 or more significant figures;\n    2. The smallest weights are 12 orders of magnitude smaller than the largest weights (setting the cutoff larger than 3 would not have a significant impact on the integral value unless there is a strong singularity at one of the end points).\n  The change of variables x(t) is an odd function, whereas its derivative w(t) is even; thus in the cpp file we will store only positive values of nodes and weights.\n  The integration first applies the trapezoid rule to [-3, 3] in steps of size 1, subsequently cutting the step size in half each time, and comparing the results. The routine stops when subsequent iterations are close enough together or the maximum integration points have been used. Notice that by cutting h in half, the previous integral can be reused; we only need evaluate the integrand at the newly added points.\n  As we assume that values at the ends of the interval hardly matter due to the rapid decay of the integrand we don't treat the end points differently.")
        .def(py::init<>(), "  Default contructor.")
        .def("clone", py::overload_cast<>(&NumA::DExpIntegrator1D::clone, py::const_));

    py::class_<NumA::GaussKronrodAdaptive, NumA::Integrator1D> gaussKronrodAdaptive(mNumA, "GaussKronrodAdaptive");

    /*py::enum_<NumA::GaussKronrodAdaptive>(gaussKronrodAdaptive)
        .value("GSL_CONTINUE", NumA::GaussKronrodAdaptive::GSL_CONTINUE)
        .value("GSL_FAILURE", NumA::GaussKronrodAdaptive::GSL_FAILURE)
        .value("GSL_SUCCESS", NumA::GaussKronrodAdaptive::GSL_SUCCESS)
        .value("GSL_EDOM", NumA::GaussKronrodAdaptive::GSL_EDOM)
        .value("GSL_ERANGE", NumA::GaussKronrodAdaptive::GSL_ERANGE)
        .value("GSL_EFAULT", NumA::GaussKronrodAdaptive::GSL_EFAULT)
        .value("GSL_EINVAL", NumA::GaussKronrodAdaptive::GSL_EINVAL)
        .value("GSL_EFAILED", NumA::GaussKronrodAdaptive::GSL_EFAILED)
        .value("GSL_FACTOR", NumA::GaussKronrodAdaptive::GSL_FACTOR)
        .value("GSL_ESANITY", NumA::GaussKronrodAdaptive::GSL_ESANITY)
        .value("GSL_ENOMEM", NumA::GaussKronrodAdaptive::GSL_ENOMEM)
        .value("GSL_EBADFUNC", NumA::GaussKronrodAdaptive::GSL_EBADFUNC)
        .value("GSL_RUNAWAY", NumA::GaussKronrodAdaptive::GSL_RUNAWAY)
        .value("GSL_MAXITER", NumA::GaussKronrodAdaptive::GSL_MAXITER)
        .value("GSL_EZERODIV", NumA::GaussKronrodAdaptive::GSL_EZERODIV)
        .value("GSL_EBADTOL", NumA::GaussKronrodAdaptive::GSL_EBADTOL)
        .value("GSL_ETOL", NumA::GaussKronrodAdaptive::GSL_ETOL)
        .value("GSL_EUNDRFLW", NumA::GaussKronrodAdaptive::GSL_EUNDRFLW)
        .value("GSL_EOVRFLW", NumA::GaussKronrodAdaptive::GSL_EOVRFLW)
        .value("GSL_ELOSS", NumA::GaussKronrodAdaptive::GSL_ELOSS)
        .value("GSL_EROUND", NumA::GaussKronrodAdaptive::GSL_EROUND)
        .value("GSL_EBADLEN", NumA::GaussKronrodAdaptive::GSL_EBADLEN)
        .value("GSL_ENOTSQR", NumA::GaussKronrodAdaptive::GSL_ENOTSQR)
        .value("GSL_ESING", NumA::GaussKronrodAdaptive::GSL_ESING)
        .value("GSL_EDIVERGE", NumA::GaussKronrodAdaptive::GSL_EDIVERGE)
        .value("GSL_EUNSUP", NumA::GaussKronrodAdaptive::GSL_EUNSUP)
        .value("GSL_EUNIMPL", NumA::GaussKronrodAdaptive::GSL_EUNIMPL)
        .value("GSL_ECACHE", NumA::GaussKronrodAdaptive::GSL_ECACHE)
        .value("GSL_ETABLE", NumA::GaussKronrodAdaptive::GSL_ETABLE)
        .value("GSL_ENOPROG", NumA::GaussKronrodAdaptive::GSL_ENOPROG)
        .value("GSL_ENOPROGJ", NumA::GaussKronrodAdaptive::GSL_ENOPROGJ)
        .value("GSL_ETOLF", NumA::GaussKronrodAdaptive::GSL_ETOLF)
        .value("GSL_ETOLX", NumA::GaussKronrodAdaptive::GSL_ETOLX)
        .value("GSL_ETOLG", NumA::GaussKronrodAdaptive::GSL_ETOLG)
        .value("GSL_EOF", NumA::GaussKronrodAdaptive::GSL_EOF)
        .export_values();*/

    gaussKronrodAdaptive.def(py::init<>());

    py::class_<NumA::Workspace>(mNumA, "Workspace")
        .def(py::init<double>())
        .def("initialise",&NumA::Workspace::initialise, py::arg("a"), py::arg("b"))
        .def("set_initial_result", &NumA::Workspace::set_initial_result, py::arg("result"), py::arg("error"))
        .def("retrieve", &NumA::Workspace::retrieve, py::arg("a"), py::arg("b"), py::arg("r"), py::arg("e"))
        .def("update", &NumA::Workspace::update, py::arg("a1"), py::arg("b1"), py::arg("area1"), py::arg("error1"), py::arg("a2"), py::arg("b2"), py::arg("area2"), py::arg("error2"))
        .def("qpsrt", &NumA::Workspace::qpsrt)
        .def("large_interval", &NumA::Workspace::large_interval)
        .def("increase_nrmax", &NumA::Workspace::increase_nrmax)
        .def("reset_nrmax", &NumA::Workspace::reset_nrmax)
        .def("sum_results", &NumA::Workspace::sum_results);

      //Wrapping of partons/partons/include/partons/modules/evolution/gpd/GPDEvolutionModule

    py::class_<PARTONS::GPDEvolutionModule, PARTONS::ModuleObject> gpdEvolutionModule(mPARTONS, "GPDEvolutionModule", "    Abstract class for GPD evolution implementation.\n\n  This class acts as an abstract (mother) class for modules implementing GPD evolution.");

    gpdEvolutionModule.attr("GPD_EVOLUTION_MODULE_CLASS_NAME") = PARTONS::GPDEvolutionModule::GPD_EVOLUTION_MODULE_CLASS_NAME;
    gpdEvolutionModule.def("configure", &PARTONS::GPDEvolutionModule::configure)
      //.def("compute", &PARTONS::GPDEvolutionModule::compute, "  Evaluate PartonDistribution.\n\n    x : Longitudinal momentum of active parton.\n    xi : Skewness variable.\n    t : Four-momentum transfer squared of hadron target (in GeV^2).\n    MuF2 : Factorization scale squared (in GeV^2).\n    MuR2 : Renormalization scale squared  (in GeV^2).\n    gpdType : Type of GPD.\n    result : Evaluated result encapsulated in a PartonDistribution object.")
        .def("getPertOrder", py::overload_cast<>(&PARTONS::GPDEvolutionModule::getPertOrder, py::const_), "  Get QCD order assumed in the calculation.")
        .def("setPertOrder", &PARTONS::GPDEvolutionModule::setPertOrder, "  Set QCD order assumed in the calculation.", py::arg("qcdOrderType"))
        .def("getRunningAlphaStrongModule", py::overload_cast<>(&PARTONS::GPDEvolutionModule::getRunningAlphaStrongModule, py::const_), "  Get pointer to RunningAlphaStrongModule object.")
        .def("setRunningAlphaStrongModule", &PARTONS::GPDEvolutionModule::setRunningAlphaStrongModule, "  Set pointer to RunningAlphaStrongModule object.", py::arg("runningAlphaStrongModule"))
        .def("getActiveFlavorsModule", py::overload_cast<>(&PARTONS::GPDEvolutionModule::getActiveFlavorsModule, py::const_), "  Get pointer to ActiveFlavorsThresholdsModule object.")
        .def("setActiveFlavorsModule", &PARTONS::GPDEvolutionModule::setActiveFlavorsModule, "  Set pointer to ActiveFlavorsThresholdsModule object.", py::arg("activeFlavorsThresholdsModule"));

      //Wrapping of partons/partons/include/partons/modules/evolution/gpd/GPDEvolutionVinnikov.h

    py::class_<PARTONS::GPDEvolutionVinnikov, PARTONS::GPDEvolutionModule> gpdEvolutionVinnikov(mPARTONS, "GPDEvolutionVinnikov", "    GPD evolution according to Vinnikov routines.\n\n  This class makes use of original Vinnikov routines for GPD evolution. For more reference see Vinnikov:2006xw");

    gpdEvolutionVinnikov.attr("classId") = PARTONS::GPDEvolutionVinnikov::classId;
    gpdEvolutionVinnikov.attr("PARAM_NAME_GRID_SIZE") = PARTONS::GPDEvolutionVinnikov::PARAM_NAME_GRID_SIZE;
    gpdEvolutionVinnikov.def(py::init<const std::string&>(), py::arg("className"))
        .def("clone", py::overload_cast<>(&PARTONS::GPDEvolutionVinnikov::clone, py::const_))
        .def("getGridSize", py::overload_cast<>(&PARTONS::GPDEvolutionVinnikov::getGridSize, py::const_))
        .def("setGridSize", &PARTONS::GPDEvolutionVinnikov::setGridSize, py::arg("gridSize"));

      //Wrapping of partons/partons/include/partons/modules/running_alpha_strong/RunningAlphaStrongModule.h
    py::class_<PARTONS::RunningAlphaStrongModule, PARTONS::ModuleObject> runningAlphaStrongModule(mPARTONS, "RunningAlphaStrongModule", "    Abstract class for modules evaluating QCD running coupling constant.\n\n  This class acts as an abstract (mother) class for modules used in the evaluation of the QCD running coupling constant.");

    runningAlphaStrongModule.attr("RUNNING_ALPHA_STRONG_MODULE_CLASS_NAME") = PARTONS::RunningAlphaStrongModule::RUNNING_ALPHA_STRONG_MODULE_CLASS_NAME;
    runningAlphaStrongModule.def("clone", py::overload_cast<>(&PARTONS::RunningAlphaStrongModule::clone, py::const_))
        .def("getNf", py::overload_cast<>(&PARTONS::RunningAlphaStrongModule::getNf, py::const_))
        .def("setNf", &PARTONS::RunningAlphaStrongModule::setNf, py::arg("nf"));

      //Wrapping of partons/partons/include/partons/modules/running_alpha_strong/RunningAlphaStrongStandard.h

    py::class_<PARTONS::RunningAlphaStrongStandard, PARTONS::RunningAlphaStrongModule> runningAlphaStrongStandard(mPARTONS, "RunningAlphaStrongStandard", "    Evaluation of the strong coupling constant in the MSbar scheme.\n\n  This module evaluates the strong running coupling constant in the MSbar scheme.\n  It takes alpha(M_Z) and quark masses (c, b and t) as inputs and computes the evolution downwards to get Lambda_QCD for different numbers of active flavors. Then alpha(M_Z) is computed at the required scale.");

    runningAlphaStrongStandard.attr("classid") = PARTONS::RunningAlphaStrongStandard::classId;
    runningAlphaStrongStandard.def(py::init<const std::string&>(), "  Constrauctor.\n\n    className : Name of the calss.", py::arg("className"))
        .def("clone", py::overload_cast<>(&PARTONS::RunningAlphaStrongStandard::clone, py::const_))
        .def("getColourNumber", py::overload_cast<>(&PARTONS::RunningAlphaStrongStandard::GetColourNumber, py::const_), "  Get number of colour (SU n_c).");

      //Wrapping of partons/partons/include/partons/modules/running_alpha_strong/RunningAlphaStrongVinnikov.h

    py::class_<PARTONS::RunningAlphaStrongVinnikov, PARTONS::RunningAlphaStrongModule> runningAlphaStrongVinnikov(mPARTONS, "RunningAlphaStrongVinnikov", "    Evaluation of the strong running coupling constant as in the Vinnikov evolution routines.\n\n    This module evaluates the strong running coupling constant as in the Vinnikov evolution routines.\n  Note: As in the original routines, the code offers the evaluation for 3, 4 or 5 active quark flavors.\n  However, by default the evaluation is done for 3 active quark flavors only.");

    runningAlphaStrongVinnikov.def(py::init<const std::string&>(), "  Constrauctor.\n\n    className : Name of the calss.", py::arg("className"))
        .def("clone", py::overload_cast<>(&PARTONS::RunningAlphaStrongVinnikov::clone, py::const_));

      //Wrapping of /partons/partons/include/partons/modules/active_flavors_thresholds/ActiveFlavorsThresholdsModule.h

    py::class_<PARTONS::ActiveFlavorsThresholdsModule, PARTONS::ModuleObject> activeFlavorsThresholdsModule(mPARTONS, "ActiveFlavorsThresholdsModule", "    Abstract class for modules defining number of quark flavors intervals.\n\n  This class acts as an abstract (mother) class for modules that define the number of quark flavors intervals.\n  These intervals are used e.g. by the evolution modules to evaluate the number of active quark flavors for a given factorization scale squared.");

    activeFlavorsThresholdsModule.attr("ACTIVE_FLAVORS_THRESHOLDS_MODULE_CLASS_NAME") = PARTONS::ActiveFlavorsThresholdsModule::ACTIVE_FLAVORS_THRESHOLDS_MODULE_CLASS_NAME;
    activeFlavorsThresholdsModule.def("clone", py::overload_cast<>(&PARTONS::ActiveFlavorsThresholdsModule::clone, py::const_))
        .def("configure", &PARTONS::ActiveFlavorsThresholdsModule::configure, py::arg("parameters"))
        .def("toString", py::overload_cast<>(&PARTONS::ActiveFlavorsThresholdsModule::toString, py::const_))
        .def("getNfInterval", py::overload_cast<double>(&PARTONS::ActiveFlavorsThresholdsModule::getNfInterval, py::const_), "  Get interval for a specofoc value of the factorization scale squared.\n\n    Mu2 : Factorization scale squared value.\n    return : Request interval. If Mu2 is smaller than the lower bound of the first defined interval returns this interval. If Mu2 is larger than the upper bound of the last defined interval returns this interval.", py::arg("Mu2"))
        .def("getNfIntervals", py::overload_cast<double, double>(&PARTONS::ActiveFlavorsThresholdsModule::getNfIntervals, py::const_), "  Get intervals for a specific range of the factorization scale squared.\n    Mu2Min : Factorization scale squared minimum value.\n    Mu2Max : Factorization scale squared maximum value.\n    return : If lowerBound > upperBound a single interval with the number of active flavors corresponding to lowerBound is returned. Otherwise, a vector containing intervals for the requested range of factorization scale.");

      //Wrapping of /partons/partons/include/partons/modules/active_flavors_thresholds/ActiveFlavorsThresholdsConstant.h

    py::class_<PARTONS::ActiveFlavorsThresholdsConstant, PARTONS::ActiveFlavorsThresholdsModule> activeFlavorsThresholdsConstant(mPARTONS, "ActiveFlavorsThresholdsConstant", "    Fixed number of active quark flavors.\n\n  This module defines one number of active quark flavors for whole range of factorization scale. The default number is 3 and can be changed by od by.");

    activeFlavorsThresholdsConstant.attr("PARAM_NAME_N_FLAVORS") = PARTONS::ActiveFlavorsThresholdsConstant::PARAM_NAME_N_FLAVORS;
    activeFlavorsThresholdsConstant.attr("calssId") = PARTONS::ActiveFlavorsThresholdsConstant::classId;
    activeFlavorsThresholdsConstant.def(py::init<const std::string>(), "  Constructor.\n\n    className : Name of the class.", py::arg("className"))
        .def("clone", py::overload_cast<>(&PARTONS::ActiveFlavorsThresholdsConstant::clone, py::const_))
        .def("configure", &PARTONS::ActiveFlavorsThresholdsConstant::configure, py::arg("parameters"))
        .def("setNFlavors", &PARTONS::ActiveFlavorsThresholdsConstant::setNFlavors, "  Set number of quark flavors.", py::arg("nFlavors"))
        .def("getNFlavors", py::overload_cast<>(&PARTONS::ActiveFlavorsThresholdsConstant::getNFlavors, py::const_), "  Get number of quark flavors.");

    py::class_<PARTONS::ActiveFlavorsThresholdsQuarkMasses, PARTONS::ActiveFlavorsThresholdsModule> activeFlavorsThresholdsQuarkMasses(mPARTONS, "ActiveFlavorsThresholdsQuarkMasses",((std::string)( "    Number of active quark flavors intervals corresponding to quark masses.\n\n    This module defines number of active quark flavors intervals corresponding to quark masses. The intervals are:\n  N. of active flavors | Min muF2 [GeV^2]     | Mac muF^2 [GeV^2]\n")
                                                                                                                                                                                                                                                                                                                                                                                          +(std::string)("  1                    | 0.                   | QUARK_UP_MASS^2\n")
                                                                                                                                                                                                                                                                                                                                                                                          +(std::string)("  2                    | QUARK_UP_MASS^2      | QUARK_DOWN_MASS^2\n")
                                                                                                                                                                                                                                                                                                                                                                                          +(std::string)("  3                    | QUARK_DOWN_MASS^2    | QUARK_STRANGE_MASS^2\n")
                                                                                                                                                                                                                                                                                                                                                                                          +(std::string)("  4                    | QUARK_STRANGE_MASS^2 | QUARK_CHARM_MASS^2\n")
                                                                                                                                                                                                                                                                                                                                                                                          +(std::string)("  5                    | QUARK_CHARM_MASS^2   | QUARK_BOTTOM_MASS^2\n")
                                                                                                                                                                                                                                                                                                                                                                                          +(std::string)("  6                    | QUARK_BOTTOM_MASS^2  | QUARK_TOP_MASS^2\n  The quark masses are defined in FundamentalPhysicalConstants class.")).c_str());
    activeFlavorsThresholdsQuarkMasses.attr("classId") = PARTONS::ActiveFlavorsThresholdsQuarkMasses::classId;
    activeFlavorsThresholdsQuarkMasses.def(py::init<const std::string&>(), "  Constructor.\n\n    className : Name of the class.")
        .def("clone", py::overload_cast<>(&PARTONS::ActiveFlavorsThresholdsQuarkMasses::clone, py::const_));

      //Wrapping of partons/partons/include/partons/modules/MathIntegratorModule.h

    py::class_<PARTONS::MathIntegratorModule> mathIntegratorModule(mPARTONS, "MathIntegratorModule");

    mathIntegratorModule.attr("PARAM_NAME_INTEGRATOR_TYPE") = PARTONS::MathIntegratorModule::PARAM_NAME_INTEGRATOR_TYPE;

    mathIntegratorModule.def(py::init<>(), "  Default Constructor.");

      //Wrapping of partons/partons/include/partons/utils/type/PhysicalType.h

    declare_PhysicalType<double>(mPARTONS, "double");
}
