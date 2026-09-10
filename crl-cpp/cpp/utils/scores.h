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

#include <concepts>
#include <cstdint>

#include "../commons/time.h"
#include "../commons/types.h"


namespace crl
{
    //=====   Scores Base   ===================================
    struct ScoreBase {};


    //=====   Time Scores   ===================================
    struct TimeScore : crl::Time, ScoreBase
    {
        using crl::Time::Time;
    };


    //=====   Time+Ranks Scores   =============================
    // Mainly used for time general classifications
    struct TimeRanksScore : ScoreBase
    {
        crl::Time     time{};
        std::uint16_t ranks_sum{ 0 };

        bool operator< (const TimeRanksScore& other) const noexcept {
            return time < other.time || (time == other.time && ranks_sum < other.ranks_sum);
        }

        bool operator== (const TimeRanksScore& other) const noexcept {
            return time == other.time && ranks_sum == other.ranks_sum;
        }
    };


    //=====   Points Scores   =================================
    struct PointsScore : ScoreBase
    {
        crl::Points points{ 0 };

        explicit inline PointsScore(const crl::Points pts) noexcept
            : points{ pts }
        {}

        inline PointsScore(const int pts) noexcept
            : points{ static_cast<crl::Points>(pts) }
        {}

        PointsScore() noexcept = default;
        PointsScore(const PointsScore&) noexcept = default;
        PointsScore(PointsScore&&) noexcept = default;

        PointsScore& operator= (const PointsScore&) noexcept = default;
        PointsScore& operator= (PointsScore&&) noexcept = default;

        virtual ~PointsScore() noexcept = default;

        bool operator< (const PointsScore& other) const noexcept {
            return points > other.points;  // Notice: sic!
        }

        bool operator== (const PointsScore& other) const noexcept {
            return points == other.points;
        }

        bool operator!= (const PointsScore& other) const noexcept {
            return points != other.points;
        }
    };


    //=====   Points+Ranks Scores   ===========================
    // Mainly used for points general classifications
    struct PointsRanksScore : ScoreBase
    {
        crl::RankId   best_rank{ static_cast<crl::RankId>(std::numeric_limits<std::int32_t>::max()) };
        crl::StageId  last_stage_best_rank{ 0 };
        crl::Points   points{ 0 };
        std::uint8_t  best_rank_count{ 0 };

        bool operator< (const PointsRanksScore& other) const noexcept;
        bool operator== (const PointsRanksScore& other) const noexcept;
    };


    //=====   Points KOM Scores   =============================
    // Mainly used for points general classifications
    struct PointsKOMScore : ScoreBase
    {
        crl::KomId       last_kom_id_win{ 0 };
        crl::Points      points{ 0 };
        crl::KomCategory best_kom_cat_win{ static_cast<crl::KomCategory>(std::numeric_limits<std::uint8_t>::max()) };
        std::uint8_t     best_kom_wins_count{ 0 };

        bool operator< (const PointsKOMScore& other) const noexcept;
        bool operator== (const PointsKOMScore& other) const noexcept;
    };


    //=====   Concepts   ======================================
    template<typename ScoreT>
    concept score = std::derived_from<ScoreT, crl::ScoreBase>;

}
