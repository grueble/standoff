#ifndef _PIECE_HPP_
#define _PIECE_HPP_

#include "Player.hpp"

namespace Piece_n
{
   // the possible directions a piece could be facing
   enum Direction_e
   {
      UP = 0,
      RIGHT = 1,
      DOWN = 2,
      LEFT = 3
   };

   class Piece_c
   {
   public:
      Piece_c();

      ~Piece_c();

      std::pair<int, int> getPosition();

      void setPosition(const int row, const int column);

      virtual void setDirection(Direction_e new_direction) = 0;

      virtual bool isValidDeployment(std::pair<int, int> deploy_position) = 0;

      bool isValidMove(std::pair<int, int> move_position);

   protected:
      // reference to the Player that owns this Piece
      // do I change this out for a player ID? this would make instantiation easier. currently never calling Player methods
      Player_c &mPlayer;

      // this Piece's current board position
      // -> this field is null if this Piece is not in play [ instead of this, may want to initialize to (-1, -1) ]
      std::pair<int, int> mPosition;

      // stores a Piece's available deployment zones
      std::vector<std::pair<int, int>> mDeploymentZones;
   }
}

#endif _PIECE_HPP_