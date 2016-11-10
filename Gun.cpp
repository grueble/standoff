#include "Gun.hpp"

using namespace Piece_n;

Gun_c::Gun_c():
   Piece_c()
{
   // Initialize mDeploymentZones
}

Gun_c::~Gun_c()
{
}

void Gun_c::setDirection(Direction_e new_direction)
{
   if (!mDirection)
   {
      // direction will be set to a default value based on deployment position
      // -> as such, we should not be dealing with this case from here - fire an error
   }
   else 
   {
      mDirection = new_direction;
   }
}