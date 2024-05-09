#pragma                 once

#include                <SFML/Graphics.hpp>
#include                <SFML/System.hpp>
#include                <SFML/Window.hpp>

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

class Explosion
{
public:
	float flash_radius;
	float size_increase_rate = 1.1f;

	static sf::Texture flash_texture;
	static sf::Texture particle_texture;
	sf::Sprite flash_sprite;
	std::vector<sf::Sprite> particles_sprites;

	sf::Clock clock;

public:
	Explosion(const v2f position = v2f(0.0f, 0.0f), float flash_radius = 20.0f)
	{
		this->flash_sprite.setPosition(position);
		this->flash_radius = flash_radius;
	}

	static bool load_textures(const string flash_texture_path, const string particle_texture_path)
	{
		if (Explosion::flash_texture.loadFromFile(flash_texture_path) == false)
		{
			std::cerr << "ERROR: Failed to load Explosion Flash texture.\n";
			return false;
		}
		if (Explosion::particle_texture.loadFromFile(particle_texture_path) == false)
		{
			std::cerr << "ERROR: Failed to load Explosion Particle texture.\n";
			return false;
		}
		return true;
	}
};
sf::Texture Explosion::flash_texture;
sf::Texture Explosion::particle_texture;