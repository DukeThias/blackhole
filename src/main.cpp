#include "vec3.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <tuple>

int main() {
  bool mousePressed = false;
  std::tuple<float, float> mousPosition;
  float geschwindigkeit_yaw = 0.001;
  float geschwindigkeit_pitch = 0.001;
  unsigned int breite = 300;
  unsigned int hoehe = 200;
  float fov = M_PI / 2;
  float aspect_ratio = (float)breite / (float)hoehe;
  std::cout << "asppekt: " << aspect_ratio << std::endl;
  float yaw = 0;
  float pitch = 0;
  float abstand = 10;
  Vec3 pos = abstand * Vec3(std::cos(pitch) * std::sin(yaw), std::sin(pitch),
                            std::cos(pitch) * std::cos(yaw));

  Vec3 forward = normal(-1 * pos);
  Vec3 right = normal(crossprod(forward, Vec3(0, 1, 0)));
  Vec3 up = normal(crossprod(right, forward));

  sf::RenderWindow window(sf::VideoMode({2400, 1600}), "blackhole");
  sf::RenderTexture canvas({breite, hoehe});
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
      [&window, &mousePressed,
       &mousPosition](const sf::Event::MouseButtonPressed &mouseButtonPressed) {
        std::cout << "Linke Maustaste gedrückt" << std::endl;
        if (mouseButtonPressed.button == sf::Mouse::Button::Left) {
          mousePressed = true;
          std::get<0>(mousPosition) = (mouseButtonPressed.position.x);
          std::get<1>(mousPosition) = (mouseButtonPressed.position.y);
        }
      };

  auto mouseMoved = [&window, &mousePressed, &mousPosition, &yaw, &pitch,
                     &geschwindigkeit_pitch, &geschwindigkeit_yaw](
                        const sf::Event::MouseMoved &mouseMoved) {
    if (mousePressed) {
      float deltax = mouseMoved.position.x - std::get<0>(mousPosition);
      float deltay = mouseMoved.position.y - std::get<1>(mousPosition);
      yaw = yaw + geschwindigkeit_yaw * deltax;
      float pitch_tmp = pitch + geschwindigkeit_pitch * deltay;
      if (pitch_tmp > (-1 * (M_PI / 2)) && pitch_tmp < M_PI / 2) {
        pitch = pitch_tmp;
      }
      std::cout << "yaw: " << yaw << std::endl;
      std::cout << "pitch: " << pitch << std::endl;
      std::get<0>(mousPosition) = (mouseMoved.position.x);
      std::get<1>(mousPosition) = (mouseMoved.position.y);
    }
  };

  while (window.isOpen()) {
    window.handleEvents(onClose, onKeyPressed, mouseButtonPressed,
                        mouseButtonReleased, mouseMoved);
    pos = abstand * Vec3(std::cos(pitch) * std::sin(yaw), std::sin(pitch),
                         std::cos(pitch) * std::cos(yaw));
    Vec3 forward = normal(-1 * pos);
    Vec3 right = normal(crossprod(forward, Vec3(0, 1, 0)));
    Vec3 up = normal(crossprod(right, forward));

    canvas.clear();

    // hier alles in 300x200 zeichnen: canvas.draw(...)
    canvas.display();

    window.clear();
    window.draw(sprite);
    window.display();
  }
}
