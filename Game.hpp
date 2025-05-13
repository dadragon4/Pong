#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "Constants.hpp"

enum class GameState {
    START,
    PLAYING,
    GAME_OVER,
    THANK_YOU
};

// Reset paddles, ball position & velocity, and switch back to PLAYING
void resetGame(
    sf::RectangleShape& leftPaddle,
    sf::RectangleShape& rightPaddle,
    sf::CircleShape&    ball,
    sf::Vector2f&       ballVelocity,
    GameState&          state
);

#endif // GAME_HPP