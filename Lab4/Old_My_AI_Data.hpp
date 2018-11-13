/*
 * This file was developed for the Fall 2018 instance of ECE244 at the University of Toronto.
 * Creator: Matthew J. P. Walker
 * Supervised by: Tarek Abdelrahman
 */

#ifndef ECE244_GALAXY_EXPLORER_STUDENT_AI_DATA_HPP
#define ECE244_GALAXY_EXPLORER_STUDENT_AI_DATA_HPP

#include <galaxy-explorer/Asteroid.hpp>
#include <galaxy-explorer/AsteroidsObserver.hpp>
#include <galaxy-explorer/GameAI.hpp>

sf::Vector2f getAsteroidDot(const Asteroid& ast);
sf::Vector2f getShipDot(const ShipState& shipState);

int getAngleToPoint (const sf::Vector2f& src, const sf::Vector2f point);
int getAngleToShoot (const Asteroid& asteroid, const ShipState& shipState, const GameInfo& gameInfo);
int getAngleToAsteroid (const Asteroid& asteroid, const ShipState& shipState, bool leftEnd);
const Asteroid* getMostDangerousAsteroid (const AsteroidsObserver& asteroidsObserver, const ShipState& shipState);
const Asteroid* getFastestToHitAsteroid (const AsteroidsObserver& asteroidsObserver, const ShipState& shipState, const GameInfo& gameInfo);

/**
 * Student editable struct for storing their ai state
 * Yes, you may change this header.
 * Maybe you want to remember the last asteroid Id you shot at?
 */
struct MyAIData {
    bool debug_on = true;
    SuggestedAction::Yawing lastYaw = SuggestedAction::YawingStop;
    int algorithm = 2;
    int destAngle = 1000;
    bool hasAngle = false;
    bool danger = false;
    int framesToMove = 1;
    int frames;
};

#endif /* ECE244_GALAXY_EXPLORER_STUDENT_AI_DATA_HPP */
