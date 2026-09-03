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


namespace crl
{
    //=====   Identifiers Types   =============================
    using RiderId  = std::uint32_t;
    using TeamId   = std::uint32_t;

    using StageId  = std::uint32_t;
    using KomId    = std::uint32_t;
    using SprintId = std::uint32_t;


    //=====   Categories   ====================================
    using KomCategory = std::uint8_t;


    //=====   Ranks   =========================================
    using RankId = std::int32_t;
    //-----   specific values   -------------------------------
    constexpr RankId RANK_DNS =  0;
    constexpr RankId RANK_DNF = -1;
    constexpr RankId RANK_DSQ = -2;


    //=====   Points Scores   =================================
    using PointScore     = std::int16_t;
    using BoardGameScore = std::int16_t;

}
