#ifndef _SLINGER_HPP_
#define _SLINGER_HPP_

#include "Piece.hpp"

namespace Piece_n
{
   class Slinger_c : public Piece-c
   {
   public:
      //default constructor
      Slinger_c();

      //default destructor
      ~Slinger_c();

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
      // this Slinger's primary direction
      Direction_e mPrimaryDirection;

      // this Slinger's secondary direction, 90 degrees clockwise of the primary direction
      Direction_e mSecondaryDirection;
   };
}

#endif _SLINGER_HPP_