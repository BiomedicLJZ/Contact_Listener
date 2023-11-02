#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <iostream>

class MyContactListener : public b2ContactListener {
public:
    MyContactListener() : hadContact(true) {}
    void BeginContact(b2Contact* contact) override {
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();

        if (fixtureA->IsSensor() || fixtureB->IsSensor()) {
            std::cout << "Cuerpo entró en el sensor" << std::endl;
        } else {
            std::cout << "¡Inicio de contacto!" << std::endl;
        }
        hadContact = true;
    }

    void EndContact(b2Contact* contact) override {
        std::cout << "¡Fin de contacto!" << std::endl;
        hadContact = false;
    }

    bool hadContact;
};

int main() {
    // Creación de una ventana SFML
    sf::RenderWindow window(sf::VideoMode(800, 600), "Ejemplo de Box2D y SFML");

    // Creación de un mundo en Box2D
    b2Vec2 gravedad(0.0f, -0.0f);
    b2World mundo(gravedad);

    // Creación del escuchador de contacto y su añadido al mundo
    MyContactListener escuchadorDeContacto;
    mundo.SetContactListener(&escuchadorDeContacto);

    // Creación de dos cuerpos dinámicos (por ejemplo, cajas)
    b2BodyDef definicionCuerpoA, definicionCuerpoB;
    definicionCuerpoA.type = b2_dynamicBody;
    definicionCuerpoA.position.Set(0.0f, 0.0f); // Center at the bottom of the window
    b2Body* cuerpoA = mundo.CreateBody(&definicionCuerpoA);

    definicionCuerpoB.type = b2_dynamicBody;
    definicionCuerpoB.position.Set(0.0f, 10.0f); // Center at the bottom of the window
    b2Body* cuerpoB = mundo.CreateBody(&definicionCuerpoB);

    // Creación de características (formas) para los cuerpos
    b2PolygonShape formaA, formaB;
    formaA.SetAsBox(1.0f, 1.0f);
    formaB.SetAsBox(1.0f, 1.0f);

    // Añadiendo las características a los cuerpos
    b2FixtureDef definicionCaracteristicaA, definicionCaracteristicaB;
    definicionCaracteristicaA.shape = &formaA;
    definicionCaracteristicaA.density = 1.0f;
    definicionCaracteristicaA.isSensor = false;
    definicionCaracteristicaB.shape = &formaB;
    definicionCaracteristicaB.density = 1.0f;
    definicionCaracteristicaA.isSensor = false; // Convertirlo en un sensor

    // Creación de un sensor para el cuerpoA
    cuerpoA->CreateFixture(&definicionCaracteristicaA);
    // Creación de caracteristicas para el cuerpoB
    definicionCaracteristicaB.shape = &formaB;

    cuerpoB->CreateFixture(&definicionCaracteristicaB);

    cuerpoB->CreateFixture(&definicionCaracteristicaB);
    cuerpoB->SetLinearVelocity(b2Vec2(0.0f, -0.2f));

    unsigned int contador = 0;

    while (window.isOpen() && escuchadorDeContacto.hadContact) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Actualización de la simulación de Box2D
        mundo.Step(1.0f / 120.0f, 6, 2);

        // Obteniendo las posiciones de los cuerpos
        b2Vec2 posicionA = cuerpoA->GetPosition();
        b2Vec2 posicionB = cuerpoB->GetPosition();

        // Limpia la ventana
        window.clear();

        // Dibuja los cuerpos en la posición actual (centrados en la ventana)
        sf::RectangleShape rectA(sf::Vector2f(20.0f, 20.0f));
        rectA.setPosition(400 - posicionA.x * 10, 300 - posicionA.y * 10);
        window.draw(rectA);

        sf::RectangleShape rectB(sf::Vector2f(20.0f, 20.0f));
        rectB.setPosition(400 - posicionB.x * 10, 300 - posicionB.y * 10);
        window.draw(rectB);

        // Muestra la ventana
        window.display();

        contador++;
        std::cout << "Paso " << contador << " (" << posicionA.x << ", " << posicionA.y << ") (" << posicionB.x << ", " << posicionB.y << ")" << std::endl;
    }

    return 0;
}
