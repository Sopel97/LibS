#pragma once

#include "LibS/Containers/Array2.h"

#include "LibS/Shapes/Box2.h"

#include "LibS/Detail.h"

#include <array>
#include <cstddef>
#include <utility>

namespace ls
{
    enum struct CellularAutomatonTopology
    {
        Finite,
        Toroidal //for toroidal space it is still required that passed coordinates are inside the bounds
    };

    template <typename RuleT, CellularAutomatonTopology TopologyV> //class representing a rule
    struct CellularAutomaton2 : protected RuleT
    {
    public:
        using RuleType = RuleT;
        using StateType = typename RuleT::StateType;
        using SizeType = detail::SizeType;
        static constexpr CellularAutomatonTopology topology = TopologyV;

        CellularAutomaton2(SizeType width, SizeType height) :
            RuleType{},
            m_cells(width, height)
        {

        }

        template <typename RuleFwdT>
        CellularAutomaton2(SizeType width, SizeType height, RuleFwdT&& rule) :
            RuleType(std::forward<RuleFwdT>(rule)),
            m_cells(width, height)
        {

        }

        CellularAutomaton2(const CellularAutomaton2&) = default;
        CellularAutomaton2(CellularAutomaton2&&) noexcept = default;
        CellularAutomaton2& operator=(const CellularAutomaton2&) = default;
        CellularAutomaton2& operator=(CellularAutomaton2&&) noexcept = default;
        ~CellularAutomaton2() = default;

        const StateType& operator()(SizeType x, SizeType y) const
        {
            return m_cells(x, y);
        }
        StateType& operator()(SizeType x, SizeType y)
        {
            return m_cells(x, y);
        }

        const StateType& at(SizeType x, SizeType y) const
        {
            return m_cells(x, y);
        }
        StateType& at(SizeType x, SizeType y)
        {
            return m_cells(x, y);
        }

        void fill(StateType state)
        {
            for (auto& cell : m_cells)
            {
                cell = state;
            }
        }
        
        //fill function must take x, y as coordinates and output valid State
        template <typename FillFunction>
        void fill(FillFunction fillingFunction)
        {
            const SizeType w = m_cells.width();
            const SizeType h = m_cells.height();

            for (SizeType x = 0; x < w; ++x)
            {
                for (SizeType y = 0; y < h; ++y)
                {
                    m_cells(x, y) = fillingFunction(x, y);
                }
            }
        }

        void iterate(SizeType numIters = 1)
        {
            const SizeType w = m_cells.width();
            const SizeType h = m_cells.height();

            Array2<StateType> nextGrid(w, h);
            while (numIters--)
            {
                for (SizeType x = 0; x < w; ++x)
                {
                    for (SizeType y = 0; y < h; ++y)
                    {
                        nextGrid(x, y) = RuleType::operator()(*this, x, y);
                    }
                }

                m_cells.swap(nextGrid);
            }
        }

        //x,y determine center of the 3x3 region
        SizeType occurencesIn3x3(const StateType& state, SizeType x, SizeType y) const 
        {
            return occurencesInRadius(state, x, y, 1);
        }

        //x,y determine center of the 5x5 region
        SizeType occurencesIn5x5(const StateType& state, SizeType x, SizeType y) const 
        {
            return occurencesInRadius(state, x, y, 2);
        }

        //x,y determine center of the region, radius is a maximal chessboard distance
        SizeType occurencesInRadius(const StateType& state, SizeType x, SizeType y, SizeType radius) const
        {
            const Vec2<SizeType> diagonalVector(radius, radius);
            const Vec2<SizeType> centerVector(x, y);
            return occurencesInRect(state, Box2<SizeType>(centerVector - diagonalVector, centerVector + diagonalVector));
        }

        SizeType occurencesInRect(const StateType& state, const Box2<SizeType>& rect) const 
        {
            const SizeType width = m_cells.width();
            const SizeType height = m_cells.height();

            SizeType quantity = 0;

            if constexpr (TopologyV == CellularAutomatonTopology::Finite)
            {
                SizeType xmin = std::max(rect.min.x, static_cast<SizeType>(0));
                SizeType ymin = std::max(rect.min.y, static_cast<SizeType>(0));
                SizeType xmax = std::min(rect.max.x, width - 1);
                SizeType ymax = std::min(rect.max.y, height - 1);

                for (SizeType xx = xmin; xx <= xmax; ++xx)
                {
                    for (SizeType yy = ymin; yy <= ymax; ++yy)
                    {
                        if (m_cells(xx, yy) == state) ++quantity;
                    }
                }
            }
            else if constexpr (TopologyV == CellularAutomatonTopology::Toroidal)
            {
                for (SizeType xx = rect.min.x; xx <= rect.max.x; ++xx)
                {
                    for (SizeType yy = rect.min.y; yy <= rect.max.y; ++yy)
                    {
                        if (m_cells((xx + width) % width, (yy + height) % height) == state) ++quantity;
                    }
                }
            }
            else
            {
                static_assert(false, "Invalid topology.");
            }

            return quantity;
        }

        //only sides
        SizeType occurencesInMooreNeighbourhood(const StateType& state, SizeType x, SizeType y) const
        {
            const SizeType width = m_cells.width();
            const SizeType height = m_cells.height();

            SizeType quantity = 0;

            if constexpr (TopologyV == CellularAutomatonTopology::Finite)
            {
                if (x > 0 && m_cells(x - 1, y) == state) ++quantity;
                if (x < width - 1 && m_cells(x + 1, y) == state) ++quantity;
                if (y > 0 && m_cells(x, y - 1) == state) ++quantity;
                if (y < height - 1 && m_cells(x, y + 1) == state) ++quantity;
            }
            else if constexpr (TopologyV == CellularAutomatonTopology::Toroidal)
            {
                if (m_cells((x + width - 1) % width, y) == state) ++quantity;
                if (m_cells((x + 1) % width, y) == state) ++quantity;
                if (m_cells(x, (y + height - 1) % height) == state) ++quantity;
                if (m_cells(x, (y + 1) % height) == state) ++quantity;
            }
            else
            {
                static_assert(false, "Invalid topology.");
            }

            return quantity;
        }

        //sides and diagonals
        SizeType occurencesInNeighbourhood(const StateType& state, SizeType x, SizeType y) const
        {
            return occurencesIn3x3(state, x, y) - (m_cells(x, y) == state);
        }

    protected:
        Array2<StateType> m_cells;
    };

    template <typename RuleT>
    using FiniteCellularAutomaton2 = CellularAutomaton2<RuleT, CellularAutomatonTopology::Finite>;

    template <typename RuleT>
    using ToroidalCellularAutomaton2 = CellularAutomaton2<RuleT, CellularAutomatonTopology::Toroidal>;



    template <typename StateT> //enum representing all possible states
    struct QuantityRule3x3
    {
    public:
        using StateType = StateT;
        using SizeType = detail::SizeType;

        QuantityRule3x3(StateType countedState, const std::array<StateType, 10u>& outputs) :
            m_countedState(countedState),
            m_outputs(outputs)
        {

        }

        template <CellularAutomatonTopology TopologyV>
        StateType operator()(const CellularAutomaton2<QuantityRule3x3, TopologyV>& automaton, SizeType x, SizeType y) const
        {
            return m_outputs[automaton.occurencesIn3x3(m_countedState, x, y)];
        }

        void setOutputForQuantity(StateType outputState, SizeType quantity)
        {
            m_outputs[quantity] = outputState;
        }

    protected:
        StateType m_countedState;
        std::array<StateType, 10u> m_outputs; //the quantity of state in 3x3 region is the index
    };



    struct ConwaysGameOfLifeRule
    {
    public:
        using SizeType = detail::SizeType;

        enum struct StateType
        {
            Dead,
            Live
        };

        ConwaysGameOfLifeRule() = default;

        template <CellularAutomatonTopology TopologyV>
        StateType operator()(const CellularAutomaton2<ConwaysGameOfLifeRule, TopologyV>& automaton, SizeType x, SizeType y) const
        {
            const SizeType numberOfLivingNeighbours = automaton.occurencesInNeighbourhood(StateType::Live, x, y);
            const auto& currentState = automaton(x, y);

            if (currentState == StateType::Live)
            {
                if (numberOfLivingNeighbours < 2) return StateType::Dead;
                else if (numberOfLivingNeighbours > 3) return StateType::Dead;
            }
            else //Dead
            {
                if (numberOfLivingNeighbours == 3) return StateType::Live;
            }

            return currentState;
        }
    };

    struct WireworldRule
    {
    public:
        using SizeType = detail::SizeType;

        enum struct StateType
        {
            Empty,
            ElectronHead,
            ElectronTail,
            Conductor
        };

        WireworldRule() = default;

        template <CellularAutomatonTopology TopologyV>
        StateType operator()(const CellularAutomaton2<WireworldRule, TopologyV>& automaton, SizeType x, SizeType y) const
        {
            StateType currentState = automaton(x, y);

            if (currentState == StateType::Empty) return StateType::Empty;
            else if (currentState == StateType::ElectronHead) return StateType::ElectronTail;
            else if (currentState == StateType::ElectronTail) return StateType::Conductor;
            else //(currentState == State::Conductor)
            {
                const SizeType numberOfElectronHeadsInNeighbourhood = automaton.occurencesInMooreNeighbourhood(StateType::ElectronHead, x, y);
                if (numberOfElectronHeadsInNeighbourhood == 1 || numberOfElectronHeadsInNeighbourhood == 2)
                    return StateType::ElectronHead;
            }

            return currentState;
        }
    };
}
