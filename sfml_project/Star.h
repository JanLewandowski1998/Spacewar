#pragma                 once

#include                <Entity.h>

typedef unsigned int    uint;
typedef std::string     string;
typedef sf::Vector2f    v2f;
typedef sf::Vector2i    v2i;
typedef sf::Vector2u    v2u;

class Star : public Entity
{
public:
    float halo_rotation_speed;
    float radius = 0.0f;

public:
    Star(const v2f position = v2f(0.0f, 0.0f), const float mass = 400000.0f /*35000*/, const float rotation_speed = 20.0f, const float halo_rotation_speed = 7.5f)
    {
        this->sprite.setPosition(position);
        this->mass = mass;
        this->name = "Star";
        this->id = Star::id_counter++;
        this->rotation_speed = rotation_speed;
        this->halo_rotation_speed = halo_rotation_speed;
    }

    bool adjust_radius()
    {
        this->radius = this->sprite.getGlobalBounds().width / 2.0f;
        return true;
    }
};