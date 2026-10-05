#ifndef VEC3_H
#define VEC3_H

// Klasse für dreidimensionale Vektoren
class Vec3 {
public:
  float x, y, z;
  Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
};

// Operatoren für Vektoren definieren
Vec3 operator+(const Vec3 &vec1, const Vec3 &vec2);

Vec3 operator-(const Vec3 &vec1, const Vec3 &vec2);
// Skalarprodukt
float operator*(const Vec3 &vec1, const Vec3 &vec2);
Vec3 operator*(const Vec3 &vec2, const float &a);

Vec3 operator*(const float &a, const Vec3 &vec2);
// Kreuzprodukt
Vec3 crossprod(const Vec3 &vec1, const Vec3 &vec2);

// Länge des Vektoren
float length(const Vec3 &vec);

// Nortmalisierung von Vektoren
Vec3 normal(const Vec3 &vec);

// Hilfsfunktion um vektoren printen zuu können
void print(const Vec3 vec);

#endif
