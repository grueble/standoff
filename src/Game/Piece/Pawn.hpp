#ifndef _PAWN_HPP_
#define _PAWN_HPP_

#include "Piece.hpp"

namespace Piece_n
{
   static const Direction_e PAWN_DIRECTION = Direction_e::NONE;

   class Pawn_c : public Piece_c
   {
   public:
      // default constructur
      Pawn_c(); // Player_n::Player_c& player_owner);

      // default destructor
      ~Pawn_c();

      // \Name: getDirection
      // \Description:
      // - gets this Piece's direction
      // \Argument:
      // - none
      // \Returns
      // - Direction_e&, this Piece's direction member field
      const Direction_e& getDirection();

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