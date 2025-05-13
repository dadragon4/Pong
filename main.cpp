#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <iostream>
#include "Constants.hpp"
#include "Game.hpp"
#include <cstdlib>
#include <ctime>

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pong");
    window.setFramerateLimit(60);
    sf::Clock clock;

    // We're keeping score ;)
    int scoreLeft = 0;
    int scoreRight = 0;
    GameState state = GameState::START;

    // Load a font
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return -1; // Error loading font
    }

    // Title text
    sf::Text titleText("P O N G", font, 50);
    titleText.setFillColor(sf::Color::White);
    auto tT = titleText.getLocalBounds();
    titleText.setOrigin(tT.left + tT.width / 2, tT.top + tT.height / 2);
    titleText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f - tT.height);
    titleText.setStyle(sf::Text::Bold);

    // Prompt text
    sf::Text startPrompt("Press Space to Start", font, 20);
    startPrompt.setFillColor(sf::Color::White);
    auto sb = startPrompt.getLocalBounds();
    startPrompt.setOrigin(sb.left + sb.width / 2.f, sb.top + sb.height / 2.f);
    float spacing = 10.f;
    startPrompt.setPosition(
        WINDOW_WIDTH / 2.f,
        (WINDOW_HEIGHT / 2.f) + (tT.height / 2.f) + spacing + (sb.height / 2.f)
    );

    // Title text bounds
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);

    // Create game over text
    sf::Text gameOverText("", font, 50);
    gameOverText.setFillColor(sf::Color::White);

    // Press Space to reset
    sf::Text promptText("Press Space to Reset", font, 20);
    promptText.setFillColor(sf::Color::White);
    auto pb = promptText.getLocalBounds();
    promptText.setOrigin(pb.left + pb.width / 2.f, pb.top + pb.height / 2.f);
    promptText.setPosition(
        WINDOW_WIDTH / 2.f,
        (WINDOW_HEIGHT / 2.f) + (tT.height / 2.f) + pb.height + spacing
    );

    // Thank You Text
    sf::Text thankYouText("Thank you for playing!", font, 50);
    thankYouText.setFillColor(sf::Color::White);
    auto tyb = thankYouText.getLocalBounds();
    thankYouText.setOrigin(tyb.left + tyb.width / 2.f, tyb.top + tyb.height / 2.f);
    thankYouText.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);

    // Left Paddle
    sf::RectangleShape leftPaddle(sf::Vector2f(PADDLE_WIDTH, PADDLE_HEIGHT));
    leftPaddle.setPosition(50, (WINDOW_HEIGHT / 2) - (PADDLE_HEIGHT / 2));
    leftPaddle.setFillColor(sf::Color::White);

    // Right Paddle
    sf::RectangleShape rightPaddle(sf::Vector2f(PADDLE_WIDTH, PADDLE_HEIGHT));
    rightPaddle.setPosition(WINDOW_WIDTH - 50 - PADDLE_WIDTH, (WINDOW_HEIGHT / 2) - (PADDLE_HEIGHT / 2));
    rightPaddle.setFillColor(sf::Color::White);

    // Ball
    sf::CircleShape ball(BALL_RADIUS);
    ball.setFillColor(sf::Color::White);
    ball.setPosition(WINDOW_WIDTH  / 2.f - BALL_RADIUS, WINDOW_HEIGHT / 2.f - BALL_RADIUS);

    // Ball velocity
    sf::Vector2f ballVelocity(BALL_SPEED_X, BALL_SPEED_Y);

    // Game loop
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        
        // event handling
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (state == GameState::START 
                && event.type == sf::Event::KeyPressed
                && event.key.code == sf::Keyboard::Space) {
                scoreLeft = scoreRight = 0; // we are resetting the score
                resetGame(leftPaddle, rightPaddle, ball, ballVelocity, state);
            }
            else if (state == GameState::GAME_OVER 
                && event.type == sf::Event::KeyPressed
                && event.key.code == sf::Keyboard::Space) {
                resetGame(leftPaddle, rightPaddle, ball, ballVelocity, state);
            }
            else if (state == GameState::THANK_YOU 
                && event.type == sf::Event::KeyPressed
                && event.key.code == sf::Keyboard::Space) {
                state = GameState::START;
            }
        }
        

        // Paddle Movement & Ball Movement
        if (state == GameState::PLAYING) {
            // Left Paddle Controls
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && leftPaddle.getPosition().y > 0)
            leftPaddle.move(0, -PADDLE_SPEED * dt);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && leftPaddle.getPosition().y + PADDLE_HEIGHT < WINDOW_HEIGHT)
            leftPaddle.move(0, PADDLE_SPEED * dt);

            // Right Paddle Controls
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && rightPaddle.getPosition().y > 0)
            rightPaddle.move(0, -PADDLE_SPEED * dt);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && rightPaddle.getPosition().y + PADDLE_HEIGHT < WINDOW_HEIGHT)
            rightPaddle.move(0, PADDLE_SPEED * dt);

            // Then move the ball
            // Ball movement
            ball.move(ballVelocity * dt);

            // Bounce off top/bottom
            if (ball.getPosition().y <= 0 || ball.getPosition().y + BALL_RADIUS * 2 >= WINDOW_HEIGHT)
            ballVelocity.y *= -1;

            // Paddle collision (basic)
            if (ball.getGlobalBounds().intersects(leftPaddle.getGlobalBounds()) ||
            ball.getGlobalBounds().intersects(rightPaddle.getGlobalBounds())) {
            ballVelocity.x *= -1;
            }

            // Ball out of bounds check
            float bx = ball.getPosition().x;
            if (bx + BALL_RADIUS * 2 < 0) {
                scoreRight++;

                if (scoreRight >= WINNING_SCORE) {
                    state = GameState::GAME_OVER;
                    gameOverText.setString("Right Player Wins!");
                } else {
                    resetGame(leftPaddle, rightPaddle, ball, ballVelocity, state);
                }
            }

            else if (bx > WINDOW_WIDTH) {
                scoreLeft++;
                
                if (scoreLeft >= WINNING_SCORE) {
                    state = GameState::GAME_OVER;
                    gameOverText.setString("Left Player Wins!");
                } else {
                    resetGame(leftPaddle, rightPaddle, ball, ballVelocity, state);
                }
            }

            //recenter the winner text
            if (state == GameState::GAME_OVER) {
                auto gO = gameOverText.getLocalBounds();
                gameOverText.setOrigin(
                    gO.left + gO.width / 2.f,
                    gO.top + gO.height / 2.f
                );
                gameOverText.setPosition(
                    WINDOW_WIDTH / 2.f,
                    (WINDOW_HEIGHT / 2.f) - (gO.height / 2.f)
                );
            }
        }
    

        // Clear, draw, and display
        window.clear();

        // Draw title (switch based on state)
        switch (state) {
            case GameState::START:
                window.draw(titleText);
                window.draw(startPrompt);
                break;
            case GameState::PLAYING:
                window.draw(leftPaddle);
                window.draw(rightPaddle);
                window.draw(ball);
                break;
            case GameState::GAME_OVER:
                window.draw(gameOverText);
                window.draw(promptText);
                break;
            case GameState::THANK_YOU:
                window.draw(thankYouText);
                break;
        }
        
        scoreText.setString(
            std::to_string(scoreLeft) + " : " + std::to_string(scoreRight)
        );

        auto bounds = scoreText.getLocalBounds();
        scoreText.setOrigin(
            bounds.left + bounds.width / 2.f,
            bounds.top + bounds.height / 2.f
        );

        scoreText.setPosition(
            WINDOW_WIDTH / 2.f,
            bounds.height / 2.f + 10.f
        );

        window.draw(scoreText);

        // Display the contents of the window
        window.display();
    }

    return 0;
}



// Compile and run instructions:

// compile program with : g++ main.cpp Game.cpp -o pong -lsfml-graphics -lsfml-window -lsfml-system
// run program with : ./pong