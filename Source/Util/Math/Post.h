#pragma once

//#include "Train.h"
#include <string>

namespace White {
  namespace Util {
    namespace Math {

      enum PostType {
        CITY = 1,
        MARKET = 2,
        STORAGE = 3
      };

      class Post {
      public:

        Post() {}
        Post(int idx, int point_idx) : idx(idx), point_idx(point_idx) {}

        int GetIdx() { return idx; }
        int GetPointIdx() { return point_idx; }
        int GetPostType() { return type; }

        void SetIdx(int idx) { this->idx = idx; }
        void SetPointIdx(int point_idx) { this->point_idx = point_idx; }

        int GetCurProduct() { return cur_product; }
        int GetCurArmor() { return cur_armor; }
        int GetMaxProduct() { return max_product; }
        int GetMaxArmor() { return max_armor; }
        int GetLevel() { return level; }
        bool Vacant() { return is_vacant; }

        void SetCurProduct(int val) { cur_product = val; }
        void SetMaxProduct(int val) { max_product = val; }
        void SetCurArmor(int val) { cur_armor = val; }
        void SetMaxArmor(int val) { max_armor = val; }
        void SetLevel(int val) { level = val; }
        void SetVacant(bool val) { is_vacant = val; }

      protected:
        void SetPostType(PostType type) { this->type = type; }

      private:
        int idx;
        int point_idx;
        PostType type;
        int cur_product;
        int max_product;
        int cur_armor;
        int max_armor;
        bool is_vacant = true;
        int level;
      };

      class Market : public Post {
      public:
        Market() { SetPostType(MARKET); }
        Market(int idx, int point_idx) : Post(idx, point_idx) {
          SetPostType(MARKET);
        }
      private:
      };

      class Storage : public Post {
      public:
        Storage() { SetPostType(STORAGE); }
        Storage(int idx, int point_idx) : Post(idx, point_idx) {
          SetPostType(STORAGE);
        }
      private:
      };

      class City : public Post {
      public:
        static const int level_cost[2];
        City() { SetPostType(CITY); }
        City(int idx, int point_idx, std::string& player_idx) : Post(idx, point_idx), player_idx(player_idx)
        {
          SetPostType(CITY);
        }

        std::string& GetPlayerIdx() { return player_idx; }
        int GetPopulation() { return population; }

        void SetPlayerIdx(std::string& player_idx) { this->player_idx = player_idx; }
        void SetPopulation(int val) { population = val; }
      private:
        std::string player_idx;
        int population;
      };
    }
  }
}
