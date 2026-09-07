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

#include <cmath>
#include <format>
#include <string>

#include "./average_speed.h"
#include "../commons/time.h"


namespace crl
{
    //=====   Average Speed Evaluations   =====================
    //---------------------------------------------------------
    AverageSpeed::AverageSpeed(const double dist, const Time& time) noexcept
        : _speed_str{ AverageSpeed::evaluate(dist, time) }
    {}

    //---------------------------------------------------------
    AverageSpeed::AverageSpeed(const double dist, crl::HMTime& hm_time) noexcept
        : _speed_str{ AverageSpeed::evaluate(dist, hm_time) }
    {}

    //---------------------------------------------------------
    const std::string AverageSpeed::get() noexcept
    {
        return _speed_str;
    }

    //---------------------------------------------------------
    const std::string AverageSpeed::_evaluate_speed(
        const double dist,
        const double seconds,
        const double time_precision
    ) noexcept
    {
        // Algo 2
        const double ref_time{ seconds / time_precision };
        const double ref_avg_speed{ 3600.0 * dist / seconds };

        int final_precision{ 0 };
        int coeff_mult{ 1 };
        double inv_coeff_mult{ 1.0 };

        do {
            const double test_speed_up{ static_cast<long>((ref_avg_speed + inv_coeff_mult) * coeff_mult) * inv_coeff_mult };
            const double test_speed_down{ static_cast<long>((ref_avg_speed - inv_coeff_mult) * coeff_mult) * inv_coeff_mult };

            const double test_time_up{ 3600.0 * dist / time_precision / test_speed_up };
            if (ref_time - test_time_up < 1.0 / time_precision / time_precision) {
                const double test_time_down{ 3600.0 * dist / time_precision / test_speed_down };
                if (test_time_down - ref_time < 1.0 / time_precision / time_precision)
                    break;
            }

            ++final_precision;
            coeff_mult *= 10;
            inv_coeff_mult /= 10.0;
        } while (final_precision < 3);

        switch (final_precision) {
        case 0:
            return std::format("{}", static_cast<long>(3600 * dist / seconds));
        case 1:
            return std::format("{:.1f}", std::round(3600.0 * 10.0 * dist / seconds) / 10.0);
        case 2:
            return std::format("{:.2f}", std::round(3600.0 * 100.0 * dist / seconds) / 100.0);
        default:
            return std::format("{:.3f}", std::round(3600.0 * 1000.0 * dist / seconds) / 1000.0);
        }
    }

}
