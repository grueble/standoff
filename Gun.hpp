#ifndef _GUN_HPP_
#define _GUN_HPP_

#include "Piece.hpp"

namespace Piece_n
{
   static const std::vector<std::pair<int, int>> DEPLOYMENT_ZONES = {};

   class Gun_c : public Piece_c
   {
   public:
      // constructor
      Gun_c(Player_c& player_owner);

      // default destructor
      ~Gun_c();

      // \Name: s=getDirection
      // \Description:
      // - gets this Piece's direction
      // \Argument:
      // - none
      // \Returns
      // - Direction_e&, this Piece's direction member field
      const Direction_e& getDirection();

      // \Name: setDirection
      // \Description:
      // - sets the mDirection member field
      // \Argument:
      // - Direction_e, the new direction to set
      // \Returns
      // - none
      void setDirection(const Direction_e& new_direction);

   protected:
      // the direction this Gun is currently facing
      Direction_e mDirection;
   };
}

#endif _GUN_HPP