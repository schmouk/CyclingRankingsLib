// mini-tests.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>

#include "commons/time.h"

int main()
{
    crl::Time t{ "103:31:15.00" };
    std::cout << std::string(t) << '\n';

    crl::Time t2{ 43, 56, 1, 97, 100 };
    std::cout << std::string(t2) << '\n';
    

    crl::MSTime ms1{ "1:02.345" };
    std::cout << std::string(ms1) << '\n';

    std::cout << "t2 < t : " << (t2 < t ? "true" : "false") << std::endl;
}
