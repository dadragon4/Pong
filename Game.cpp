#include "Game.hpp"
#include "Constants.hpp"
#include <SFML/Graphics.hpp>

void resetGame(
    sf::RectangleShape& leftPaddle,
    sf::RectangleShape& rightPaddle,
    sf::CircleShape&    ball,
    sf::Vector2f&       ballVelocity,
    GameState&          state
) {
    // Center paddles
    leftPaddle.setPosition(
        50.f,
        (WINDOW_HEIGHT - PADDLE_HEIGHT) / 2.f
    );
    rightPaddle.setPosition(
        WINDOW_WIDTH - 50.f - PADDLE_WIDTH,
        (WINDOW_HEIGHT - PADDLE_HEIGHT) / 2.f
    );

    // Center the ball
    ball.setPosition(
        WINDOW_WIDTH / 2.f - BALL_RADIUS,
        WINDOW_HEIGHT / 2.f - BALL_RADIUS
    );

    // Randomize ball direction
    float vx = BALL_SPEED_X * ((std::rand() % 2) ? 1.f : -1.f);
    float vy = BALL_SPEED_X * ((std::rand() % 2) ? 1.f : -1.f);


    // Restore initial velocity
    ballVelocity.x =  vx;
    ballVelocity.y =  vy;

    // Switch state
    state = GameState::PLAYING;
}