#ifndef _GUN_HPP_
#define _GUN_HPP_

#include "Piece.hpp"

namespace Piece_n
{
   class Gun_c : public Piece_c
   {
   public:
      // default constructor
      Gun_c();

      // default destructor
      ~Gun_c();

      // getSprite function to load an image for drawing

      // \Name: setDirection
      // \Description:
      // - sets the mDirection member field
      // \Argument:
      // - Direction_e, the new direction to set
      // \Returns
      // - none
      void setDirection(Direction_e const& new_direction);

   protected:
      // the direction this Gun is currently facing
      Direction_e mDirection;
   };
}

#endif _GUN_HPP