#include <cmath>

#include <galaxy-explorer/GameAI.hpp>
#include <galaxy-explorer/MyAIData.hpp>

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
        my_game_ai->danger = false;
        //asteroidToHit = getFastestToHitAsteroid(asteroid_observer, ship_state, game_info);
    } else my_game_ai->danger = true;
    
    if (asteroidToHit == nullptr) {
        my_game_ai->frames++;
        if (my_game_ai->frames >= my_game_ai->framesToMove) {
            my_game_ai->frames = 0;
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
        } else return SuggestedAction{ SuggestedAction::YawingStop, SuggestedAction::FiringTry };
        /*if (ship_state.millidegree_rotation > maxAngle * 1000) {
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
        }*/
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
        
        if (my_game_ai->danger) {
            int angleToFire_L = getAngleToAsteroid(*asteroidToHit, ship_state, true);
            int angleToFire_R = getAngleToAsteroid(*asteroidToHit, ship_state, false);
            int shipAngle = ship_state.millidegree_rotation;
            if (shipAngle < angleToFire_L) {
                my_game_ai->lastYaw = SuggestedAction::YawingClockwise;
                return SuggestedAction{ SuggestedAction::YawingClockwise, SuggestedAction::FiringTry };
            } if (shipAngle > angleToFire_R) {
                my_game_ai->lastYaw = SuggestedAction::YawingAntiClockwise;
                return SuggestedAction{ SuggestedAction::YawingAntiClockwise, SuggestedAction::FiringTry };
            } else {
                //my_game_ai->lastYaw = SuggestedAction::YawingStop;
                //return SimpleActions::STOP_YAWING_AND_FIRE;
                return SuggestedAction{ my_game_ai->lastYaw, SuggestedAction::FiringTry };
            }
        } else {
            /*if (!my_game_ai->hasAngle) {
                int angleToFire = getAngleToShoot(*asteroidToHit, ship_state, game_info);
                my_game_ai->destAngle = angleToFire;
                my_game_ai->hasAngle = true;
            }*/
            
            int angle = getAngleToShoot(*asteroidToHit, ship_state, game_info);
            int shipAngle = ship_state.millidegree_rotation;
            if (shipAngle < angle) {
                    my_game_ai->lastYaw = SuggestedAction::YawingClockwise;
                    return SuggestedAction{ SuggestedAction::YawingClockwise, SuggestedAction::FiringTry };
                } if (shipAngle > angle) {
                    my_game_ai->lastYaw = SuggestedAction::YawingAntiClockwise;
                    return SuggestedAction{ SuggestedAction::YawingAntiClockwise, SuggestedAction::FiringTry };
                } else {
                    //my_game_ai->lastYaw = SuggestedAction::YawingStop;
                    my_game_ai->hasAngle = false;
                    return SuggestedAction{ my_game_ai->lastYaw, SuggestedAction::FiringTry };
                    //return SimpleActions::STOP_YAWING_AND_FIRE;
                }
        }
    }

}

int getAngleToShoot (const Asteroid& asteroid, const ShipState& shipState, const GameInfo& gameInfo) {
    const float correctionFactor = 100;
    sf::Vector2f shipDot = getShipDot(shipState);
    sf::Vector2f astDot = getAsteroidDot(asteroid);
    int rotationAngle = getAngleToPoint(shipDot, astDot);
    float t = (float) rotationAngle / (float) gameInfo.ship_rotation_speed_millidegrees_per_frame;
    sf::Vector2f newPoint = astDot + (sf::Vector2f) asteroid.getVelocity() * t;
    rotationAngle = getAngleToPoint(shipDot, newPoint);
    sf::Vector2i vel = asteroid.getVelocity();
    int velMag = vel.x * vel.x + vel.y * vel.y;
    velMag = (int) std::sqrt(velMag);
    float correction = correctionFactor * velMag;
    if (rotationAngle < 0)
        correction *= -1;
    return rotationAngle + correction;
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

const Asteroid* getFastestToHitAsteroid (const AsteroidsObserver& asteroidsObserver, const ShipState& shipState, const GameInfo& gameInfo) {
    if (asteroidsObserver.asteroids().isEmpty())
        return nullptr;
    const Asteroid* asteroid = &asteroidsObserver.asteroids().front();
    float timeToHit = -1.0;

    sf::Vector2f shipDot( shipState.hitbox.left + shipState.hitbox.width/2.0f,
                          shipState.hitbox.top + shipState.hitbox.height/2.0f );

    for (const AsteroidListItem* it = asteroidsObserver.asteroids().begin();
            it != asteroidsObserver.asteroids().end();
            it = it->getNext()
    ) {
        Asteroid ast = it->getData();
        sf::Vector2f asteroidDot (ast.getCurrentHitbox().left + ast.getCurrentHitbox().width / 2,
                                    ast.getCurrentHitbox().top + ast.getCurrentHitbox().height / 2);
        //sf::Vector2i velocity = ast.getVelocity();
        int milliangle = getAngleToShoot(ast, shipState, gameInfo);
        /*if (shipDot.x > ast.getCurrentHitbox().left && shipDot.x < ast.getCurrentHitbox().left + ast.getCurrentHitbox().width)
            milliangle = 0;
        else if (shipDot.x > ast.getCurrentHitbox().left)
            milliangle = getAngleToAsteroid(ast, shipState, true);
        else
            milliangle = getAngleToAsteroid(ast, shipState, false);*/
        
        float rotationTime = std::abs((float) milliangle / (float) gameInfo.ship_rotation_speed_millidegrees_per_frame);
        sf::Vector2f shipToAst (asteroidDot.x - shipDot.x, asteroidDot.y - shipDot.y);
        float distance = shipToAst.x * shipToAst.x + shipToAst.y * shipToAst.y;
        float shotTime = distance / gameInfo.phaser_pulse_speed;
        float totalTime = shotTime + rotationTime;
        if (timeToHit < 0 || totalTime < timeToHit) {
            timeToHit = totalTime;
            asteroid = &it->getData();
        }
    }
    return asteroid;
}

sf::Vector2f getShipDot(const ShipState& shipState) {
    sf::Vector2f shipDot( shipState.hitbox.left + shipState.hitbox.width/2,
                          shipState.hitbox.top + shipState.hitbox.height/2 );
    return shipDot;
}

sf::Vector2f getAsteroidDot(const Asteroid& ast) {
    sf::Vector2f asteroidDot (ast.getCurrentHitbox().left + ast.getCurrentHitbox().width / 2,
                                    ast.getCurrentHitbox().top + ast.getCurrentHitbox().height / 2);
    return asteroidDot;
}
