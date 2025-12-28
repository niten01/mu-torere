#include "mu-torere/RetrogradeSolver.hpp"
#include "mu-torere/Position.hpp"
#include <queue>
#include <vector>

namespace mutorere {

auto RetrogradeSolver::solveFrom(const Position& start) const
    -> Tablebase {
  static constexpr auto maxStates = 2000;
  using NodeID                    = int;

  std::vector<Position> states;
  states.reserve(maxStates);

  std::unordered_map<Position, NodeID, PositionHasher> stateIDs;
  stateIDs.reserve(maxStates);

  std::vector<std::vector<NodeID>> succ;
  std::vector<std::vector<NodeID>> pred;

  auto getID = [&](const Position& position) {
    auto it = stateIDs.find(position);
    if (it != stateIDs.end()) {
      return it->second;
    }

    auto newID = static_cast<NodeID>(states.size());
    states.push_back(position);
    stateIDs[position] = newID;
    succ.emplace_back();
    pred.emplace_back();
    return newID;
  };

  std::queue<NodeID> queue;
  int s0 = getID(start);
  queue.push(s0);

  while (!queue.empty()) {
    auto stateID      = queue.front();
    const auto& state = states[stateID];
    queue.pop();

    if (rules_.isTerminal(state) || !succ[stateID].empty()) {
      continue;
    }

    auto moves = rules_.legalMoves(state);
    for (auto move : moves) {
      Position pos = rules_.apply(state, move);
      auto nextID  = getID(pos);

      succ[stateID].push_back(nextID);
      pred[nextID].push_back(stateID);
      queue.push(nextID);
    }
  }

  auto N = static_cast<int>(states.size());

  std::vector<Result> result(N, Result::Unknown);

  std::vector<int> remote(N, 0);
  std::vector<int> numGoodChildren(N, 0);
  std::vector<int> bestLossRemote(N, 0);

  std::queue<NodeID> worklist;
  for (int i = 0; i < N; i++) {
    numGoodChildren[i] = static_cast<int>(succ[i].size());
    if (numGoodChildren[i] == 0) {
      result[i] = Result::Loss;
      remote[i] = 0;
      worklist.push(i);
    }
  }

  while (!worklist.empty()) {
    auto curID = worklist.front();
    worklist.pop();

    for (auto predID : pred[curID]) {
      if (result[predID] != Result::Unknown) {
        continue;
      }

      if (result[curID] == Result::Loss) {
        result[predID] = Result::Win;
        remote[predID] = remote[curID] + 1;
        worklist.push(predID);
      } else if (result[curID] == Result::Win) {
        numGoodChildren[predID]--;
        bestLossRemote[predID] =
            std::max(bestLossRemote[predID], remote[curID] + 1);

        if (numGoodChildren[predID] == 0) {
          result[predID] = Result::Loss;
          remote[predID] = bestLossRemote[predID];
          worklist.push(predID);
        }
      }
    }
  }

  Tablebase tb;
  for (int i = 0; i < N; i++) {
    Outcome o{};
    o.remoteness = (result[i] == Result::Unknown) ? 0 : remote[i];
    o.result     = result[i];
    tb.store(states[i], o);
  }

  return tb;
}

} // namespace mutorere
