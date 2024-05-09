#pragma                 once

#include                <Entity.h>

typedef unsigned int    uint;
typedef std::string     string;
typedef sf::Vector2f    v2f;
typedef sf::Vector2i    v2i;
typedef sf::Vector2u    v2u;

class Missile : public Entity
{
public:
    float velocity_default_magnitude = 500.0f;
    float theta;
    float theta_radians;
    static sf::Texture texture;

    sf::Time flickering_period = sf::seconds(0.075f);
    sf::Clock flickering_clock;

public:
    Missile(const v2f position = v2f(0.0f, 0.0f), float theta = 0.0f,  const v2f velocity_vector = v2f(0.0f, 0.0f))
    {
        this->sprite.setPosition(position);
        this->sprite.setTexture(Missile::texture);
        this->id = Missile::id_counter;
        this->name = "Missile_" + std::to_string(Missile::id_counter++);
        this->mass = 0.0f;

        this->theta = theta;
        this->theta_radians = this->theta * __pi__ / 180.0f;
        this->velocity_vector.x = this->velocity_default_magnitude * std::cos(this->theta_radians);
        this->velocity_vector.y = this->velocity_default_magnitude * std::sin(this->theta_radians);

        // Apply rotation:
        this->center_origin();
        this->sprite.setRotation(theta + 90.0f);

        this->flickering_clock.restart();
    }

    Missile(const v2f position = v2f(0.0f, 0.0f), const float theta = 0.0f)
    {
        this->sprite.setPosition(position);
        this->sprite.setTexture(Missile::texture);
        this->id = Missile::id_counter;
        this->name = "Missile_" + std::to_string(Missile::id_counter++);
        this->theta = theta;
        this->mass = 0.0f;
    }

    static bool load_texture(const string path)
    {
        if (Missile::texture.loadFromFile(path) == false)
        {
            std::cerr << "ERROR: Failed to load Missile texture.\n";
            return false;
        }
        return true;
    }

    bool set_sprite_texture()
    {
        this->sprite.setTexture(Missile::texture);
        return true;
    }
};
sf::Texture Missile::texture;