// mini-tests.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>

#include "commons/time.h"

int main()
{
    crl::Time t{ "103:31:15 1/5" };
    std::cout << std::string(t) << '\n';

    crl::Time t2{ 43, 56, 1, 97, 100 };
    std::cout << std::string(t2) << '\n';
    
}
