#pragma once
#include "mu-torere/Engine.hpp"
#include "mu-torere/MuTorereRules.hpp"
#include "mu-torere/Player.hpp"
#include "mu-torere/Position.hpp"
#include "mu-torere/RetrogradeSolver.hpp"
#include <memory>
#include <raylib-cpp.hpp>
#include <raylib.h>

namespace mutorere {

class Game {
public:
  Game();

  void run();

private:
  void updateLogic();
  void draw();

  [[nodiscard]] auto testHitTile(const raylib::Vector2& mousePos) const
      -> std::optional<size_t>;

  void initEngine(const Position& position);

  [[nodiscard]] auto hasPawnOn(size_t tile) const -> bool;

private:
  static constexpr int s_WindowSize   = 800;
  static constexpr float s_AIDelay    = 0.5F;
  static constexpr float s_TileRadius = 32.F;
  static constexpr float s_PawnRadius = 28.F;
  static constexpr float s_RingRadius = 300.F;

  enum class State : uint8_t {
    ChooseSide,
    Play,
    GameOver
  };

  raylib::Window window_;
  float AITimer_{};
  raylib::Vector2 boardCenter_;
  std::array<raylib::Vector2, Position::NumTiles> tiles_;
  State state_{ State::Play };
  Position position_;
  MuTorereRules rules_;
  std::unique_ptr<Engine> engine_;
  Player humanSide_{ Player::White };
};

} // namespace mutorere
