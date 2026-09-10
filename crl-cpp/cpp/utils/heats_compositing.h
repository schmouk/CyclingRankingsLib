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

#include <algorithm>
#include <concepts>
#include <cstdint>
#include <type_traits>
#include <vector>

#include "../commons/time.h"
#include "../commons/types.h"
#include "../utils/random.h"


namespace crl
{
    //=====   Heats Compositing Base Class   ==================
    template<typename CompetitorT = crl::Bib>
    class HeatsCompositionBase
    {
    public:
        //-----   Types Wrappers   ----------------------------
        using competitors_list_type = std::vector<CompetitorT>;
        using heat_type             = std::vector<CompetitorT>;
        using heats_list_type       = std::vector<heat_type>;


        //-----   Constructors / Destructor   -----------------
        HeatsCompositionBase(
            Rand& rand
        ) noexcept
            : _competitors_list{}
            , _rand_ptr{ &rand }
        {}

        HeatsCompositionBase(
            const competitors_list_type& competitors
        ) noexcept
            : _competitors_list{ competitors }
            , _rand_ptr{ nullptr }
        {}

        HeatsCompositionBase(
            Rand& rand,
            const competitors_list_type& competitors
        ) noexcept
            : _competitors_list{ competitors }
            , _rand_ptr{ &rand }
        {}

        HeatsCompositionBase() noexcept = default;
        virtual ~HeatsCompositionBase() noexcept = default;

        HeatsCompositionBase(const HeatsCompositionBase&) = default;
        HeatsCompositionBase(HeatsCompositionBase&&) = default;
        HeatsCompositionBase& operator=(const HeatsCompositionBase&) = default;
        HeatsCompositionBase& operator=(HeatsCompositionBase&&) = default;


        //-----   Operations   --------------------------------
        virtual heats_list_type compose_n_heats(
            const unsigned int heats_nb
        ) = 0;

        virtual heats_list_type compose_heats(
            const unsigned int competitors_min_count  // The min number of competitors per heat
        ) = 0;


    protected:
        competitors_list_type _competitors_list{};
        crl::Rand*            _rand_ptr{ nullptr };

    };


    //=====   Fully Random Heats Compositing Base Class   =====
    template<typename CompetitorT = crl::Bib>
    struct FullyRandomHeatsComposition : public HeatsCompositionBase<CompetitorT>
    {
        using MyBaseClass           = HeatsCompositionBase<CompetitorT>;
        using competitors_list_type = MyBaseClass::competitors_list_type;
        using heats_list_type       = MyBaseClass::heats_list_type;

        //-----   Constructors / Destructor   -----------------
        FullyRandomHeatsComposition(
            Rand& rand
        ) noexcept
            : MyBaseClass{ rand }
        {}

        FullyRandomHeatsComposition(
            Rand& rand,
            const competitors_list_type& competitors
        ) noexcept
            : MyBaseClass{ rand, competitors }
        {}

        FullyRandomHeatsComposition() noexcept = default;
        FullyRandomHeatsComposition(const FullyRandomHeatsComposition&) noexcept = default;
        FullyRandomHeatsComposition(FullyRandomHeatsComposition&&) noexcept = default;
        FullyRandomHeatsComposition& operator= (const FullyRandomHeatsComposition&) noexcept = default;
        FullyRandomHeatsComposition& operator= (FullyRandomHeatsComposition&&) noexcept = default;

        virtual ~FullyRandomHeatsComposition() noexcept = default;


        //-----   Operations   --------------------------------
        heats_list_type compose_n_heats(
            const unsigned int heats_nb
        ) noexcept;

        heats_list_type compose_heats(
            const unsigned int competitors_min_count  // The min number of competitors per heat
        ) noexcept;

    };


    //=====   Forward/Backward Heats Compositing Base Class   =====
    template<typename CompetitorT = crl::Bib>
    struct FrwdBkwdHeatsComposition : public HeatsCompositionBase<CompetitorT>
    {
        using MyBaseClass = HeatsCompositionBase<CompetitorT>;
        using competitors_list_type = MyBaseClass::competitors_list_type;
        using heats_list_type = MyBaseClass::heats_list_type;

        //-----   Constructors / Destructor   -----------------
        FrwdBkwdHeatsComposition() noexcept
            : MyBaseClass{}
        {}

        FrwdBkwdHeatsComposition(const competitors_list_type& competitors) noexcept
            : MyBaseClass{ competitors }
        {}

        FrwdBkwdHeatsComposition(const FrwdBkwdHeatsComposition&) noexcept = default;
        FrwdBkwdHeatsComposition(FrwdBkwdHeatsComposition&&) noexcept = default;
        FrwdBkwdHeatsComposition& operator= (const FrwdBkwdHeatsComposition&) noexcept = default;
        FrwdBkwdHeatsComposition& operator= (FrwdBkwdHeatsComposition&&) noexcept = default;

        virtual ~FrwdBkwdHeatsComposition() noexcept = default;


        //-----   Operations   --------------------------------
        heats_list_type compose_n_heats(
            const unsigned int heats_nb
        ) noexcept;

        heats_list_type compose_heats(
            const unsigned int competitors_count
        ) noexcept;

        heats_list_type compose_heats(
            const unsigned int competitors_count,
            const unsigned int nb_competitors_per_heat
        ) noexcept;

    };

}


//=====   Local Implementations   =============================
namespace crl
{
    //=====   Fully Random Heats Compositing Base Class   =====
    //---------------------------------------------------------
    template<typename CompetitorT>
    FullyRandomHeatsComposition<CompetitorT>::heats_list_type FullyRandomHeatsComposition<CompetitorT>::compose_n_heats(
        const unsigned int heats_nb
    ) noexcept
    {
        const unsigned int competitors_min_count{ static_cast<unsigned int>(this->_competitors_list.size()) / heats_nb };

        return compose_heats(competitors_min_count);
    }


    //---------------------------------------------------------
    template<typename CompetitorT>
    FullyRandomHeatsComposition<CompetitorT>::heats_list_type FullyRandomHeatsComposition<CompetitorT>::compose_heats(
        const unsigned int competitors_min_count
    ) noexcept
    {
        const unsigned int competitors_count{ static_cast<unsigned int>(this->_competitors_list.size()) };
        const unsigned int min_competitors_per_heat{ std::max(competitors_min_count, 2U) };
        const unsigned int heats_count{ competitors_count / min_competitors_per_heat };
        
        heats_list_type heats_list{ heats_count };

        this->_rand_ptr->shuffle(this->_competitors_list);

        auto heats_it{ heats_list.rbegin() };
        for (auto& comp : this->_competitors_list) {
            heats_it->push_back(comp);
            if (++heats_it == heats_list.rend())
                heats_it = heats_list.rbegin();
        }

        return heats_list;
    }

    //=====   Forward/Backward Heats Compositing Base Class   =====
    //---------------------------------------------------------
    template<typename CompetitorT>
    FrwdBkwdHeatsComposition<CompetitorT>::heats_list_type FrwdBkwdHeatsComposition<CompetitorT>::compose_n_heats(
        const unsigned int heats_nb
    ) noexcept
    {
        return compose_heats(static_cast<unsigned int>(this->_competitors_list.size()));
    }

    //---------------------------------------------------------
    template<typename CompetitorT>
    FrwdBkwdHeatsComposition<CompetitorT>::heats_list_type FrwdBkwdHeatsComposition<CompetitorT>::compose_heats(
        const unsigned int competitors_count
    ) noexcept
    {
        return compose_heats(competitors_count, 2);
    }

    //---------------------------------------------------------
    template<typename CompetitorT>
    FrwdBkwdHeatsComposition<CompetitorT>::heats_list_type FrwdBkwdHeatsComposition<CompetitorT>::compose_heats(
        const unsigned int competitors_count,
        const unsigned int nb_competitors_per_heat
    ) noexcept
    {
        const unsigned int heats_count{
            competitors_count / nb_competitors_per_heat +
            (competitors_count % nb_competitors_per_heat != 0)
        };

        heats_list_type heats_list{ heats_count };

        std::partial_sort(
            this->_competitors_list.begin(),
            this->_competitors_list.begin() + competitors_count,
            this->_competitors_list.end()
        );

        unsigned int comp_nb{ competitors_count };
        auto fwd_heats_it{ heats_list.begin() };
        auto bwd_heats_it{ heats_list.rbegin() };
        auto comp_it{ this->_competitors_list.begin() };
        for (unsigned int comp_nb{ competitors_count }; comp_nb != 0; --comp_nb ) {
            if (fwd_heats_it != heats_list.end()) {
                fwd_heats_it->push_back(*comp_it);
                ++comp_it;
                ++fwd_heats_it;
            }
            else if (bwd_heats_it != heats_list.rend()) {
                bwd_heats_it->push_back(*comp_it);
                ++comp_it;
                ++bwd_heats_it;
            }
            else {
                fwd_heats_it = heats_list.begin();
                bwd_heats_it = heats_list.rbegin();
                fwd_heats_it->push_back(*comp_it);
                ++comp_it;
                ++fwd_heats_it;
            }
        }

        return heats_list;
    }

}
