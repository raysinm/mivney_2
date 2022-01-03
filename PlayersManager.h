#ifndef _PLAYERS_MANAGER_H
#define _PLAYERS_MANAGER_H

#include "./course_files/library2.h"

#include "./UnionFind.h"
#include "./DynamicHashtable.h"
#include "./AVLRanktree.h"

  
//TODO: need to change int* to ScoreArray - doesnt recognize
typedef AVLRank::AVLTree<int, int, int*> RankTree;  //HAVE TO THINK: do we add rank to AVLTree? (Key, Data, Rank)
                                                        //do we keep array of scores in rank or in data?

namespace PM{

    class Failure : std::exception{};

    class PlayersManager{
        
        int players_num; 

        class ScoreArray{
            friend PlayersManager;
            int* scores;
            int size; // ! turn to const? also in PM

            public:
            
            explicit ScoreArray(int scale):size(scale+1){
                scores = new int[size]; // ! initialized to all zeros? ----added this
                for(int i = 0; i < size; i++){  // * still O(1)
                    scores[i] = 0;
                }  
                // * added +1 so we can refer to the level by level num and not level-1 ---NICE!
            }

            void operator+=(const ScoreArray& other){  
                        for(int i=1; i < size; i++)
                        {
                            scores[i] += other.scores[i];
                        }
                    };
            
            void operator-=(const ScoreArray& other){  
                        for(int i=1; i < size; i++)
                        {
                            scores[i] -= other.scores[i];
                        }
                    };
            
            bool IsEmpty(){
                for(int i = 1; i < size; i++){
                    if(scores[i] != 0){
                        return false;
                    }
                }
                return true;
            }

            void Refresh(const ScoreArray& left, const ScoreArray& right){
                //......
            }
            
            //USE: RankTree<int, int, ScoreArray> (the third parameter is the rank)
        };

        class GroupData{
            friend PlayersManager;
            int group_id,
                group_size; //num of players, because *unionfind group size* is not relevant in PlayersManager
            RankTree group_levels;
        };

        class PlayerData{
            friend PlayersManager;
            int player_id, owner_group_id, score, level;
            public:
            PlayerData(int id, int group_id, int score, int level=0):
                player_id(id), owner_group_id(group_id), score(score), level(level){}
        };

        typedef UF::UnionFind<GroupData> UnionFind; 
        typedef DHash::DynamicHashtable<int, PlayerData> HashTable;

        UnionFind groups;
        HashTable all_players_hash;
        RankTree all_players_by_level;

        public:

        int k;
        int scale;
        
        PlayersManager(int k, int scale): k(k), scale(scale), groups(k), all_players_hash(), all_players_by_level(){};
        PlayersManager(const PlayersManager&) = delete;
        ~PlayersManager() = default;
        
        StatusType MergeGroups(int GroupID1, int GroupID2);
        StatusType AddPlayer(int PlayerID, int GroupID, int score);
        StatusType RemovePlayer(int PlayerID);
        StatusType IncreasePlayerIDLevel(int PlayerID, int LevelIncrease);
        StatusType ChangePlayerIDScore(int PlayerID, int NewScore);
        StatusType GetPercentOfPlayersWithScoreInBounds(int GroupID, int score, int lowerLevel, int higherLevel,
                                                        double * players);
        StatusType AverageHighestPlayerLevelByGroup(int GroupID, int m, double * level);
        StatusType GetPlayersBound(int GroupID, int score, int m,
                                    int * LowerBoundPlayers, int * HigherBoundPlayers);


    };

}

#endif