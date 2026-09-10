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
#include <string>

#include "./scores.h"


namespace crl
{
    //=====   Competitor Descr   ==============================
    template<crl::score ScoreT>
    struct CompetitorDescr : ScoreT
    {
        crl::Bib ident{ 0 };
        ScoreT   score{};

        inline CompetitorDescr() noexcept {};

        explicit inline CompetitorDescr(const crl::Bib id, const ScoreT& score) noexcept
            : ident{ id }, score{ score }
        {}

        inline CompetitorDescr(const int id, const ScoreT& score) noexcept
            : ident{ static_cast<crl::Bib>(id) }, score{ score }
        {}

        ~CompetitorDescr() noexcept = default;

        CompetitorDescr(const CompetitorDescr&) noexcept = default;
        CompetitorDescr(CompetitorDescr&&) noexcept = default;

        CompetitorDescr& operator= (const CompetitorDescr&) noexcept = default;
        CompetitorDescr& operator= (CompetitorDescr&&) noexcept = default;


        bool operator< (const CompetitorDescr& other) const noexcept {
            return score < other.score;
        }

    };


    //=====   Specializations   ===============================   
    using PointsCompetitorDescr = CompetitorDescr<crl::PointsScore>;

}
