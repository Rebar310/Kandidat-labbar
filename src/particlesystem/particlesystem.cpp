#include <particlesystem/particlesystem.h>


#include <algorithm>

ParticleSystem::ParticleSystem(size_t maxParticles) : maxParticles(maxParticles) {}

void ParticleSystem::addEmitter(std::unique_ptr<Emitter> emitter) {
    emitters.push_back(std::move(emitter));
}

void ParticleSystem::addEffect(std::unique_ptr<Effect> effect) {
    effects.push_back(std::move(effect));
}

void ParticleSystem::update(double time, float speed) {
    float dt = static_cast<float>((time - prevTime) * speed);
    prevTime = time;

    // 1. Generation
    for (auto& emitter : emitters) emitter->emit(particles, dt);

    // Limit particle count
    if (particles.size() > maxParticles) particles.resize(maxParticles);

    // 2. Apply effects
    for (auto& effect : effects) effect->apply(particles, dt);

    // 3. Integrate motion + reduce lifetime
    for (auto& p : particles) {
        p.position += p.velocity * dt;
        p.lifetime -= dt;
    }

    // 4. Remove dead particles
    particles.erase(std::remove_if(particles.begin(), particles.end(),
                                   [](const Particle& p) { return p.lifetime <= 0.0f; }),
                    particles.end());

    updateRenderBuffers();
}

void ParticleSystem::updateRenderBuffers() {
    positions.clear();
    sizes.clear();
    colors.clear();

    for (const auto& p : particles) {
        positions.push_back(p.position);
        sizes.push_back(p.size);
        colors.push_back(p.color);
    }
}

const std::vector<glm::vec2>& ParticleSystem::getPosition() const { return positions; }

const std::vector<float>& ParticleSystem::getSize() const { return sizes; }

const std::vector<glm::vec4>& ParticleSystem::getColor() const { return colors; }

// Uniform Emitter Implementation
// =======================

UniformEmitter::UniformEmitter(glm::vec2 position, float spawnRate, float speed)
    : position(position)
    , spawnRate(spawnRate)
    , speed(speed)
    , gen(std::random_device{}()) // Generates random particle seed/ pattern for directions 
    , angleDist(0.0f, 2.0f * 3.1415926f) // Angle around the full circle for explosion effect
    , lifetimeDist(1.0f, 3.0f) {} // random lifetimes so particles die at the same time

void UniformEmitter::emit(std::vector<Particle>& particles, float dt) {
    accumulator += spawnRate * dt; // to spawn an integer number of particles 

    int particlesToSpawn = static_cast<int>(accumulator);
    accumulator -= particlesToSpawn;

    for (int i = 0; i < particlesToSpawn; ++i) {

        float angle = angleDist(gen);

        glm::vec2 velocity = speed * glm::vec2(std::cos(angle), std::sin(angle));

        Particle p;
        p.position = position;
        p.velocity = velocity;
        p.color = glm::vec4(1.0f, 0.5f, 0.1f, 1.0f);
        p.size = 5.0f;
        p.lifetime = lifetimeDist(gen);

        particles.push_back(p);
    }
}

// Gravity Effect Implementation
// =======================

GravityEffect::GravityEffect(glm::vec2 center, float strength)
    : center(center), strength(strength) {}

void GravityEffect::apply(std::vector<Particle>& particles, float dt) { // Function to modify particle velocities
    for (auto& p : particles) { // We iterate through every particle in the system

        glm::vec2 dir = center - p.position; // a vector pointing from the particle to the gravity center
        float distance = glm::length(dir) + 0.001f;  // avoid division by zero

        glm::vec2 force = strength * dir / (distance * distance); // F = strength × direction / r²

        p.velocity += force * dt; // newtons andra lag
    }
}

// Directional Emitter Implementation
// =======================

DirectionalEmitter::DirectionalEmitter(glm::vec2 position, glm::vec2 direction, float spawnRate,
                                       float speed)
    : position(position)
    , direction(glm::normalize(direction))
    , spawnRate(spawnRate)
    , speed(speed)
    , gen(std::random_device{}())
    , lifetimeDist(1.0f, 3.0f) {}

void DirectionalEmitter::emit(std::vector<Particle>& particles, float dt) {
    accumulator += spawnRate * dt;

    int particlesToSpawn = static_cast<int>(accumulator);
    accumulator -= particlesToSpawn;

    for (int i = 0; i < particlesToSpawn; ++i) {

        Particle p;
        p.position = position;
        p.velocity = direction * speed;
        p.color = glm::vec4(0.2f, 0.6f, 1.0f, 1.0f);
        p.size = 5.0f;
        p.lifetime = lifetimeDist(gen);

        particles.push_back(p);
    }
}

// Wind Effect Implementation
// =======================

WindEffect::WindEffect(glm::vec2 force) : force(force) {}

void WindEffect::apply(std::vector<Particle>& particles, float dt) {
    for (auto& p : particles) {
        p.velocity += force * dt;
    }
}
