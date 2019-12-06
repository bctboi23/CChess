// io.c

#include "stdio.h"
#include "defs.h"

char *PrSq(const int sq) {

  static char SqStr[3];

  int file = FilesBrd[sq];
  int rank = RanksBrd[sq];

  sprintf(SqStr, "%c%c", ('a' + file), ('1' + rank));


  return SqStr;
}

char *PrMove (const int move) {

  static char MvStr[6];

  int ff = FilesBrd[FROMSQ(move)];
  int rf = RanksBrd[FROMSQ(move)];
  int ft = FilesBrd[TOSQ(move)];
  int rt = RanksBrd[TOSQ(move)];

  int promoted = PROMOTED(move);

  if (promoted) {
    char pChar = 'q';
    if (IsKn(promoted)) {
      pChar = 'n';
    }
    else if (IsRQ(promoted) && !IsBQ(promoted)) {
      pChar = 'r';
    }
    else if (!IsRQ(promoted) && IsBQ(promoted)) {
      pChar = 'b';
    }
    sprintf(MvStr, "%c%c%c%c%c", ('a' + ff), ('1' + rf), ('a' + ft), ('1' + rt), pChar);
  }
  else {
    printf(MvStr, "%c%c%c%c", ('a' + ff), ('1' + rf), ('a' + ft), ('1' + rt));
  }

  return MvStr;
}

int ParseMove(char *ptrChar, S_BOARD *pos) {

  if (ptrChar[1] > '8' || ptrChar[1] < 1) return NOMOVE;
  if (ptrChar[3] > '8' || ptrChar[3] < 1) return NOMOVE;
  if (ptrChar[0] > 'h' || ptrChar[0] < 'a') return NOMOVE;
  if (ptrChar[2] > 'h' || ptrChar[2] < 'a') return NOMOVE;

  int from = FR2SQ(ptrChar[0] - 'a', ptrChar[1] - '1');
  int to = FR2SQ(ptrChar[2] - 'a', ptrChar[3] - '1');

  printf("ptrChar: %s from: %d to: %d\n", ptrChar, from, to);

  ASSERT(SqOnBoard(from) && SqOnBoard(to));

  S_MOVELIST list[1];
  GenerateAllMoves(pos, list);
  int MoveNum = 0;
  int Move = 0;
  int PromPce = EMPTY;

  for (MoveNum = 0; MoveNum < list->count; ++MoveNum) {
    Move = list->moves[MoveNum].move;
    if (FROMSQ(Move) == from && TOSQ(Move) == to) {
      PromPce = PROMOTED(Move);
      if (PromPce != EMPTY) {
        if (IsRQ(PromPce) && !IsBQ(PromPce) && ptrChar[4] == 'r') {
          return Move;
        }
        else if (!IsRQ(PromPce) && IsBQ(PromPce) && ptrChar[4] == 'b') {
          return Move;
        }
        else if (IsRQ(PromPce) && IsBQ(PromPce) && ptrChar[4] == 'q') {
          return Move;
        }
        else if (IsKn(PromPce) && ptrChar[4] == 'n') {
          return Move;
        }
        continue;
      }
      return Move;
    }
  }
  return NOMOVE;
}

void PrintMoveList (const S_MOVELIST *list) {
  int index =0; int score = 0; int move = 0;
  printf("MoveList:\n",list->count);

  for (index = 0; index < list->count; ++index) {

    move = list->moves[index].move;
    score = list->moves[index].score;

    printf("Move:%d > %s (score:%d)\n", index + 1, PrMove(move), score);
  }
  printf("MoveList Total %d Moves:\n\n", list->count);
}
