#include "./PlayersManager.h"

namespace PM {

StatusType PlayersManager::MergeGroups(int GroupID1, int GroupID2) {
    if(GroupID1 == GroupID2){
        return SUCCESS;
    }
    groups.Union(GroupID1, GroupID2);  //INSIDE UF: merge the ranktrees of the groups somehow (or should it be outside? maybe it should.)
     /* RankTreeScoreArray& tree7 = groups.Find(7).group_levels_scores;
    tree7.printTreeRank(137);
    tree7.printTreeRank(132); */
    
    
     
    return SUCCESS;
}

StatusType PlayersManager::AddPlayer(int PlayerID, int GroupID, int score) {
    if (all_players_hash.Exists(PlayerID)) {
        throw Failure();
    }

    GroupData &group = groups.Find(GroupID);
    PlayerData new_player(PlayerID, GroupID, score);  //!should we allocate on heap or on stack

    all_players_hash.Insert(PlayerID, new_player);

    ++group.group_level_0[score];
    ++level_0[score];

    //group.group_level_0.Print();
    //level_0.Print();
    /*     modifyRankTreesByPlayerScores(&group.group_levels, 0, score, scale, PLAYER_ADD);
    modifyRankTreesByPlayerScores(&all_players_by_level, 0, score, scale, PLAYER_ADD); */
    //* not needed because level is 0

    ++group.group_size;
    ++players_num;

    
    //all_players_by_level.printTreeRank(24);

    return SUCCESS;
}

StatusType PlayersManager::RemovePlayer(int PlayerID) {
    if (!all_players_hash.Exists(PlayerID)) {
        throw Failure();
    }

    PlayerData *player_data = &all_players_hash.Find(PlayerID);
    GroupData &group = groups.Find(player_data->owner_group_id);

    if (player_data->level != 0) {
        modifyRankTreesByPlayerScores(&group.group_levels_scores, player_data->level, player_data->score, scale, PLAYER_REMOVE);
        modifyRankTreesSums(&group.group_levels_sums, player_data->level, player_data->score, scale, PLAYER_REMOVE);
        modifyRankTreesMulti(&group.group_levels_multi, player_data->level, player_data->score, scale, PLAYER_REMOVE);
        /* group.group_levels_sums.AVLGetRank(player_data->level) -= 1;
        group.group_levels_multi.AVLGetRank(player_data->level) -= player_data->level; */

        //ModifyTreesIfLevelEmpty(player_data->level, &group);

        modifyRankTreesByPlayerScores(&all_players_by_level, player_data->level, player_data->score, scale, PLAYER_REMOVE);
        modifyRankTreesSums(&all_players_by_level_sums, player_data->level, player_data->score, scale, PLAYER_REMOVE);
        modifyRankTreesMulti(&all_players_by_level_multi, player_data->level, player_data->score, scale, PLAYER_REMOVE);

        /* all_players_by_level_sums.AVLGetRank(player_data->level) -= 1;
        all_players_by_level_sums.AVLGetRank(player_data->level) -= player_data->level;

        ModifyTreesIfLevelEmpty(player_data->level, nullptr);
 */
    } else if (player_data->level == 0) {
        --group.group_level_0[player_data->score];
        --level_0[player_data->score];
    }
    --group.group_size;

    all_players_hash.Remove(PlayerID);

    --players_num;

    /* RankTreeScoreArray& tree7 = groups.Find(7).group_levels_scores;
    tree7.printTreeRank(); */
    //all_players_by_level.printTreeRank();

    return SUCCESS;
}

StatusType PlayersManager::IncreasePlayerIDLevel(int PlayerID, int LevelIncrease) {
    if (!all_players_hash.Exists(PlayerID)) {
        throw Failure();
    }

    PlayerData &player_data = all_players_hash.Find(PlayerID);
    GroupData &group = groups.Find(player_data.owner_group_id);

    int new_level = player_data.level + LevelIncrease;

    if (player_data.level == 0) {
        --level_0[player_data.score];
        --group.group_level_0[player_data.score];

    } else {
        modifyRankTreesByPlayerScores(&group.group_levels_scores, player_data.level, player_data.score, scale, PLAYER_REMOVE);
        modifyRankTreesSums(&group.group_levels_sums, player_data.level, player_data.score, scale, PLAYER_REMOVE);
        modifyRankTreesMulti(&group.group_levels_multi, player_data.level, player_data.score, scale, PLAYER_REMOVE);

        /* group.group_levels_sums.AVLGetRank(player_data.level) -= 1;
        group.group_levels_multi.AVLGetRank(player_data.level) -= player_data.level;

        ModifyTreesIfLevelEmpty(player_data.level, &group); */

        modifyRankTreesByPlayerScores(&all_players_by_level, player_data.level, player_data.score, scale, PLAYER_REMOVE);
        modifyRankTreesSums(&all_players_by_level_sums, player_data.level, player_data.score, scale, PLAYER_REMOVE);
        modifyRankTreesMulti(&all_players_by_level_multi, player_data.level, player_data.score, scale, PLAYER_REMOVE);

        //ModifyTreesIfLevelEmpty(player_data.level, nullptr);
    }
    //*group players ScoreArray update

    modifyRankTreesByPlayerScores(&group.group_levels_scores, new_level, player_data.score, scale, PLAYER_ADD);
    modifyRankTreesSums(&group.group_levels_sums, new_level, player_data.score, scale, PLAYER_ADD);
    modifyRankTreesMulti(&group.group_levels_multi, new_level, player_data.score, scale, PLAYER_ADD);
    //*all players ScoreArray update

    modifyRankTreesByPlayerScores(&all_players_by_level, new_level, player_data.score, scale, PLAYER_ADD);
    modifyRankTreesSums(&all_players_by_level_sums, new_level, player_data.score, scale, PLAYER_ADD);
    modifyRankTreesMulti(&all_players_by_level_multi, new_level, player_data.score, scale, PLAYER_ADD);

    //! notice that modifylevels() also removes the level nodes in the tree in case there are no more players left
    //! (but only in case level != 0)

    player_data.level = new_level;

    /* RankTreeScoreArray& tree7 = groups.Find(7).group_levels_scores;
    tree7.printTreeRank(137);
    tree7.printTreeRank(132); */
    

    

    //all_players_by_level.printTreeRank(24);

    return SUCCESS;
}

StatusType PlayersManager::ChangePlayerIDScore(int PlayerID, int NewScore) {
    if (!all_players_hash.Exists(PlayerID)) {
        throw Failure();
    }

    PlayerData &player_data = all_players_hash.Find(PlayerID);
    GroupData &group = groups.Find(player_data.owner_group_id);
    
    if (player_data.level == 0) {
        --(group.group_level_0[player_data.score]);
        ++(group.group_level_0[NewScore]);

        --(level_0[player_data.score]);
        ++(level_0[NewScore]);

    } else {
        modifyRankTreesByPlayerScores(&group.group_levels_scores, player_data.level, player_data.score, scale, PLAYER_REMOVE);
        modifyRankTreesByPlayerScores(&group.group_levels_scores, player_data.level, NewScore , scale, PLAYER_ADD);

        modifyRankTreesByPlayerScores(&all_players_by_level, player_data.level, player_data.score, scale, PLAYER_REMOVE);
        modifyRankTreesByPlayerScores(&all_players_by_level, player_data.level, NewScore, scale, PLAYER_ADD);
    }
    player_data.score = NewScore;
    /* RankTreeScoreArray& tree7 = groups.Find(7).group_levels_scores;
    tree7.printTreeRank(); */
    //all_players_by_level.printTreeRank(24);

    return SUCCESS;
}
//tryyyyy
StatusType PlayersManager::GetPercentOfPlayersWithScoreInBounds(int GroupID, int score, int lowerLevel, int higherLevel, double *players) {

    if (lowerLevel > higherLevel) {
        return FAILURE;
    }

    if (score <= 0 || score > scale) {
        *players = 0;
        return SUCCESS;
    }  // ! probily need to be checked in the end to return all failiures first

    int players_in_score = 0;
    int total_players_in_range = 0;

    if (lowerLevel <= 0 && higherLevel >= 0) {
        if (GroupID == 0) {
            players_in_score += level_0[score];
            total_players_in_range += level_0.MembersAmount();
            //level_0.Print();
        } else {
            GroupData &group = groups.Find(GroupID);
            players_in_score += group.group_level_0[score];
            total_players_in_range += group.group_level_0.MembersAmount();
            //group.group_level_0.Print();
        }
    }

    if (higherLevel > 0) {
        
        
        RankTreeScoreArray *tree;
        //tree->printTreeData();
        if (GroupID == 0) {  //*all players
            tree = &all_players_by_level;
            //TODO: check if lower/higher bound lower than minimum key in tree
        } else {  //*group players
            GroupData &group = groups.Find(GroupID);
            tree = &group.group_levels_scores;
        }

        ScoreArray scores_in_range(scale);
        auto result = tree->RankInRange(lowerLevel, higherLevel, &scores_in_range);
        //scores_in_range.Print();
        if (result == AVLRank::RankStatus::RANK_OUT_OF_RANGE) {
            if (!(lowerLevel <= 0 && higherLevel >= 0)) {
                throw Failure();
            }
        }
        
        //tree->printTreeData();
        else {
            players_in_score += scores_in_range[score];
            total_players_in_range += scores_in_range.MembersAmount();
        }
    }
    if(total_players_in_range == 0){
        throw Failure();
    }
    *players = (double(players_in_score) / double(total_players_in_range)) * 100.0;

    return SUCCESS;
}

//! LOTS OF CALCULATIONS, HIGH RISK FOR BUG!!
StatusType PlayersManager::AverageHighestPlayerLevelByGroup(int GroupID, int m, double *level) {  //TODOOOOOOOOOOOOOO

    //int left_over = 0;

    if (GroupID == 0) {  //*all players
        if (m > players_num) {
            throw Failure();
        }

        *level += all_players_by_level_sums.AvgHighRank(m, all_players_by_level_multi);

        /* int players_num = 0;
        int levels_sum = 0; */

    } else {  //* group players
        GroupData &group = groups.Find(GroupID);
        if (m > group.group_size) {
            throw Failure();
        }

        RankTreeInt &group_tree_sums = group.group_levels_sums;
        RankTreeInt &group_tree_multi = group.group_levels_multi;

        *level = group_tree_sums.AvgHighRank(m, group_tree_multi);
    }
    return SUCCESS;
    /* int players_num = 0, level_sum = 0, level_counter = 0;
        double level_avg = 0, total_level_avg = 0; */

    //TODO: use AvgHighRank(m, all_players_by_level_multi); - will give you avg calculated
}

//  OTHER FUNCTIONS

void modifyRankTreesByPlayerScores(RankTreeScoreArray *tree, int level, int score, int scale, const PlayerAction &action) {
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

    if (!(tree->AVLExist(level)) && action == PLAYER_ADD) {  //*if level doesnt exist in tree, we add it
        tree->AVLInsert(level, 1, player_score_as_array);
        return;

    } else{
        tree->updateRank(level, player_score_as_array);
    }

    int &players_in_level = tree->AVLGet(level);
    players_in_level += change_to_level;
    if (level != 0 && players_in_level == 0) {
        tree->AVLRemove(level);
    }
}

void modifyRankTreesSums(RankTreeInt *tree, int level, int score, int scale, const PlayerAction &action) {
    int change_to_level;

    if (action == PLAYER_ADD) {
        //*for updating data:
        change_to_level = 1;
    } else if (action == PLAYER_REMOVE) {
        change_to_level = -1;
    }

    if (!(tree->AVLExist(level)) && action == PLAYER_ADD) {  //*if level doesnt exist in tree, we add it
        tree->AVLInsert(level, 1, change_to_level);
        return;
    } else {
        tree->updateRank(level, change_to_level);
    }

    int &players_in_level = tree->AVLGet(level);
    players_in_level += change_to_level;
    if (level != 0 && players_in_level == 0) {
        tree->AVLRemove(level);
    }
}

void modifyRankTreesMulti(RankTreeInt *tree, int level, int score, int scale, const PlayerAction &action) {
    int change_to_level;

    if (action == PLAYER_ADD) {
        //*for updating data:
        change_to_level = 1;
    } else{
        change_to_level = -1;
    }

    if (!(tree->AVLExist(level)) && action == PLAYER_ADD) {  //*if level doesnt exist in tree, we add it
        tree->AVLInsert(level, 1, level);
        return;
    } else if (action == PLAYER_ADD){
        tree->updateRank(level, level);
    }else if(action == PLAYER_REMOVE){
        tree->updateRank(level, -level);
    }

    int &players_in_level = tree->AVLGet(level);
    players_in_level += change_to_level;
    if (level != 0 && players_in_level == 0) {
        tree->AVLRemove(level);
    }
}

/* 
    void ModifyTreesIfLevelEmpty(int level, int change, GroupData* group) {

        if(group == nullptr){
            if(change > 0 && !all_players_by_level.AVLExist(level){
            ScoreArray
            all_players_by_level->AVLInsert(level, 0, 0);
            all_players_by_level_sums->AVLRemove(level);
            all_players_by_level_multi->AVLRemove(level);
            }
            int &players_in_level = all_players_by_level->AVLGet(level);
            players_in_level += change;

            if (level != 0 && players_in_level == 0) {
            all_players_by_level->AVLRemove(level);
            all_players_by_level_sums->AVLRemove(level);
            all_players_by_level_multi->AVLRemove(level);
        }
        else{
            int &players_in_level = group->group_levels_scores->AVLGet(level);
            players_in_level += change_to_level;

            if (level != 0 && players_in_level == 0) {
            group->group_levels_scores->AVLRemove(level);
            group->group_levels_sums->AVLRemove(level);
            group->group_levels_multi->AVLRemove(level);
        }
    } */

/* void modifyRankTreesLevels(RankTree *tree, int level, int scale, const PlayerAction &action) {


    int &num_of_players_in_level = tree->AVLGet(level);
    if (action == LEVEL_ADD1) {
        ++num_of_players_in_level;
    } else if (action == LEVEL_SUB1) {
        --num_of_players_in_level;
    }
 */

}  // namespace PM