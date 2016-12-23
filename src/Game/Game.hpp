#ifndef _GAME_HPP_
#define _GAME_HPP_

#include <map>

#include "Player.hpp"
#include "ResourceManager/ResourceManager.hpp"

namespace Game_n
{
   class Game_c
   {
   public:
      // constructor
      Game_c(const ResourceManager_n::ResourceManager_c& resource_manager);

      // default destructor
      ~Game_c();

      // \Name: start
      // \Description:
      // - initializes the board and game state
      // \Argument:
      // - none
      // \Returns
      // - none
      void start();

      // \Name: move
      // \Description:
      // - moves a piece
      // \Argument:
      // - Player_c&, the player moving the piece
      // - Piece_c&, the piece to move
      // - int, the x coordinate of the move position
      // - int, the y coordinate of the move position
      // \Returns
      // - none
      // void move(Player_n::Player_c& player, Piece_n::Piece_c& piece, int move_x, int move_y);

      // \Name: shootout
      // \Description:
      // - initiates a shootout event for both players
      // \Argument:
      // - none
      // \Returns
      // - none
      void shootout();

      // \Name: currentPlayer
      // \Description:
      // - returns the player whose turn it is
      // \Argument:
      // - none
      // \Returns
      // - Player_c&, the player whose turn it is
      Player_n::Player_c& currentPlayer();

      // \Name: nextPlayer
      // \Description:
      // - passes the turn to the player whose turn it is not
      // \Argument:
      // - none
      // \Returns
      // - Player_c&, the player the turn was passed to
      Player_n::Player_c& nextPlayer();

      handleLmbDown()
   protected:
      // \Name: findUserById
      // \Description:
      // - returns the User_c object associated with id parameter
      // \Argument:
      // - int, the user's ID to search for
      // \Returns
      // - const User_c&, a reference to the user whose ID was passed
      // const User_c& findUserById(int id);

      // \Name: drawPieces
      // \Description:
      // - draws the reserve Piece_c objects in their proper places
      // \Argument:
      // - none
      // \Returns
      // - none     
      void drawPieces(); 

      // \Name: drawReservePieces
      // \Description:
      // - draws the reserve Piece_c objects in their proper places
      // \Argument:
      // - none
      // \Returns
      // - none
      // void drawReservePieces();

      // \Name: drawLivePieces
      // \Description:
      // - draws the live Piece_c objects in their proper places
      // \Argument:
      // - none
      // \Returns
      // - none
      // void drawLivePieces();

      // \Name: drawDeadPieces
      // \Description:
      // - draws the dead Piece_c objects in their proper places
      // \Argument:
      // - none
      // \Returns
      // - none
      // void drawDeadPieces();

      // \Name: gameOver
      // \Description:
      // - checks for a win state from either player
      // \Argument:
      // - none
      // \Returns
      // - bool, true if the current player is the winner
      bool gameOver();

      // store the two Player_c objects created to handle game events
      Player_n::Player_c& mPlayer1;
      Player_n::Player_c& mPlayer2;

      // a map that matches a string representation of an int key to a user's ID
      // std::map<char, int> mUserIds;

      // may want to add a container for spectator IDs
      // std::map<char, int> mSpectatorIds;

      // tracks the number of turns the game has been active
      int mCurrentTurn; 

      // key associated with the ID of the user whose turn it is
      char mCurrentUser;

      // reference to the database of all users? (for findUserById)
   };
}

#endif _GAME_HPP_