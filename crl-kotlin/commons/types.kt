package com.github.schmouk.cyclingrankingslib

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

//=====   Identifiers Types   =============================
typealias RiderId = UInt
typealias TeamId = UInt

typealias StageId = UInt
typealias KomId = UInt
typealias SprintId = UInt


//=====   Categories   ====================================
typealias KomCategory = UByte


//=====   Ranks   =========================================
typealias RankId = Int
const val RANK_DNS: RankId = 0
const val RANK_DNF: RankId = -1
const val RANK_DSQ: RankId = -2


//=====   Points Scores   =================================
typealias PointScore = Short
typealias BoardGameScore = Short
