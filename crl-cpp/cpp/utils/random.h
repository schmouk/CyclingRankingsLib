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


#include <algorithm>
#include <cstdint>
#include <type_traits>
#include <utility>
#include <vector>


namespace crl
{
    //=====   The pseudo-random numbers algorithm   ===========
    // (PCG XSH RS 64/32 (LCG) implementation)
    // (see https://www.cs.hmc.edu/tr/hmc-cs-2014-0905.pdf)
    class Rand final
    {
    public:
        //-----   Constructors / Destructor   -----------------
        Rand() noexcept;
        Rand(const std::uint64_t seed_state);

        Rand(const Rand&) = delete;
        Rand(Rand&&) = delete;

        ~Rand() noexcept = default;


        //-----   Operators   ---------------------------------
        const double operator() () noexcept;

        template<typename T = std::uint64_t>
            requires std::is_arithmetic_v<T>
        const T operator() (const T max) noexcept;

        template<typename T = std::uint64_t>
            requires std::is_arithmetic_v<T>
        std::vector<T> operator() (const std::vector<T>& min, const std::vector<T>& max) noexcept;


        //-----   Operations   --------------------------------
        void seed() noexcept;
        void seed(const std::uint64_t seed_state) noexcept;

        template<typename T>
        void shuffle(std::vector<T>& seq);

        template<typename T>
        void shuffle(std::vector<T>& seq, std::size_t n);

        const double uniform() noexcept;


    private:
        //-----------------------------------------------------
        static constexpr double _NORMALIZE{
            1.0 / (static_cast<long double>(0x1'0000'0000ULL))
        };
        
        std::uint64_t _internal_state{ 0 };

        //-----------------------------------------------------
        class _SplitMix final
        {
        public:
            _SplitMix() noexcept;
            _SplitMix(const std::uint64_t seed) noexcept;

            const std::uint64_t operator() () noexcept;

            virtual ~_SplitMix() noexcept = default;

        private:
            std::uint64_t _state{ 0 };
        };

    };


    //=====   IMPLEMENTATIONS   ===============================
    //---------------------------------------------------------
    template<typename T>
        requires std::is_arithmetic_v<T>
    const T Rand::operator() (const T max) noexcept
    {
        return static_cast<T>(max * uniform());
    }

    //---------------------------------------------------------
    template<typename T>
        requires std::is_arithmetic_v<T>
    std::vector<T> Rand::operator() (const std::vector<T>& min, const std::vector<T>& max) noexcept
    {
        return static_cast<T>(min + operator()(max - min));
    }

    //---------------------------------------------------------
    template<typename T>
    void Rand::shuffle(std::vector<T>& seq)
    {
        shuffle(seq, seq.size());
    }

    //---------------------------------------------------------
    template<typename T>
    void Rand::shuffle(std::vector<T>& seq, std::size_t n)
    {
        n = std::min(n, seq.size());
        
        if (n != 0) {
            for (std::size_t i = 0; i < n - 1; ++i) {
                const std::size_t index{ operator()(i, n) };
                std::swap(seq[i], seq[index]);
            }
        }
    }

}
