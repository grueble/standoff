#include "Slinger.hpp"

using namespace Piece_n;

Slinger_c::Slinger_c(Player_c& player_owner) :
   Piece_c(player_owner)
{
   mDeploymentZones = DEPLOYMENT_ZONES;
}

Slinger_c::~Slinger_c()
{
}

const Direction_e& Slinger_c::getDirection()
{

}

void Slinger_c::setDirection(const Direction_e& new_direction)
{
   if (!mDirection)
   {
      // direction will be set to a default value based on deployment position
      // -> as such, we should not be dealing with this case from here - fire an error
   }
   else 
   {
      if (mPrimaryDirection != new_direction)
      {
         mPrimaryDirection = new_direction;
         mSecondaryDirection = (new_direction - 1) % 4;
      }
      else
      {
         // fire an error message
      }
   }
}