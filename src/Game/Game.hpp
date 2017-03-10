#ifndef _GAME_HPP_
#define _GAME_HPP_

#include <memory>
#include "Piece.hpp"

namespace Game_n
{
   typedef std::shared_ptr<Piece_n::Piece_c> PiecePtr;

   // the board's upper left hand corner in "screen tile" coordinates
   // -> the window's upper left hand corner is at (0, 0)
   static const std::pair<int,int>& BOARD_COORD = std::make_pair(5, 1); 

   // the board's side length
   static const int& BOARD_SIDE_LENGTH = 9;

   // the valid gun/slinger deployment positions for both players
   static const std::vector<std::pair<int, int>> P1_GUNSLINGER_DEPLOYMENT_ZONES =
   {
      std::make_pair(BOARD_COORD.first + 1, BOARD_COORD.second + (BOARD_SIDE_LENGTH - 1)),
      std::make_pair(BOARD_COORD.first + 3, BOARD_COORD.second + (BOARD_SIDE_LENGTH - 1)),
      std::make_pair(BOARD_COORD.first + 7, BOARD_COORD.second + (BOARD_SIDE_LENGTH - 1)),
      std::make_pair(BOARD_COORD.first + (BOARD_SIDE_LENGTH - 1), BOARD_COORD.second + 1),
      std::make_pair(BOARD_COORD.first + (BOARD_SIDE_LENGTH - 1), BOARD_COORD.second + 3),
      std::make_pair(BOARD_COORD.first + (BOARD_SIDE_LENGTH - 1), BOARD_COORD.second + 7)
   };

   static const std::vector<std::pair<int, int>> P2_GUNSLINGER_DEPLOYMENT_ZONES =
   {
      std::make_pair(BOARD_COORD.first + 1, BOARD_COORD.second),
      std::make_pair(BOARD_COORD.first + 3, BOARD_COORD.second),
      std::make_pair(BOARD_COORD.first + 7, BOARD_COORD.second),
      std::make_pair(BOARD_COORD.first, BOARD_COORD.second + 1),
      std::make_pair(BOARD_COORD.first, BOARD_COORD.second + 3),
      std::make_pair(BOARD_COORD.first, BOARD_COORD.second + 7)
   };

   // starting "screen tile" coordinates for reserve piece positions
   static const std::pair<int,int>& P1_RESERVE_COORD = std::make_pair(15, 9);
   static const std::pair<int,int>& P2_RESERVE_COORD = std::make_pair(3, 1);

   // scoring "screen tile" coordinates
   static const std::pair<int, int>& P1_SCORING_COORD = std::make_pair(13, 9);
   static const std::pair<int, int>& P2_SCORING_COORD = BOARD_COORD;

   // the briefcase's starting position
   static const std::pair<int, int>& BRIEFCASE_COORD = std::make_pair(BOARD_COORD.first + 4, BOARD_COORD.second + 4);

   // piece totals
   static const int& NUM_PAWNS = 4;
   static const int& NUM_GUNS = 6;
   static const int& NUM_SLINGERS = 2;

   enum Action_e
   {
      NONE,
      PRE_DEPLOY,
      DEPLOY,
      MOVE,
      ROTATE,
      SHOOTOUT,
   };

   // stores all of the data about a player
   struct Player_s
   {
      Player_s() :
         mUserId(0)
      {

      }

      int mUserId;
      std::vector<PiecePtr> mPieces;
   };

   // stores data about a staged move
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

      // \Name: move
      // \Description:
      // - moves a piece
      // \Argument:
      // - const std::pair<int, int>&, the click position in screen tile coordinates
      // \Returns
      // - none
      void move(const std::pair<int, int>& screen_tile_coord);

      // \Name: rotate
      // \Description:
      // - initiates a shootout event
      // \Argument:
      // - Direction
      // \Returns
      // - none
      void rotate(const Piece_n::Direction_e& rotate_direction);

      // \Name: shootout
      // \Description:
      // - initiates a shootout event for both players
      // \Argument:
      // - none
      // \Returns
      // - none
      void shootout();

      // \Name: nextPlayer
      // \Description:
      // - passes the turn to the player whose turn it is not
      // \Argument:
      // - none
      // \Returns
      // - none
      void nextPlayer();

      // \Name: revertMove
      // \Description:
      // - reverts a moved piece to its previous position
      // \Argument:
      // - none
      // \Returns
      // - none
      void revertMove();

      // \Name: getCurrentPiece
      // \Description:
      // - returns the currently seleced piece
      // \Argument:
      // - none
      // \Returns
      // - Piece_c&, the currently selected piece
      PiecePtr getCurrentPiece();

      // \Name: setCurrentPiece
      // \Description:
      // - sets the currently seleced piece
      // \Argument:
      // - Piece_c&, the piece to set as current
      // \Returns
      // - none
      void setCurrentPiece(PiecePtr piece_ptr);

      std::pair<int, int>& getBriefcasePosition();

      // \Name: getPlayer1Pieces
      // \Description:
      // - returns a particular player's pieces
      // \Argument:
      // - none
      // \Returns
      // - const std::vector<Piece_n::Piece_c>&, the passed player's pieces
      const std::vector<PiecePtr>& getPlayer1Pieces();

      // \Name: getPlayer2Pieces
      // \Description:
      // - returns a particular player's pieces
      // \Argument:
      // - none
      // \Returns
      // - const std::vector<Piece_n::Piece_c>&, the passed player's pieces
      const std::vector<PiecePtr>& getPlayer2Pieces();

      // \Name: getCurrentMove
      // \Description:
      // - reverts a moved piece to its previous position
      // \Argument:
      // - none
      // \Returns
      // - none
      Move_s& Game_c::getCurrentMove();

      // \Name: gameOver
      // \Description:
      // - checks for a win state from either player
      // \Argument:
      // - none
      // \Returns
      // - bool, true if the current player is the winner
      bool gameOver();

      // flag used to ascertain the current player; true if p1, false o/w
      int mCurrentTurn;

   protected:
      // \Name: initPieces
      // \Description:
      // - initializes a player's pieces
      // \Argument:
      // - Player_s&, the player to initialize pieces for 
      // - bool, true if player is mPlayer1, false o/w
      // \Returns
      // - none
      void initPieces(); //Player_s& player, bool player1);

      // \Name: detectHit
      // \Description:
      // - determines if a piece hits during a shootout
      // \Argument:
      // - none
      // \Returns
      // - none     
      void detectHit(Piece_n::Piece_c& piece, std::vector<PiecePtr>& pieces);

      // stores references to the two Player_s objects initialized on construction
      Player_s mPlayer1;
      Player_s mPlayer2;

      // pointer to the current piece, if any
      PiecePtr mCurrentPiece;

      std::pair<int, int> mBriefcasePosition;

      // struct containing info about a staged move
      Move_s mCurrentMove;
   };
}

#endif _GAME_HPP_