#ifndef _PAWN_HPP_
#define _PAWN_HPP_

#include "Piece.hpp"

namespace Piece_n
{
   class Pawn_c : public Piece_c
   {
      // default constructur
      Pawn_c();

      // default destructor
      ~Pawn_c();

      // getSprite function to load an image for drawing

      // \Name: setDirection
      // \Description:
      // - a Pawn has no mDirection member field; this will fire a warning
      // \Argument:
      // - n/a
      // \Returns
      // - none
      void setDirection(Direction_e new_direction);
   };
}

#endif _PAWN_HPP_