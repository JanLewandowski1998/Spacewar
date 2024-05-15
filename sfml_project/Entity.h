#pragma                 once

#include                <SFML/Graphics.hpp>
#include                <SFML/System.hpp>
#include                <SFML/Window.hpp>

#include                <unordered_map>
#include                <filesystem>
#include                <iostream>
#include                <string>
#include                <vector>
#include                <cmath>

#include                <Utilities.h>

typedef unsigned int    uint;
typedef std::string     string;
typedef sf::Vector2f    v2f;
typedef sf::Vector2i    v2i;
typedef sf::Vector2u    v2u;

class Entity
{
public:
    string name;
    uint id;
    static uint id_counter;

    sf::Texture texture;
    sf::Sprite sprite;

    std::vector<sf::Texture> additional_textures;
    std::vector<sf::Sprite> additional_sprites;

    float mass;
    v2f total_force_vector;
    v2f velocity_vector;
    float max_velocity = 260.00f;

    float rotation_speed;

public:
    Entity(const v2f position = v2f(0.0f, 0.0f), const float mass = 10.0f, const float rotation_speed = 15.0f)
    {
        this->sprite.setPosition(position);
        this->mass = mass;
        this->rotation_speed = rotation_speed;
    }

    bool draw(sf::RenderWindow& window)
    {
        sf::RenderStates states;
        states.blendMode = sf::BlendAlpha;
        for (int i = this->additional_sprites.size() - 1; i >= 0; i--)
        {
            window.draw(this->additional_sprites[i], states);
        }
        window.draw(this->sprite, states);
        return true;
    }

    bool draw(sf::RenderWindow& window, const std::vector<int> order)
    {
        sf::RenderStates states;
        states.blendMode = sf::BlendAlpha;
        for (int i = 0; i < order.size(); i++)
        {
            int index = order[i];
            if (index >= 0 && index < this->additional_sprites.size())
            {
                window.draw(this->additional_sprites[index], states);
            }
            else if (index == -1)
            {
                window.draw(this->sprite, states);
            }
            else
            {
                return false;
            }
        }
        return true;
    }

    bool set_position(const v2f position)
    {
        for (int i = this->additional_sprites.size() - 1; i >= 0; i--)
        {
            this->additional_sprites[i].setPosition(position);
        }
        this->sprite.setPosition(position);
        return true;
    }

    bool center_origin()
    {
        for (int i = this->additional_sprites.size() - 1; i >= 0; i--)
        {
            sf::FloatRect bounds = this->additional_sprites[i].getLocalBounds();
            this->additional_sprites[i].setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
        }
        sf::FloatRect bounds = this->sprite.getLocalBounds();
        this->sprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
        return true;
    }

    bool set_velocity_vector(const v2f velocity_vector)
    {
        this->velocity_vector = velocity_vector;
        return true;
    }

    bool rotate(const sf::Time elapsed, const float rotation_speed, const enum __Rotations__ rotation_direction, const int index = -2)
    {
        float rotation = rotation_speed * elapsed.asSeconds() * ((int)rotation_direction * 2 - 1) * (-1);

        if (index == -2)
        {
            for (int i = this->additional_sprites.size() - 1; i >= 0; i--)
            {
                this->additional_sprites[i].rotate(rotation);
            }
            this->sprite.rotate(rotation);
        }
        else if (index == -1)
        {
            this->sprite.rotate(rotation);
        }
        else if (index < this->additional_sprites.size())
        {
            this->additional_sprites[index].rotate(rotation);
        }
        else
            return false;
        return true;
    }

    bool rotate(const sf::Time elapsed, const enum __Rotations__ rotation_direction, const int index = -2)
    {
        return this->rotate(elapsed, this->rotation_speed, rotation_direction, index);
    }

    bool load_texture(const string path)
    {
        if (this->texture.loadFromFile(path) == false)
        {
            std::cerr << "ERROR: Failed to load '" << name << "' (id=" << this->id << ") texture.\n";
            return false;
        }
        this->sprite.setTexture(this->texture);
        return true;
    }

    bool load_additional_texture(const string path)
    {
        const uint last = this->additional_textures.size();

        sf::Texture new_texture;
        this->additional_textures.push_back(new_texture);

        if (additional_textures[last].loadFromFile(path) == false)
        {
            std::cerr << "ERROR: Failed to load additional '" << name << "' (id=" << this->id << ") texture.\n";
            return false;
        }

        sf::Sprite new_sprite;
        this->additional_sprites.push_back(new_sprite);
        this->additional_sprites[last].setTexture(this->additional_textures[last]);

        this->additional_sprites[last].setPosition(this->sprite.getPosition());

        return true;
    }

    bool set_scale(const v2f scale)
    {
        for (int i = this->additional_sprites.size() - 1; i >= 0; i--)
        {
            this->additional_sprites[i].setScale(scale);
        }
        this->sprite.setScale(scale);
        return true;
    }

    bool set_scale(const float scale)
    {
        for (int i = this->additional_sprites.size() - 1; i >= 0; i--)
        {
            this->additional_sprites[i].setScale(v2f(scale, scale));
        }
        this->sprite.setScale(v2f(scale, scale));
        return true;
    }

    float get_average_scale()
    {
        v2f scale = this->sprite.getScale();
        if (scale.x == scale.y)
        {
            return scale.x;
        }
        else
        {
            return (scale.x + scale.y) / 2.0f;
        }
    }

    bool set_additional_sprite_scale(const float scale, const uint index)
    {
        uint size = this->additional_sprites.size();
        if (index < size)
        {
            this->additional_sprites[size - 1].setScale(v2f(scale, scale));
        }
        else
        {
            return false;
        }
        return true;
    }

    float calculate_distance(const Entity& other)
    {
        v2f position_1 = this->sprite.getPosition();
        v2f position_2 = other.sprite.getPosition();
        float distance = std::sqrt((position_1.x - position_2.x) * (position_1.x - position_2.x) +
            (position_1.y - position_2.y) * (position_1.y - position_2.y));
        return distance;
    }

    v2f calculate_gravity_force_vector(const Entity& other)
    {
        v2f force;

        float r = calculate_distance(other);
        if (r == 0.0f)
            return v2f(0.0f, 0.0f);

        float magnitude = __gravitational_constant__ * other.mass * this->mass / (std::pow(r, 2.0f));

        float d_x = other.sprite.getPosition().x - this->sprite.getPosition().x;
        float d_y = other.sprite.getPosition().y - this->sprite.getPosition().y;

        float theta = std::atan2(d_y, d_x);

        float force_x = magnitude * std::cos(theta);
        float force_y = magnitude * std::sin(theta);

        force = v2f(force_x, force_y);

        return force;
    }

    float calculate_velocity_magnitude()
    {
        return std::sqrt(this->velocity_vector.x * this->velocity_vector.x +
            this->velocity_vector.y * this->velocity_vector.y);
    }

    bool update_velocity(sf::Time t)
    {
        float acceeration_x = this->total_force_vector.x / this->mass;
        float acceeration_y = this->total_force_vector.y / this->mass;

        float delta_velocity_x = acceeration_x * t.asSeconds();
        float delta_velocity_y = acceeration_y * t.asSeconds();

        this->velocity_vector += v2f(delta_velocity_x, delta_velocity_y);

        float magnitude = this->calculate_velocity_magnitude();
        if (magnitude > this->max_velocity)
            this->velocity_vector = this->velocity_vector * (this->max_velocity / magnitude);

        return true;
    }

    bool update_position(sf::Time t)
    {
        float delta_x = this->velocity_vector.x * t.asSeconds();
        float delta_y = this->velocity_vector.y * t.asSeconds();

        this->sprite.move(v2f(delta_x, delta_y));

        return true;
    }

    bool calculate_force_and_update_velocity(const Entity& attractor, const sf::Time elapsed)
    {
        v2f force_from_attractor = this->calculate_gravity_force_vector(attractor);
        this->total_force_vector = force_from_attractor;

        this->update_velocity(elapsed);
        this->update_position(elapsed);

        return true;
    }

    bool teleport(const v2u window_size)
    {
        v2f entity_sprite_position = this->sprite.getPosition();
        if (entity_sprite_position.x > window_size.x)
            this->sprite.setPosition(0, entity_sprite_position.y); // Move to the left side
        else if (entity_sprite_position.x < 0)
            this->sprite.setPosition(window_size.x, entity_sprite_position.y); // Move to the right side
        if (entity_sprite_position.y > window_size.y)
            this->sprite.setPosition(entity_sprite_position.x, 0); // Move to the top
        else if (entity_sprite_position.y < 0)
            this->sprite.setPosition(entity_sprite_position.x, window_size.y); // Move to the bottom

        return true;
    }
};
uint Entity::id_counter = 0;