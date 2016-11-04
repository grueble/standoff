#ifndef _PAWN_HPP_
#define _PAWN_HPP_

#include "Piece.hpp"

namespace Piece_n
{
   class Pawn_c : public Piece_c
   {
      Pawn_c();

      ~Pawn_c();

      // getSprite function to load an image for drawing

      void setDirection(Direction_e new_direction);

      bool isValidDeployment(std::pair<int, int> deploy_position);
   }
}

#endif _PAWN_HPP_