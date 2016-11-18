#include <vector>

#include "Game.hpp"
#include "Piece.hpp"

using namespace Game_n;

Game_c::Game_c(int first_user_id, int second_user_id) :
   mPlayer1(Player_n::Player_c(first_user_id)),
   mPlayer2(Player_n::Player_c(second_user_id))
{
   // store off the IDs of the participating users
   mUserIds['1'] = first_user_id;
   mUserIds['2'] = second_user_id;

   // instantiate two new Player objects to handle in-game events
   // mPlayer1 = Player_c(findUserById(first_user_id));
   // mPlayer2 = Player_c(findUserById(second_user_id));
   // mPlayer1 = Player_n::Player_c(first_user_id);
   // mPlayer2 = Player_n::Player_c(second_user_id);
}

Game_c::~Game_c()
{
}

void Game_c::start()
{
   // load the initial board image (feat. "manholes", deploy zones, etc.)

   mCurrentTurn = 0; 

   while (!gameOver())
   {
      mCurrentTurn++;

      // based on who's turn it is, wait for a move from that user

      drawReservePieces();
      drawLivePieces();
      drawDeadPieces();
   }
}

// const User_c& Game_c::findUserById(int user_id)
// {
   // search the SQL DB for a certain USER, then return
   // do we want to use the User_c& or simply their ID
// }

void Game_c::drawReservePieces()
{
   const std::vector<Piece_n::Piece_c>& p1_reserve_pieces = mPlayer1.getReservePieces();
   std::vector<Piece_n::Piece_c>::const_iterator p1_it;
   for (p1_it = p1_reserve_pieces.begin(); p1_it != p1_reserve_pieces.end(); ++p1_it)
   {

   }

   const std::vector<Piece_n::Piece_c>& p2_reserve_pieces = mPlayer2.getReservePieces();
   std::vector<Piece_n::Piece_c>::const_iterator p2_it;
   for (p2_it = p2_reserve_pieces.begin(); p2_it != p2_reserve_pieces.end(); ++p2_it)
   {

   }
}

void Game_c::drawLivePieces()
{
   const std::vector<Piece_n::Piece_c>& p1_live_pieces = mPlayer1.getLivePieces();
   std::vector<Piece_n::Piece_c>::const_iterator p1_it;
   for (p1_it = p1_live_pieces.begin(); p1_it != p1_live_pieces.end(); ++p1_it)
   {

   }

   const std::vector<Piece_n::Piece_c>& p2_live_pieces = mPlayer2.getLivePieces();
   std::vector<Piece_n::Piece_c>::const_iterator p2_it;
   for (p2_it = p2_live_pieces.begin(); p2_it != p2_live_pieces.end(); ++p2_it)
   {

   }
}

void Game_c::drawDeadPieces()
{
   const std::vector<Piece_n::Piece_c>& p1_dead_pieces = mPlayer1.getDeadPieces();
   std::vector<Piece_n::Piece_c>::const_iterator p1_it;
   for (p1_it = p1_dead_pieces.begin(); p1_it != p1_dead_pieces.end(); ++p1_it)
   {

   }

   const std::vector<Piece_n::Piece_c>& p2_dead_pieces = mPlayer2.getDeadPieces();
   std::vector<Piece_n::Piece_c>::const_iterator p2_it;
   for (p2_it = p2_dead_pieces.begin(); p2_it != p2_dead_pieces.end(); ++p2_it)
   {

   }
}

bool Game_c::gameOver()
{
   return false;
}