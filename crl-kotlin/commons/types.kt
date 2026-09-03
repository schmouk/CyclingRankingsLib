package com.github.schmouk.cyclingrankingslib

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
