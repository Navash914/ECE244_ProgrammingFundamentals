/*
 * This file was developed for the Fall 2018 instance of ECE244 at the University of Toronto.
 * Creator: Matthew J. P. Walker
 * Supervised by: Tarek Abdelrahman
 */

#ifndef ECE244_GALAXY_EXPLORER_STUDENT_AI_DATA_HPP
#define ECE244_GALAXY_EXPLORER_STUDENT_AI_DATA_HPP

#include "Asteroid.hpp"
#include "AsteroidsObserver.hpp"
#include "GameAI.hpp"

// Returns angle between two points in millidegrees,
int getAngleToPoint (const sf::Vector2f& src, const sf::Vector2f point);

// Gets angle to left or right end of asteroid hitbox from ship.
int getAngleToAsteroid (const Asteroid& asteroid, const ShipState& shipState, bool leftEnd);

// Gets asteroid that will hit the ship the fastest.
const Asteroid* getMostDangerousAsteroid (const AsteroidsObserver& asteroidsObserver, const ShipState& shipState);

/**
 * Student editable struct for storing their ai state
 * Yes, you may change this header.
 * Maybe you want to remember the last asteroid Id you shot at?
 */
struct MyAIData {
    bool debug_on = true;
    SuggestedAction::Yawing lastYaw = SuggestedAction::YawingStop;
};

#endif /* ECE244_GALAXY_EXPLORER_STUDENT_AI_DATA_HPP */
