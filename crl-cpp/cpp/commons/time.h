#pragma once

/**
* This file is part of project CyclingRankingsLibrary
* (https://github.com/schmouk/CyclingRankingsLib)
*
* Copyright (C) 2026 Philippe Schmouker
*
* License is GNU GENERAL PUBLIC LICENSE Version 3
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program (see:
* https://github.com/schmouk/CyclingRankingsLib/blob/main/LICENSE).
* If not, see <https://www.gnu.org/licenses/>.
*/

#include <cstdint>
#include <string>


namespace crl
{
    /** This file declares next classes:
    *
    * class SecondFraction; // Fractions of seconds coded as a fractional value : numerator (value) and denominator (precision).
    * class Time;     // Base class for all time scores classes.
    * 
    * class HMSTime;  // Time scores set as "<hours>::<minutes>::<seconds>[.<frac>]".
    * class HMTime;   // Time scores set as "<hours>::<minutes>" - i.e. no seconds.
    * class MSTime;   // Time scores set as "<minutes>::<seconds>[.<frac>]".
    * class STime;    // Time scores set as "<seconds>[.<frac>]".
    /**/ 


    //=====   Time Fraction of Seconds   ======================
    class SecondFraction
    {
    public:
        std::uint16_t value;      // Notice: value may be greater than precision (in a few circumstances)
        std::uint16_t precision;  // Notice: mostly frequent precision values are 5, 10, 100 or 1000 (-th of second)

        operator double() const noexcept;
        operator std::string() const noexcept;

        SecondFraction& operator+= (const SecondFraction&) noexcept;

        const bool operator<(const SecondFraction& other) const noexcept;
        const bool operator==(const SecondFraction& other) const noexcept;


    private:
        static std::uint32_t _gcd(std::uint32_t a, std::uint32_t b) noexcept;
        static std::uint32_t _lcm(std::uint32_t a, std::uint32_t b) noexcept;

    };


    //=====   Time Scores   ===================================
    class Time
    {
    public:
        //-----   Constructors / Destructor   -----------------
        Time(const std::uint16_t h, const std::uint8_t m, const std::uint8_t s, const std::uint16_t frac_val, const std::uint16_t frac_prec) noexcept;
        Time(const std::uint16_t h, const std::uint8_t m, const std::uint8_t s, const SecondFraction frac) noexcept;
        Time(const std::uint16_t h, const std::uint8_t m, const std::uint8_t s) noexcept;

        explicit Time(const double time, const int precision = 0) noexcept;  // mostly used values for precision: 0, 5, 10, 100, 1000

        explicit Time(const std::string& time) noexcept;
        explicit Time(const char* time) noexcept;

        Time() noexcept = default;
        virtual ~Time() noexcept = default;

        Time(const Time&) noexcept = default;
        Time(Time&&) noexcept = default;


        //-----   Assignment operators   ----------------------
        Time& operator= (const Time&) noexcept = default;
        Time& operator= (Time&&) noexcept = default;

        Time& operator= (std::string& time) noexcept;
        Time& operator= (const char* time) noexcept;

        Time& operator+= (const Time& other) noexcept;
        Time& operator-= (const Time& other) noexcept;


        //-----   Operators   ---------------------------------
        operator long() noexcept;
        operator unsigned long() const noexcept;
        operator double() const noexcept;
        operator std::string() const noexcept;

        Time operator+ (const Time& other) noexcept;
        Time operator- (const Time& other) noexcept;  // Notice: evaluates gaps

        const bool operator==(const Time& other) const noexcept;
        const bool operator!=(const Time& other) const noexcept;
        const bool operator<(const Time& other) const noexcept;
        const bool operator<=(const Time& other) const noexcept;
        const bool operator>(const Time& other) const noexcept;
        const bool operator>=(const Time& other) const noexcept;


        //-----   Operations   --------------------------------
        const bool is_ok() const noexcept;
        const std::string& get_error_message() const noexcept;


    protected:
        //-----------------------------------------------------
        std::int32_t   _seconds{ 0 };
        SecondFraction _fraction{ 0, 1 };
        std::string    _error_msg{};

        //-----------------------------------------------------
        virtual void _evaluate_time(const std::string& time_str) noexcept;

        const bool _evaluate_hms_ratio(const std::string& time_str) noexcept;
        const bool _evaluate_hms_frac(const std::string& time_str) noexcept;
        const bool _evaluate_ms_ratio(const std::string& time_str) noexcept;
        const bool _evaluate_ms_frac(const std::string& time_str) noexcept;
        const bool _evaluate_s_ratio(const std::string& time_str) noexcept;
        const bool _evaluate_s_frac(const std::string& time_str) noexcept;


    private:
        //-----------------------------------------------------
        void _evaluate_data(const std::uint16_t h, const std::uint8_t m, const std::uint8_t s, const SecondFraction frac) noexcept;
        void _evaluate_data(const std::uint16_t h, const std::uint8_t m, const std::uint8_t s) noexcept;
        void _evaluate_frac(const std::string& frac_str) noexcept;
        void _evaluate_frac(const std::string& frac_value, const std::string& frac_precision) noexcept;

    };


    //=====   HMS Time Scores   ===============================
    using HMSTime = Time;


    //=====   HMTime   ========================================
    class HMTime : public Time
    {
    public:
        HMTime(const std::uint16_t h, const std::uint8_t m) noexcept;

        explicit HMTime(const std::string& time) noexcept;
        explicit HMTime(const char* time) noexcept;

        virtual ~HMTime() noexcept = default;

        HMTime(const HMTime&) noexcept = default;
        HMTime(HMTime&&) noexcept = default;


    protected:
        void _evaluate_time(const std::string& str) noexcept override;

    };


    //=====   MSTime   ========================================
    class MSTime : public Time
    {
    public:
        MSTime(const std::uint8_t m, const std::uint8_t s, const std::uint16_t frac_val, const std::uint16_t frac_prec) noexcept;
        MSTime(const std::uint8_t m, const std::uint8_t s, const SecondFraction frac) noexcept;
        MSTime(const std::uint8_t m, const std::uint8_t s) noexcept;

        explicit MSTime(const double time, const int precision = 0) noexcept;  // mostly used values for precision: 0, 5, 10, 100, 1000

        explicit MSTime(const std::string& time) noexcept;
        explicit MSTime(const char* time) noexcept;

        virtual ~MSTime() noexcept = default;

        MSTime(const MSTime&) noexcept = default;
        MSTime(MSTime&&) noexcept = default;


    protected:
        void _evaluate_time(const std::string& str) noexcept override;

    };


    //=====   STime   =========================================
    class STime : public Time
    {
    public:
        STime(const std::uint8_t s, const std::uint16_t frac_val, const std::uint16_t frac_prec) noexcept;
        STime(const std::uint8_t s, const SecondFraction frac) noexcept;
        STime(const unsigned int s) noexcept;

        explicit STime(const double time, const int precision = 0) noexcept;  // mostly used values for precision: 0, 5, 10, 100, 1000

        explicit STime(const std::string& time) noexcept;
        explicit STime(const char* time) noexcept;

        virtual ~STime() noexcept = default;

        STime(const STime&) noexcept = default;
        STime(STime&&) noexcept = default;


    protected:
        void _evaluate_time(const std::string& str) noexcept override;

    };

}
