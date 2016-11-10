#include "Slinger.hpp"

using namespace Piece_n;

Slinger_c::Slinger_c():
   Piece_c()
{
   // Initialize mDeploymentZones
}

Slinger_c::~Slinger_c()
{
}

void Slinger_c::setDirection(Direction_e new_direction)
{
   if (!mDirection)
   {
      // direction will be set to a default value based on deployment position
      // -> as such, we should not be dealing with this case from here - fire an error
   }
   else 
   {
      mPrimaryDirection = new_direction;
      mSecondaryDirection = (new_direction + 1) % 4;
   }
}