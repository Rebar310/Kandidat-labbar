#pragma once


#include <glm/glm.hpp>
#include <vector>
#include <memory>
# include <random>

// Test kommentar

// =======================
// Particle Representation
// =======================
struct Particle {
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec4 color;
    float size;
    float lifetime;
};

// =======================
// Emitter Interface
// =======================
class Emitter {
public:
    virtual ~Emitter() = default;

    // Spawn new particles
    virtual void emit(std::vector<Particle>& particles, float dt) = 0;
};

// =======================
// Effect Interface
// =======================
class Effect {
public:
    virtual ~Effect() = default;

    // Modify existing particles
    virtual void apply(std::vector<Particle>& particles, float dt) = 0;
};

// Uniform Emitter Defenition
// =======================

class UniformEmitter : public Emitter {
public:
    UniformEmitter(glm::vec2 position, float spawnRate, float speed);

    void emit(std::vector<Particle>& particles, float dt) override;

private:
    glm::vec2 position;
    float spawnRate;
    float speed;

    float accumulator = 0.0f;

    std::mt19937 gen;
    std::uniform_real_distribution<float> angleDist;
    std::uniform_real_distribution<float> lifetimeDist;
};

// Gravity Effect Defenition
// =======================

class GravityEffect : public Effect {
public:
    GravityEffect(glm::vec2 center, float strength);

    void apply(std::vector<Particle>& particles, float dt) override;

private:
    glm::vec2 center;
    float strength;
};

// Directional Emitter Defenition
// =======================

class DirectionalEmitter : public Emitter {
public:
    DirectionalEmitter(glm::vec2 position, glm::vec2 direction, float spawnRate, float speed);

    void emit(std::vector<Particle>& particles, float dt) override;

private:
    glm::vec2 position;
    glm::vec2 direction;
    float spawnRate;
    float speed;

    float accumulator = 0.0f;

    std::mt19937 gen; // for random partcle pattern generation each time
    std::uniform_real_distribution<float> lifetimeDist;
};

// Wind Effect Defenition
// =======================

class WindEffect : public Effect {
public:
    WindEffect(glm::vec2 force);

    void apply(std::vector<Particle>& particles, float dt) override;

private:
    glm::vec2 force;
};



// =======================
// Particle System
// =======================
class ParticleSystem {
public:
    ParticleSystem(size_t maxParticles);

    void update(double time, float speed);

    void addEmitter(std::unique_ptr<Emitter> emitter);
    void addEffect(std::unique_ptr<Effect> effect);

    const std::vector<glm::vec2>& getPosition() const;
    const std::vector<float>& getSize() const;
    const std::vector<glm::vec4>& getColor() const;

private:
    std::vector<Particle> particles;

    std::vector<std::unique_ptr<Emitter>> emitters;
    std::vector<std::unique_ptr<Effect>> effects;

    size_t maxParticles;
    double prevTime = 0.0;

    // Cached rendering buffers
    std::vector<glm::vec2> positions;
    std::vector<float> sizes;
    std::vector<glm::vec4> colors;

    void updateRenderBuffers();
};
