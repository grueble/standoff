#include "Gun.hpp"

using namespace Piece_n;

Gun_c::Gun_c() : // Player_n::Player_c& player_owner) :
   Piece_c(), // player_owner)
   mDirection(Piece_n::Direction_e::NONE)
{
   mDeploymentZones = GUN_DEPLOYMENT_ZONES;
}

Gun_c::~Gun_c()
{
}

const Direction_e& Gun_c::getDirection()
{
   return mDirection;
}

void Gun_c::setDirection(const Direction_e& new_direction)
{
   if (mDirection != new_direction)
   {
      mDirection = new_direction;
   }
}