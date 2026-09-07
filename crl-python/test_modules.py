#!/usr/bin/env python
"""Quick functional test of the translated modules."""

from commons.time import SecondFraction, Time, HMSTime, MSTime, STime, FltTime, StrTime, \
                         InternationalTimeSeps, DutchTimeSeps, EuropeanTimeSeps, FrenchTimeSeps
from commons.types import RANK_DNS, RANK_DNF, RANK_DSQ
from commons.teams import TeamComposition


if __name__ == "__main__":
    # Test SecondFraction
    frac = SecondFraction(5, 10)
    print(f'SecondFraction(5, 10) = {float(frac):.3f}')
    print(f'str(SecondFraction(5, 10)) = {str(frac)}')

    # Test Time
    t1 = Time(1, 30, 45)
    print(f'Time(1, 30, 45) = {str(t1)}')

    t2 = MSTime(20, 30)
    print(f'MSTime(20, 30) = {str(t2)}')
    t3 = t1 + t2
    print(f'Time(1, 30, 45) + MSTime(20, 30) = {str(t3)}')
    t3.set("00:01:23.456")
    print(f'Time("00:01:23.456") = {str(t3)}')


    t02 = Time(43, 56, 1, 97, 100)
    t02.set_hms_sep(DutchTimeSeps);
    print(f'Time(43, 56, 1, 97, 100) = {str(t02)}')

    t03 = Time("1u15m27,38")
    t03.set_hms_sep(FrenchTimeSeps)
    print(f'Time("1u15m27,38") in French = {str(t03)}')
 
    """
    crl::MSTime ms1{ "1:02.345" };
    std::cout << std::string(ms1) << '\n';

    std::cout << "t2 < t : " << (t2 < t ? "true" : "false") << std::endl;
    """

    print(f'{t3} < {t1} -> {t3 < t1}')
    print(f'{t3} >= {t1} -> {t3 >= t1}')
    print(f'{t3} <= {t1} -> {t3 <= t1}')
    print(f'{t3} > {t1} -> {t3 > t1}')
    print(f'{t3} == {t1} -> {t3 == t1}')
    print(f'{t3} != {t1} -> {t3 != t1}')

    # Test from string
    t4 = StrTime('2:15:30.5')
    print(f'Time("2:15:30.5") = {str(t4)}')

    # Test deep copy
    t1.set(t4);
    print(f'deep copy of {str(t4)} = {str(t1)}')

    # Test constants
    print(f'RANK_DNS = {RANK_DNS}')
    print(f'RANK_DNF = {RANK_DNF}')
    print(f'RANK_DSQ = {RANK_DSQ}')

    # Test TeamComposition
    team: TeamComposition = [1, 2, 3, 4, 5]
    print(f'TeamComposition = {team}')

    print('\nAll functional tests passed!')
