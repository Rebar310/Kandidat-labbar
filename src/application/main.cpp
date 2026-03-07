#include <rendering/window.h>
// #include <example/randomsystem.h>
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
    //example::RandomSystem system{numParticles};

    // 1. My particle system
    ParticleSystem system{numParticles};  

    float speed = 1.0f;
    bool running = true;
    // 2. Adding selection state variables for emitters/effects
    int selectedEmitter = 0;  // 0 = Uniform, 1 = Directional
    int selectedEffect = 0;   // 0 = Gravity, 1 = Wind
    int prevEmitter = -1;
    int prevEffect = -1;

    while (running) {
        window.beginFrame();

        // ===== Rebuild system if selection changed =====
        if (selectedEmitter != prevEmitter || selectedEffect != prevEffect) {
            // Create a new clean system
            system = ParticleSystem(numParticles);

            // Add selected emitter 
            if (selectedEmitter == 0) {
                system.addEmitter(
                    std::make_unique<UniformEmitter>(glm::vec2(0.0f, 0.0f),// spawn position
                        200.0f,// spawnrate
                        0.5f));//speed
                // std::make_unique creates an emitter of type unique_ptr<Emitter> since my particlesys. stores that type
                // unique_ptr<Emitter> is equivalent to new UniformEmitter(...) but safer. 
                //Automatically deleted when no longer needed + Only one owner
                
            } else {
                system.addEmitter(std::make_unique<DirectionalEmitter>(
                    glm::vec2(-0.5f, 0.0f), glm::vec2(1.0f, 0.0f), 200.0f, 0.7f));
            }

            // Add selected effect
            if (selectedEffect == 0) {
                system.addEffect(std::make_unique<GravityEffect>(glm::vec2(0.0f, 0.0f), 1.5f));
            } else {
                system.addEffect(std::make_unique<WindEffect>(glm::vec2(0.2f, 0.0f)));
            }

            prevEmitter = selectedEmitter;
            prevEffect = selectedEffect;
        }

        // Now update normally
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
            // ===== Emitter Selection =====
            window.text("Emitter Type");

            if (window.button("Uniform")) {
                selectedEmitter = 0;
            }
            if (window.button("Directional")) {
                selectedEmitter = 1;
            }

            // ===== Effect Selection =====
            window.text("Effect Type");

            if (window.button("Gravity")) {
                selectedEffect = 0;
            }
            if (window.button("Wind")) {
                selectedEffect = 1;
            }

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
