/**
* This file is part of project CyclingRankingsLibrary
* (https://github.com/schmouk/CyclingRankingsLib)
*
* copyright (c) 2026 Philippe Schmouker
*
* License is GNU GENERAL PUBLIC LICENSE Version 3
* (full text available at:
*  https://github.com/schmouk/CyclingRankingsLib?tab=GPL-3.0-1-ov-file)
*/

#include <cmath>
#include <cstdint>
#include <format>
#include <limits>
#include <string>

#include "./time.h"


namespace crl
{
    //=====   Time Fraction of Seconds   ======================
    //---------------------------------------------------------
    SecondFraction::operator double() const noexcept
    {
        try {
            return static_cast<double>(numerator) / static_cast<double>(denominator);
        }
        catch (...) {
            return 0.0;
        }
    }

    //---------------------------------------------------------
    SecondFraction::operator std::string() const noexcept
    {
        switch (denominator) {
        case 0: case 1:
            return "";
        case 10:
            return std::format(".{}", numerator);
        case 100:
            return std::format(".{:02d}", numerator);
        case 1000:
            return std::format(".{:03d}", numerator);

        default:
            return std::format(" {}/{}", numerator, denominator);
        }
    }

    //---------------------------------------------------------
    SecondFraction& SecondFraction::operator+= (const SecondFraction& other) noexcept
    {
        if (denominator == other.denominator) {
            numerator += other.numerator;
        }
        else {
            const std::uint32_t _common_denominator{ _lcm(denominator, other.denominator) };
            const std::uint32_t _numerator{
                numerator * (_common_denominator / denominator) +
                other.numerator * (_common_denominator / other.denominator)
            };
            const std::uint32_t gcd{ _gcd(_numerator, _common_denominator) };

            numerator = _numerator / gcd;
            denominator = _common_denominator / gcd;
        }

        return *this;
    }

    //---------------------------------------------------------
    std::uint32_t SecondFraction::_gcd(std::uint32_t a, std::uint32_t b) noexcept
    {
        while (b != 0) {
            std::uint32_t tmp{ b };
            b = a % b;
            a = tmp;
        }
        return a;
    }

    //---------------------------------------------------------
    std::uint32_t SecondFraction::_lcm(std::uint32_t a, std::uint32_t b) noexcept
    {
        return (a / _gcd(a, b)) * b;
    }


    //=====   Time Scores   ===================================
    //---------------------------------------------------------
    Time::Time(const std::uint16_t h, const std::uint8_t m, const std::uint8_t s, const SecondFraction frac) noexcept
    {
        _evaluate_data(h, m, s, frac);
    }

    //---------------------------------------------------------
    Time::Time(const std::uint16_t h, const std::uint8_t m, const std::uint8_t s) noexcept
    {
        _evaluate_data(h, m, s);
    }

    //---------------------------------------------------------
    Time::Time(const std::uint8_t m, const std::uint8_t s, const SecondFraction frac) noexcept
    {
        _evaluate_data(0, m, s, frac);
    }

    //---------------------------------------------------------
    Time::Time(const std::uint8_t m, const std::uint8_t s) noexcept
    {
        _evaluate_data(0, m, s);
    }

    //---------------------------------------------------------
    Time::Time(const std::uint8_t s, const SecondFraction frac) noexcept
    {
        _evaluate_data(0, 0, s, frac);
    }

    //---------------------------------------------------------
    Time::Time(const unsigned int s) noexcept
    {
        if (s < 60)
            _evaluate_data(0, 0, s);
        else
            _evaluate_data(s / 3600, (s % 3600) / 60, s % 60);
    }

    //---------------------------------------------------------
    Time::Time(const double time, const int precision) noexcept
    {
        double _int_part, _frac_part;
        _frac_part = std::modf(time, &_int_part);

        _seconds = static_cast<unsigned int>(_int_part);
        if (precision == 0) {
            _fraction = SecondFraction{ 0, 1 };
        }
        else {
            _fraction = SecondFraction{
                static_cast<std::uint16_t>(precision * _frac_part),
                static_cast<std::uint16_t>(precision)
            };
        }  
    }

    //---------------------------------------------------------
    Time& Time::operator+= (const Time& other) noexcept
    {
        // Notice: this is a special operation:
        // fractions of seconds are just added together, and are not added to seconds.
        _seconds  += other._seconds;
        _fraction += other._fraction;

        return *this;
    }

    //---------------------------------------------------------
    Time& Time::operator-= (const Time& other) noexcept
    {
        // Notice: substaction of times involves only seconds and not fractions of
        if (other._seconds <= _seconds)
            _seconds -= other._seconds;
        else {
            _seconds = 0;
            _error_msg = std::format("can't substract {} seconds from {} seconds, result set to 0", other._seconds, _seconds);
        }

        return *this;
    }

    //---------------------------------------------------------
    Time::operator long() noexcept
    {
        if (_seconds <= std::numeric_limits<long>::max()) {
            return static_cast<long>(_seconds);
        }
        else {
            _error_msg = std::format(
                "{} is a too big time value to be converted to a long integer, forced to {}",
                _seconds,
                std::numeric_limits<long>::max()
            );
            return std::numeric_limits<long>::max();
        }
    }

    //---------------------------------------------------------
    Time::operator unsigned long() const noexcept
    {
        return static_cast<unsigned long>(_seconds);
    }

    //---------------------------------------------------------
    Time::operator double() const noexcept
    {
        return static_cast<double>(_seconds) + static_cast<double>(_fraction);
    }

    //---------------------------------------------------------
    Time::operator std::string() const noexcept
    {
        const std::uint16_t h{ static_cast<std::uint16_t>(_seconds / 3600) };
        const std::uint8_t  m{ static_cast<std::uint8_t>((_seconds % 3600) / 60) };
        const std::uint8_t  s{ static_cast<std::uint8_t>(_seconds % 60 )};
        const std::string   frac{ std::string(_fraction) };

        if (h > 0)
            return std::format("{}:{:02d}:{:02d}{}", h, m, s, frac);
        
        if (m > 0)
            return std::format("{:2d}:{:02d}{}", m, s, frac);

        return std::format("{:2d}{}", s, frac);
    }

    //---------------------------------------------------------
    Time Time::operator+ (const Time& other) noexcept
    {
        Time t{ *this };
        return t += other;
    }

    //---------------------------------------------------------
    Time Time::operator- (const Time& other) noexcept
    {
        Time t{ *this };
        return t -= other;
    }

    //---------------------------------------------------------
    const bool Time::is_ok() const noexcept
    {
        return _error_msg.empty();
    }

    //---------------------------------------------------------
    const std::string& Time::get_error_message() const noexcept
    {
        return _error_msg;
    }

    //-----------------------------------------------------
    void Time::_evaluate_data(const std::uint16_t h, const std::uint8_t m, const std::uint8_t s, const SecondFraction frac) noexcept
    {
        if (m >= 60) {
            _error_msg = std::format("bad value for minutes: {}", m);
        }
        else if (s >= 60) {
            _error_msg = std::format("bad value for seconds: {}", s);
        }
        else {
            _seconds = 3600 * h + 60 * m + s;
            _fraction = frac;
            _error_msg.clear();
        }
    }
    
    //-----------------------------------------------------
    void Time::_evaluate_data(const std::uint16_t h, const std::uint8_t m, const std::uint8_t s) noexcept
    {
        _evaluate_data(h, m, s, SecondFraction{});
    }

}