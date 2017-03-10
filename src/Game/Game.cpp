#include <vector>
#include <iostream>

#include "Game.hpp"
#include "Piece.hpp"

using namespace Game_n;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - Constructor and Destructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Game_c::Game_c() :
   mCurrentTurn(1),
   mCurrentPiece(nullptr),
   mBriefcasePosition(BRIEFCASE_COORD)
{
   initPieces();
}

Game_c::~Game_c()
{
   mPlayer1.mPieces.clear();
   mPlayer2.mPieces.clear();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - Public Action Functions
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Game_c::move(const std::pair<int, int>& screen_tile_coord)
{
   // if the mouse click event's "screen tile" coordinate is within the board's bounds...
   if (screen_tile_coord.first >= BOARD_COORD.first &&
       screen_tile_coord.first < BOARD_COORD.first + BOARD_SIDE_LENGTH &&
       screen_tile_coord.second >= BOARD_COORD.second &&
       screen_tile_coord.second < BOARD_COORD.second + BOARD_SIDE_LENGTH)
   {
      std::cout << "Valid Click" << std::endl;

      // the briefcase moves with any piece that starts a move from its position
      if (mCurrentPiece->getPosition() == BRIEFCASE_COORD) 
      { 
         mBriefcasePosition = screen_tile_coord; 
      }

      // a piece moved from reserve waits for a direction
      if (mCurrentPiece->getPlayState() == Piece_n::RESERVE) 
      {
         if (mCurrentPiece->getPieceType() != Piece_n::PAWN)
         {
            mCurrentMove.mCurrentAction = PRE_DEPLOY;
         }
         else // unless that piece is a pawn
         {
            mCurrentMove.mCurrentAction = DEPLOY;
         }
      }
      else // live pieces cannot be rotated after a move
      {
         mCurrentMove.mCurrentAction = MOVE;
      }

      // store data about the move
      mCurrentMove.mMovedPiece = mCurrentPiece;
      mCurrentMove.mPrevPosition = mCurrentPiece->getPosition();
      mCurrentPiece->setPosition(screen_tile_coord);
   }
   else // invalid move
   {
       mCurrentPiece = nullptr;
   }
}

void Game_c::rotate(const Piece_n::Direction_e& rotate_direction)
{
   if (mCurrentPiece->getPlayState() == Piece_n::LIVE)
   {
      // a piece is fully deployed after both a move and a rotation
      if (mCurrentMove.mCurrentAction == PRE_DEPLOY) 
      { 
         mCurrentMove.mCurrentAction = DEPLOY;
      }
      else // o/w this is just a standard rotation
      {
         mCurrentMove.mCurrentAction = ROTATE;
      }

      // store data about the move
      mCurrentMove.mMovedPiece = mCurrentPiece;
      mCurrentMove.mPrevDirection = mCurrentPiece->getDirection();
      mCurrentPiece->setDirection(rotate_direction);
   }
}

void Game_c::shootout()
{
   std::vector<PiecePtr>::iterator p1_it;
   for (p1_it = mPlayer1.mPieces.begin(); p1_it != mPlayer1.mPieces.end(); ++p1_it)
   {
      if ((*p1_it)->getPlayState() != Piece_n::RESERVE && (*p1_it)->getPieceType() != Piece_n::PAWN)
      {
         detectHit(**p1_it, mPlayer2.mPieces);
      }
   }
   std::vector<PiecePtr>::iterator p2_it;
   for (p2_it = mPlayer2.mPieces.begin(); p2_it != mPlayer2.mPieces.end(); ++p2_it)
   {
      if ((*p2_it)->getPlayState() != Piece_n::RESERVE && (*p2_it)->getPieceType() != Piece_n::PAWN)
      {
         detectHit(**p2_it, mPlayer1.mPieces);
      }
   }

   for (p1_it = mPlayer1.mPieces.begin(); p1_it != mPlayer1.mPieces.end(); ++p1_it)
   {
      if ((*p1_it)->getPlayState() == Piece_n::DEAD)
      {
         p1_it = mPlayer1.mPieces.erase(p1_it);
      }
   }

   for (p2_it = mPlayer2.mPieces.begin(); p2_it != mPlayer2.mPieces.end(); ++p2_it)
   {
      if ((*p2_it)->getPlayState() == Piece_n::DEAD)
      {
         p2_it = mPlayer2.mPieces.erase(p2_it);
      }
   }
}

void Game_c::nextPlayer()
{
   // null stored pointers
   mCurrentMove.mMovedPiece = nullptr;
   mCurrentPiece = nullptr;

   // reset current action
   mCurrentMove.mCurrentAction = NONE;

   // increment the turn
   mCurrentTurn++;
}

void Game_c::revertMove()
{
   switch (mCurrentMove.mCurrentAction)
   {
      case (PRE_DEPLOY):
      {
         mCurrentMove.mMovedPiece->setPosition(mCurrentMove.mPrevPosition);
         mCurrentMove.mMovedPiece = nullptr;
         break;
      }
      case (DEPLOY) :
      {
         mCurrentMove.mMovedPiece->setPosition(mCurrentMove.mPrevPosition);
         mCurrentMove.mMovedPiece->setDirection(mCurrentMove.mPrevDirection);
         mCurrentMove.mMovedPiece = nullptr;
         break;
      }
      case (MOVE) :
      {
         mCurrentMove.mMovedPiece->setPosition(mCurrentMove.mPrevPosition);
         mCurrentMove.mMovedPiece = nullptr;
         break;
      }
      case (ROTATE) :
      {
         mCurrentMove.mMovedPiece->setDirection(mCurrentMove.mPrevDirection);
         mCurrentMove.mMovedPiece = nullptr;
         break;
      }
      default :
      {
         break;
      }
   }

   if (mCurrentPiece != nullptr)
   {
      mCurrentPiece = nullptr;
   }

   mCurrentMove.mCurrentAction = NONE;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - Getters and Setters
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

PiecePtr Game_c::getCurrentPiece()
{
   return mCurrentPiece;
}

void Game_c::setCurrentPiece(PiecePtr piece_ptr)
{
   mCurrentPiece = piece_ptr;
}

std::pair<int, int>& Game_c::getBriefcasePosition()
{
   return mBriefcasePosition;
}

const std::vector<PiecePtr>& Game_c::getPlayer1Pieces()
{
   return mPlayer1.mPieces;
}

const std::vector<PiecePtr>& Game_c::getPlayer2Pieces()
{
   return mPlayer2.mPieces;
}

Move_s& Game_c::getCurrentMove()
{
   return mCurrentMove;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - initPieces
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Game_c::initPieces()
{
   // initialize player 1's starting reserve position (screen tile coordinates)
   std::pair<int, int> reserve_position = P1_RESERVE_COORD;

   // create the set of valid deployment positions for player 1
   std::vector<std::pair<int, int>> p1_pawn_deployment_zones = P1_GUNSLINGER_DEPLOYMENT_ZONES;
   std::vector<std::pair<int, int>>::iterator p1_it;
   p1_it = p1_pawn_deployment_zones.begin();
   p1_it = p1_pawn_deployment_zones.insert(
      p1_it,
      {
         std::make_pair(BOARD_COORD.first + 1, BOARD_COORD.second + 4),
         std::make_pair(BOARD_COORD.first + 2, BOARD_COORD.second + 3),
      std::make_pair(BOARD_COORD.first + 3, BOARD_COORD.second + 2),
      std::make_pair(BOARD_COORD.first + 4, BOARD_COORD.second + 1)
      }
   );

   // add pawns
   for (int i = 0; i < NUM_PAWNS; ++i)
   {
      mPlayer1.mPieces.push_back(
         std::make_shared<Piece_n::Piece_c>(
            Piece_n::PAWN,
            P1_GUNSLINGER_DEPLOYMENT_ZONES,
            reserve_position,
            Piece_n::PLAYER_ONE));
      reserve_position.second--;
   }

   // place guns on a new row
   reserve_position = P1_RESERVE_COORD;
   reserve_position.first++;

   // add guns
   for (int i = 0; i < NUM_GUNS; ++i)
   {
      mPlayer1.mPieces.push_back(
         std::make_shared<Piece_n::Piece_c>(
            Piece_n::GUN,
            P1_GUNSLINGER_DEPLOYMENT_ZONES,
            reserve_position,
            Piece_n::PLAYER_ONE));
      reserve_position.second--;
   }

   // place slingers on a new row
   reserve_position = P1_RESERVE_COORD;
   reserve_position.first = reserve_position.first + 2;

   // add slingers
   for (int i = 0; i < NUM_SLINGERS; ++i)
   {
      mPlayer1.mPieces.push_back(
         std::make_shared<Piece_n::Piece_c>(
            Piece_n::SLINGER,
            P1_GUNSLINGER_DEPLOYMENT_ZONES,
            reserve_position,
            Piece_n::PLAYER_ONE));
      reserve_position.second--;
   }

   // initialize player 2's starting reserve position (screen tile coordinates)
   reserve_position = P2_RESERVE_COORD;

   // create the set of valid deployment positions for player 2
   std::vector<std::pair<int, int>> p2_pawn_deployment_zones = P2_GUNSLINGER_DEPLOYMENT_ZONES;
   std::vector<std::pair<int, int>>::iterator p2_it;
   p2_it = p1_pawn_deployment_zones.begin();
   p2_it = p1_pawn_deployment_zones.insert(
      p2_it,
      {
         std::make_pair(BOARD_COORD.first + 4, BOARD_COORD.second + 7),
         std::make_pair(BOARD_COORD.first + 5, BOARD_COORD.second + 5),
      std::make_pair(BOARD_COORD.first + 6, BOARD_COORD.second + 6),
      std::make_pair(BOARD_COORD.first + 7, BOARD_COORD.second + 4)
      }
   );

   // add pawns
   for (int i = 0; i < NUM_PAWNS; ++i)
   {
      mPlayer2.mPieces.push_back(
         std::make_shared<Piece_n::Piece_c>(
            Piece_n::PAWN,
            P2_GUNSLINGER_DEPLOYMENT_ZONES,
            reserve_position,
            Piece_n::PLAYER_TWO));
      reserve_position.second++;
   }

   // place guns on a new row
   reserve_position = P2_RESERVE_COORD;
   reserve_position.first--;

   // add guns
   for (int i = 0; i < NUM_GUNS; ++i)
   {
      mPlayer2.mPieces.push_back(
         std::make_shared<Piece_n::Piece_c>(
            Piece_n::GUN,
            P2_GUNSLINGER_DEPLOYMENT_ZONES,
            reserve_position,
            Piece_n::PLAYER_TWO));
      reserve_position.second++;
   }

   // place slingers on a new row
   reserve_position = P2_RESERVE_COORD;
   reserve_position.first = reserve_position.first - 2;

   // add slingers
   for (int i = 0; i < NUM_SLINGERS; ++i)
   {
      mPlayer2.mPieces.push_back(
         std::make_shared<Piece_n::Piece_c>(
            Piece_n::SLINGER,
            P2_GUNSLINGER_DEPLOYMENT_ZONES,
            reserve_position,
            Piece_n::PLAYER_TWO));
      reserve_position.second++;
   }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - detectHit
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Game_c::detectHit(Piece_n::Piece_c& piece, std::vector<PiecePtr>& pieces)
{
   std::cout << "Hit Detection" << std::endl;

   // true if the supplied piece is a slinger; triggers additional hit detections
   bool slinger_flag = false;
   if (piece.getPieceType() == Piece_n::SLINGER) { slinger_flag = true; }

   // true once if we have already hit a piece (monodirectional)
   bool hit_piece_flag = false;
   bool slinger_hit_piece_flag = false;

   // stores the hit piece closest to the shooter
   PiecePtr hit_piece;
   PiecePtr slinger_hit_piece;

   switch (piece.getDirection())
      {
      case Piece_n::UP :
      {
         std::vector<PiecePtr>::iterator it;
         for (it = pieces.begin(); it != pieces.end(); ++it)
         {
            if ((*it)->getPlayState() != Piece_n::RESERVE)
            {
               if ((*it)->getPosition().first == piece.getPosition().first &&
                  (*it)->getPosition().second < piece.getPosition().second)
               {
                  if (hit_piece_flag &&
                     (*it)->getPosition().second > hit_piece->getPosition().second)
                  {
                     hit_piece = *it;
                  }
                  else
                  {
                     hit_piece = *it;
                     hit_piece_flag = true;
                  }
               }
               if (slinger_flag)
               {
                  if ((*it)->getPosition().second == piece.getPosition().second &&
                     (*it)->getPosition().first > piece.getPosition().first)
                  {
                     if (slinger_hit_piece_flag &&
                        (*it)->getPosition().first < slinger_hit_piece->getPosition().first)
                     {
                        slinger_hit_piece = *it;
                     }
                     else
                     {
                        slinger_hit_piece = *it;
                        slinger_hit_piece_flag = true;
                     }
                  }
               }
            }
         }

         hit_piece->nextPlayState();

         if (slinger_flag)
         {
            slinger_hit_piece->nextPlayState();
         }
         break;
      }
      case Piece_n::DOWN :
      {
         std::vector<PiecePtr>::iterator it;
         for (it = pieces.begin(); it != pieces.end(); ++it)
         {
            if ((*it)->getPlayState() != Piece_n::RESERVE)
            {
               if ((*it)->getPosition().first == piece.getPosition().first &&
                  (*it)->getPosition().second > piece.getPosition().second)
               {
                  if (hit_piece_flag &&
                     (*it)->getPosition().second < hit_piece->getPosition().second)
                  {
                     hit_piece = *it;
                  }
                  else
                  {
                     hit_piece = *it;
                     hit_piece_flag = true;
                  }
               }
               if (slinger_flag)
               {
                  if ((*it)->getPosition().second == piece.getPosition().second &&
                     (*it)->getPosition().first < piece.getPosition().first)
                  {
                     if (slinger_hit_piece_flag &&
                        (*it)->getPosition().first > slinger_hit_piece->getPosition().first)
                     {
                        slinger_hit_piece = *it;
                     }
                     else
                     {
                        slinger_hit_piece = *it;
                        slinger_hit_piece_flag = true;
                     }
                  }
               }
            }
         }

         hit_piece->nextPlayState();

         if (slinger_flag)
         {
            slinger_hit_piece->nextPlayState();
         }
         break;
      }
      case Piece_n::LEFT :
      {
         std::vector<PiecePtr>::iterator it;
         for (it = pieces.begin(); it != pieces.end(); ++it)
         {
            if ((*it)->getPlayState() != Piece_n::RESERVE)
            {
               if ((*it)->getPosition().second == piece.getPosition().second &&
                  (*it)->getPosition().first < piece.getPosition().first)
               {
                  if (hit_piece_flag &&
                     (*it)->getPosition().first > hit_piece->getPosition().first)
                  {
                     hit_piece = *it;
                  }
                  else
                  {
                     hit_piece = *it;
                     hit_piece_flag = true;
                  }
               }
               if (slinger_flag)
               {
                  if ((*it)->getPosition().first == piece.getPosition().first &&
                     (*it)->getPosition().second < piece.getPosition().second)
                  {
                     if (slinger_hit_piece_flag &&
                        (*it)->getPosition().second > hit_piece->getPosition().second)
                     {
                        slinger_hit_piece = *it;
                     }
                     else
                     {
                        slinger_hit_piece = *it;
                        slinger_hit_piece_flag = true;
                     }
                  }
               }
            }
         }

         hit_piece->nextPlayState();

         if (slinger_flag)
         {
            slinger_hit_piece->nextPlayState();
         }
         break;
      }
      case Piece_n::RIGHT :
      {
         std::vector<PiecePtr>::iterator it;
         for (it = pieces.begin(); it != pieces.end(); ++it)
         {
            if ((*it)->getPlayState() != Piece_n::RESERVE)
            {
               if ((*it)->getPosition().second == piece.getPosition().second &&
                  (*it)->getPosition().first > piece.getPosition().first)
               {
                  if (hit_piece_flag &&
                     (*it)->getPosition().first < hit_piece->getPosition().first)
                  {
                     hit_piece = *it;
                  }
                  else
                  {
                     hit_piece = *it;
                     hit_piece_flag = true;
                  }
               }
               if (slinger_flag)
               {
                  if ((*it)->getPosition().first == piece.getPosition().first &&
                     (*it)->getPosition().second > piece.getPosition().second)
                  {
                     if (slinger_hit_piece_flag &&
                        (*it)->getPosition().second < hit_piece->getPosition().second)
                     {
                        slinger_hit_piece = *it;
                     }
                     else
                     {
                        slinger_hit_piece = *it;
                        slinger_hit_piece_flag = true;
                     }
                  }
               }
            }
         }

         hit_piece->nextPlayState();

         if (slinger_flag)
         {
            slinger_hit_piece->nextPlayState();
         }
         break;
      }
   }

   //std::vector<PiecePtr>::iterator it;
   //for (it = pieces.begin(); it != pieces.end(); ++it)
   //{
   //   if ((*it)->getPlayState() != Piece_n::RESERVE)
   //   {
   //      switch (piece.getDirection())
   //      {
   //      case Piece_n::UP:
   //      {
   //         if ((*it)->getPosition().first == piece.getPosition().first &&
   //            (*it)->getPosition().second < piece.getPosition().second)
   //         {
   //            if (hit_piece_flag &&
   //               (*it)->getPosition().second > hit_piece->getPosition().second)
   //            {
   //               hit_piece = *it;
   //            }
   //            else
   //            {
   //               hit_piece = *it;
   //               hit_piece_flag = true;
   //            }
   //         }
   //         if (slinger_flag)
   //         {
   //            if ((*it)->getPosition().second == piece.getPosition().second &&
   //               (*it)->getPosition().first > piece.getPosition().first)
   //            {
   //               if (slinger_hit_piece_flag &&
   //                  (*it)->getPosition().first < slinger_hit_piece->getPosition().first)
   //               {
   //                  slinger_hit_piece = *it;
   //               }
   //               else
   //               {
   //                  slinger_hit_piece = *it;
   //                  slinger_hit_piece_flag = true;
   //               }
   //            }
   //         }
   //         break;
   //      }
   //      case Piece_n::DOWN:
   //      {
   //         if ((*it)->getPosition().first == piece.getPosition().first &&
   //            (*it)->getPosition().second > piece.getPosition().second)
   //         {
   //            if (hit_piece_flag &&
   //               (*it)->getPosition().second < hit_piece->getPosition().second)
   //            {
   //               hit_piece = *it;
   //            }
   //            else
   //            {
   //               hit_piece = *it;
   //               hit_piece_flag = true;
   //            }
   //         }
   //         if (slinger_flag)
   //         {
   //            if ((*it)->getPosition().second == piece.getPosition().second &&
   //               (*it)->getPosition().first < piece.getPosition().first)
   //            {
   //               if (slinger_hit_piece_flag &&
   //                  (*it)->getPosition().first > slinger_hit_piece->getPosition().first)
   //               {
   //                  slinger_hit_piece = *it;
   //               }
   //               else
   //               {
   //                  slinger_hit_piece = *it;
   //                  slinger_hit_piece_flag = true;
   //               }
   //            }
   //         }
   //         break;
   //      }
   //      case Piece_n::LEFT:
   //      {
   //         if ((*it)->getPosition().second == piece.getPosition().second &&
   //            (*it)->getPosition().first < piece.getPosition().first)
   //         {
   //            if (hit_piece_flag &&
   //               (*it)->getPosition().first > hit_piece->getPosition().first)
   //            {
   //               hit_piece = *it;
   //            }
   //            else
   //            {
   //               hit_piece = *it;
   //               hit_piece_flag = true;
   //            }
   //         }
   //         if (slinger_flag)
   //         {
   //            if ((*it)->getPosition().first == piece.getPosition().first &&
   //               (*it)->getPosition().second < piece.getPosition().second)
   //            {
   //               if (slinger_hit_piece_flag &&
   //                  (*it)->getPosition().second > hit_piece->getPosition().second)
   //               {
   //                  slinger_hit_piece = *it;
   //               }
   //               else
   //               {
   //                  slinger_hit_piece = *it;
   //                  slinger_hit_piece_flag = true;
   //               }
   //            }
   //         }
   //         break;
   //      }
   //      case Piece_n::RIGHT:
   //      {
   //         if ((*it)->getPosition().second == piece.getPosition().second &&
   //            (*it)->getPosition().first > piece.getPosition().first)
   //         {
   //            if (hit_piece_flag &&
   //               (*it)->getPosition().first < hit_piece->getPosition().first)
   //            {
   //               hit_piece = *it;
   //            }
   //            else
   //            {
   //               hit_piece = *it;
   //               hit_piece_flag = true;
   //            }
   //         }
   //         if (slinger_flag)
   //         {
   //            if ((*it)->getPosition().first == piece.getPosition().first &&
   //               (*it)->getPosition().second > piece.getPosition().second)
   //            {
   //               if (slinger_hit_piece_flag &&
   //                  (*it)->getPosition().second < hit_piece->getPosition().second)
   //               {
   //                  slinger_hit_piece = *it;
   //               }
   //               else
   //               {
   //                  slinger_hit_piece = *it;
   //                  slinger_hit_piece_flag = true;
   //               }
   //            }
   //         }
   //         break;
   //      }
   //      }

   //      // any hit pieces are set to DEAD at the end of a loop
   //      hit_piece->nextPlayState();
   //      if (slinger_flag)
   //      {
   //         slinger_hit_piece->nextPlayState();
   //      }
   //   }
   //}
}

bool Game_c::gameOver()
{
   return false;
}