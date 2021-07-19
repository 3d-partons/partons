#include "../../../../include/partons/modules/gpd/GPDBDMMS21.h"

#include <ElementaryUtils/PropertiesManager.h>
#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <cmath>
#include <utility>

#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../include/partons/utils/PartonContent.h"

namespace PARTONS {

const unsigned int GPDBDMMS21::classId =
    BaseObjectRegistry::getInstance()->registerBaseObject(
        new GPDBDMMS21("GPDBDMMS21"));

GPDBDMMS21::GPDBDMMS21(const std::string &className) : GPDModule(className) {

  // reference scale
  m_MuF2_ref = 1.;

  // available GPD types
  m_listGPDComputeTypeAvailable.insert(
      std::make_pair(GPDType::H, &GPDModule::computeH));
}

GPDBDMMS21::GPDBDMMS21(const GPDBDMMS21 &other) : GPDModule(other) {}

GPDBDMMS21::~GPDBDMMS21() {}

GPDBDMMS21 *GPDBDMMS21::clone() const { return new GPDBDMMS21(*this); }

void GPDBDMMS21::resolveObjectDependencies() {}

void GPDBDMMS21::configure(const ElemUtils::Parameters &parameters) {
  GPDModule::configure(parameters);
}

void GPDBDMMS21::isModuleWellConfigured() {
  GPDModule::isModuleWellConfigured();
}

void GPDBDMMS21::initModule() { GPDModule::initModule(); }

PartonDistribution GPDBDMMS21::computeH() {

  // result
  double trans = 0.;

  // absolute x
  double absX = fabs(m_x);

  // to cut the GPD close to x = 1
  if (fabs(m_xi) >= 1. || absX >= 1.) {
    trans = 0.;
  }
  // DGLAP region
  else if (absX >= fabs(m_xi)) {
    trans = pow(m_xi, 2) * (pow(absX, 2) - pow(m_xi, 2)) * pow(absX - 1, 5) *
            (1.9781153654109946851 * pow(10., -8) * absX +
             9.8905768270549734256 * pow(10., -8) * pow(absX, 2) +
             5.4207095851873525136 * pow(10., -6) * pow(absX, 3) +
             0.000026312301779772364694 * pow(absX, 4) -
             0.0021598622937353977786 * pow(absX, 5) -
             0.011008702138310537654 * pow(absX, 6) -
             0.56305855622681362802 * pow(absX, 7) +
             7.0072252445919459353 * pow(absX, 8) -
             15.538399226028916540 * pow(absX, 9) -
             44.371667827646332913 * pow(absX, 10) +
             181.94334698775348832 * pow(absX, 11) -
             97.011276613079764504 * pow(absX, 12) -
             181.72143366229912121 * pow(absX, 13) +
             53.025592131162241212 * pow(absX, 14) +
             158.14112838159099855 * pow(absX, 15) +
             36.688915854074584821 * pow(absX, 16) -
             56.114891535498807746 * pow(absX, 17) -
             39.657921691446077944 * pow(absX, 18) -
             7.9315843382892155887 * pow(absX, 19) -
             4.5799625973059142699 * pow(10., -6) * absX * pow(m_xi, 2) -
             0.000022899812986529571349 * pow(absX, 2) * pow(m_xi, 2) +
             0.0027154455327521476970 * pow(absX, 3) * pow(m_xi, 2) +
             0.013760426167652975056 * pow(absX, 4) * pow(m_xi, 2) -
             0.28836205489179660897 * pow(absX, 5) * pow(m_xi, 2) +
             10.591771592324523145 * pow(absX, 6) * pow(m_xi, 2) -
             98.315849249571176612 * pow(absX, 7) * pow(m_xi, 2) +
             155.79653880803078638 * pow(absX, 8) * pow(m_xi, 2) +
             1157.8242830169957934 * pow(absX, 9) * pow(m_xi, 2) -
             4741.3251010147422627 * pow(absX, 10) * pow(m_xi, 2) +
             5284.8979877142756907 * pow(absX, 11) * pow(m_xi, 2) +
             728.14359319075315680 * pow(absX, 12) * pow(m_xi, 2) -
             2983.4027503189065284 * pow(absX, 13) * pow(m_xi, 2) -
             1018.8906581020362782 * pow(absX, 14) * pow(m_xi, 2) +
             855.19510500029474521 * pow(absX, 15) * pow(m_xi, 2) +
             639.64983674072894688 * pow(absX, 16) * pow(m_xi, 2) +
             102.54889746562029949 * pow(absX, 17) * pow(m_xi, 2) -
             15.863168676578431177 * pow(absX, 18) * pow(m_xi, 2) -
             3.1726337353156862355 * pow(absX, 19) * pow(m_xi, 2) -
             0.00079628510480319772166 * absX * pow(m_xi, 4) -
             0.0039814255240159886083 * pow(absX, 2) * pow(m_xi, 4) +
             0.97810774434163170660 * pow(absX, 3) * pow(m_xi, 4) -
             18.553693906623295123 * pow(absX, 4) * pow(m_xi, 4) +
             80.019322701584110464 * pow(absX, 5) * pow(m_xi, 4) +
             205.57669342342535603 * pow(absX, 6) * pow(m_xi, 4) -
             1108.9720925829313412 * pow(absX, 7) * pow(m_xi, 4) -
             5292.4515355562200669 * pow(absX, 8) * pow(m_xi, 4) +
             32931.486866169982130 * pow(absX, 9) * pow(m_xi, 4) -
             55942.856316219247591 * pow(absX, 10) * pow(m_xi, 4) +
             25457.226013562047867 * pow(absX, 11) * pow(m_xi, 4) +
             14923.022390386173170 * pow(absX, 12) * pow(m_xi, 4) -
             5965.5382819086752892 * pow(absX, 13) * pow(m_xi, 4) -
             5267.5836235431551521 * pow(absX, 14) * pow(m_xi, 4) -
             681.29777879677809702 * pow(absX, 15) * pow(m_xi, 4) +
             231.40303918672976096 * pow(absX, 16) * pow(m_xi, 4) +
             44.870548399427869420 * pow(absX, 17) * pow(m_xi, 4) -
             0.88128714869880173208 * pow(absX, 18) * pow(m_xi, 4) -
             0.17625742973976034642 * pow(absX, 19) * pow(m_xi, 4) -
             0.24125260424052791049 * absX * pow(m_xi, 6) +
             1.9943886251995084972 * pow(absX, 2) * pow(m_xi, 6) +
             44.423991242391992362 * pow(absX, 3) * pow(m_xi, 6) -
             388.41593759720796164 * pow(absX, 4) * pow(m_xi, 6) -
             524.67033235850301142 * pow(absX, 5) * pow(m_xi, 6) +
             10202.070562178728021 * pow(absX, 6) * pow(m_xi, 6) -
             15249.732327475805994 * pow(absX, 7) * pow(m_xi, 6) -
             57095.368928101023144 * pow(absX, 8) * pow(m_xi, 6) +
             192110.31232352455849 * pow(absX, 9) * pow(m_xi, 6) -
             179121.24408611189640 * pow(absX, 10) * pow(m_xi, 6) +
             18406.489385218757264 * pow(absX, 11) * pow(m_xi, 6) +
             31455.322140932687386 * pow(absX, 12) * pow(m_xi, 6) +
             3515.3877982545091957 * pow(absX, 13) * pow(m_xi, 6) -
             1712.5462403270528480 * pow(absX, 14) * pow(m_xi, 6) -
             317.07878705916505185 * pow(absX, 15) * pow(m_xi, 6) +
             15.894038128903448588 * pow(absX, 16) * pow(m_xi, 6) +
             3.1788076257806897176 * pow(absX, 17) * pow(m_xi, 6) +
             0.97024584933205212617 * pow(m_xi, 8) -
             22.821221936891537592 * absX * pow(m_xi, 8) +
             80.546663366834170180 * pow(absX, 2) * pow(m_xi, 8) +
             461.56501780904158235 * pow(absX, 3) * pow(m_xi, 8) +
             587.65889317797101130 * pow(absX, 4) * pow(m_xi, 8) -
             30942.846621894570688 * pow(absX, 5) * pow(m_xi, 8) +
             116058.28987373951330 * pow(absX, 6) * pow(m_xi, 8) -
             106136.24140963659717 * pow(absX, 7) * pow(m_xi, 8) -
             172560.74571146452891 * pow(absX, 8) * pow(m_xi, 8) +
             374932.30846467831723 * pow(absX, 9) * pow(m_xi, 8) -
             175075.44024096178211 * pow(absX, 10) * pow(m_xi, 8) -
             20654.346082834079890 * pow(absX, 11) * pow(m_xi, 8) +
             8810.6358164337153885 * pow(absX, 12) * pow(m_xi, 8) +
             1573.7524068122202844 * pow(absX, 13) * pow(m_xi, 8) -
             117.73422279657674585 * pow(absX, 14) * pow(m_xi, 8) -
             23.546844559315349169 * pow(absX, 15) * pow(m_xi, 8) +
             14.952833221954524282 * pow(m_xi, 10) -
             56.315554285042311432 * absX * pow(m_xi, 10) -
             1594.9038256576631971 * pow(absX, 2) * pow(m_xi, 10) +
             9533.2068012653309691 * pow(absX, 3) * pow(m_xi, 10) +
             8797.1674504581463096 * pow(absX, 4) * pow(m_xi, 10) -
             164654.50613438123593 * pow(absX, 5) * pow(m_xi, 10) +
             378000.78902134162791 * pow(absX, 6) * pow(m_xi, 10) -
             217468.08809982818278 * pow(absX, 7) * pow(m_xi, 10) -
             217336.82472243109333 * pow(absX, 8) * pow(m_xi, 10) +
             254648.05826653967989 * pow(absX, 9) * pow(m_xi, 10) -
             38774.842040282214100 * pow(absX, 10) * pow(m_xi, 10) -
             6902.9304818604595745 * pow(absX, 11) * pow(m_xi, 10) +
             532.52370387248952846 * pow(absX, 12) * pow(m_xi, 10) +
             106.50474077449790569 * pow(absX, 13) * pow(m_xi, 10) -
             53.007998147416263552 * pow(m_xi, 12) +
             2429.8528315988857087 * absX * pow(m_xi, 12) -
             21351.325462984932197 * pow(absX, 2) * pow(m_xi, 12) +
             60407.737431538303098 * pow(absX, 3) * pow(m_xi, 12) +
             3402.5646475321875629 * pow(absX, 4) * pow(m_xi, 12) -
             286936.74861146392456 * pow(absX, 5) * pow(m_xi, 12) +
             428790.42237029855502 * pow(absX, 6) * pow(m_xi, 12) -
             125978.51411127411373 * pow(absX, 7) * pow(m_xi, 12) -
             108872.51935779705046 * pow(absX, 8) * pow(m_xi, 12) +
             44740.517983802130789 * pow(absX, 9) * pow(m_xi, 12) -
             1859.3641735903611877 * pow(absX, 10) * pow(m_xi, 12) -
             371.87283471807223753 * pow(absX, 11) * pow(m_xi, 12) -
             853.29844833922602184 * pow(m_xi, 14) +
             13236.674848582701410 * absX * pow(m_xi, 14) -
             65719.669334875971679 * pow(absX, 2) * pow(m_xi, 14) +
             119859.88542902686035 * pow(absX, 3) * pow(m_xi, 14) -
             10652.576069538706354 * pow(absX, 4) * pow(m_xi, 14) -
             194199.58887279685824 * pow(absX, 5) * pow(m_xi, 14) +
             164028.73526016797471 * pow(absX, 6) * pow(m_xi, 14) -
             10607.750816680038307 * pow(absX, 7) * pow(m_xi, 14) -
             11848.731198908898554 * pow(absX, 8) * pow(m_xi, 14) +
             1601.1111621099985052 * pow(absX, 9) * pow(m_xi, 14) -
             2298.8021027316481529 * pow(m_xi, 16) +
             22302.756908501618752 * absX * pow(m_xi, 16) -
             71566.303830252158958 * pow(absX, 2) * pow(m_xi, 16) +
             82286.518749752046173 * pow(absX, 3) * pow(m_xi, 16) -
             442.91596095874039645 * pow(absX, 4) * pow(m_xi, 16) -
             47196.680019254635270 * pow(absX, 5) * pow(m_xi, 16) +
             13994.198385616129654 * pow(absX, 6) * pow(m_xi, 16) +
             39.220299591306186264 * pow(absX, 7) * pow(m_xi, 16) -
             145.98740448131537559 * pow(absX, 8) * pow(m_xi, 16) -
             2275.5125529794829239 * pow(m_xi, 18) +
             14444.913950371349354 * absX * pow(m_xi, 18) -
             28770.250505356937508 * pow(absX, 2) * pow(m_xi, 18) +
             17387.476362618452112 * pow(absX, 3) * pow(m_xi, 18) +
             2446.0343387147832737 * pow(absX, 4) * pow(m_xi, 18) -
             2021.5120025470624955 * pow(absX, 5) * pow(m_xi, 18) +
             134.62595327551924933 * pow(absX, 6) * pow(m_xi, 18) -
             913.10064154886320225 * pow(m_xi, 20) +
             3527.0113185998332592 * absX * pow(m_xi, 20) -
             3564.3766684119398832 * pow(absX, 2) * pow(m_xi, 20) +
             495.42631216184088147 * pow(absX, 3) * pow(m_xi, 20) +
             51.307015970142626343 * pow(absX, 4) * pow(m_xi, 20) -
             139.70810880715583728 * pow(m_xi, 22) +
             252.57036625594730332 * absX * pow(m_xi, 22) -
             39.456318679884862690 * pow(absX, 2) * pow(m_xi, 22) -
             6.1171615640755502789 * pow(m_xi, 24)) /
            pow(1 - pow(m_xi, 2), 17);
  }
  // ERBL region
  else {
    trans = absX * m_xi * (pow(absX, 2) - pow(m_xi, 2)) *
            (-1.7773666992943329387 * pow(absX, 8) -
             10.370510860523957791 * pow(absX, 10) +
             145.98740448131537559 * pow(absX, 12) -
             1.9781153654109946851 * pow(10., -8) * m_xi -
             5.1239922803757033108 * pow(10., -6) * pow(absX, 2) * m_xi +
             0.0022381068586968210246 * pow(absX, 4) * m_xi +
             0.52984978684825497497 * pow(absX, 6) * m_xi +
             25.890615725640473924 * pow(absX, 8) * m_xi -
             351.83984196968805298 * pow(absX, 10) * m_xi -
             3.3627961211986909647 * pow(10., -7) * pow(m_xi, 2) -
             0.000087107868766386956284 * pow(absX, 2) * pow(m_xi, 2) +
             0.038047816597845957418 * pow(absX, 4) * pow(m_xi, 2) -
             10.335622269227429726 * pow(absX, 6) * pow(m_xi, 2) +
             261.21121208806439382 * pow(absX, 8) * pow(m_xi, 2) -
             134.62595327551924933 * pow(absX, 10) * pow(m_xi, 2) +
             1.5534460882270924017 * pow(10., -6) * pow(m_xi, 3) -
             0.0035681157906092190176 * pow(absX, 2) * pow(m_xi, 3) +
             0.67223398146627485790 * pow(absX, 4) * pow(m_xi, 3) -
             93.482817693917441024 * pow(absX, 6) * pow(m_xi, 3) +
             422.54770143258561504 * pow(absX, 8) * pow(m_xi, 3) +
             0.000058691426263368004089 * pow(m_xi, 4) -
             0.052295613038783575496 * pow(absX, 2) * pow(m_xi, 4) +
             19.951779424585110861 * pow(absX, 4) * pow(m_xi, 4) -
             329.38563291057568950 * pow(absX, 6) * pow(m_xi, 4) -
             51.307015970142626343 * pow(absX, 8) * pow(m_xi, 4) +
             0.0014011796927368399125 * pow(m_xi, 5) -
             1.4408519441839956259 * pow(absX, 2) * pow(m_xi, 5) +
             85.697547193537273931 * pow(absX, 4) * pow(m_xi, 5) -
             28.927315409761905341 * pow(absX, 6) * pow(m_xi, 5) +
             0.017572303842736308887 * pow(m_xi, 6) -
             9.1775269071209085712 * pow(absX, 2) * pow(m_xi, 6) +
             74.584872853564762301 * pow(absX, 4) * pow(m_xi, 6) +
             39.456318679884862690 * pow(absX, 6) * pow(m_xi, 6) +
             0.38379821284177021108 * pow(m_xi, 7) -
             19.646125334208574221 * pow(absX, 2) * pow(m_xi, 7) -
             52.388487136347140032 * pow(absX, 4) * pow(m_xi, 7) +
             0.87693289090585982442 * pow(m_xi, 8) +
             14.045782444018862029 * pow(absX, 2) * pow(m_xi, 8) +
             6.1171615640755502789 * pow(absX, 4) * pow(m_xi, 8) -
             0.66398966230367550442 * pow(m_xi, 9) -
             2.7721078351021553388 * pow(absX, 2) * pow(m_xi, 9) +
             0.19188594448238966661 * pow(m_xi, 10) +
             0.011287408498964098036 * pow(m_xi, 11)) /
            pow(1 + m_xi, 17);
  }

  // symmetry
  if (m_x < 0.)
    trans = -trans;

  // multiply
  trans *= -3 * pow(10, 7);

  // set result
  QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
  QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
  QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);
  GluonDistribution gluonDistribution(0.);

  quarkDistribution_u.setQuarkDistribution(trans);
  quarkDistribution_u.setQuarkDistributionPlus(2 * trans);
  quarkDistribution_u.setQuarkDistributionMinus(0.);

  PartonDistribution partonDistribution;
  partonDistribution.addQuarkDistribution(quarkDistribution_u);
  partonDistribution.addQuarkDistribution(quarkDistribution_d);
  partonDistribution.addQuarkDistribution(quarkDistribution_s);
  partonDistribution.setGluonDistribution(gluonDistribution);

  // return
  return partonDistribution;
}

} /* namespace PARTONS */
