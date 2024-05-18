#pragma                 once

#include                <Entity.h>
#include                <Missile.h>
#include                <Exhaust_Particle.h>

typedef unsigned int    uint;
typedef std::string     string;
typedef sf::Vector2f    v2f;
typedef sf::Vector2i    v2i;
typedef sf::Vector2u    v2u;

class Ship : public Entity
{
public:
    float thrust_force = 2500.0f;
    float current_thrust_force = 0.0f;

    uint health;
    uint health_max = 100;

    uint energy;
    uint energy_max = 100;

    float rotational_acceleration_value = 400.0f;
    std::unordered_map<__Rotations__, float> rotational_acceleration;
    std::unordered_map<__Rotations__, float> rotational_speed;

    std::vector<Missile> missiles;
    std::vector<Exhaust_Particle> exhaust_particles;
    std::vector<Exhaust_Particle> thrust_particles;

    sf::Time time_to_reach_full_rotation_speed = sf::seconds(0.5f);
    sf::Clock rotation_timer;

    sf::Time missile_cooldown = sf::seconds(0.12f);
    sf::Clock missile_cooldown_timer;

    sf::Time exhaust_particle_cooldown = sf::seconds(0.015f);
    sf::Clock exhaust_particle_cooldown_timer;
    sf::Time exhaust_partice_lifetime = sf::seconds(1.25f);

    sf::Time thrust_particle_cooldown = sf::seconds(0.000f);
    sf::Clock thrust_particle_cooldown_timer;
    sf::Time thrust_partice_lifetime = sf::seconds(0.7f);

    sf::Color missile_color = sf::Color(255, 255, 255);
    sf::Color exhaust_max_color_subtraction = sf::Color(128, 128, 128);
    sf::Color exhaust_color_tinting = sf::Color(0, 0, 0);
    enum class Exhaust_Partice_Colorization_Method {RANDOMIZED_SUBTRACTION, RANDOMIZED_TINTING, TINTING, TINTING_EXCEPT_WHITE};

public:
    Ship(string name = "", v2f const position = v2f(0.0f, 0.0f), const float mass = 10.0f, const float rotation_speed = 220.0f)
    {
        this->name = name;
        if (this->name == "")
            this->name = "Ship_" + std::to_string(Ship::id_counter);
        this->id = id_counter++;

        this->sprite.setPosition(position);
        this->rotation_speed = rotation_speed;
        this->max_velocity = 260.0f;

        this->mass = mass;

        this->health = this->health_max;
        this->energy = this->energy_max;

        rotation_timer.restart();
        missile_cooldown_timer.restart();
        exhaust_particle_cooldown_timer.restart();
        thrust_particle_cooldown_timer.restart();

        this->rotational_acceleration[__Rotations__::CLOCKWISE] = 0.0f;
        this->rotational_acceleration[__Rotations__::COUNTERCLOCKWISE] = 0.0f;
        this->rotational_speed[__Rotations__::CLOCKWISE] = 0.0f;
        this->rotational_speed[__Rotations__::COUNTERCLOCKWISE] = 0.0f;
    }

    bool draw_missiles(sf::RenderWindow& window)
    {
        for (uint i = 0; i < this->missiles.size(); i++)
        {
            this->missiles[i].draw(window);
        }
        return true;
    }

    bool draw_exhaust_particles(sf::RenderWindow& window)
    {
        for (uint i = 0; i < this->exhaust_particles.size(); i++)
        {
            this->exhaust_particles[i].draw(window);
        }
        for (uint i = 0; i < this->thrust_particles.size(); i++)
        {
            this->thrust_particles[i].draw(window);
        }
        return true;
    }

    bool draw(sf::RenderWindow& window)
    {
        this->draw_missiles(window);
        this->draw_exhaust_particles(window);
        this->Entity::draw(window);
        return true;
    }

    bool shoot_missile()
    {
        if (this->missile_cooldown_timer.getElapsedTime() > this->missile_cooldown)
        {
            this->missile_cooldown_timer.restart();

            v2f position = this->sprite.getPosition();
            float velocity_angle = this->sprite.getRotation();
            float rotation_angle_radians = (velocity_angle)*__pi__ / 180.0f;

            float offset_x = std::cos(rotation_angle_radians) * this->get_average_scale() * 0.7f * (this->sprite.getLocalBounds().width / 2.0f);
            float offset_y = std::sin(rotation_angle_radians) * this->get_average_scale() * 0.7f * (this->sprite.getLocalBounds().width / 2.0f);

            position += v2f(offset_x, offset_y);

            this->missiles.push_back(Missile(position,
                                             velocity_angle,
                                             this->velocity_vector)
            );

            this->missiles[this->missiles.size() - 1].set_scale(3.0f);
            this->missiles[this->missiles.size() - 1].sprite.setColor(this->missile_color);
        }
        
        return true;
    }

    bool colorize_exhaust_particle(Exhaust_Particle& particle, const Ship::Exhaust_Partice_Colorization_Method exhaust_partice_colorization_method = Ship::Exhaust_Partice_Colorization_Method::TINTING)
    {
        if (exhaust_partice_colorization_method == Ship::Exhaust_Partice_Colorization_Method::RANDOMIZED_SUBTRACTION)
        {
            sf::Color randomized_hue = sf::Color(255 - rand() % (this->exhaust_max_color_subtraction.r + 1),
                                                 255 - rand() % (this->exhaust_max_color_subtraction.g + 1),
                                                 255 - rand() % (this->exhaust_max_color_subtraction.b + 1));
            particle.sprite.setColor(randomized_hue);
        }
        else if (exhaust_partice_colorization_method == Ship::Exhaust_Partice_Colorization_Method::RANDOMIZED_TINTING)
        {
            float random_saturation = (rand() % (100 + 1)) / 100.0f;
            sf::Color randomized_hue = this->exhaust_color_tinting;
            randomized_hue.r = randomized_hue.r + ((255 - randomized_hue.r) * random_saturation);
            randomized_hue.g = randomized_hue.g + ((255 - randomized_hue.g) * random_saturation);
            randomized_hue.b = randomized_hue.b + ((255 - randomized_hue.b) * random_saturation);
            particle.sprite.setColor(randomized_hue);
        }
        else if (exhaust_partice_colorization_method == Ship::Exhaust_Partice_Colorization_Method::TINTING)
        {
            particle.sprite.setColor(this->exhaust_color_tinting);
        }
        else if (exhaust_partice_colorization_method == Ship::Exhaust_Partice_Colorization_Method::TINTING_EXCEPT_WHITE)
        {
            // TODO
            particle.sprite.setColor(this->exhaust_color_tinting);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool try_generate_exhaust_particle()
    {
        v2f position = this->sprite.getPosition();
        const float velocity_angle = this->sprite.getRotation() + 180.0f;
        const float rotation_angle_radians = (velocity_angle) * __pi__ / 180.0f;

        float random_angle_offset = (float)(rand() % 100) / 100.0f * 4.0f;
        random_angle_offset *= ((rand() % 2) * 2.0f) - 1.0f; //Clockwise or Counterclockwise 

        const float offset_x = std::cos(rotation_angle_radians) * this->get_average_scale() * 0.7f * (this->sprite.getLocalBounds().width / 2.0f);
        const float offset_y = std::sin(rotation_angle_radians) * this->get_average_scale() * 0.7f * (this->sprite.getLocalBounds().width / 2.0f);
        position += v2f(offset_x, offset_y);

        if (this->exhaust_particle_cooldown_timer.getElapsedTime() > this->exhaust_particle_cooldown)
        {
            this->exhaust_particle_cooldown_timer.restart();
            this->exhaust_particles.push_back(Exhaust_Particle(position,
                                                               velocity_angle + random_angle_offset,
                                                               this->velocity_vector,
                                                               true,
                                                               1));

            this->exhaust_particles[this->exhaust_particles.size() - 1].set_scale(3.0f);
            this->colorize_exhaust_particle(this->exhaust_particles[this->exhaust_particles.size() - 1], Ship::Exhaust_Partice_Colorization_Method::RANDOMIZED_TINTING);
        }
        if (this->thrust_particle_cooldown_timer.getElapsedTime() > this->thrust_particle_cooldown)
        {
            this->thrust_particle_cooldown_timer.restart();
            this->thrust_particles.push_back(Exhaust_Particle(position,
                                                              velocity_angle,
                                                              this->velocity_vector,
                                                              false,
                                                              1));

            this->thrust_particles[this->thrust_particles.size() - 1].set_scale(3.0f);
            //this->colorize_exhaust_particle(this->thrust_particles[this->thrust_particles.size() - 1], Ship::Exhaust_Partice_Colorization_Method::TINTING);
        }
        return true;
    }

    bool thrust_on()
    {
        this->try_generate_exhaust_particle();
        this->current_thrust_force = this->thrust_force;
        return true;
    }

    bool thrust_off()
    {
        this->current_thrust_force = 0.0f;
        return true;
    }

    bool side_acceeration_on(const enum __Rotations__ rotation_direction)
    {
        this->rotational_acceleration[rotation_direction] = this->rotational_acceleration_value;
        return true;
    }

    bool side_acceeration_off(const enum __Rotations__ rotation_direction)
    {
        this->rotational_acceleration[rotation_direction] = 0.0f;
        return true;
    }

    v2f calculate_thrust_force_vector()
    {
        float theta = this->sprite.getRotation();
        theta *= __pi__ / 180.0f;
        float force_x = this->current_thrust_force * std::cos(theta) * (1.0f);
        float force_y = this->current_thrust_force * std::sin(theta) * (1.0f);
        return v2f(force_x, force_y);
    }

    bool calculate_combined_force_and_update_velocity(const Entity& attractor, const sf::Time elapsed)
    {
        v2f force_from_attractor = this->calculate_gravity_force_vector(attractor);
        v2f force_from_thrust = this->calculate_thrust_force_vector();
        v2f total_force_vector = force_from_attractor + force_from_thrust;
        this->total_force_vector = total_force_vector;

        this->update_velocity(elapsed);
        this->update_position(elapsed);

        return true;
    }

    bool update_exhaust_particles(const sf::Time elapsed)
    {
        for (uint i = 0; i < this->exhaust_particles.size(); i++)
        {
            this->exhaust_particles[i].update_position(elapsed);
            this->exhaust_particles[i].rotate(elapsed, this->exhaust_particles[i].rotation_direction);
            sf::Time time = this->exhaust_particles[i].clock.getElapsedTime();
            float ratio = time.asSeconds() / this->exhaust_partice_lifetime.asSeconds();
            if (ratio > 1)
            {
                this->exhaust_particles.erase(this->exhaust_particles.begin() + i);
                i--;
                continue;
            }
            else
            {
                uint r, g, b;
                r = this->exhaust_particles[i].sprite.getColor().r;
                g = this->exhaust_particles[i].sprite.getColor().g;
                b = this->exhaust_particles[i].sprite.getColor().b;
                this->exhaust_particles[i].sprite.setColor(sf::Color(r, g, b, 255 - (int)(ratio * 255)));
                this->exhaust_particles[i].sprite.setScale(this->exhaust_particles[i].sprite.getScale() * (1.0f + this->exhaust_particles[i].size_increase_rate * elapsed.asSeconds()));
            }
        }

        for (uint i = 0; i < this->thrust_particles.size(); i++)
        {
            this->thrust_particles[i].update_position(elapsed);
            //this->thrust_particles[i].rotate(elapsed, this->thrust_particles[i].rotation_speed * 3.0f, this->thrust_particles[i].rotation_direction);
            sf::Time time = this->thrust_particles[i].clock.getElapsedTime();
            float ratio = time.asSeconds() / this->thrust_partice_lifetime.asSeconds();
            if (ratio > 1)
            {
                this->thrust_particles.erase(this->thrust_particles.begin() + i);
                i--;
                continue;
            }
            else
            {
                uint r, g, b;
                r = this->thrust_particles[i].sprite.getColor().r;
                g = this->thrust_particles[i].sprite.getColor().g;
                b = this->thrust_particles[i].sprite.getColor().b;
                this->thrust_particles[i].sprite.setColor(sf::Color(r, g, b, 255 - (int)(ratio * 255)));
                //this->thrust_particles[i].sprite.setScale(this->thrust_particles[i].sprite.getScale() * 1.00f);
            }
        }
        return true;
    }

    bool update_missiles(const sf::Time elapsed)
    {
        for (uint i = 0; i < this->missiles.size(); i++)
        {
            this->missiles[i].update_position(elapsed);
            if (this->missiles[i].flickering_clock.getElapsedTime() > this->missiles[i].flickering_period)
            {
                float scale_x = this->missiles[i].sprite.getScale().x;
                float scale_y = this->missiles[i].sprite.getScale().y;
                this->missiles[i].sprite.setScale(scale_x * (-1.0f), scale_y);
                this->missiles[i].flickering_clock.restart();
            }
        }
        return true;
    }

    bool update_rotation(const sf::Time elapsed)
    {
        float delta_clockwise, delta_counterclockwise;
        delta_clockwise = this->rotational_acceleration[__Rotations__::CLOCKWISE] * elapsed.asSeconds();
        delta_counterclockwise = this->rotational_acceleration[__Rotations__::COUNTERCLOCKWISE] * elapsed.asSeconds();

        this->rotational_speed[__Rotations__::CLOCKWISE] = this->rotational_speed[__Rotations__::CLOCKWISE] + delta_clockwise;
        this->rotational_speed[__Rotations__::CLOCKWISE] = this->rotational_speed[__Rotations__::CLOCKWISE] - delta_counterclockwise;
        this->rotational_speed[__Rotations__::COUNTERCLOCKWISE] = this->rotational_speed[__Rotations__::COUNTERCLOCKWISE] + delta_counterclockwise;
        this->rotational_speed[__Rotations__::COUNTERCLOCKWISE] = this->rotational_speed[__Rotations__::COUNTERCLOCKWISE] - delta_clockwise;

        this->rotational_speed[__Rotations__::CLOCKWISE] = std::min(this->rotation_speed, this->rotational_speed[__Rotations__::CLOCKWISE]);
        this->rotational_speed[__Rotations__::CLOCKWISE] = std::max(0.0f, this->rotational_speed[__Rotations__::CLOCKWISE]);
        this->rotational_speed[__Rotations__::COUNTERCLOCKWISE] = std::min(this->rotation_speed, this->rotational_speed[__Rotations__::COUNTERCLOCKWISE]);
        this->rotational_speed[__Rotations__::COUNTERCLOCKWISE] = std::max(0.0f, this->rotational_speed[__Rotations__::COUNTERCLOCKWISE]);

        this->rotate(elapsed, this->rotational_speed[__Rotations__::CLOCKWISE], __Rotations__::CLOCKWISE, -1);
        this->rotate(elapsed, this->rotational_speed[__Rotations__::COUNTERCLOCKWISE], __Rotations__::COUNTERCLOCKWISE, -1);

        return true;
    }
};