#include "Slinger.hpp"

using namespace Piece_n;

Slinger_c::Slinger_c() : //Player_n::Player_c& player_owner) :
   Piece_c(), //player_owner)
   mPrimaryDirection(Direction_e::NONE),
   mSecondaryDirection(Direction_e::NONE)
{
   mDeploymentZones = SLINGER_DEPLOYMENT_ZONES;
}

Slinger_c::~Slinger_c()
{
}

const Direction_e& Slinger_c::getDirection()
{
   return mPrimaryDirection;
}

void Slinger_c::setDirection(const Direction_e& new_direction)
{
   if (mPrimaryDirection != new_direction)
   {
      mPrimaryDirection = new_direction;
      mSecondaryDirection = static_cast<Direction_e>((new_direction - 1) % 4);
   }
}