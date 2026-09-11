// mini-tests.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>

#include "commons/time.h"
#include "commons/types.h"
#include "utils/average_speed.h"
#include "utils/competitor_descr.h"
#include "utils/heats_compositing.h"
#include "utils/random.h"
#include "utils/scores.h"

int main()
{
    crl::Rand rnd1{ 0xabcd'1230'4560'7890 };

    crl::TeamsCompositionsList<int, int> teams_compos{
        {1, {1, 2, 3}},
        {2, {11, 12, 13, 14}},
        {3, {21, 22, 23, 24, 25, 26}}
    };
    crl::FullyRandomTeamsBestDispatchHeatsComposition<int, int> heats_best_dispatch{
        rnd1,
        teams_compos
    };
    auto heats_best_dispatch_compos{ heats_best_dispatch.compose_n_heats(3) };

    /** /
    crl::FullyRandomHeatsComposition<crl::RiderId> heats1{ rnd1, {1, 2, 3, 4, 5, 6, 7} };
    crl::FullyRandomHeatsComposition<crl::RiderId>::heats_list_type heats1_compos{ heats1.compose_heats(2) };

    using Perf = crl::PointsCompetitorDescr;
    using Points = crl::PointsScore;
    std::vector<Perf> competitors2{
        Perf{1, 123},
        Perf{2, 1},
        Perf{3, -1},
        Perf{4, 16},
        Perf{5, 25},
        Perf{6, 21},
        Perf{7, 13}
    };
    crl::FrwdBkwdHeatsComposition<Perf> heats2{ competitors2 };
    crl::FrwdBkwdHeatsComposition<Perf>::heats_list_type heats2_compos{ heats2.compose_heats(5) };

    crl::Finals_12_34_Composition<Perf> heats3{ std::vector<Perf>{ Perf{1, 123}, Perf{4, 16}, Perf{5, 25} } };  //competitors2 };
    crl::Finals_12_34_Composition<Perf>::heats_list_type heats3_compos{ heats3.compose_finals() };
    /**/

    /** /
    crl::AverageSpeed avg{205, crl::HMSTime(5, 15, 29)};
    std::cout << avg.get() << " km/h\n";
    std::cout << avg.evaluate(205.0, crl::HMTime(5, 15)) << " km/h\n";
    std::cout << avg.evaluate(0.2, crl::STime(9.873, 1000)) << " km/h\n";
    std::cout << avg.evaluate(0.2, crl::STime(11, crl::SecondFraction(0, 5))) << " km/h\n";
    std::cout << avg.evaluate(0.2, crl::STime(11, crl::SecondFraction(1, 5))) << " km/h\n";
    std::cout << avg.evaluate(0.2, crl::STime(11, crl::SecondFraction(2, 5))) << " km/h\n";
    std::cout << avg.evaluate(0.2, crl::STime(11.099, 1000)) << " km/h\n";
    std::cout << avg.evaluate(0.2, crl::STime(11.100, 1000)) << " km/h\n";
    std::cout << avg.evaluate(0.2, crl::STime(11.101, 1000)) << " km/h\n";
    /**/

    /** /
    crl::Time t{ "103:31:15.00" };
    std::cout << std::string(t) << '\n';

    crl::Time t2{ 43, 56, 1, 97, 100 };
    t2.set_hms_sep(crl::DutchTimeSeps);
    std::cout << std::string(t2) << '\n';
    
    crl::Time t3{ "1u15m27,38" };
    t3.set_hms_sep(crl::FrenchTimeSeps);
    std::cout << std::string(t3) << '\n';

    crl::MSTime ms1{ "1:02.345" };
    std::cout << std::string(ms1) << '\n';

    crl::HMTime thm{ 4, 5, crl::FrenchTimeSeps };
    std::cout << std::string(thm) << " / ";
    thm.set_hms_sep(crl::InternationalTimeSeps);
    std::cout << std::string(thm) << '\n';

    std::cout << "t2 < t : " << (t2 < t ? "true" : "false") << std::endl;
    /**/
}
