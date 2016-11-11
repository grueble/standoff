#ifndef _PAWN_HPP_
#define _PAWN_HPP_

#include "Piece.hpp"

namespace Piece_n
{
   static const std::vector<std::pair<int, int>> DEPLOYMENT_ZONES = {};

   class Pawn_c : public Piece_c
   {
      // default constructur
      Pawn_c(Player_c& player_owner);

      // default destructor
      ~Pawn_c();

      // getSprite function to load an image for drawing

      // \Name: setDirection
      // \Description:
      // - a Pawn has no mDirection member field; this will fire a warning
      // \Argument:
      // - none
      // \Returns
      // - none
      void setDirection(const Direction_e& new_direction);
   };
}

#endif _PAWN_HPP_