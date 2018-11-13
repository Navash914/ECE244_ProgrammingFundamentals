#include <cmath>

//#include <galaxy-explorer/GameAI.hpp>
//#include <galaxy-explorer/MyAIData.hpp>

#include "GameAI.hpp"
#include "MyAIData.hpp"
#include "AsteroidsObserver.hpp"

#include <cmath>

#include <SFML/Graphics.hpp>


GameAI::GameAI(const GameInfo& game_info, sf::RenderTarget* debug_rt)
{
	this->debug_rt = debug_rt;
	this->game_info = game_info;
	this->asteroid_observer = AsteroidsObserver(this);
	this->my_game_ai = new MyAIData();

	// customState().debug_on = false;
}

GameAI::~GameAI() {
	delete my_game_ai;
}

SuggestedAction GameAI::suggestAction(const ShipState& ship_state) {
    debug_rt->clear(sf::Color::Transparent);
    
    // Current Best: 40 -> 25,858
    const int maxAngle = 40;
    
    if (my_game_ai->debug_on) {
        if (!asteroidsObserver().asteroids().isEmpty()) {
            const sf::IntRect ship_hb = ship_state.hitbox;
            sf::RectangleShape ship_rect(sf::Vector2f(ship_hb.width, ship_hb.height));
            ship_rect.setPosition(ship_hb.left, ship_hb.top);
            ship_rect.setOutlineThickness(33.0f); // if lines are too thin, they won't show up sometimes
            ship_rect.setOutlineColor(sf::Color::Yellow);
            ship_rect.setFillColor(sf::Color::Transparent);
            debug_rt->draw(ship_rect);
        }
    }
        
    const Asteroid* asteroidToHit = getMostDangerousAsteroid(asteroid_observer, ship_state);
    
    if (asteroidToHit == nullptr) {
        // No asteroid to hit. Shoot randomly within max angle range.
        if (ship_state.millidegree_rotation > maxAngle * 1000) {
            my_game_ai->lastYaw = SuggestedAction::YawingAntiClockwise;
            return SuggestedAction{ SuggestedAction::YawingAntiClockwise, SuggestedAction::FiringTry };
        } else if (ship_state.millidegree_rotation < maxAngle * -1000) {
            my_game_ai->lastYaw = SuggestedAction::YawingClockwise;
            return SuggestedAction{ SuggestedAction::YawingClockwise, SuggestedAction::FiringTry };
        } else if (my_game_ai->lastYaw == SuggestedAction::YawingStop) {
            my_game_ai->lastYaw = SuggestedAction::YawingClockwise;
            return SuggestedAction{ SuggestedAction::YawingClockwise, SuggestedAction::FiringTry };
        } else {
            return SuggestedAction{ my_game_ai->lastYaw, SuggestedAction::FiringTry };
        }
    } else {
        if (my_game_ai->debug_on) {
            if (!asteroidsObserver().asteroids().isEmpty()) {
                const sf::IntRect first_ast_hb = asteroidToHit->getCurrentHitbox();
                sf::RectangleShape ast_rect(sf::Vector2f(first_ast_hb.width, first_ast_hb.height));
                ast_rect.setPosition(first_ast_hb.left, first_ast_hb.top);
                ast_rect.setOutlineThickness(33.0f); // if lines are too thin, they won't show up sometimes
                ast_rect.setOutlineColor(sf::Color::Yellow);
                ast_rect.setFillColor(sf::Color::Transparent);
                debug_rt->draw(ast_rect);

            }
        }
        
        // Find angle to asteroid
        int angleToFire_L = getAngleToAsteroid(*asteroidToHit, ship_state, true);
        int angleToFire_R = getAngleToAsteroid(*asteroidToHit, ship_state, false);
        int shipAngle = ship_state.millidegree_rotation;
        
        // Look to asteroid and shoot
        if (shipAngle < angleToFire_L) {
            my_game_ai->lastYaw = SuggestedAction::YawingClockwise;
            return SuggestedAction{ SuggestedAction::YawingClockwise, SuggestedAction::FiringTry };
        } if (shipAngle > angleToFire_R) {
            my_game_ai->lastYaw = SuggestedAction::YawingAntiClockwise;
            return SuggestedAction{ SuggestedAction::YawingAntiClockwise, SuggestedAction::FiringTry };
        } else {
            return SuggestedAction{ my_game_ai->lastYaw, SuggestedAction::FiringTry };
        }
    }

}

int getAngleToPoint (const sf::Vector2f& src, const sf::Vector2f point) {
    const sf::Vector2f forward(0.0, -1.0);
    sf::Vector2f srcToPoint ( point.x - src.x, point.y - src.y );
    float dotProduct = srcToPoint.x * forward.x + srcToPoint.y * forward.y;
    float magProduct = (float) std::sqrt(srcToPoint.x * srcToPoint.x + srcToPoint.y * srcToPoint.y);
    float angle = (std::acos(dotProduct / magProduct) * 180.0f * 7.0f) / 22.0f;
    angle *= 1000;
    if (point.x < src.x)
        angle *= -1;

    return (int) angle;
}

int getAngleToAsteroid (const Asteroid& asteroid, const ShipState& shipState, bool leftEnd) {
    sf::Vector2f forward(0.0, -1.0);
    sf::Vector2f shipDot( shipState.hitbox.left + shipState.hitbox.width/2.0f,
                            shipState.hitbox.top + shipState.hitbox.height/2.0f );
    sf::Vector2f asteroidDot;
    asteroidDot.y = asteroid.getCurrentHitbox().top + asteroid.getCurrentHitbox().height;
    asteroidDot.x = asteroid.getCurrentHitbox().left;
    if (!leftEnd)
        asteroidDot.x += asteroid.getCurrentHitbox().width;
    return getAngleToPoint(shipDot, asteroidDot);
}

const Asteroid* getMostDangerousAsteroid (const AsteroidsObserver& asteroidsObserver, const ShipState& shipState) {
    if (asteroidsObserver.asteroids().isEmpty())
        return nullptr;
    const Asteroid* asteroid = nullptr;
    float maxDanger = 0;

    sf::Vector2i shipDot( shipState.hitbox.left + shipState.hitbox.width/2,
                          shipState.hitbox.top + shipState.hitbox.height/2 );

    for (const AsteroidListItem* it = asteroidsObserver.asteroids().begin();
            it != asteroidsObserver.asteroids().end();
            it = it->getNext()
    ) {
        Asteroid ast = it->getData();
        sf::Vector2i asteroidDot (ast.getCurrentHitbox().left + ast.getCurrentHitbox().width / 2,
                                    ast.getCurrentHitbox().top + ast.getCurrentHitbox().height / 2);
        sf::Vector2i velocity = ast.getVelocity();
        for (int i=0; i<3; i++) {
            int hitPoint;
            switch (i) {
                case 0:
                    hitPoint = shipState.hitbox.top;
                    break;
                case 1:
                    hitPoint = shipDot.y;
                    break;
                case 2:
                    hitPoint = shipState.hitbox.top + shipState.hitbox.height;
                    break;
            }
            float t = std::abs(hitPoint - asteroidDot.y) / (float) velocity.y;
            sf::Vector2i finalPos (asteroidDot.x + velocity.x * t, asteroidDot.y + velocity.y * t);
            sf::IntRect finalHitbox (finalPos.x - ast.getCurrentHitbox().width / 2, finalPos.y - ast.getCurrentHitbox().height / 2,
                                        ast.getCurrentHitbox().width, ast.getCurrentHitbox().height);
            bool willHit = shipState.hitbox.intersects(finalHitbox);
            if (willHit) {
                float danger = 1.0f / t;
                if (danger > maxDanger) {
                    maxDanger = danger;
                    asteroid = &it->getData();
                }
                break;
            }
        }
        
    }
    return asteroid;
}
