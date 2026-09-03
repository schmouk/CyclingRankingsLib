"""
This file is part of project CyclingRankingsLibrary
(https://github.com/schmouk/CyclingRankingsLib)

Copyright (C) 2026 Philippe Schmouker

License is GNU GENERAL PUBLIC LICENSE Version 3

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program (see:
https://github.com/schmouk/CyclingRankingsLib/blob/main/LICENSE).
If not, see <https://www.gnu.org/licenses/>.
"""

#=====   Identifiers Types   ==================================
type RiderId = int  # std::uint32_t
type TeamId = int  # std::uint32_t

type StageId = int  # std::uint32_t
type KomId = int  # std::uint32_t
type SprintId = int  # std::uint32_t


#=====   Categories   =========================================
type KomCategory = int  # std::uint8_t


#=====   Ranks   ==============================================
type RankId = int  # std::int32_t
#-----   specific values   ------------------------------------
RANK_DNS: RankId = 0
RANK_DNF: RankId = -1
RANK_DSQ: RankId = -2


#=====   Points Scores   ======================================
type PointScore = int  # std::int16_t
type BoardGameScore = int  # std::int16_t
