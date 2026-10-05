#include "vec3.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <tuple>

Vec3 richtung(float x, float y, float breite, float aspekt, float fov,
              float hoehe, Vec3 forward, Vec3 up, Vec3 right) {
  // fdies Funktion gib für einen Pixel auf dem Bildschirm die Richtung des
  // Lichts aus der Kamera in jene Richtung u und v sind koordinaten auf einer
  // Bildebene mit dem Abstand abstand von der Kamera
  float tmp = std::tan(fov / 2);
  float u = (2 * (x + 0.5) / breite - 1) * aspekt * tmp;
  float v = (1 - 2 * (y + 0.5) / hoehe) * tmp;
  return normal(forward + u * right + v * up);
};
// Richtung als Farbe
// sf::Color color_from_vec(Vec3 richtung) {
//   return sf::Color((richtung.x + 1) * 127.5, (richtung.y + 1) * 127.5,
//                  (richtung.z + 1) * 127.5);
//};

// Richtung zu Schachbrettmuster
//
// sf::Color color_from_vec(Vec3 richtung) {
//  float k = 10;
//  float phi = std::atan2(richtung.x, richtung.z);
//  float theta = std::asin(richtung.y);
//  if (static_cast<int>(floor(phi * k) + floor(theta * k)) % 2 == 0) {
//    return sf::Color::White;
//  } else {
//    return sf::Color::Black;
//  }
//}

// Sterne

sf::Color color_from_vec(Vec3 richtung) {
  int n = 40;
  int x = floor(richtung.x * n);
  int y = floor(richtung.y * n);
  int z = floor(richtung.z * n);
}
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
      float pitch_tmp = pitch - geschwindigkeit_pitch * deltay;
      if (pitch_tmp > (-1 * (M_PI / 2)) && pitch_tmp < M_PI / 2) {
        pitch = pitch_tmp;
      }
      std::cout << "yaw: " << yaw << std::endl;
      std::cout << "pitch: " << pitch << std::endl;
      std::get<0>(mousPosition) = (mouseMoved.position.x);
      std::get<1>(mousPosition) = (mouseMoved.position.y);
    }
  };

  std::vector<std::uint8_t> pixel(breite * hoehe * 4);

  sf::Texture tex;
  (void)tex.resize({breite, hoehe});
  sf::Sprite sprite(tex);
  sprite.setScale({8.f, 8.f});

  while (window.isOpen()) {
    window.handleEvents(onClose, onKeyPressed, mouseButtonPressed,
                        mouseButtonReleased, mouseMoved);
    pos = abstand * Vec3(std::cos(pitch) * std::sin(yaw), std::sin(pitch),
                         std::cos(pitch) * std::cos(yaw));
    Vec3 forward = normal(-1 * pos);
    Vec3 right = normal(crossprod(forward, Vec3(0, 1, 0)));
    Vec3 up = normal(crossprod(right, forward));

    for (int x = 0; x < breite; x++) {
      for (int y = 0; y < hoehe; y++) {
        sf::Color color = color_from_vec(richtung(
            x, y, breite, aspect_ratio, fov, hoehe, forward, up, right));

        int i = (y * breite + x) * 4;
        pixel[i] = color.r;
        pixel[i + 1] = color.g;
        pixel[i + 2] = color.b;
        pixel[i + 3] = 255;
      }
    }
    tex.update(pixel.data());
    window.clear();
    window.draw(sprite);
    window.display();
  }
}
