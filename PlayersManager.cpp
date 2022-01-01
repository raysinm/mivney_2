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
        int* group_level_0_scores = group.group_players.GetZero();
        ++group_level_0_scores[score];
        ++group.group_size;

        ++players_num;
        return SUCCESS;

    }


}