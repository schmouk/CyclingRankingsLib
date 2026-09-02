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

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <format>
#include <limits>
#include <regex>
#include <string>

#include "./time.h"


namespace crl
{
    //=====   Time Fraction of Seconds   ======================
    //---------------------------------------------------------
    SecondFraction::operator double() const noexcept
    {
        try {
            return static_cast<double>(value) / static_cast<double>(precision);
        }
        catch (...) {
            return 0.0;
        }
    }

    //---------------------------------------------------------
    SecondFraction::operator std::string() const noexcept
    {
        switch (precision) {
        case 0: case 1:
            return "";
        case 10:
            return std::format(".{}", value);
        case 100:
            return std::format(".{:02d}", value);
        case 1000:
            return std::format(".{:03d}", value);

        default:
            return std::format(" {}/{}", value, precision);
        }
    }

    //---------------------------------------------------------
    SecondFraction& SecondFraction::operator+= (const SecondFraction& other) noexcept
    {
        if (precision == other.precision) {
            value += other.value;
        }
        else {
            const std::uint32_t _common_denominator{ _lcm(precision, other.precision) };
            const std::uint32_t _numerator{
                value * (_common_denominator / precision) +
                other.value * (_common_denominator / other.precision)
            };
            const std::uint32_t gcd{ _gcd(_numerator, _common_denominator) };

            value = _numerator / gcd;
            precision = _common_denominator / gcd;
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
    Time::Time(const std::uint16_t h, const std::uint8_t m, const std::uint8_t s, const std::uint16_t frac_val, const std::uint16_t frac_prec) noexcept
    {
        _evaluate_data(h, m, s, SecondFraction{ frac_val, frac_prec });
    }

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
    Time::Time(const std::uint8_t m, const std::uint8_t s, const std::uint16_t frac_val, const std::uint16_t frac_prec) noexcept
    {
        _evaluate_data(0, m, s, SecondFraction{ frac_val, frac_prec });
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
    Time::Time(const std::uint8_t s, const std::uint16_t frac_val, const std::uint16_t frac_prec) noexcept
    {
        _evaluate_data(0, 0, s, SecondFraction{ frac_val, frac_prec });
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
    Time::Time(const std::string& time) noexcept
    {
        _evaluate_time(time);
    }

    //---------------------------------------------------------
    Time::Time(const char* time) noexcept
    {
        if (time)
            _evaluate_time(std::string(time));
    }

    //---------------------------------------------------------
    Time& Time::operator= (std::string& time) noexcept
    {
        return *this = Time(time);
    }

    //---------------------------------------------------------
    Time& Time::operator= (const char* time) noexcept
    {
        return *this = Time(time);
    }

    //---------------------------------------------------------
    Time& Time::operator+= (const Time& other) noexcept
    {
        if (!is_ok()) {
            _error_msg = std::format("attempt to add an erroneous time ({}) -> no addition", _error_msg);
        }
        else if (other.is_ok()) {
            // Notice: this is a special operation:
            // fractions of seconds are just added together, and are not added to seconds.
            _seconds += other._seconds;
            _error_msg.clear();
            _fraction += other._fraction;
        }
        else {
            _error_msg = std::format("attempt to add an erroneous time ({}) -> no addition", other._error_msg);
        }

        return *this;
    }

    //---------------------------------------------------------
    Time& Time::operator-= (const Time& other) noexcept
    {
        if (!is_ok()) {
            _error_msg = std::format("attempt to add an erroneous time ({}) -> no substraction", _error_msg);
        }
        else if (other.is_ok()) {
            // Notice: substraction of times involves only seconds and not fractions of
            if (other._seconds <= _seconds) {
                _error_msg.clear();
                _seconds -= other._seconds;
            }
            else {
                _error_msg = std::format("can't substract {} seconds from {} seconds, result set to 0", other._seconds, _seconds);
                _seconds = 0;
            }
        }
        else {
            _error_msg = std::format("attempt to substract an erroneous time ({}) -> no substraction", other._error_msg);
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
        if (!is_ok())
            return _error_msg;

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
        if (!is_ok())
            return *this;

        if (!other.is_ok())
            return other;

        // evaluates gaps, always gets a positive value
        const double t1{ static_cast<double>(*this) };
        const double t2{ static_cast<double>(other) };

        const int final_precision{ std::max<int>(_fraction.precision, other._fraction.precision) };

        if (t1 <= t2)
            return Time(t2 - t1, final_precision);
        else
            return Time(t1 - t2, final_precision);
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

    //-----------------------------------------------------
    void Time::_evaluate_frac(const std::string& frac_str) noexcept
    {
        if (frac_str.empty()) {
            _fraction = { 0, 1 };
            return;
        }

        const std::uint16_t frac_value{ static_cast<std::uint16_t>(std::stoi(frac_str)) };
        switch (frac_str.size()) {
        case 1:
            _fraction = { frac_value, 10 };
            break;
        case 2:
            _fraction = { frac_value, 100 };
            break;
        case 3:
            _fraction = { frac_value, 1000 };
            break;
        default:
            _fraction = { static_cast<std::uint16_t>(std::stoi(frac_str.substr(0, 3))), 1000 };
            break;
        }
    }

    //-----------------------------------------------------
    void Time::_evaluate_frac(const std::string& frac_value, const std::string& frac_precision) noexcept
    {
        _fraction.value = std::stoi(frac_value);
        _fraction.precision = std::stoi(frac_precision);
        if (_fraction.precision == 0)
            _error_msg = "bad value for precision on fractions of seconds: 0";
    }

    //-----------------------------------------------------
    void Time::_evaluate_time(const std::string& time_str) noexcept
    {
        std::smatch time_matches;

        // HHH:MM:SS frac_val/frac_precision
        if (std::regex_search(time_str, time_matches,
                std::regex("^(\\d\\d*):(\\d\\d):(\\d\\d) (\\d+)/(\\d+)")))
        {
            // Match h, m, s and fraction!
            const std::int32_t h{ std::stol(time_matches[1]) };
            const std::int32_t m{ std::stol(time_matches[2]) };
            const std::int32_t s{ std::stol(time_matches[3]) };
            _seconds = 3600 * h + 60 * m + s;

            _evaluate_frac(time_matches[4], time_matches[5]);
            return;
        }

        // HHH:MM:SS.frac
        if (std::regex_search(time_str, time_matches,
                std::regex("^(\\d\\d*):(\\d\\d):(\\d\\d)(\\.(\\d+))?")))
        {
            // Match h, m, s and maybe fraction!
            const std::int32_t h{ std::stol(time_matches[1]) };
            const std::int32_t m{ std::stol(time_matches[2]) };
            const std::int32_t s{ std::stol(time_matches[3]) };
            _seconds = 3600 * h + 60 * m + s;

            _evaluate_frac(time_matches[5]);
            return;
        }

        // MM:SS frac_val/frac_precision
        if (std::regex_search(time_str, time_matches,
                std::regex("^(\\d\\d?):(\\d\\d) (\\d+)/(\\d+)")))
        {
            // Match m, s and fraction!
            const std::int32_t m{ std::stol(time_matches[1]) };
            const std::int32_t s{ std::stol(time_matches[2]) };
            _seconds = 60 * m + s;

            _evaluate_frac(time_matches[3], time_matches[4]);
            return;
        }

        // MM:SS.frac
        if (std::regex_search(time_str, time_matches,
            std::regex("^(\\d\\d*):(\\d\\d)(\\.(\\d+))?")))
        {
            // Match m, s and maybe fraction!
            const std::int32_t m{ std::stol(time_matches[1]) };
            const std::int32_t s{ std::stol(time_matches[2]) };
            _seconds = 60 * m + s;

            _evaluate_frac(time_matches[4]);
            return;
        }

        // SS frac_val/frac_precision
        if (std::regex_search(time_str, time_matches,
            std::regex("^(\\d\\d?) (\\d+)/(\\d+)")))
        {
            // Match s and fraction!
            _seconds = std::stol(time_matches[1]);
            _evaluate_frac(time_matches[2], time_matches[3]);
            return;
        }

        // SS.frac
        if (std::regex_search(time_str, time_matches,
            std::regex("^(\\d\\d?)(\\.(\\d+))?")))
        {
            // Match s and maybe fraction!
            _seconds = std::stol(time_matches[1]);
            _evaluate_frac(time_matches[3]);
            return;
        }

    }

}