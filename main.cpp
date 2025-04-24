// library
#include <SFML/Graphics.hpp>
#include <cmath>

// main program
int main()
{
    // create window
    sf::RenderWindow window(sf::VideoMode({600, 600}), "Title");

    // load image
    sf::Image image("Images/Girl.png");
    unsigned int image_w = image.getSize().x;
    unsigned int image_h = image.getSize().y;

    auto center = sf::Vector2f(image.getSize()) / 2.f;
    center.y -= 230.f;

    // create convex shape
    sf::ConvexShape shape(360);
    for (int i = 0;  i < 181;  i++)
    {
        // radius
        float r = i / 180.f;
        r = (r - 0.3f*r*r + 0.3f*std::cos(i*M_PI/180.f)) * 800.f;
        
        // 0-180
        shape.setPoint(i, sf::Vector2f(r, sf::degrees(i - 90.f)));

        // 359-181
        if (i > 0 and i < 180)
            shape.setPoint(360 - i, sf::Vector2f(r, sf::degrees(-i - 90.f)));
    }
    shape.setPosition(center);

    // create render texture
    sf::RenderTexture render(image.getSize());
    render.draw(shape);

    // convert render texture to mask
    sf::Image mask = render.getTexture().copyToImage();
    mask.flipVertically();

    // masking
    for (unsigned int y = 0;  y < image_h;  y++)
    {
        for (unsigned int x = 0;  x < image_w;  x++)
        {
            // get color of pixel
            sf::Color color = image.getPixel({x, y});
            
            if (mask.getPixel({x, y}).a == 0)
            {
                color.a = 0;
            }

            // set color of pixel
            image.setPixel({x, y}, color);
        }
    }

    // save image to file
    // bool success = image.saveToFile("Images/Girl 2.png");

    // create texture from image
    sf::Texture texture(image);
    texture.setSmooth(true);

    // create sprite from texture
    sf::Sprite sprite(texture);
    float scale = static_cast<float>(window.getSize().x) / image_w;
    sprite.setScale({scale, scale});

    shape.setScale({scale, scale});

    // window loop
    while (window.isOpen())
    {
        // handle events
        while (auto event = window.pollEvent())
        {
            // on close button release
            if (event->is<sf::Event::Closed>())
            {
                // close window
                window.close();
            }

            // when window is resized
            else if (event->is<sf::Event::Resized>())
            {
                // update view
                window.setView(sf::View(sf::FloatRect({0.f, 0.f}, sf::Vector2f(window.getSize()))));
            }
        }

        // fill background color
        window.clear(sf::Color(64, 64, 64));

        // draw sprite
        window.draw(sprite);
        // window.draw(shape);
        
        // display
        window.display();
    }

    // program end successfully
    return 0;
}