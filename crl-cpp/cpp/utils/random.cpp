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

/*
* Notice: this implemetation is directly inspired by the code of
* library CppRandLib (see https://github.com/schmouk/CppRandLib)
* which is provided there under the MIT License.
*/


#include <chrono>
#include <cstdint>

#include "./random.h"


namespace crl
{
    //---------------------------------------------------------
    Rand::Rand() noexcept
    {
        seed();
    }

    //---------------------------------------------------------
    Rand::Rand(const std::uint64_t seed_state)
    {
        seed(seed_state);
    }

    //---------------------------------------------------------
    const double Rand::operator() () noexcept
    {
        return uniform();
    }

    //---------------------------------------------------------
    void Rand::seed() noexcept
    {
        _internal_state = _SplitMix()();
    }

    //---------------------------------------------------------
    void Rand::seed(const std::uint64_t seed_state) noexcept
    {
        _internal_state = _SplitMix(seed_state)();
    }

    //---------------------------------------------------------
    const double Rand::uniform() noexcept
    {
        // Notice: this is the implementation of
        // the PCG XSH RS 64/32 (LCG) algorithm.
        const std::uint64_t previous_state{ _internal_state };
        const unsigned int random_shift{ previous_state >> 61 };  // Notice: random shift is set with the 3 upper bits of the internal state

        // evaluates next state value
        _internal_state = 0x5851'f42d'4c95'7f2dULL * previous_state + 0x1405'7b7e'f767'814fULL;

        // computes the permutated output
        const std::uint32_t next_value{
            static_cast<std::uint32_t>(
                (previous_state ^ (previous_state >> 22)) >> (22 + random_shift)
            )
        };

        // returns a uniformaly distributed value in range [0.0, 1.0) on a 32-bits mantissa
        return next_value * _NORMALIZE;
    }

    //---------------------------------------------------------
    Rand::_SplitMix::_SplitMix() noexcept
        : _state{
            static_cast<std::uint64_t>(
                std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::high_resolution_clock::now().time_since_epoch()
                ).count()
            )
          }
    {}

    //---------------------------------------------------------
    Rand::_SplitMix::_SplitMix(const std::uint64_t seed) noexcept
        : _state{ seed }
    {}

    //---------------------------------------------------------
    const std::uint64_t Rand::_SplitMix::operator() () noexcept
    {
        // this is the 'Golden' Gamma value : std::uint64_t( ((1.0 + math.sqrt(5)) / 2.0) * 1<<64 )
        constexpr std::uint64_t GOLDEN_GAMMA{ 0x9e37'79b9'7f4a'7c15ULL };

        _state += GOLDEN_GAMMA;

        std::uint64_t z{ _state };
        z = (z ^ (z >> 30)) * 0xbf58'476d'1ce4'e5b9ULL;
        z = (z ^ (z >> 27)) * 0x94d0'49bb'1331'11ebULL;
        return z ^ (z >> 31);
    }

}
