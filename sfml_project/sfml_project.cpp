#include                <SFML/Graphics.hpp>
#include                <SFML/System.hpp>
#include                <SFML/Window.hpp>

#include                <filesystem>
#include                <iostream>
#include                <string>
#include                <vector>
#include                <cmath>

#include                <Missile.h>
#include                <Ship.h>
#include                <Star.h>
#include                <Utilities.h>

typedef unsigned int    uint;
typedef std::string     string;
typedef sf::Vector2f    v2f;
typedef sf::Vector2i    v2i;
typedef sf::Vector2u    v2u;

const string initialize(sf::RenderWindow& window)
{
    std::filesystem::path current_path = std::filesystem::current_path();
    const string current_path_string = current_path.string();
    std::cout << "Current path: '" << current_path_string << "'\n";

    __window_center__ = v2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

    return current_path_string;
}

bool handle_inputs(sf::RenderWindow& window, Ship& player_1, Ship& player_2, const sf::Time elapsed)
{
    if (window.hasFocus() == false)
    {
        return true;
    }

    // Blue player input handling:
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        player_1.thrust_on();
    }
    else
    {
        player_1.thrust_off();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        //player_1.rotate_gradually(elapsed, __Rotations__::COUNTERCLOCKWISE);
        player_1.side_acceeration_on(__Rotations__::COUNTERCLOCKWISE);
    }
    else
    {
        player_1.side_acceeration_off(__Rotations__::COUNTERCLOCKWISE);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        //player_1.rotate_gradually(elapsed, __Rotations__::CLOCKWISE);
        player_1.side_acceeration_on(__Rotations__::CLOCKWISE);
    }
    else
    {
        player_1.side_acceeration_off(__Rotations__::CLOCKWISE);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
    {
        player_1.rotational_speed[__Rotations__::CLOCKWISE] = 0.0f;
        player_1.rotational_speed[__Rotations__::COUNTERCLOCKWISE] = 0.0f;
        player_1.side_acceeration_off(__Rotations__::CLOCKWISE);
        player_1.side_acceeration_off(__Rotations__::COUNTERCLOCKWISE);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        player_1.shoot_missile();
    }

    // Red player input handing:
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        player_2.thrust_on();
    }
    else
    {
        player_2.thrust_off();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        //player_2.rotate_gradually(elapsed, __Rotations__::COUNTERCLOCKWISE);
        player_2.side_acceeration_on(__Rotations__::COUNTERCLOCKWISE);
    }
    else
    {
        player_2.side_acceeration_off(__Rotations__::COUNTERCLOCKWISE);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        //player_2.rotate_gradually(elapsed, __Rotations__::CLOCKWISE);
        player_2.side_acceeration_on(__Rotations__::CLOCKWISE);
    }
    else
    {
        player_2.side_acceeration_off(__Rotations__::CLOCKWISE);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
    {
        player_2.rotational_speed[__Rotations__::CLOCKWISE] = 0.0f;
        player_2.rotational_speed[__Rotations__::COUNTERCLOCKWISE] = 0.0f;
        player_2.side_acceeration_off(__Rotations__::CLOCKWISE);
        player_2.side_acceeration_off(__Rotations__::COUNTERCLOCKWISE);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        player_2.shoot_missile();
    }

    return true;
}

bool game(sf::RenderWindow& window)
{
    // Initialize the window and path:
    const string current_path_string = initialize(window);

    // Game speed:
    const float game_speed = 1.0f;

    // Initialize Blue Player:
    v2f player_1_position = v2f(window.getSize().x * 0.1f, window.getSize().y * 0.1f);
    Ship player_1("Alice", player_1_position);
    //player_1.load_texture(current_path_string + "\\Textures\\Ships\\ship_default.png");
    //player_1.load_texture(current_path_string + "\\Textures\\Ships\\ship_rock_raiders.png");
    //player_1.load_texture(current_path_string + "\\Textures\\Ships\\ship_blacktron.png");
    //player_1.load_texture(current_path_string + "\\Textures\\Ships\\ship_power_miners.png");
    //player_1.load_texture(current_path_string + "\\Textures\\Ships\\ship_x.png");
    //player_1.load_texture(current_path_string + "\\Textures\\Ships\\ship_angel_2.png");
    player_1.load_texture(current_path_string + "\\Textures\\Ships\\ship_millenium_falcon.png");
    player_1.missile_color = sf::Color(67, 255, 255); // Cyan
    //player_1.missile_color = sf::Color(238, 215, 50); // Blacktron
    //player_1.missile_color = sf::Color(52, 205, 108); // Spyrius / Rock Raiders / Power Miners
    //player_1.missile_color = sf::Color(91, 247, 193); // Angel
    //player_1.missile_color = sf::Color(181, 59, 61); // Falcon
    player_1.exhaust_color_tinting = player_1.missile_color;
    player_1.set_scale(3.0f);
    player_1.center_origin();

    // Initialize Red Player :
    v2f player_2_position = v2f(window.getSize().x * 0.9f, window.getSize().y * 0.9f);
    Ship player_2("Bob", player_2_position);
    //player_2.load_texture(current_path_string + "\\Textures\\Ships\\ship_spyrius.png");
    //player_2.load_texture(current_path_string + "\\Textures\\Ships\\ship_blacktron.png");
    player_2.load_texture(current_path_string + "\\Textures\\Ships\\ship_synthwave.png");
    //player_2.missile_color = sf::Color(52, 205, 108); // Spyrius / Rock Raiders / Power Miners
    player_2.missile_color = sf::Color(0, 255, 166); // Synth
    //player_2.missile_color = sf::Color(238, 215, 50); // Blacktron
    player_2.exhaust_color_tinting = player_2.missile_color;
    player_2.set_scale(3.0f);
    player_2.center_origin();
    //bool rotate(const sf::Time elapsed, float rotation_speed, const enum __Rotations__ rotation_direction, const int index = -2)
    player_2.sprite.rotate(180.0f);

    // Initialize Star (or The White Dwarf):
    Star star(__window_center__);
    star.load_texture(current_path_string + "\\Textures\\star.png");
    star.load_additional_texture(current_path_string + "\\Textures\\star_halo_9.png");
    star.set_scale(3.0f);
    star.set_additional_sprite_scale(6.0, 0);
    star.center_origin();
    star.adjust_radius();

    // Initalize satelites:
    std::vector<Entity> satellites;
    //satellites.push_back(Entity(star.sprite.getPosition() + v2f(0.0f, -480.0f), 10.0f, 10.0f)); satellites[0].set_velocity_vector(v2f(-80.0f, 0.0f));
    satellites.push_back(Entity(star.sprite.getPosition() + v2f(0.0f, -455.0f), 10.0f, 10.0f)); satellites[0].set_velocity_vector(v2f(-87.0f, 0.0f));
    satellites.push_back(Entity(star.sprite.getPosition() + v2f(800.0f, 100.0f), 10.0f, 10.0f)); satellites[1].set_velocity_vector(v2f(20.0f, -40.0f));
    satellites.push_back(Entity(star.sprite.getPosition() + v2f(-800.0f, -200.0f), 10.0f, 10.0f)); satellites[2].set_velocity_vector(v2f(-27.0f, 39.0f));
    satellites.push_back(Entity(star.sprite.getPosition() + v2f(10.0f, 420.0f), 10.0f, 10.0f)); satellites[3].set_velocity_vector(v2f(84.0f, 0.0f));
    satellites.push_back(Entity(star.sprite.getPosition() + v2f(-300.0f, 420.0f), 10.0f, 10.0f)); satellites[4].set_velocity_vector(v2f(82.0f, -15.0f));
    satellites[0].rotation_speed = 90.0f; satellites[1].rotation_speed = 44.0f; satellites[2].rotation_speed = 7.0f; satellites[3].rotation_speed = 75.0f; satellites[4].rotation_speed = 35.0f;
    satellites[0].name = "Hygiea"; satellites[1].name = "Amalthea"; satellites[2].name = "Psyche"; satellites[3].name = "Parthenope"; satellites[4].name = "Thule";
    for (uint i = 0; i < satellites.size(); i++)
    {
        satellites[i].load_texture(current_path_string + "\\Textures\\Satellites\\" + satellites[i].name + ".png");
        satellites[i].set_scale(3.0f);
        satellites[i].center_origin();
    }

    // Initiaize Missiles' static texture:
    Missile::load_texture(current_path_string + "\\Textures\\missile.png");

    // Initialize Exhaust Particles' static texture:
    Exhaust_Particle::load_texture(current_path_string + "\\Textures\\exhaust_particle.png");
    Exhaust_Particle::load_thrust_texture(current_path_string + "\\Textures\\thrust_particle.png");

    // Initialize Background:
    sf::Texture background_texture;
    if (!background_texture.loadFromFile(current_path_string + "\\Textures\\background.png"))
    {
        std::cerr << "ERROR: Failed to load background.\n";
    }
    sf::Sprite background(background_texture);
    background.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
    sf::FloatRect bounds = background.getLocalBounds();
    background.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
    background.setScale(v2f(1.5, 1.5));

    // Initialize Clock:
    sf::Clock clock;

    // Game loop:
    while (window.isOpen())
    {
        sf::Time elapsed = clock.restart();
        elapsed = elapsed * game_speed;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();
        }

        // Input handling:
        handle_inputs(window, player_1, player_2, elapsed);

        // Force calculations and velocity update:
        player_1.calculate_combined_force_and_update_velocity(star, elapsed);
        player_2.calculate_combined_force_and_update_velocity(star, elapsed);

        // Satellites:
        for (uint i = 0; i < satellites.size(); i++)
        {
            satellites[i].calculate_force_and_update_velocity(star, elapsed);
            satellites[i].rotate(elapsed, __Rotations__::COUNTERCLOCKWISE);
        }
        
        // Teleport:
        player_1.teleport(window.getSize());
        player_2.teleport(window.getSize());

        // Rotate the star and its halo:
        star.rotate(elapsed, __Rotations__::COUNTERCLOCKWISE, -1);
        star.rotate(elapsed, __Rotations__::COUNTERCLOCKWISE, 0);

        // Update exhaust particles positions:
        player_1.update_exhaust_particles(elapsed);
        player_2.update_exhaust_particles(elapsed);

        // Update rotation:
        player_1.update_rotation(elapsed);
        player_2.update_rotation(elapsed);

        // Update missiles positions:
        player_1.update_missiles(elapsed);
        player_2.update_missiles(elapsed);

        // Rotate the background:
        background.rotate(star.rotation_speed / 15.0f * elapsed.asSeconds());

        // Drawing and displaying:
        window.clear(sf::Color(15, 10, 25));
        window.draw(background);
        for (uint i = 0; i < satellites.size(); i++)
        {
            satellites[i].draw(window);
        }
        player_1.draw(window);
        player_2.draw(window);
        star.draw(window, std::vector<int>({-1, 0}));
        window.display();
    }

    return true;
}

int main()
{
    sf::VideoMode fullscreen_mode = sf::VideoMode::getFullscreenModes()[0];
    sf::VideoMode desktop_mode = sf::VideoMode::getDesktopMode();
    //sf::RenderWindow window(sf::VideoMode(1080, 720), "SFML Starwar!");
    //sf::RenderWindow window(desktop_mode, "SFML Starwar!");
    sf::RenderWindow window(fullscreen_mode, "SFML Starwar!", sf::Style::Fullscreen);
    window.setFramerateLimit(60);
    game(window);
    return 0;
}