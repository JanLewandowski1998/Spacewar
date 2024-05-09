#pragma                 once

#include                <Entity.h>

typedef unsigned int    uint;
typedef std::string     string;
typedef sf::Vector2f    v2f;
typedef sf::Vector2i    v2i;
typedef sf::Vector2u    v2u;

class Exhaust_Particle : public Entity
{
public:
    float velocity_default_magnitude = 250.0f;
    static sf::Texture texture;

    float theta;
    float theta_radians;
    enum __Rotations__ rotation_direction;

    float size_increase_rate = 1.1f;

    sf::Clock clock;

public:
    Exhaust_Particle(const v2f position = v2f(0.0f, 0.0f), const float theta = 0.0f, const v2f ship_velocity_vector = v2f(0.0f, 0.0f))
    {
        this->sprite.setPosition(position);
        this->sprite.setTexture(Exhaust_Particle::texture);
        this->id = Exhaust_Particle::id_counter;
        this->name = "Exhaust_Particle_" + std::to_string(Exhaust_Particle::id_counter++);
        this->mass = 0.0f;

        this->theta = theta;
        this->theta_radians = this->theta * __pi__ / 180.0f;
        this->velocity_vector.x = this->velocity_default_magnitude * std::cos(this->theta_radians);
        this->velocity_vector.y = this->velocity_default_magnitude * std::sin(this->theta_radians);
        this->velocity_vector += ship_velocity_vector * 0.5f;

        // Apply random rotation:
        this->center_origin();
        float random_rotation = (float)(rand() % 360 + 1);
        this->sprite.setRotation(random_rotation);

        this->rotation_speed = 90.0f;

        rotation_direction = __Rotations__(rand() % 2);
        this->clock.restart();
    }

    static bool load_texture(const string path)
    {
        if (Exhaust_Particle::texture.loadFromFile(path) == false)
        {
            std::cerr << "ERROR: Failed to load Exhaust Particle texture.\n";
            return false;
        }
        return true;
    }

    bool set_sprite_texture()
    {
        this->sprite.setTexture(Exhaust_Particle::texture);
        return true;
    }
};
sf::Texture Exhaust_Particle::texture;