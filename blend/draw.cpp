#include "stdio.h"
#include <SFML/Graphics.hpp>
#include <immintrin.h>
#include <alpha.hpp>


int makeAlpha(const char *front_name, const char *back_name)
{
    if (!front_name || !back_name)
    {
        printf("Invalid string\n");
        return -1;
    }

    sf::RenderWindow window(sf::VideoMode(width, heigth), "alpha");

    sf::Image frontIm;
    frontIm.loadFromFile(front_name);
    sf::Uint32 *front = (sf::Uint32 *) frontIm.getPixelsPtr();

    sf::Image backIm;
    backIm.loadFromFile(back_name);
    sf::Uint32 *back  = (sf::Uint32 *) backIm.getPixelsPtr();

    sf::Uint32 *src   = (sf::Uint32 *) calloc(width * heigth, sizeof(sf::Uint32));

    sf::Texture texture;
    texture.create(width, heigth);

    sf::Sprite  sprite(texture);

    sf::Font font;
    font.loadFromFile("font.ttf");

    sf::Text FPS("0", font, 20);

    sf::Clock     clock;
    double        fps = 0;
    std::string   str;

    while (window.isOpen())
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) window.close();
        
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            if (useSSE(front, back, src))  return -1;
        }
        else
        {
            if (noSSE(front, back, src))   return -1;
        }

    
        texture.update((const sf::Uint8 *) src);
        
        window.clear();
        window.draw(sprite);
        window.display();

        fps = 1. / (clock.getElapsedTime().asSeconds());
        clock.restart();

        str = std::to_string(fps);
        
        FPS.setString(str);
        window.draw(FPS);
        window.display();
    }

    free(src);
    return 0;
}