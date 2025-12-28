#include "mu-torere/Game.hpp"
#include "mu-torere/Engine.hpp"
#include "mu-torere/MuTorereRules.hpp"
#include "mu-torere/Position.hpp"
#include "mu-torere/RetrogradeSolver.hpp"
#include <algorithm>
#include <cmath>
#include <numbers>
#include <ranges>
#include <raylib.h>
#include <raymath.h>

namespace mutorere {

Game::Game()
    : window_{ s_WindowSize, s_WindowSize, "Mu-Torere" } {
  SetTargetFPS(60);

  boardCenter_ =
      raylib::Vector2{ s_WindowSize / 2.0F, s_WindowSize / 2.0F };

  for (auto i : std::views::iota(0, Position::NumTiles)) {
    float angle =
        (static_cast<float>(i) *
         (2.0F * std::numbers::pi_v<float> / Position::RingSize)) -
        std::numbers::pi_v<float>;
    tiles_.at(i) = raylib::Vector2{
      boardCenter_.x + (s_RingRadius * std::cos(angle)),
      boardCenter_.y + (s_RingRadius * std::sin(angle))
    };
  }
  tiles_.at(Position::CenterTile) = boardCenter_;
}

void Game::run() {
  position_ = rules_.initialPosition();
  initEngine(position_);

  while (!window_.ShouldClose()) {
    updateLogic();
    draw();
  }
}

void Game::updateLogic() {
  if (rules_.isTerminal(position_)) {
    state_ = State::GameOver;
  } else if (position_.sideToMove == humanSide_) {
    if (!raylib::Mouse::IsButtonPressed(MOUSE_BUTTON_LEFT)) {
      return;
    }

    auto mousePos{ raylib::Mouse::GetPosition() };
    auto hitTile{ testHitTile(mousePos) };
    if (!hitTile) {
      return;
    }

    auto moves{ rules_.legalMoves(position_) };
    auto isLegal{ std::ranges::any_of(moves, [&](const Move& m) {
      return m.from == *hitTile;
    }) };

    if (!isLegal || !hasPawnOn(*hitTile)) {
      return;
    }
    position_ =
        rules_.apply(position_, Move{ static_cast<uint8_t>(*hitTile) });
  } else {
    AITimer_ += GetFrameTime();
    if (AITimer_ < s_AIDelay) {
      return;
    }

    AITimer_ = 0.0F;
    auto move{ engine_->bestMove(position_) };
    if (!move) {
      // will be handled next frame
      return;
    }

    position_ = rules_.apply(position_, *move);
  }
}

void Game::draw() {
  BeginDrawing();
  window_.ClearBackground(RAYWHITE);

  DrawText(
      "Mu-Torere - click on highlighted piece to move it to empty tile.",
      20, 20, 20, DARKGRAY);

  // edges
  for (int i = 0; i < Position::RingSize; ++i) {
    int j{ (i + 1) % Position::RingSize };
    DrawLineEx(tiles_.at(i), tiles_.at(j), 3.0F, GRAY);
    DrawLineEx(tiles_.at(i), tiles_.at(Position::CenterTile), 2.0F,
               LIGHTGRAY);
  }

  // pawns
  std::bitset<Position::NumTiles> legalFrom{};
  auto moves{ rules_.legalMoves(position_) };
  if (position_.sideToMove == humanSide_) {
    for (auto move : moves) {
      legalFrom.set(move.from);
    }
  }

  for (int i = 0; i < Position::NumTiles; i++) {
    auto center = tiles_.at(i);

    DrawCircleV(center, s_TileRadius, WHITE);
    DrawCircleLinesV(center, s_TileRadius, DARKGRAY);

    if (legalFrom.test(i)) {
      DrawCircleV(center, s_TileRadius, GREEN);
    }

    if (position_.whiteMask.test(i)) {
      DrawCircleV(center, s_PawnRadius, LIGHTGRAY);
      DrawCircleLinesV(center, s_PawnRadius, BLACK);
    } else if (position_.blackMask.test(i)) {
      DrawCircleV(center, s_PawnRadius, BLACK);
    }
  }

  EndDrawing();
}

auto Game::testHitTile(const raylib::Vector2& mousePos) const
    -> std::optional<size_t> {
  float r2     = s_PawnRadius * s_PawnRadius;
  size_t best  = -1;
  float bestD2 = r2;
  for (auto [i, tile] : std::views::enumerate(tiles_)) {
    float d2 = (mousePos - tile).LengthSqr();
    if (d2 < bestD2) {
      bestD2 = d2;
      best   = i;
    }
  }

  if (best == -1ULL) {
    return std::nullopt;
  }
  return best;
}

void Game::initEngine(const Position& position) {
  RetrogradeSolver solver{ rules_ };
  auto tb{ solver.solveFrom(position) };
  engine_ = std::make_unique<Engine>(rules_, std::move(tb));
}

auto Game::hasPawnOn(size_t tile) const -> bool {
  if (humanSide_ == Player::White) {
    return position_.whiteMask.test(tile);
  }
  return position_.blackMask.test(tile);
}
} // namespace mutorere
