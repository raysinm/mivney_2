#include "./PlayersManager.h"
#include "./UnionFind.h"
#include "./DynamicHashtable.h"
#include "./AVLRanktree.h"

namespace PM{

    StatusType PlayersManager::MergeGroups(int GroupID1, int GroupID2){
        groups.Union(GroupID1, GroupID2);   //INSIDE UF: merge the ranktrees of the groups somehow (or should it be outside? maybe it should.)
        return SUCCESS;
    }

    StatusType PlayersManager::AddPlayer(int PlayerID, int GroupID, int score){
        if(all_players_hash.Exists(PlayerID)){
            throw Failure();
        }
        PlayerData* new_player = new PlayerData(PlayerID, GroupID, score);
        all_players_hash.Add(PlayerID, *new_player);

        int* level_0_scores = all_players_by_level.GetZero();
        ++level_0_scores[score];

        GroupData& group = groups.Find(GroupID);
        int* group_level_0_scores = group.group_levels.GetZero();
        ++group_level_0_scores[score];
        ++group.group_size;

        ++players_num;
        return SUCCESS;

    }

    StatusType PlayersManager::RemovePlayer(int PlayerID){
        if(!all_players_hash.Exists(PlayerID)){
            throw Failure();
        }
        
        PlayerData& player_data = all_players_hash.Get(PlayerID);
        GroupData& group = groups.Find(player_data.owner_group_id);
        RankTree& group_levels = group.group_levels;

        if(player_data.level != 0){

            //! better to change rank INSIDE AVL
            ScoreArray& player_score_array_group = group_levels.updateRank(player_data.level, player_data.score, );   //TODO: add GetRank to AVLRank tree
            if (player_score_array_group.IsEmpty()){
                group_levels.AVLRemove(player_data.level);
            }

            ScoreArray& player_score_array_all = all_players_by_level.GetRank(player_data.level);
            if (player_score_array_all.IsEmpty()){
                all_players_by_level.AVLRemove(player_data.level);
            }
        }
        else if(player_data.level == 0){
            //TODO: mess with the level-0, seperate node in group tree and all players tree
        }

        --group.group_size;

        all_players_hash.Remove(PlayerID);
        
        --players_num;

        return SUCCESS;
    }


}