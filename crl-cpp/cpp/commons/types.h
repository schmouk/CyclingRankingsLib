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
    using PointScore = std::int16_t;

}
