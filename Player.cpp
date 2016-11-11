#include "Player.hpp"

static const int NUM_PAWNS = 4;
static const int NUM_GUNS = 6;
static const int NUM_SLINGERS = 2;

using namespace Player_n;

// Constructor
Player_c::Player_c()
{
   init_pieces();
}

// Destructor
Player_c::~Player_c()
{
}

// GetReservePieces
std::vector<Piece>& Player_c::getReservePieces()
{
   return mReservePieces;
}

// GetLivePieces
std::vector<Piece>& Player_c::getLivePieces()
{
   return mLivePieces;
}

// GetDeadPieces
std::vector<Piece>& Player_c::getDeadPieces()
{
   return mDeadPieces;
}

void deploy(Piece_c& reserve_piece, const std::pair<int, int>& deploy_position)
{
   if (reserve_piece.mPlayStatus == Piece_n::RESERVE)
   {
      reserve_piece.setPosition(deploy_position);
   }
}

void move(Piece_c& live_piece, const std::pair<int, int>& move_position)
{
   if (live_piece.mPlayStatus == Piece_n::LIVE)
   {
      live_piece.setPosition(move_position);
   }
}

void rotate(Piece_c& live_piece, const Piece_n::Direction_e& rotate_direction)
{
   if (live_piece.mPlayStatus == Piece_n::LIVE)
   {
      live_piece.setDirection(rotate_direction);
   }
}

void Player_c::initShootout()
{
   mGame.shootout();
}

void Player_c::initPieces()
{
   // add pawns
   for (int i = 0; i < NUM_PAWNS; ++i)
   {
      Pawn new_pawn = new Pawn(*this);
      mPieces.push_back(new_pawn);
   }

   // add guns
   for (int i = 0; i < NUM_GUNS; ++i)
   {
      Gun new_gun = new Gun(*this); 
      mPieces.push_back(new_gun);
   }

   // add slingers
   for (int i = 0; i < NUM_SLINGERS; ++i)
   {
      Slinger new_slinger = new Slinger(*this);
      mPieces.push_back(new_slinger);
   }
}
