#include "vec3.h"
#include <cmath>
#include <iostream>
// Addition
Vec3 operator+(const Vec3 &vec1, const Vec3 &vec2) {
  return Vec3(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z);
}
// Subtraktion
Vec3 operator-(const Vec3 &vec1, const Vec3 &vec2) {
  return Vec3(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z);
}

// Skalar-Multiplikation
Vec3 operator*(const Vec3 &vec, const float &a) {
  return Vec3(a * vec.x, a * vec.y, a * vec.z);
}
// Skalar-Multiplikation aber Skalar zuerst genannt
Vec3 operator*(const float &a, const Vec3 &vec) { return vec * a; }

// Skalarprodukt
float operator*(const Vec3 &vec1, const Vec3 &vec2) {
  return (vec1.x * vec2.x) + (vec1.y * vec2.y) + (vec1.z * vec2.z);
}

// Kreuzprodukt
Vec3 crossprod(const Vec3 &vec1, const Vec3 &vec2) {
  return Vec3(vec1.y * vec2.z - vec1.z * vec2.y,
              vec1.z * vec2.x - vec1.x * vec2.z,
              vec1.x * vec2.y - vec1.y * vec2.x);
}

// Länge des Vektoren
float length(const Vec3 &vec) {
  return std::sqrt(std::pow(vec.x, 2) + std::pow(vec.y, 2) +
                   std::pow(vec.z, 2));
}
// Nortmalisierung von Vektoren
Vec3 normal(const Vec3 &vec) {
  float len = length(vec);
  return Vec3(vec.x / len, vec.y / len, vec.z / len);
}
// Hilfsfunktion um vektoren printen zuu können
void print(const Vec3 vec) {
  std::cout << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")"
            << std::endl;
}
