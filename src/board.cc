#include "board.hpp"

// you might need additional includes
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

/**************************************************************************/
/* your function definitions                                              */
/**************************************************************************/

void InitBoard(Board& b) {
  // below was minimal to get tests to actually compile and given test case to
  // fail
  for(int i = 0; i < Board::kBoardHeight; i++) {
    for (int w = 0; w < Board::kBoardWidth; w++) {
      b.board[i][w] = DiskType::kEmpty;
    }
  }
}
bool BoardLocationInBounds(int row, int col) {
  return row <= Board::kBoardHeight - 1 && row >= 0 && col <= Board::kBoardWidth - 1 && col >= 0;
}
void DropDiskToBoard(Board& b, DiskType disk, int col) {
  bool failed = true;
  if (!BoardLocationInBounds(1, col)) {
    throw std::runtime_error("");
  }
  for (int i = 0; i < Board::kBoardHeight; i++) {
    if (b.board[i][col] == DiskType::kEmpty) {
      b.board[i][col] = disk;
      failed = false;
      break;
    }
  }
  if (failed) {
    throw std::runtime_error("");
  }
}
bool CheckHorizontal (Board& b, DiskType disk) {
  for (int i = 0; i < Board::kBoardHeight; i++) {
    int check = 0;
    for (int w = 0; w < Board::kBoardWidth; w++) {
      if (check == 4) {
        return true;
      }
      if (b.board[i][w] == disk) {
        check++;
      } else {
        check = 0;
      }
    }
    if (check == 4) {
      return true;
    }
  }
  return false;
}
bool CheckVert (Board& b, DiskType disk) {
  for (int i = 0; i < Board::kBoardWidth; i++) {
    int check = 0;
    for (int w = 0; w < Board::kBoardHeight; w++) {
      if (check == 4) {
        return true;
      }
      if (b.board[w][i] == disk) {
        check++;
      } else {
        check = 0;
      }
    }
    if (check == 4) {
      return true;
    }
  }
  return false;
}
/* idea for checking diagonals. Use a while loop that stops when the next one goes out of bounds and then check across the diagonal*/
bool CheckRightDiag (Board& b, DiskType disk) {
  //checking the special diagonal that doesn't work if you just run on the bottoms. 
  if ((b.board[2][1] == disk && b.board[3][2] == disk && b.board[4][3] == disk) && (b.board[1][0] == disk || b.board[Board::kBoardHeight - 1][4] == disk)) { //should be ands + leaves out [5][4]
    return true;
  } 
  if (b.board[2][0] == disk && b.board[3][1] == disk && b.board[4][2] == disk && b.board[Board::kBoardHeight - 1][3] == disk) {
    return true;
  }
  //checking the rest of them
  int starting_col = 0;
  while (starting_col < 4) {
    int row = 0;
    int col = starting_col;
    int count = 0;
    while (BoardLocationInBounds(row, col)) {
      if (count == 4) {
        return true;
      } 
      if (b.board[row][col] == disk) {
        count++;
      } else {
        count = 0;
      }
      row++;
      col++;
    }
    if (count == 4) {
      return true;
    }
    starting_col++;
  }
  return false;
}
bool CheckLeftDiag (Board& b, DiskType disk) {
  //checking the special diagonal that doesn't work if you just run on the bottoms. 
  if ((b.board[2][Board::kBoardWidth - 2] == disk && b.board[3][4] == disk && b.board[4][3] == disk) && (b.board[1][Board::kBoardWidth - 1] == disk || b.board[Board::kBoardHeight - 1][2] == disk)) {
    return true;
  } 
  if (b.board[2][Board::kBoardWidth - 1] == disk && b.board[3][Board::kBoardWidth - 2] == disk && b.board[4][4] == disk && b.board[Board::kBoardHeight - 1][3] == disk) {
    return true;
  }
  //checking the rest of them
  int starting_col = Board::kBoardWidth - 1;
  while (starting_col != 1) {
    int row = 0;
    int col = starting_col;
    int count = 0;
    while (BoardLocationInBounds(row, col)) {
      if (count == 4) {
        return true;
      } 
      if (b.board[row][col] == disk) {
        count++;
      } else {
        count = 0;
      }
      row++;
      col--;
    }
    if (count == 4) {
      return true;
    }
    starting_col = starting_col - 1;
  }
  return false;
}
bool SearchForWinner(Board& b, DiskType disk, WinningDirection to_check) {
  switch (to_check) {
    case WinningDirection::kHorizontal :
      return CheckHorizontal(b, disk);
      break;
    case WinningDirection::kVertical :
      return CheckVert(b,disk);
      break;
    case WinningDirection::kRightDiag:
      return CheckRightDiag(b, disk);
      break;
    case WinningDirection::kLeftDiag:
      return CheckLeftDiag(b, disk);
      break;
    default:
      break;
  }
  return false;
}
bool CheckForWinner(Board& b, DiskType disk) {
  return SearchForWinner(b, disk, WinningDirection::kHorizontal) || SearchForWinner(b, disk, WinningDirection::kVertical) || SearchForWinner(b, disk, WinningDirection::kLeftDiag) || SearchForWinner(b, disk, WinningDirection::kRightDiag); 
}
/**************************************************************************/
/* provided to you                                                        */
/**************************************************************************/
std::string BoardToStr(const Board& b) {
  constexpr int kTotalWidth = Board::kBoardWidth * 11 - Board::kBoardHeight - 1;
  std::stringstream ss;
  ss << std::endl;
  for (int row = Board::kBoardHeight; row > 0; --row) {
    ss << " |";
    for (int col = 0; col < Board::kBoardWidth; ++col) {
      std::string value_here;
      value_here.push_back(static_cast<char>(b.board[row - 1][col]));
      ss << ' ' << CenterStr(value_here, Board::kBoardWidth + 1) << '|';
    }
    ss << std::endl;
    ss << " |" << std::setw(kTotalWidth) << std::setfill('-') << '|'
       << std::endl;
  }
  ss << " |";
  for (int col = 0; col < Board::kBoardWidth; ++col) {
    ss << ' ' << CenterStr(std::to_string(col), Board::kBoardWidth + 1) << '|';
  }
  return ss.str();
}

std::string CenterStr(const std::string& str, int col_width) {
  // quick and easy (but error-prone) implementation
  auto padl = (col_width - str.length()) / 2;
  auto padr = (col_width - str.length()) - padl;
  std::string strf = std::string(padl, ' ') + str + std::string(padr, ' ');
  return strf;
}