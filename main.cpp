#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <random>

class Particle {
public:
    float x, y;
    float vx, vy;

    // Constructor with random velocity
    Particle(float startX, float startY)
        : x(startX), y(startY), 
          vx((rand() % 100) / 25.0f - 2.0f), 
          vy((rand() % 100) / 25.0f - 2.0f) 
    {}

    void update(float dt) {
        // Update position
        x += vx * dt;
        y += vy * dt;

        // Bounce around within an 800x600 window
        if (x < 0 || x > 1900) {
            vx = -vx;
        }
        if (y < 0 || y > 900) {
            vy = -vy;
        }
    }

    // Calculate the color based on speed
    sf::Color getColor() const {
        float speed = std::sqrt(vx * vx + vy * vy); // Compute speed (magnitude of velocity)
        float maxSpeed = 9.0f; // Define maximum speed (adjust to fit your simulation)

        // Normalize speed to the range [0, 1]
        float t = std::min(speed / maxSpeed, 1.0f);

        uint8_t r = static_cast<uint8_t>(300 * t);      
        uint8_t g = static_cast<uint8_t>(250 * t);      
        uint8_t b = static_cast<uint8_t>(180 * t);            

        return sf::Color(r, g, b);
    }
};

bool areColliding(const Particle& p1, const Particle& p2) {
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    float distance = std::sqrt(dx * dx + dy * dy);
    return distance < 80.0f; // Radius1 + Radius2 = 10 + 10
};

void resolveCollision(Particle& p1, Particle& p2) {
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    float distance = std::sqrt(dx * dx + dy * dy);
    if (distance == 0) return;

    float overlap = 80.0f - distance;  // Total overlap

    // Normalize collision vector
    float nx = dx / distance;
    float ny = dy / distance;

    // Separate particles
    p1.x += nx * (overlap / 2);
    p1.y += ny * (overlap / 2);
    p2.x -= nx * (overlap / 2);
    p2.y -= ny * (overlap / 2);

    // Exchange velocities
    std::swap(p1.vx, p2.vx);
    std::swap(p1.vy, p2.vy);
};

static std::random_device rd;
static std::mt19937 gen(rd()); // Mersenne Twister RNG
std::uniform_real_distribution<float> radiusDist(5.0f, 15.0f); // Random radius range [5.0, 15.0]

int main() {

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Create a window with resolution 1500x800

    sf::Vector2u resolution(1900, 900);

    sf::RenderWindow window(sf::VideoMode(resolution), "Particle Simulator");

    // Create particles

    std::vector<Particle> particles;

    for (int i = 0; i < 50; ++i) {

        particles.emplace_back(rand() % 1900, rand() % 900);

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

        float dt = 0.5f;
        for (auto& particle : particles) {
            particle.update(dt);
            // Boundary collisions
            if (particle.x < 10.0f) {
                particle.x = 10.0f;
                particle.vx = -particle.vx;
            }
            if (particle.x > 1850.0f) {
                particle.x = 1850.0f;
                particle.vx = -particle.vx;
            }
            if (particle.y < 10.0f) {
                particle.y = 10.0f;
                particle.vy = -particle.vy;
            }
            if (particle.y > 850.0f) {
                particle.y = 850.0f;
                particle.vy = -particle.vy;
            }
        }

        // Check and resolve particle collisions
        for (size_t i = 0; i < particles.size(); ++i) {
            for (size_t j = i + 1; j < particles.size(); ++j) {
                if (areColliding(particles[i], particles[j])) {
                    resolveCollision(particles[i], particles[j]);
                }
            }
        }

        // Render particles

        window.clear(sf::Color::Black);

        for (const auto& particle : particles) {

            sf::CircleShape shape(40); // A circle with a radius of 10

            shape.setPosition(sf::Vector2f(particle.x, particle.y));

            shape.setFillColor(particle.getColor());

            window.draw(shape);

        }

        for (size_t i = 0; i < particles.size(); ++i) {
        for (size_t j = i + 1; j < particles.size(); ++j) {
        float dx = particles[j].x - particles[i].x;
        float dy = particles[j].y - particles[i].y;
        float dist = std::sqrt(dx * dx + dy * dy);

        if (dist < 80.0f) { // Assuming particles have a radius of 10
            std::swap(particles[i].vx, particles[j].vx);
            std::swap(particles[i].vy, particles[j].vy);
        }
    }
}

        window.display();

    }

    return 0;

}