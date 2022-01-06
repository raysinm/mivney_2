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
    all_players_hash.Insert(PlayerID, *new_player);

    modifyRankTreesByPlayerScores(&group.group_levels, 0, score, scale, PLAYER_ADD);
    modifyRankTreesByPlayerScores(&all_players_by_level, 0, score, scale, PLAYER_ADD);

    modifyRankTreesLevels(&group.group_levels, 0, LEVEL_ADD1);
    modifyRankTreesLevels(&all_players_by_level, 0, LEVEL_ADD1);

    ++group.group_size;
    ++players_num;

    return SUCCESS;
}

StatusType PlayersManager::RemovePlayer(int PlayerID) {
    if (!all_players_hash.Exists(PlayerID)) {
        throw Failure();
    }

    PlayerData &player_data = all_players_hash.Find(PlayerID);
    GroupData &group = groups.Find(player_data.owner_group_id);

    //! better to change rank INSIDE AVL
    //! AVL has to check if level == 0 (or key == min_key)

    modifyRankTreesByPlayerScores(&group.group_levels, player_data.level, player_data.score, scale, PLAYER_REMOVE);
    modifyRankTreesByPlayerScores(&all_players_by_level, player_data.level, player_data.score, scale, PLAYER_REMOVE);

    modifyRankTreesLevels(&group.group_levels, player_data.level, LEVEL_SUB1);
    modifyRankTreesLevels(&all_players_by_level, player_data.level, LEVEL_SUB1);

    --group.group_size;

    all_players_hash.Remove(PlayerID);

    --players_num;

    return SUCCESS;
}

StatusType PlayersManager::IncreasePlayerIDLevel(int PlayerID, int LevelIncrease) {
    if (!all_players_hash.Exists(PlayerID)) {
        throw Failure();
    }

    PlayerData &player_data = all_players_hash.Find(PlayerID);
    GroupData &group = groups.Find(player_data.owner_group_id);

    int new_level = player_data.level + LevelIncrease;

    //*group players ScoreArray update
    modifyRankTreesByPlayerScores(&group.group_levels, player_data.level, player_data.score, scale, PLAYER_REMOVE);
    modifyRankTreesByPlayerScores(&group.group_levels, new_level, player_data.score, scale, PLAYER_ADD);

    //*all players ScoreArray update
    modifyRankTreesByPlayerScores(&all_players_by_level, player_data.level, player_data.score, scale, PLAYER_REMOVE);
    modifyRankTreesByPlayerScores(&all_players_by_level, new_level, player_data.score, scale, PLAYER_ADD);

    //*group players level num of players update
    modifyRankTreesLevels(&group.group_levels, player_data.level, LEVEL_SUB1);
    modifyRankTreesLevels(&group.group_levels, new_level, LEVEL_ADD1);

    //*all players level num of players update
    modifyRankTreesLevels(&all_players_by_level, player_data.level, LEVEL_SUB1);
    modifyRankTreesLevels(&all_players_by_level, new_level, LEVEL_ADD1);

    //! notice that modifylevels() also removes the level nodes in the tree in case there are no more players left
    //! (but only in case level != 0)

    player_data.level = new_level;

    return SUCCESS;
}

StatusType PlayersManager::ChangePlayerIDScore(int PlayerID, int NewScore) {
    if (!all_players_hash.Exists(PlayerID)) {
        throw Failure();
    }

    PlayerData &player_data = all_players_hash.Find(PlayerID);
    GroupData &group = groups.Find(player_data.owner_group_id);

    ScoreArray &score_array = group.group_levels.AVLGetRank(player_data.level);
    --score_array[player_data.score];
    ++score_array[NewScore];

    player_data.score = NewScore;
}

StatusType PlayersManager::GetPercentOfPlayersWithScoreInBounds(int GroupID, int score, int lowerLevel, int higherLevel, double *players) {
    if (GroupID == 0) {  //*all players
    }
}

//! LOTS OF CALCULATIONS, HIGH RISK FOR BUG!!
StatusType PlayersManager::AverageHighestPlayerLevelByGroup(int GroupID, int m, double *level) {    
    if (GroupID == 0) {  //*all players
        if (m > players_num) {
            throw Failure();
        }

        int players_num = 0;
        int levels_sum = 0;
        for (auto iter = all_players_by_level.rbegin(); iter != all_players_by_level.rend(); ++iter) {
            int &current_level = iter.IKey();
            int &players_num_in_level = iter.IData();

            levels_sum += current_level * players_num_in_level;

            players_num += players_num_in_level;

            if (players_num >= m) {
                levels_sum -= (m - players_num) * current_level;  //*substracting those that are too many
                break;
            }
        }

        *level = double(levels_sum);

    } else {  //* group players
        GroupData &group = groups.Find(GroupID);
        if (m > group.group_size) {
            throw Failure();
        }

        RankTree &group_tree = group.group_levels;

        int players_num = 0, level_sum = 0, level_counter = 0;
        double level_avg = 0, total_level_avg = 0;

        for (auto iter = group_tree.rbegin(); iter != group_tree.rend(); ++iter) {
            int &current_level = iter.IKey();
            int num_of_players_in_level = iter.IData();

            level_counter++;
            players_num += num_of_players_in_level;

            if (players_num >= m) {
                //* final calcultion
                num_of_players_in_level -= (m - players_num);
                level_sum = num_of_players_in_level * current_level;

                level_avg = double(level_sum) / double(num_of_players_in_level);
                total_level_avg += level_avg;
                break;
            }

            level_sum = num_of_players_in_level * current_level;
            level_avg = double(level_sum) / double(num_of_players_in_level);
            total_level_avg += level_avg;
        }

        *level = total_level_avg / double(level_counter);
    }

    return SUCCESS;
}

//  OTHER FUNCTIONS

void modifyRankTreesByPlayerScores(RankTree *tree, int level, int score, int scale, const PlayerAction &action) {
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

    tree->updateRank(level, player_score_as_array);
    int &players_in_level = tree->AVLGet(level);
    players_in_level += change_to_level;
}

void modifyRankTreesLevels(RankTree *tree, int level, const PlayerAction &action) {
    int &num_of_players_in_level = tree->AVLGet(level);
    if (action == LEVEL_ADD1) {
        ++num_of_players_in_level;
    } else if (action == LEVEL_SUB1) {
        --num_of_players_in_level;
    }

    if (level != 0 && num_of_players_in_level == 0) {
        tree->AVLRemove(level);
    }
}

}  // namespace PM