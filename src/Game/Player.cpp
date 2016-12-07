#include "Player.hpp"

static const int NUM_PAWNS = 4;
static const int NUM_GUNS = 6;
static const int NUM_SLINGERS = 2;

using namespace Player_n;

Player_c::Player_c(const int& user_id) :
   mUserId(user_id)
{
   initPieces();
}

Player_c::~Player_c()
{
   // free up the Piece container
}

std::vector<Piece_n::Piece_c>& Player_c::getPieces()
{
   return mPieces;
}

std::vector<Piece_n::Piece_c>& Player_c::getReservePieces()
{
   std::vector<Piece_n::Piece_c> reserve_pieces;
   std::vector<Piece_n::Piece_c>::iterator it;
   for(it = this->getPieces().begin(); it != this->getPieces().end(); ++it)
   {
      if (it->getPlayState() == Piece_n::PlayState_e::RESERVE)
      {
         reserve_pieces.push_back(*it);
      }
   }
   return reserve_pieces;
}

std::vector<Piece_n::Piece_c>& Player_c::getLivePieces()
{
   std::vector<Piece_n::Piece_c> live_pieces;
   std::vector<Piece_n::Piece_c>::iterator it;
   for(it = this->getPieces().begin(); it != this->getPieces().end(); ++it)
   {
      if (it->getPlayState() == Piece_n::PlayState_e::RESERVE)
      {
         live_pieces.push_back(*it);
      }
   }
   return live_pieces;
}

std::vector<Piece_n::Piece_c>& Player_c::getDeadPieces()
{
   std::vector<Piece_n::Piece_c> dead_pieces;
   std::vector<Piece_n::Piece_c>::iterator it;
   for(it = this->getPieces().begin(); it != this->getPieces().end(); ++it)
   {
      if (it->getPlayState() == Piece_n::PlayState_e::RESERVE)
      {
         dead_pieces.push_back(*it);
      }
   }
   return dead_pieces;
}

void deploy(Piece_n::Piece_c& reserve_piece, const std::pair<int, int>& deploy_position)
{
   if (reserve_piece.getPlayState() == Piece_n::RESERVE)
   {
      reserve_piece.setPosition(deploy_position);
      reserve_piece.nextPlayState();

      // set direction to default for a particular deployment
   }
   else
   {
      // fire an error
   }
}

void move(Piece_n::Piece_c& live_piece, const std::pair<int, int>& move_position)
{
   if (live_piece.getPlayState() == Piece_n::LIVE)
   {
      live_piece.setPosition(move_position);
   }
   else
   {
      // fire an error
   }
}

void rotate(Piece_n::Piece_c& live_piece, const Piece_n::Direction_e& rotate_direction)
{
   if (live_piece.getPlayState() == Piece_n::LIVE)
   {
      if (live_piece.getPieceType() != Piece_n::PAWN)
      {
         live_piece.setDirection(rotate_direction);
      }
   }
}

void Player_c::initShootout()
{
   // mGame.shootout();
}

void Player_c::initPieces()
{
   // add pawns
   for (int i = 0; i < NUM_PAWNS; ++i)
   {
      Piece_n::Piece_c* new_pawn = new Piece_n::Piece_c(Piece_n::PAWN, PAWN_DEPLOYMENT_ZONES);
      mPieces.push_back(*new_pawn);
   }

   // add guns
   for (int i = 0; i < NUM_GUNS; ++i)
   {
      Piece_n::Piece_c* new_gun = new Piece_n::Piece_c(Piece_n::GUN, GUN_DEPLOYMENT_ZONES);
      mPieces.push_back(*new_gun);
   }

   // add slingers
   for (int i = 0; i < NUM_SLINGERS; ++i)
   {
      Piece_n::Piece_c* new_slinger = new Piece_n::Piece_c(Piece_n::SLINGER, SLINGER_DEPLOYMENT_ZONES);
      mPieces.push_back(*new_slinger);
   }
}
