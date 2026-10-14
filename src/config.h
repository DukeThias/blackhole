#pragma once
namespace cfg {
inline constexpr float geschwindigkeit_yaw = 0.001;
inline constexpr float geschwindigkeit_pitch = 0.001;

inline constexpr unsigned int breite_real = 2400;
inline constexpr unsigned int hoehe_real = 1600;

inline constexpr unsigned int breite = 1200;
inline constexpr unsigned int hoehe = 800;

inline constexpr unsigned int breite_perform = 300;
inline constexpr unsigned int hoehe_perform = 200;

inline constexpr float abstand_start = 15;

inline constexpr float d = 0.03;

inline constexpr float schwarzschield_radius = 1;

inline constexpr float akkretionsscheibe_radius_min =
    2.6 * schwarzschield_radius;
inline constexpr float akkretionsscheibe_radius_max =
    9.0 * schwarzschield_radius;

inline constexpr float r_max = 20;

inline constexpr float sternProb = 0.18f;

inline constexpr float lichtgeschwindigkeit_faktor = 0.7;

inline constexpr float belichtung = 4;

inline constexpr int anzahl_ringe_as = 7;

inline constexpr int anzahl_winkel = 12;

inline constexpr int random_hash_zahl = 1000;
} // namespace cfg
