#include"./course_files/library2.h" 
#include "./PlayersManager.h"

typedef PM::PlayersManager Manager;

void *Init(int k, int scale){
    if(scale <= 0 || scale > 200 || k <= 0){
        return NULL;
    }
    try{
        PM::PlayersManager* DS = new PM::PlayersManager(k, scale);
        return (void*)DS;

    }catch(std::bad_alloc& e){
        return NULL;
    }
}

StatusType MergeGroups(void *DS, int GroupID1, int GroupID2){
    if(DS == NULL || GroupID1 <= 0 || GroupID2 <= 0 ||
        GroupID1 > ((Manager*)DS)->k || GroupID2 > ((Manager*)DS)->k){
            return INVALID_INPUT;
        }
    try{
        return ((Manager*)DS)->MergeGroups(GroupID1, GroupID2);
    }
    catch(PM::Failure& e){
        return FAILURE;
    }
}

StatusType AddPlayer(void *DS, int PlayerID, int GroupID, int score){
    if(DS==NULL || GroupID <= 0 || GroupID > ((Manager*)DS)->k ||
        PlayerID <= 0 || score <= 0 || score > ((Manager*)DS)->scale){
            return INVALID_INPUT;
        }
    return ((Manager*)DS)->AddPlayer(PlayerID, GroupID, score);
}


StatusType RemovePlayer(void *DS, int PlayerID){
    if(DS==NULL || PlayerID <= 0){
            return INVALID_INPUT;
        }
    return ((Manager*)DS)->RemovePlayer(PlayerID);
}

StatusType IncreasePlayerIDLevel(void *DS, int PlayerID, int LevelIncrease){
    if(DS==NULL || PlayerID <= 0 || LevelIncrease <= 0){
            return INVALID_INPUT;
        }
    return ((Manager*)DS)->IncreasePlayerIDLevel(PlayerID, LevelIncrease);
}

StatusType ChangePlayerIDScore(void *DS, int PlayerID, int NewScore){
        if(DS==NULL || PlayerID <= 0 || NewScore <= 0 || NewScore > ((Manager*)DS)->scale){
            return INVALID_INPUT;
        }
    return ((Manager*)DS)->ChangePlayerIDScore(PlayerID, NewScore);
}

StatusType GetPercentOfPlayersWithScoreInBounds(void *DS, int GroupID, int score, int lowerLevel, int higherLevel,
                                                double * players){
                                                    
    if(DS == NULL || players == NULL || GroupID <= 0 || GroupID > ((Manager*)DS)->k){
        return INVALID_INPUT;
    }
    return ((Manager*)DS)->GetPercentOfPlayersWithScoreInBounds(GroupID, score, lowerLevel, higherLevel, players);
}

StatusType AverageHighestPlayerLevelByGroup(void *DS, int GroupID, int m, double * level){
        
    if(DS == NULL || level == NULL || GroupID <= 0 || GroupID > ((Manager*)DS)->k || m < 0){
        return INVALID_INPUT;
    }
    return ((Manager*)DS)->AverageHighestPlayerLevelByGroup(GroupID, m, level);
}

StatusType GetPlayersBound(void *DS, int GroupID, int score, int m,
                                         int * LowerBoundPlayers, int * HigherBoundPlayers){

}

void Quit(void** DS){
    delete (Manager*)(*DS);
    *DS = NULL;
}

