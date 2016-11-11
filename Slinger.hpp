#ifndef _SLINGER_HPP_
#define _SLINGER_HPP_

#include "Piece.hpp"

namespace Piece_n
{
   static const std::vector<std::pair<int, int>> DEPLOYMENT_ZONES = {};

   class Slinger_c : public Piece-c
   {
   public:
      // constructor
      Slinger_c(Player_c& player_owner);

      // default destructor
      ~Slinger_c();

      // \Name: s=getDirection
      // \Description:
      // - gets this Piece's direction
      // \Argument:
      // - none
      // \Returns
      // - Direction_e&, this Piece's direction member field
      const Direction_e& getDirection()

      // \Name: setDirection
      // \Description:
      // - sets the mDirection member field
      // \Argument:
      // - Direction_e, the new direction to set
      // \Returns
      // - none
      void setDirection(const Direction_e& new_direction);

   protected:
      // this Slinger's primary direction
      Direction_e mPrimaryDirection;

      // this Slinger's secondary direction, 90 degrees counter-clockwise from the primary direction
      Direction_e mSecondaryDirection;
   };
}

#endif _SLINGER_HPP_