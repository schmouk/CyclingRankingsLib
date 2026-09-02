"""
This file is part of project CyclingRankingsLibrary
(https://github.com/schmouk/CyclingRankingsLib)

copyright (c) 2026 Philippe Schmouker

License is GNU GENERAL PUBLIC LICENSE Version 3
(full text available at:
 https://github.com/schmouk/CyclingRankingsLib?tab=GPL-3.0-1-ov-file)
"""

from typing import TypeAlias

# ===== Identifiers Types =============================
RiderId: TypeAlias = int  # std::uint32_t
TeamId: TypeAlias = int  # std::uint32_t

StageId: TypeAlias = int  # std::uint32_t
KomId: TypeAlias = int  # std::uint32_t
SprintId: TypeAlias = int  # std::uint32_t


# ===== Categories ====================================
KomCategory: TypeAlias = int  # std::uint8_t


# ===== Ranks =========================================
RankId: TypeAlias = int  # std::int32_t
# ----- specific values -------------------------------
RANK_DNS: int = 0
RANK_DNF: int = -1
RANK_DSQ: int = -2


# ===== Points Scores =================================
PointScore: TypeAlias = int  # std::int16_t
BoardGameScore: TypeAlias = int  # std::int16_t
