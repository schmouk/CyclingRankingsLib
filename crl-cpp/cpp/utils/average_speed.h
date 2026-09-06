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

#include <string>

#include "../commons/time.h"


namespace crl
{
    //=====   Average Speed Evaluations   =====================
    // Evaluation of average speeds with an adaptative precision
    class AverageSpeed final
    {
    public:
        //-----   Constructors / Destructor   -----------------
        AverageSpeed(const double dist, const Time& time) noexcept;
        explicit AverageSpeed(const double dist, crl::HMTime& hm_time) noexcept;

        AverageSpeed() noexcept = default;
        ~AverageSpeed() noexcept = default;

        AverageSpeed(const AverageSpeed&) noexcept = default;
        AverageSpeed(AverageSpeed&&) noexcept = default;

        AverageSpeed& operator=(const AverageSpeed&) noexcept = default;
        AverageSpeed& operator=(AverageSpeed&&) noexcept = default;

        //-----   Evaluating speeds with adaptive precision   -----
        template<crl::type_type TimeT>
        static const std::string evaluate(const double dist, const TimeT& time) noexcept
        {
            return _evaluate_speed(
                dist,
                time,
                time.get_precision()  // Notice: precision never equals 0
            );
        }

        template<>
        static const std::string evaluate(const double dist, const crl::HMTime& hm_time) noexcept
        {
            return _evaluate_speed(
                dist,
                hm_time,
                1.0 / 60.0  // Notice: precision is 1 minute
            );
        }

        template<crl::type_type TimeT>
        const std::string get(const double dist, TimeT& time) noexcept
        {
            return _speed_str = AverageSpeed::evaluate(dist, time);
        }

        template<>
        const std::string get(const double dist, crl::HMTime& hm_time) noexcept
        {
            return _speed_str = AverageSpeed::evaluate(dist, hm_time);
        }

        //-----   Accessor   ----------------------------------
        const std::string get() noexcept;


    private:
        //-----------------------------------------------------
        std::string _speed_str{ "unevaluated speed" };

        //-----------------------------------------------------
        static const std::string _evaluate_speed(
            const double dist,
            const double seconds,
            const double time_precision
        ) noexcept;
    };


    /** /
    //=====   Local Implementations   =========================
    //---------------------------------------------------------
    template<crl::type_type TimeT>
    const std::string AverageSpeed::evaluate(const double dist, TimeT& time) noexcept
    {
        return _evaluate_speed(
            dist,
            time,
            1.0 / time.get_precision()  // Notice: precision never equals 0
        );
    }

    //---------------------------------------------------------
    template<>
    const std::string AverageSpeed::evaluate(const double dist, crl::HMTime& time) noexcept
    {
        return _evaluate_speed(
            dist,
            time,
            1.0 / 60.0  // Notice: precision is 1 minute
        );
    }

    //---------------------------------------------------------
    template<crl::type_type TimeT>
    const std::string AverageSpeed::get(const double dist, TimeT& time) noexcept
    {
        return _speed_str = AverageSpeed::evaluate(dist, time);
    }

    //---------------------------------------------------------
    template<>
    const std::string AverageSpeed::get(const double dist, crl::HMTime& hm_time) noexcept
    {
        return _speed_str = AverageSpeed::evaluate(dist, hm_time);
    }
    /**/
}
