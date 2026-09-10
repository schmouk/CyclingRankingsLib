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

#include "./scores.h"

#include "../commons/time.h"


namespace crl
{
    //=====   Points+Ranks Scores   ===========================
    //---------------------------------------------------------
    bool PointsRanksScore::operator< (const PointsRanksScore& other) const noexcept
    {
        // greater points wins
        if (points != other.points)
            return points > other.points;
        
        // if equal, the best rank ever wins
        if (best_rank != other.best_rank)
            return best_rank < other.best_rank;

        // if equal, the greater best-ranks count wins
        if (best_rank_count != other.best_rank_count)
            return best_rank_count > other.best_rank_count;

        // everything else being equal, the more recent best-rank wins
        return last_stage_best_rank > other.last_stage_best_rank;
    }

    //---------------------------------------------------------
    bool PointsRanksScore::operator== (const PointsRanksScore& other) const noexcept
    {
        return points == other.points &&
            best_rank == other.best_rank &&
            best_rank_count == other.best_rank_count &&
            last_stage_best_rank == other.last_stage_best_rank;
    }


    //=====   Points KOM Scores   =============================
    //---------------------------------------------------------
    bool PointsKOMScore::operator< (const PointsKOMScore& other) const noexcept
    {
        // greater points wins
        if (points != other.points)
            return points > other.points;

        // if equal, the best KOM category win wins
        if (best_kom_cat_win != other.best_kom_cat_win)
            return best_kom_cat_win < other.best_kom_cat_win;

        // if equal, the greater wins count wins
        if (best_kom_wins_count != other.best_kom_wins_count)
            return best_kom_wins_count > other.best_kom_wins_count;

        // everythong else bein equal, the more recent KOM win wins
        return last_kom_id_win > other.last_kom_id_win;
    }

    //---------------------------------------------------------
    bool PointsKOMScore::operator== (const PointsKOMScore& other) const noexcept
    {
        return points == other.points &&
            best_kom_cat_win == other.best_kom_cat_win &&
            best_kom_wins_count == other.best_kom_wins_count &&
            last_kom_id_win == other.last_kom_id_win;
    }

}
