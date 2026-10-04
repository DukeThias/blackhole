#include "vec3.h"
#include <SFML/Graphics.hpp>
#include <iostream>

int main() {

  print(Vec3(2, 3, 34));
  bool mousePressed = false;
  sf::RenderWindow window(sf::VideoMode({2400, 1600}), "blackhole");
  sf::RenderTexture canvas({300, 200});
  sf::Sprite sprite(canvas.getTexture());
  sprite.setScale({8.f, 8.f}); // 300x200 * 4 = 1200x800

  const auto onClose = [&window](const sf::Event::Closed &) { window.close(); };

  const auto onKeyPressed = [&window](const sf::Event::KeyPressed &keyPressed) {
    if (keyPressed.scancode == sf::Keyboard::Scancode::Escape)
      window.close();
  };

  auto mouseButtonReleased =
      [&window, &mousePressed](
          const sf::Event::MouseButtonReleased &mouseButtonReleased) {
        if (mouseButtonReleased.button == sf::Mouse::Button::Left) {
          std::cout << "Linke Maustaste losgelassen" << std::endl;
          mousePressed = false;
        }
      };

  auto mouseButtonPressed =
      [&window,
       &mousePressed](const sf::Event::MouseButtonPressed &mouseButtonPressed) {
        std::cout << "Linke Maustaste gedrückt" << std::endl;
        if (mouseButtonPressed.button == sf::Mouse::Button::Left) {
          mousePressed = true;
        }
      };

  auto mouseMoved = [&window,
                     &mousePressed](const sf::Event::MouseMoved &mouseMoved) {
    if (mousePressed) {
      std::cout << "maus gezogen x" << mouseMoved.position.x << std::endl;
      std::cout << "maus gezogen y" << mouseMoved.position.y << std::endl;
    }
  };

  while (window.isOpen()) {
    window.handleEvents(onClose, onKeyPressed, mouseButtonPressed,
                        mouseButtonReleased, mouseMoved);

    canvas.clear();
    // hier alles in 300x200 zeichnen: canvas.draw(...)
    canvas.display();

    window.clear();
    window.draw(sprite);
    window.display();
  }
}
