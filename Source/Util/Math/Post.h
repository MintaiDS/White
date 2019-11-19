#pragma once

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

protected:
    void SetPostType(PostType type) { this->type = type; }

private:
    int idx;
    int point_idx;
    PostType type;
};

class Market : public Post {
public:
    Market() { SetPostType(MARKET); }
    Market(int idx, int point_idx): Post(idx, point_idx) {
        SetPostType(MARKET);
    }
};

class Storage : public Post {
public:
    Storage() { SetPostType(STORAGE); }
    Storage(int idx, int point_idx): Post(idx, point_idx) {
        SetPostType(STORAGE);
    }
};

class City : public Post {
public:
    City() { SetPostType(CITY); }
    City(int idx, int point_idx, std::string& player_idx) : Post(idx, point_idx), player_idx(player_idx) {
        SetPostType(CITY);
    }
    
    std::string& GetPlayerIdx() { return player_idx; }

    void SetPlayerIdx(std::string& player_idx) { this->player_idx = player_idx; }

private:
    std::string player_idx;
};
