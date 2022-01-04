#include "./PlayersManager.h"

namespace PM {

StatusType PlayersManager::MergeGroups(int GroupID1, int GroupID2) {
    groups.Union(GroupID1, GroupID2);  //INSIDE UF: merge the ranktrees of the groups somehow (or should it be outside? maybe it should.)
    return SUCCESS;
}

StatusType PlayersManager::AddPlayer(int PlayerID, int GroupID, int score) {
    if (all_players_hash.Exists(PlayerID)) {
        throw Failure();
    }

    GroupData &group = groups.Find(GroupID);

    PlayerData *new_player = new PlayerData(PlayerID, GroupID, score);
    all_players_hash.Add(PlayerID, *new_player);

    modifyRankTreesByPlayerScores(&group.group_levels, 0, score, scale, PLAYER_ADD);
    modifyRankTreesByPlayerScores(&all_players_by_level, 0, score, scale, PLAYER_ADD);


    ++group.group_size;

    ++players_num;
    return SUCCESS;
}

StatusType PlayersManager::RemovePlayer(int PlayerID) {
    if (!all_players_hash.Exists(PlayerID)) {
        throw Failure();
    }

    PlayerData &player_data = all_players_hash.Get(PlayerID);
    GroupData &group = groups.Find(player_data.owner_group_id);

    //! better to change rank INSIDE AVL
    //! AVL has to check if level == 0 (or key == min_key)

    modifyRankTreesByPlayerScores(&group.group_levels, player_data.level, player_data.score, scale, PLAYER_REMOVE);
    modifyRankTreesByPlayerScores(&all_players_by_level, player_data.level, player_data.score, scale, PLAYER_REMOVE);

    /*         ScoreArray single_1_in_score_index(scale);  //on stack
        single_1_in_score_index[player_data.score] = -1; //now its an array with zeros except for in index score 
        
        group.group_levels.updateRank(player_data.level, &single_1_in_score_index);

        all_players_by_level.updateRank(player_data.level, &single_1_in_score_index);

        int& group_num_of_players_in_level = group.group_levels.AVLGet(player_data.level);
        group_num_of_players_in_level--;

        int& all_num_of_players_in_level = all_players_by_level.AVLGet(player_data.level);
        all_num_of_players_in_level--; */

    --group.group_size;

    all_players_hash.Remove(PlayerID);

    --players_num;

    return SUCCESS;
}

StatusType PlayersManager::IncreasePlayerIDLevel(int PlayerID, int LevelIncrease) {
    if (!all_players_hash.Exists(PlayerID)) {
        throw Failure();
    }

    PlayerData &player_data = all_players_hash.Get(PlayerID);
    GroupData &group = groups.Find(player_data.owner_group_id);

    int new_level = player_data.level + LevelIncrease;

    modifyRankTreesByPlayerScores(&group.group_levels, player_data.level, player_data.score, scale, PLAYER_REMOVE);
    modifyRankTreesByPlayerScores(&group.group_levels, new_level, player_data.score, scale, PLAYER_ADD);

    modifyRankTreesByPlayerScores(&all_players_by_level, player_data.level, player_data.score, scale, PLAYER_REMOVE);
    modifyRankTreesByPlayerScores(&all_players_by_level, new_level, player_data.score, scale, PLAYER_ADD);
    /*         ScoreArray player_score_as_array(scale);


        player_score_as_array[player_data.score] = -1;
        group.group_levels.updateRank(player_data.level, &player_score_as_array);
        all_players_by_level.updateRank(player_data.level, &player_score_as_array);

        player_score_as_array[player_data.score] = 1;
        group.group_levels.updateRank(new_level, &player_score_as_array);
        all_players_by_level.updateRank(new_level, &player_score_as_array);

        
        */

    player_data.level = new_level;

    return SUCCESS;
}

void modifyRankTreesByPlayerScores(RankTree* tree, int level, int score, int scale, const PlayerAction &action) {
    ScoreArray player_score_as_array(scale);
    int change_to_level;
    if (action == PLAYER_ADD) {
        //*for updating rank:
        player_score_as_array[score] = 1;
        //*for updating data:
        change_to_level = 1;
    } else if (action == PLAYER_REMOVE) {
        player_score_as_array[score] = -1;
        change_to_level = -1;
    }

    tree->updateRank(level, &player_score_as_array);
    int &players_in_level = tree->AVLGet(level);
    players_in_level += change_to_level;
}

}  // namespace PM