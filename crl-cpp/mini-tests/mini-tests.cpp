// mini-tests.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>

#include "commons/time.h"
#include "utils/average_speed.h"

int main()
{
    crl::AverageSpeed avg{ 205, crl::HMSTime(5, 15, 29) };
    std::cout << avg.get() << " km/h\n";
    std::cout << avg.evaluate(205.0, crl::HMTime(5, 15)) << " km/h\n";
    std::cout << avg.evaluate(0.2, crl::STime(9.873, 1000)) << " km/h\n";
    std::cout << avg.evaluate(0.2, crl::STime(11, crl::SecondFraction(0, 5))) << " km/h\n";
    std::cout << avg.evaluate(0.2, crl::STime(11, crl::SecondFraction(1, 5))) << " km/h\n";
    std::cout << avg.evaluate(0.2, crl::STime(11, crl::SecondFraction(2, 5))) << " km/h\n";
    std::cout << avg.evaluate(0.2, crl::STime(11.099, 1000)) << " km/h\n";
    std::cout << avg.evaluate(0.2, crl::STime(11.100, 1000)) << " km/h\n";
    std::cout << avg.evaluate(0.2, crl::STime(11.101, 1000)) << " km/h\n";

    crl::Time t{ "103:31:15.00" };
    std::cout << std::string(t) << '\n';

    crl::Time t2{ 43, 56, 1, 97, 100 };
    std::cout << std::string(t2) << '\n';
    

    crl::MSTime ms1{ "1:02.345" };
    std::cout << std::string(ms1) << '\n';

    std::cout << "t2 < t : " << (t2 < t ? "true" : "false") << std::endl;
}
