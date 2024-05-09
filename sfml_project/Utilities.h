#pragma                 once

#include                <SFML/Graphics.hpp>
#include                <SFML/System.hpp>
#include                <SFML/Window.hpp>

#include                <filesystem>
#include                <iostream>
#include                <string>
#include                <vector>
#include                <cmath>

#include                <Entity.h>

typedef unsigned int    uint;
typedef std::string     string;
typedef sf::Vector2f    v2f;
typedef sf::Vector2i    v2i;
typedef sf::Vector2u    v2u;

v2f __window_center__;

const float __pi__ = 3.141592;
const float __gravitational_constant__ = 10.0f;

enum __Rotations__
{
    CLOCKWISE,
    COUNTERCLOCKWISE,
};

sf::Texture blend_textures(sf::Texture bottom_texture, sf::Texture top_texture)
{
    v2u render_size;
    if (bottom_texture.getSize().x >= top_texture.getSize().x && bottom_texture.getSize().y >= top_texture.getSize().y)
    {
        render_size = bottom_texture.getSize();
    }
    else
    {
        render_size = top_texture.getSize();
    }

    // Create a render texture to render the combined textures:
    sf::RenderTexture render_texture;
    render_texture.create(render_size.x, render_size.y);
    // Clear the render texture:
    render_texture.clear(sf::Color::Transparent);
    // Draw the first texture onto the render texture:
    render_texture.draw(sf::Sprite(bottom_texture));

    // Draw the second texture onto the render texture with blending considering alpha channel:
    sf::BlendMode blend_mode;
    blend_mode.colorSrcFactor = sf::BlendMode::Factor::SrcAlpha;
    blend_mode.colorDstFactor = sf::BlendMode::Factor::OneMinusSrcAlpha;
    blend_mode.alphaSrcFactor = sf::BlendMode::Factor::SrcAlpha;
    blend_mode.alphaDstFactor = sf::BlendMode::Factor::OneMinusSrcAlpha;
    render_texture.draw(sf::Sprite(top_texture), blend_mode);

    // Display the render texture:
    render_texture.display();

    // Get the combined texture from the render texture:
    return render_texture.getTexture();;
}