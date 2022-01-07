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
    const int size;

   public:
    explicit ScoreArray(const int scale) : size(scale + 1) {
        scores = new int[size]{0};           // ! initialized to all zeros? ----added this
        // * added +1 so we can refer to the level by level num and not level-1 ---NICE!
    };

    ~ScoreArray() {
        delete[] scores;
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
};

typedef AVLRank::AVLTree<int, int, ScoreArray> RankTree;

class PlayersManager {
    int players_num;

    class GroupData {
        friend PlayersManager;
        //int group_id,
        int group_size;  //num of players, because *unionfind group size* is not relevant in PlayersManager
        RankTree group_levels;
        
       public:
        GroupData() : group_size(0), group_levels() {}

        void operator+=(GroupData &other) { //operator for mergining groups
            group_size += other.group_size;
            group_levels.AVLMerge(other.group_levels);    
        }
        
        ~GroupData() = default;
    };

    class PlayerData {
        friend PlayersManager;
        int player_id, owner_group_id, score, level;

       public:
        PlayerData(int id, int group_id, int score, int level = 0) : player_id(id), owner_group_id(group_id), score(score), level(level){};
    };

    typedef UF::UnionFind<GroupData> UnionFind;
    typedef DH::DynamicHashtable<PlayerData> HashTable;

    UnionFind groups;
    HashTable all_players_hash;
    RankTree all_players_by_level;

    friend void modifyRankTreesByPlayerScores(RankTree *tree, int level, int score, int scale, const PlayerAction &action);
    friend void modifyRankTreesLevels(RankTree *tree, int level, const PlayerAction &action);
   public:
    int k;
    int scale;

    PlayersManager(int k, int scale) : k(k), scale(scale), groups(k), all_players_hash(), all_players_by_level(){};
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