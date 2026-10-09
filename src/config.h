#pragma once
namespace cfg {
inline constexpr float geschwindigkeit_yaw = 0.001;
inline constexpr float geschwindigkeit_pitch = 0.001;

inline constexpr unsigned int breite_real = 2400;
inline constexpr unsigned int hoehe_real = 1600;

inline constexpr unsigned int breite = 600;
inline constexpr unsigned int hoehe = 400;

inline constexpr float abstand = 15;

inline constexpr float d = 0.03;

inline constexpr float schwarzschield_radius = 1;

inline constexpr float akkretionsscheibe_radius_min =
    2.6 * schwarzschield_radius;
inline constexpr float akkretionsscheibe_radius_max =
    7.0 * schwarzschield_radius;

inline constexpr float r_max = 20;

inline constexpr float sternProb = 0.38f;
} // namespace cfg
