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
        delete[] scores;
        scores = new_scores;
        return *this;
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

    friend std::ostream& operator<<(std::ostream& os, const ScoreArray& array){
        for(int i = 1; i< array.size; i++){
            os << array.scores[i] << ", "; 
        }
        return os;
    }

    int MembersAmount(){
        int total = 0;
        for(int i = 1; i < size; i++){
            total += scores[i];
        }
        return total;
    }
    void Print(){
        std::cout << std::endl;
        for(int i = 1; i< size; i++){
            std::cout << scores[i] << ", "; 
        }
        std::cout << std::endl;
    }


};
/* 
class EverythingRank{
    const int scale;
    ScoreArray score_array;
    int players_num;
    int players_num_times_level;
    friend PlayersManager;

    public:
    EverythingRank(const int& scale): scale(scale), score_array(scale), players_num(0), players_num_times_level(0){};
    EverythingRank(const int& scale, const int& player_num_times_level, const int& players_num=1):
                    scale(scale), score_array(scale), players_num(players_num), players_num_times_level(players_num_times_level){};
    
    EverythingRank& operator+=(const EverythingRank& other){
        score_array += other.score_array;
        players_num += other.players_num;
        players_num_times_level += other.players_num_times_level;
    }
    EverythingRank& operator-=(const EverythingRank& other){
        score_array -= other.score_array;
        players_num -= other.players_num;
        players_num_times_level -= other.players_num_times_level;
    }
    bool operator<=(const Everything& other){
        return players_num <= other.players_num;
    }
    void AddToScoreArray(const int& index, const int& change){
        score_array[index] += change;
    }

}; */

typedef AVLRank::AVLTree<int, int, int> RankTreeInt;
typedef AVLRank::AVLTree<int, int, ScoreArray> RankTreeScoreArray;   //!so that all classes know the scale without having to look for it in PlayersManager. relevant for c'tor of GroupData


class GroupData {
    friend class PlayersManager;
    //int group_id,
    int group_size;  //num of players, because *unionfind group size* is not relevant in PlayersManager
    RankTreeScoreArray group_levels_scores;
    RankTreeInt group_levels_sums;   //with num_of_players
    RankTreeInt group_levels_multi;    //with num_of_players_times_level
    
    ScoreArray group_level_0;
    
    public:
    GroupData(int scale) : group_size(0), group_levels_scores(), group_levels_sums(), group_levels_multi(), group_level_0(scale){}
    GroupData(const GroupData& other): group_level_0(other.group_level_0){
        if (other.group_size !=0){
            return;
        }
    }

    GroupData& operator= (const GroupData& other){
        if(this->group_size != 0 || other.group_size != 0){
            return *this;
        }
        //delete group_level_0;
        group_level_0 = other.group_level_0;
        return *this;
    }
    GroupData& operator+=(GroupData &other) { //operator for mergining groups
        group_size += other.group_size;
        group_levels_scores.AVLMerge(other.group_levels_scores);  
        group_levels_sums.AVLMerge(other.group_levels_sums);
        group_levels_multi.AVLMerge(other.group_levels_multi);
        group_level_0 += other.group_level_0;
        return *this;
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
    RankTreeScoreArray all_players_by_level;
    RankTreeInt all_players_by_level_sums;
    RankTreeInt all_players_by_level_multi;
    
    ScoreArray level_0;

    //friend void modifyRankTrees(RankTreeInt *tree, int level, int score, const PlayerAction &action);
    friend void modifyRankTreesByPlayerScores(RankTreeScoreArray *tree, int level, int score, int scale, const PlayerAction &action);
    friend void modifyRankTreesSums(RankTreeInt * tree, int level, int score, int scale, const PlayerAction &action);
    friend void modifyRankTreesMulti(RankTreeInt * tree, int level, int score, int scale, const PlayerAction &action);
    //void ModifyTreesIfLevelEmpty(int level, GroupData* group);
   public:
    const int k;
    const int scale;

    PlayersManager(const int& k, const int& scale) : k(k), scale(scale), groups(k, scale), all_players_hash(),
                                                    all_players_by_level(), all_players_by_level_sums(), all_players_by_level_multi(), level_0(scale){}
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

void modifyRankTreesByPlayerScores(RankTreeScoreArray *tree, int level, int score, int scale, const PlayerAction &action);
void modifyRankTreesSums(RankTreeInt * tree, int level, int score, int scale, const PlayerAction &action);
void modifyRankTreesMulti(RankTreeInt * tree, int level, int score, int scale, const PlayerAction &action);

}  // namespace PM

#endif



