#include <rendering/window.h>
#include <example/randomsystem.h>
#include <particlesystem/particlesystem.h>

#include <fmt/format.h>

#include <cmath>
#include <vector>
#include <random>
#include <algorithm>

/*
 * This application represents the "Client" the uses your particle system
 */
int main() try {
    rendering::Window window("Particle System v0.0.2 pre-release alpha", 850, 850);

    // Creates an example system with a 1000 particles.
    // This should be replaced with your new particle system
    const size_t numParticles = 1000;
    example::RandomSystem system{numParticles};

    float speed = 1.0f;
    bool running = true;

    while (running) {
        window.beginFrame();

        // Update the example system.
        // This should be replaced with a call to the new particle system
        system.update(window.time(), speed);

        // Clear screen with color
        window.clear({0, 0, 0, 1});

        // Draw particles
        // This should also be adjusted to work with the new particle system
        window.drawPoints(system.getPosition(), system.getSize(), system.getColor());

        // Example user interface, extend for your needs.
        // Might add controls for effects and emitters etc.
        {
            window.beginGuiWindow("UI");
            window.text("I'm text!");
            window.sliderFloat("Speed", speed, 0.001f, 10.0f);
            if (window.button("Close application")) {
                running = false;
            }
            window.endGuiWindow();
        }

        window.endFrame();
        running = running && !window.shouldClose();
    }

    return EXIT_SUCCESS;
} catch (const std::exception& e) {
    fmt::print("{}", e.what());
    return EXIT_FAILURE;
}
