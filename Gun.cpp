#include "Gun.hpp"

using namespace Piece_n;

Gun_c::Gun_c(Player_c& player_owner) :
   Piece_c(player_owner)
{
   mDeploymentZones = DEPLOYMENT_ZONES;
}

Gun_c::~Gun_c()
{
}

const Direction_e& Gun_c::getDirection()
{
   if (mDirection)
   {
      return mDirection;
   }
   else
   {
      // fire an error
   }
}

void Gun_c::setDirection(const Direction_e& new_direction)
{
   if (!mDirection)
   {
      // direction will be set to a default value based on deployment position
      // -> as such, we should not be dealing with this case from here - fire an error
   }
   else 
   {
      if (mDirection != new_direction)
      {
         mDirection = new_direction;
      }
      else
      {
         // fire an error message
      }
   }
}