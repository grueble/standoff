#ifndef _GAME_HPP_
#define _GAME_HPP_

#include "Piece.hpp"
#include <cstddef>
#include <memory>

namespace Game_n
{
   // defines a smart pointer wrapper for the piece class
   typedef std::shared_ptr<Piece_n::Piece_c> PiecePtr;

   // the board's upper left hand corner in "screen tile" coordinates
   // -> the window's upper left hand corner is at (0, 0)
   static const std::pair<int,int>& BOARD_COORD = std::make_pair(5, 1); 

   // the types of actions that can be executed by the game logic
   enum Action_e
   {
      NONE,
      PRE_DEPLOY,
      DEPLOY,
      MOVE,
      ROTATE,
      SHOOTOUT,
   };

   // stores data about a staged move prior to confirmation
   struct Move_s
   {
      Move_s() :
         mMovedPiece(nullptr),
         mCurrentAction(NONE)
      {

      }

      PiecePtr mMovedPiece;
      std::pair<int, int> mPrevPosition;
      Piece_n::Direction_e mPrevDirection;
      Action_e mCurrentAction;
   };

   class Game_c
   {
   public:
      // constructor
      Game_c();

      // default destructor
      ~Game_c();

      // moves a piece if the passed coordinate is valid 
      void move(const std::pair<int, int>& screen_tile_coord);

      // rotates a piece if the passed direction is valid
      void rotate(const Piece_n::Direction_e& rotate_direction);

      // initiates a shootout event
      void shootout();

      // passes the turn to the player whose turn it is not
      void nextPlayer();

      // reverts a moved piece to its previous state
      void revertMove();

      // returns the currently selected piece
      PiecePtr getCurrentPiece();

      // selects the passed piece
      void setCurrentPiece(PiecePtr piece_ptr);

      // returns the current position of the briefcase game object
      std::pair<int, int>& getBriefcasePosition();

      // return Player 1's pieces
      const std::vector<PiecePtr>& getPlayer1Pieces();

      // return Player 2's pieces
      const std::vector<PiecePtr>& getPlayer2Pieces();

      // returns the current Move_s structure
      Move_s& getCurrentMove();

      // checks for a win state from either player
      bool gameOver();

      // flag used to ascertain the current player; true if p1, false o/w
      int mCurrentTurn;

   protected:
      // the board's side length, used for validating moves
      static const int& BOARD_SIDE_LENGTH;

      /* 
       * starting "screen tile" coordinates used for calculating 
       * each player's reserve piece positions
       */
      static const std::pair<int,int>& P1_RESERVE_COORD;
      static const std::pair<int,int>& P2_RESERVE_COORD;

      // scoring "screen tile" coordinates for each player
      static const std::pair<int, int>& P1_SCORING_COORD;
      static const std::pair<int, int>& P2_SCORING_COORD;

      // the briefcase's starting position
      static const std::pair<int, int>& BRIEFCASE_COORD;

      // piece totals by piece type 
      static const int NUM_PAWNS;
      static const int NUM_GUNS;
      static const int NUM_SLINGERS;

      // initializes both player's pieces
      void initPieces();

      // determines if a piece hits during a shootout  
      void detectHit(Piece_n::Piece_c& piece, std::vector<PiecePtr>& pieces);

      // stores references to the two player's piece lists
      std::vector<PiecePtr> mPlayer1Pieces;
      std::vector<PiecePtr> mPlayer2Pieces;

      // pointer to the current piece, if any
      PiecePtr mCurrentPiece;

      // the current position of the briefcase game object
      std::pair<int, int> mBriefcasePosition;

      // structure containing info about a staged move
      Move_s mCurrentMove;
   };
}

#endif
