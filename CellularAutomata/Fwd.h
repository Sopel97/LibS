#pragma once

namespace ls
{
    enum struct CellularAutomatonTopology;

    template <typename RuleT, CellularAutomatonTopology TopologyV> //class representing a rule
    struct CellularAutomaton2;

    template <typename StateT> //enum representing all possible states
    struct QuantityRule3x3;

    struct ConwaysGameOfLifeRule;

    struct WireworldRule;
}
