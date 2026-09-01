#pragma once

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

#include <cstdint>
#include <string>
#include <string_view>


namespace crl
{
    //=====   Time Fraction of Seconds   ======================
    class SecondFraction
    {
    public:
        std::uint16_t numerator;    // Notice: numerator may be greater than denominator (in a few circumstances)
        std::uint16_t denominator;  // Notice: mostly frequent values are 5, 10, 100 or 1000

        operator double() const noexcept;
        operator std::string() const noexcept;

        SecondFraction& operator+= (const SecondFraction&) noexcept;


    private:
        static std::uint32_t _gcd(std::uint32_t a, std::uint32_t b) noexcept;
        static std::uint32_t _lcm(std::uint32_t a, std::uint32_t b) noexcept;

    };


    //=====   Time Scores   ===================================
    class Time
    {
    public:
        //-----   Constructors / Destructor   -----------------
        Time(const std::uint16_t h, const std::uint8_t m, const std::uint8_t s, const SecondFraction frac) noexcept;
        Time(const std::uint16_t h, const std::uint8_t m, const std::uint8_t s) noexcept;
        Time(const std::uint8_t  m, const std::uint8_t s, const SecondFraction frac) noexcept;
        Time(const std::uint8_t  m, const std::uint8_t s) noexcept;
        Time(const std::uint8_t  s, const SecondFraction frac) noexcept;
        explicit Time(const unsigned int s) noexcept;

        explicit Time(const double time, const int precision = 0) noexcept;  // mostly used values for precision: 0, 5, 10, 100, 1000

        explicit Time(std::string_view time) noexcept;

        virtual ~Time() noexcept = default;

        Time(const Time&) noexcept = default;
        Time(Time&&) noexcept = default;


        //-----   Assignment operators   ----------------------
        Time& operator= (const Time&) noexcept = default;
        Time& operator= (Time&&) noexcept = default;

        Time& operator+= (const Time& other) noexcept;
        Time& operator-= (const Time& other) noexcept;


        //-----   Operators   ---------------------------------
        operator long() noexcept;
        operator unsigned long() const noexcept;
        operator double() const noexcept;
        operator std::string() const noexcept;

        Time operator+ (const Time& other) noexcept;
        Time operator- (const Time& other) noexcept;


        //-----   Operations   --------------------------------
        const bool is_ok() const noexcept;
        const std::string& get_error_message() const noexcept;


    private:
        //-----------------------------------------------------
        std::int32_t   _seconds{ 0 };
        SecondFraction _fraction{ 0, 1 };
        std::string    _error_msg{};

        //-----------------------------------------------------
        void _evaluate_data(const std::uint16_t h, const std::uint8_t m, const std::uint8_t s, const SecondFraction frac) noexcept;
        void _evaluate_data(const std::uint16_t h, const std::uint8_t m, const std::uint8_t s) noexcept;

    };

}
