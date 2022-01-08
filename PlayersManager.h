#include "./AVLRanktree.h"
#include "./DynamicHashtable.h"
#include "./UnionFind.h"
#include "./course_files/library2.h"

#ifndef _PLAYERS_MANAGER_H
#define _PLAYERS_MANAGER_H

namespace PM {

class Failure : std::exception {};

typedef enum {
    PLAYER_ADD,
    PLAYER_REMOVE
} PlayerAction;

class ScoreArray {
    int *scores;
    int size;

   public:
    explicit ScoreArray(const int& scale) : size(scale + 1) {
        scores = new int[size]{0};           // ! initialized to all zeros? ----added this
        // * added +1 so we can refer to the level by level num and not level-1 ---NICE!
    };

    ScoreArray(const ScoreArray& other): size(other.size){
        scores = new int[other.size];
        for(int i = 0; i < other.size; i++){
            scores[i] = other.scores[i];
        }
    }

    ~ScoreArray() {
        delete[] scores;
    }



    ScoreArray& operator=(const ScoreArray& other){
        if(this == &other){
            return *this;
        }
        this->size = other.size;
        int* new_scores = new int[other.size];
        for(int i = 0; i < size; i++){
            new_scores[i] = other.scores[i];
        }
        delete scores;
        scores = new_scores;
    }


    void operator+=(const ScoreArray &other) {
        for (int i = 1; i < size; i++) {
            scores[i] += other.scores[i];
        }
    }

    void operator-=(const ScoreArray &other) {
        for (int i = 1; i < size; i++) {
            scores[i] -= other.scores[i];
        }
    }

    int &operator[](int index) {
        return scores[index];
    }

    bool operator!() {
        for (int i = 1; i < size; i++) {
            if (scores[i] != 0) {
                return false;
            }
        }
        return true;
    }

    int& MembersAmount(){
        int total;
        for(int i = 0; i < size; i++){
            total += scores[i];
        }
        return total;
    }
};

typedef AVLRank::AVLTree<int, int, ScoreArray> RankTree;
int global_scale;   //!so that all classes know the scale without having to look for it in PlayersManager. relevant for c'tor of GroupData


class GroupData {
    friend PlayersManager;
    //int group_id,
    int group_size;  //num of players, because *unionfind group size* is not relevant in PlayersManager
    RankTree group_levels;
    ScoreArray group_level_0;
    
    public:
    GroupData() : group_size(0), group_levels(), group_level_0(global_scale){}

    void operator+=(GroupData &other) { //operator for mergining groups
        group_size += other.group_size;
        group_levels.AVLMerge(other.group_levels);    
    }
    
    ~GroupData() = default;
};


class PlayersManager {
    int players_num;


    class PlayerData {
        friend PlayersManager;
        int player_id, owner_group_id, score, level;

       public:
        PlayerData(int id, int group_id, int score, int level = 0) : player_id(id), owner_group_id(group_id), score(score), level(level){};
    };

    friend GroupData;

    typedef UF::UnionFind<GroupData> UnionFind;
    typedef DH::DynamicHashtable<PlayerData> HashTable;

    UnionFind groups;
    HashTable all_players_hash;
    RankTree all_players_by_level;
    ScoreArray level_0;

    friend void modifyRankTreesByPlayerScores(RankTree *tree, int level, int score, int scale, const PlayerAction &action);
    friend void modifyRankTreesLevels(RankTree *tree, int level, const PlayerAction &action);
   public:
    const int k;
    const int scale;

    PlayersManager(const int& k, const int& scale) : k(k), scale(scale), groups(k), all_players_hash(), all_players_by_level(), level_0(scale){
        global_scale = scale;
    };
    PlayersManager(const PlayersManager &) = delete;
    ~PlayersManager() = default;

    StatusType MergeGroups(int GroupID1, int GroupID2);
    StatusType AddPlayer(int PlayerID, int GroupID, int score);
    StatusType RemovePlayer(int PlayerID);
    StatusType IncreasePlayerIDLevel(int PlayerID, int LevelIncrease);
    StatusType ChangePlayerIDScore(int PlayerID, int NewScore);
    StatusType GetPercentOfPlayersWithScoreInBounds(int GroupID, int score, int lowerLevel, int higherLevel,
                                                    double *players);
    StatusType AverageHighestPlayerLevelByGroup(int GroupID, int m, double *level);
    StatusType GetPlayersBound(int GroupID, int score, int m,
                               int *LowerBoundPlayers, int *HigherBoundPlayers);
};

}  // namespace PM

#endif