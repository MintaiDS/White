#pragma once
#include <string>


class Train {
public:
  Train() {}
  Train(int idx, int line_idx, int position, std::string& player_idx) : idx(idx), line_idx(line_idx), position(position), player_idx(player_idx) {}

  int GetIdx() { return idx; }
  int GetPosition() { return position; }
  int GetLineIdx() { return line_idx; }
  std::string& GetPlayerIdx() { return player_idx; }

  void SetIdx(int idx) { this->idx = idx; }
  void SetPosition(int position) { this->position = position; }
  void SetLineIdx(int line_idx) { this->line_idx = line_idx; }
  void SetPlayerIdx(std::string& player_idx) { this->player_idx = player_idx; }

private:
  int idx;
  int line_idx;
  int position;
  std::string player_idx;
};