#include <vector>

#include "Game.hpp"
#include "Piece.hpp"

using namespace Game_n;

Game_c::Game_c(int first_user_id, int second_user_id)
{
   // store off the IDs of the participating users
   mUserIds['1'] = first_user_id;
   mUserIds['2'] = second_user_id;

   // instantiate two new Player objects to handle in-game events
   mPlayer1 = Player_c(findUserById(first_user_id));
   mPlayer2 = Player_c(findUserById(second_user_id));
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

      draw();
   }
}

const User_c& Game_c::findUserById(int user_id)
{
   // search the SQL DB for a certain USER, then return
   // do we want to use the User_c& or simply their ID
}

void Game_c::drawReservePieces()
{
   const std::vector<Piece_c>& p1_reserve_pieces = mPlayer1.GetReservePieces();
   std::vector<Piece_c>::iterator p1_it;
   for (p1_it = p1_reserve_pieces.begin(); p1_it != p1_reserve_pieces.end(); ++p1_it)
   {

   }

   const std::vector<Piece_c>& p2_reserve_pieces = mPlayer2.GetReservePieces();
   std::vector<Piece_c>::iterator p1_it;
   for (p1_it = p1_reserve_pieces.begin(); p1_it != p1_reserve_pieces.end(); ++p1_it)
   {

   }
}

void Game_c::drawLivePieces()
{
   const std::vector<Piece_c>& p1_live_pieces = mPlayer1.GetLivePieces();
   std::vector<Piece_c>::iterator p1_it;
   for (p1_it = p1_live_pieces.begin(); p1_it != p1_live_pieces.end(); ++p1_it)
   {

   }

   const std::vector<Piece_c>& p2_live_pieces = mPlayer2.GetLivePieces();
   std::vector<Piece_c>::iterator p1_it;
   for (p1_it = p1_live_pieces.begin(); p1_it != p1_live_pieces.end(); ++p1_it)
   {

   }
}

void Game_c::drawDeadPieces()
{
   const std::vector<Piece_c>& p1_dead_pieces = mPlayer1.GetDeadPieces();
   std::vector<Piece_c>::iterator p1_it;
   for (p1_it = p1_dead_pieces.begin(); p1_it != p1_dead_pieces.end(); ++p1_it)
   {

   }

   const std::vector<Piece_c>& p2_dead_pieces = mPlayer2.GetDeadPieces();
   std::vector<Piece_c>::iterator p1_it;
   for (p1_it = p1_dead_pieces.begin(); p1_it != p1_dead_pieces.end(); ++p1_it)
   {

   }
}

bool Game::gameOver()
{

}