#include "config.h"
#include "vec3.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/Angle.hpp>
#include <algorithm>
#include <atomic>
#include <iostream>
#include <math.h>
#include <string>
#include <thread>
#include <tuple>

using namespace cfg;
sf::Font font("Jetbrains.ttf");
// hash funktion
unsigned int hash3(int x, int y, int z) {
  unsigned int h = (unsigned int)x * 73856093u ^ (unsigned int)y * 19349663u ^
                   (unsigned int)z * 83492791u;
  h ^= h >> 13;
  h *= 0x5bd1e995u;
  h ^= h >> 15;
  return h;
}
float zufall01(unsigned int h) { return (h & 0xFFFFFF) / 16777216.0f; }

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

// abstand der sternensphäre in würfeleinheiten der kamera
sf::Color stern_from_vec(Vec3 richtung, const float &fov, const int &breite) {
  int n = 40;

  // größe von stern ausrechnen damit er einen Pixel groß ist
  float r_stern = std::tan(0.5 * (fov / breite)) * 2.5;

  // genaue position der richtung mal den abstand (genauer Punkt auf der
  // "Sternensphäre")
  float pos_x = richtung.x * n;
  float pos_y = richtung.y * n;
  float pos_z = richtung.z * n;

  // gerundeter Punkt von oben, um "Sternensphäöre in 3d-Würfelkoordinaten
  // umzurechnen"
  int koord_x = (int)std::floor(pos_x);
  int koord_y = (int)std::floor(pos_y);
  int koord_z = (int)std::floor(pos_z);

  // rechne die Relative Position im Würfel aus (um entfernung zum
  // eigentlichen
  // stern ausrechnen zu können)
  float pos_in_cube_x = pos_x - koord_x, pos_in_cube_y = pos_y - koord_y,
        pos_in_cube_z = pos_z - koord_z;

  // 1/50 chance, dass würfel einen stern beinhaltet, durch abgefuckte hash
  // funktion
  unsigned int hash_for_cube = hash3(koord_x, koord_y, koord_z);

  if (zufall01(hash_for_cube) < sternProb) { // wenn stern enthalten ist
    // koordinaten des Sterns im Würfel (feste zufallszahlen zwischen 0 und 1)
    float stern_x = zufall01(hash3(koord_x + 1000, koord_y, koord_z));
    float stern_y = zufall01(hash3(koord_x, koord_y + 2000, koord_z));
    float stern_z = zufall01(hash3(koord_x, koord_y, koord_z + 3000));

    // abstand zwischen Punkt auf "Sternsphäre" und nächstem Stern (durch
    // sqrt(x²+y²+z²))
    // float d = std::sqrt((pos_in_cube_x - stern_x) * (pos_in_cube_x -
    // stern_x)
    // +
    //                    (pos_in_cube_y - stern_y) * (pos_in_cube_y -
    // stern_y)
    //                    + (pos_in_cube_z - stern_z) * (pos_in_cube_z -
    //                    stern_z));

    Vec3 pos_stern_real =
        Vec3(koord_x + stern_x, koord_y + stern_y, koord_z + stern_z);

    float d = length(normal(richtung) - normal(pos_stern_real));
    // wenn distanz zum stern kleiner als 0.15, dann pixel in der
    // dazugehörigen
    // helligkeit (je nach abstand heller) weiß ferben
    if (d < r_stern) {
      float hell = 1.f - d / r_stern;
      // r g und b jeweils mal helligkeit, die abhängig von distanz zum stern
      int v = (int)(hell * 255);
      // ist
      return sf::Color(v, v, v, 255);
    }
  }
  return sf::Color::Black;
}

sf::Color akkretionsscheibe_farbe(float &dist, Vec3 &richtung) {
  float tmp =
      1.f - (dist - akkretionsscheibe_radius_min) /
                (akkretionsscheibe_radius_max - akkretionsscheibe_radius_min);
  tmp = std::clamp(tmp, 0.f, 1.f);
  return sf::Color(255, static_cast<std::uint8_t>(128 + tmp * 127),
                   static_cast<std::uint8_t>(tmp * 255), 255);
}

sf::Color color_from_vec(Vec3 richtung, const float &fov, const int &breite,
                         const Vec3 &cam_pos) {
  Vec3 pos = cam_pos;
  float len = length(cam_pos);
  float h = length(crossprod(pos, richtung));
  float h2 = h * h;
  pos = pos + richtung * d * len;
  float y_pos_alt = pos.y;
  // h ist irgendwie wichtig für die physik
  while (len > schwarzschield_radius &&
         !(len > r_max && (pos * richtung) > 0)) {
    // je weiter weg von schwarzem loch desto größer die schritte
    float dt = d * len; // wird zweimal benutzt deshalb einmal ausrechnen
    float faktor = -1.5 * schwarzschield_radius * h2 / std::pow(len, 5);
    richtung = normal(richtung + pos * dt * faktor);
    pos = pos + richtung * dt;
    len = length(pos);
    float tmp = length(pos);
    if (pos.y > 0) {
      if (y_pos_alt < 0) {
        if (tmp < akkretionsscheibe_radius_max &&
            tmp > akkretionsscheibe_radius_min) {
          return akkretionsscheibe_farbe(tmp, richtung);
        }
      }
    } else {
      if (y_pos_alt > 0) {
        if (tmp < akkretionsscheibe_radius_max &&
            tmp > akkretionsscheibe_radius_min) {
          return akkretionsscheibe_farbe(tmp, richtung);
        }
      }
    }
    y_pos_alt = pos.y;
  }

  if (len < schwarzschield_radius) {
    return sf::Color::Black;
  }

  else {
    return stern_from_vec(richtung, fov, breite);
  }
  // r < rs → Pixel schwarz, Schleife beenden
  // r > rmax (z. B. 20) und Skalarprodukt(pos, vel) > 0 (läuft nach außen) →
  // Himmelsfarbe(vel), beenden Beschleunigung: a = −1,5 · rs · h² / r⁵ · pos
  // vel = vel + a · dt, danach vel normalisieren pos = pos + vel · dt
}

void calc_line(const float &hoehe, const float &x, const int &breite,
               const float &aspect_ratio, const float &fov, const Vec3 &forward,
               const Vec3 &up, const Vec3 &right, const Vec3 &pos,
               std::vector<std::uint8_t> &pixel) {
  for (int y = 0; y < hoehe; y++) {
    sf::Color color = color_from_vec(
        richtung(x, y, breite, aspect_ratio, fov, hoehe, forward, up, right),
        fov, breite, pos);

    int i = (y * breite + x) * 4;
    pixel[i] = color.r;
    pixel[i + 1] = color.g;
    pixel[i + 2] = color.b;
    pixel[i + 3] = 255;
  }
}
int main() {
  bool mousePressed = false;
  std::tuple<float, float> mousPosition;
  float fov = M_PI / 2;
  float aspect_ratio = (float)breite / (float)hoehe;
  float yaw = 0;
  float pitch = 0;
  Vec3 pos = abstand * Vec3(std::cos(pitch) * std::sin(yaw), std::sin(pitch),
                            std::cos(pitch) * std::cos(yaw));
  float yaw_alt = yaw + 1;
  float pitch_alt = pitch;

  Vec3 forward = normal(-1 * pos);
  Vec3 right = normal(crossprod(forward, Vec3(0, 1, 0)));
  Vec3 up = normal(crossprod(right, forward));

  sf::RenderWindow window(sf::VideoMode({breite_real, hoehe_real}),
                          "blackhole");
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
          mousePressed = false;
        }
      };

  auto mouseButtonPressed =
      [&window, &mousePressed,
       &mousPosition](const sf::Event::MouseButtonPressed &mouseButtonPressed) {
        if (mouseButtonPressed.button == sf::Mouse::Button::Left) {
          mousePressed = true;
          std::get<0>(mousPosition) = (mouseButtonPressed.position.x);
          std::get<1>(mousPosition) = (mouseButtonPressed.position.y);
        }
      };

  auto mouseMoved = [&window, &mousePressed, &mousPosition, &yaw,
                     &pitch](const sf::Event::MouseMoved &mouseMoved) {
    if (mousePressed) {
      float deltax = mouseMoved.position.x - std::get<0>(mousPosition);
      float deltay = mouseMoved.position.y - std::get<1>(mousPosition);
      yaw = yaw + geschwindigkeit_yaw * deltax;
      float pitch_tmp = pitch - geschwindigkeit_pitch * deltay;
      if (pitch_tmp > (-1 * (M_PI / 2)) && pitch_tmp < M_PI / 2) {
        pitch = pitch_tmp;
      }
      std::get<0>(mousPosition) = (mouseMoved.position.x);
      std::get<1>(mousPosition) = (mouseMoved.position.y);
    }
  };

  std::vector<std::uint8_t> pixel(breite * hoehe * 4);

  sf::Texture tex;
  (void)tex.resize({breite, hoehe});
  sf::Sprite sprite(tex);
  sprite.setScale({static_cast<float>(breite_real) / breite,
                   static_cast<float>(hoehe_real) / hoehe});
  sf::Text fps(font);
  fps.setCharacterSize(24);
  fps.setFillColor(sf::Color::Red);

  // Uhr für fps
  sf::Clock clock = sf::Clock();
  int frames = 0;
  clock.start();

  while (window.isOpen()) {
    window.handleEvents(onClose, onKeyPressed, mouseButtonPressed,
                        mouseButtonReleased, mouseMoved);
    pos = abstand * Vec3(std::cos(pitch) * std::sin(yaw), std::sin(pitch),
                         std::cos(pitch) * std::cos(yaw));
    Vec3 forward = normal(-1 * pos);
    Vec3 right = normal(crossprod(forward, Vec3(0, 1, 0)));
    Vec3 up = normal(crossprod(right, forward));
    if (!(yaw_alt == yaw && pitch_alt == pitch)) {
      yaw_alt = yaw;
      pitch_alt = pitch;
      std::atomic<int> i{0};
      auto lamda = [&] {
        while (true) {
          if (i >= breite) {
            break;
          }
          int k = i.fetch_add(1);
          calc_line(hoehe, k, breite, aspect_ratio, fov, forward, up, right,
                    pos, pixel);
        }
      };

      std::vector<std::thread> threads;

      for (int j = 0; j < 24; j++) {
        threads.emplace_back(lamda);
      }

      for (int j = 0; j < 24; j++) {
        threads[j].join();
      }
    }
    tex.update(pixel.data());
    window.clear();
    float time = clock.getElapsedTime().asSeconds();
    if (time > 1) {
      fps.setString(std::to_string((int)(frames / time)));
      frames = 0;
      clock.restart();
    } else {
      frames++;
    }
    window.draw(sprite);
    window.draw(fps);
    window.display();
  }
}
