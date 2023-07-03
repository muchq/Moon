#ifndef CPP_GOLF_SERVICE_GOLF_LIB_H
#define CPP_GOLF_SERVICE_GOLF_LIB_H

#include <deque>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "cpp/cards/card.h"

namespace golf {
using namespace cards;

enum class Position { TopLeft, TopRight, BottomLeft, BottomRight };

class Player {
 public:
  Player(std::string _name, Card tl, Card tr, Card bl, Card br)
      : name(_name), topLeft(tl), topRight(tr), bottomLeft(bl), bottomRight(br) {}
  const int score();
  const std::vector<Card> allCards();

 private:
  const int cardValue(Card c);
  const std::string name;
  const Card topLeft;
  const Card topRight;
  const Card bottomLeft;
  const Card bottomRight;
};

class GameState {
 public:
  GameState(std::deque<Card> _drawPile, std::deque<Card> _discardPile, std::vector<Player> _players,
            int _whoseTurn, int _whoKnocked)
      : drawPile(_drawPile),
        discardPile(_discardPile),
        players(_players),
        whoseTurn(_whoseTurn),
        whoKnocked(_whoKnocked) {}
  const bool isOver();
  const GameState swapForDrawPile(int player, Position Position);
  const GameState swapForDiscardPile(int player, Position Position);
  const GameState knock(int player);
  const std::unordered_set<Player> winners();

 private:
  const std::deque<Card> drawPile;
  const std::deque<Card> discardPile;
  const std::vector<Player> players;
  const int whoseTurn;
  const int whoKnocked;
};

}  // namespace golf

#endif