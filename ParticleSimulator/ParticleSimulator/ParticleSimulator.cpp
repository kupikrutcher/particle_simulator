#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include <cstdlib>
#include <ctime>

class Particle {
public:
    float x, y;
    float vx, vy;

    Particle(float startX, float startY)
        : x(startX), y(startY), vx((rand() % 100) / 25.0f - 2.0f), vy((rand() % 100) / 25.0f - 2.0f) {
    }

    void update(float dt) {
        x += vx * dt;
        y += vy * dt;

        // Bounce around within an 800x600 window
        if (x < 0 || x > 800) {
            vx = -vx;
        }
        if (y < 0 || y > 600) {
            vy = -vy;
        }
    }
};

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Create a window with resolution 800x600
    sf::Vector2u resolution(800, 600);
    sf::RenderWindow window(sf::VideoMode(resolution), "Particle Simulator");

    // Create particles
    std::vector<Particle> particles;
    for (int i = 0; i < 50; ++i) {
        particles.emplace_back(rand() % 800, rand() % 600);
    }

    while (window.isOpen()) {
        // Handle events
        std::optional<sf::Event> event;
        while ((event = window.pollEvent())) {
            // Close the window when the event type is Closed
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        // Update particles
        float dt = 0.1f; // Fixed time step
        for (auto& particle : particles) {
            particle.update(dt);
        }

        // Render particles
        window.clear(sf::Color::Black);
        for (const auto& particle : particles) {
            sf::CircleShape shape(5); // A circle with a radius of 5
            shape.setPosition(sf::Vector2f(particle.x, particle.y));
            shape.setFillColor(sf::Color::White);
            window.draw(shape);
        }

        window.display();
    }

    return 0;
}
