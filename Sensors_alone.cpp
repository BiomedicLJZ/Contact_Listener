#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <iostream>


const float SCALE = 100.f;

int main()
{
    // Create SFML window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Box2D Sensor Test");
    window.setFramerateLimit(60);

    // Create Box2D world
    b2Vec2 gravity(0.f, 9.8f);
    b2World world(gravity);

    // Create ground body
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(400.f / SCALE, 500.f / SCALE);

    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(200.f / SCALE, 20.f / SCALE);

    groundBody->CreateFixture(&groundBox, 0.f);

    // Create dynamic body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(400.f / SCALE, 200.f / SCALE);

    b2Body* body = world.CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(20.f / SCALE, 20.f / SCALE);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.f;
    fixtureDef.friction = 0.3f;

    // Set the fixture as a sensor
    fixtureDef.isSensor = true;

    body->CreateFixture(&fixtureDef);

    // SFML loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Step the Box2D world
        world.Step(1/60.f, 6, 2);

        // Manually check contacts
        for (b2Contact* contact = world.GetContactList(); contact; contact = contact->GetNext())
        {
            if (contact->IsTouching() &&
                (contact->GetFixtureA() == body->GetFixtureList() ||
                 contact->GetFixtureB() == body->GetFixtureList()))
            {
                std::cout << "Sensor has made contact!" << std::endl;
                break;
            }
        }

        window.clear();

        // Draw ground
        sf::RectangleShape ground(sf::Vector2f(400.f, 40.f));
        ground.setPosition(200.f, 480.f);
        window.draw(ground);

        // Draw dynamic body
        sf::RectangleShape dynamicBody(sf::Vector2f(40.f, 40.f));
        dynamicBody.setFillColor(sf::Color::Red);
        dynamicBody.setPosition(SCALE * body->GetPosition().x, SCALE * body->GetPosition().y);
        dynamicBody.setOrigin(20.f, 20.f);
        window.draw(dynamicBody);

        window.display();
    }
    return 0;
}
