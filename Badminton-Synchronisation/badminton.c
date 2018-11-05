#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/shm.h>

int base_n;// n taken as input
int players,referees,tot_ppl;// players, referees and total people
int cur_gen; // number of currently generated people
int cur_game_number = 0; // tracks current game being played
int  organizer = 1;// whether organizer is available or not
int ready = 0; // keeps track of whether two players and a referee are ready to start a game
pthread_mutex_t group_mutex;// mutex  for forming groups

typedef struct people
{
  int entry_num;
  int type;// 1 if player and 2 if referee
  int currently;// stage of activity
} people;

people people_data[1000];

// calcukates the asociated game number to a particular type of person
int calculate_cur_game(people *current){
int cur_game;
if(current->type==1)
  cur_game = (current->entry_num-1)/2;
else if(current->type==2)
  cur_game = current->entry_num-1;
return cur_game;
}

int try_forming_groups(int cur_game)
{
  pthread_mutex_lock(&group_mutex);
  int flag = 0;
  int i,flag_players=0,flag_referee=0;
  for (i=0; i<cur_gen; ++i) {
      if (people_data[i].currently==2){
               if (people_data[i].type == 1 && (people_data[i].entry_num-1)/2==cur_game)
                  flag_players += 1;
               if (people_data[i].type == 2 && (people_data[i].entry_num-1)==cur_game)
                  flag_referee += 1;
         }
  }
  if (flag_players>=2 && flag_referee>=1)
  {
    flag=1;
    int i,flag_players=0,flag_referee=0;
      for (i=0; i<cur_gen; ++i)
      {
          if(flag_players>=2 && flag_referee>=1)
          break;
          if (people_data[i].currently==2)
          {
                   if (people_data[i].type == 1 && (people_data[i].entry_num-1)/2==cur_game)
                   {
                     if(flag_players<2)
                      flag_players += 1;
                    people_data[i].currently = 3;
                  }
                   if (people_data[i].type == 2 && (people_data[i].entry_num-1)==cur_game)
                   {
                     if(flag_referee<1)
                      flag_referee += 1;
                    people_data[i].currently = 3;
                  }
          }
      }
      cur_game_number+=1;
      printf("Group for game number %d has been formed\n", cur_game);
      organizer = 0;
  }
  pthread_mutex_unlock(&group_mutex);
  return flag;
}


void enterAcademy(void *ptr){
    people *cur = (people *)ptr;
    printf("%s %d has entered the academy\n",(cur->type==1?"Player":"Referee"), cur->entry_num);
    cur->currently = 1;// has entered academy
    return;
}

void meetOrganizer(void *ptr){
  people *cur = (people *)ptr;
  printf("%s %d has met the organizer\n", (cur->type==1?"Player":"Referee"), cur->entry_num);
  cur->currently = 2;// has met organizer
  return;
}

void enterCourt(void *ptr){
  people *cur = (people *)ptr;
  printf("%s %d has entered the court\n", (cur->type==1?"Player":"Referee"), cur->entry_num);
  cur->currently = 3;// has entered court
  return;
}

void warmUp(void *ptr){
    people *cur = (people *)ptr;
    printf("%s %d is warming up and getting ready to play!\n", (cur->type==1?"Player":"Referee"), cur->entry_num);
    ready++;
    sleep(1);
    cur->currently = 4;// has finished warming up
    return;
}

void adjustEquipment(void *ptr){
    people *cur = (people *)ptr;
    printf("%s %d is adjusting equipment while players warm up...\n", (cur->type==1?"Player":"Referee"), cur->entry_num);
    ready++;
    sleep(0.5);
    cur->currently = 4;// has finished adjusting equipment
    return;
}

void startGame(int game_num){
    printf("\nSit tight! Game %d has started\n", game_num);
    return;
}

void* IAmNewHere(void *ptr)
{
  people *current = (people *) ptr;
  int cur_game = calculate_cur_game(current);

  enterAcademy(ptr);// player has entered the academy
  
  while(organizer==0)
    continue;// player/referee waits while the organizer is busy with others
  meetOrganizer(ptr);// the moment organizer becomes free, player/referee meets the organizer

  int group_made = try_forming_groups(cur_game);// the person checks if it can form a group of three
  while(group_made==0){// person waits till he is included in a group
    if (current->currently==3)
      break;
  }

  enterCourt(ptr);// person enters court after forming the group
  if(current->type == 1)
  {
    // the player warms up while waiting for the referee to adjust equipment
    warmUp(ptr);
    return NULL;
  }
  else if(current->type ==2)
  {
    // the referee adjusts equipment while player warms up
    adjustEquipment(ptr);
    // referee waits while two players have finished warming up
    while(ready<3) continue;
    // determining game number--might be faulty
    startGame(cur_game+1);// or use cur_game_number
    ready=0;// put ready back to zero as no players are ready anymore
    organizer=1;//organizer is free again
    return NULL;
  }
}

int main(){

  printf("Enter n (2*n players and n referees will come to play) : ");
  scanf("%d",&base_n);

  tot_ppl = 3*base_n;
  players = 2*base_n;
  referees = base_n;

  int ref_made = 0;
  int players_made = 0;

  int i;
  int wait = rand()%3;
  double probability;
  if (pthread_mutex_init(&group_mutex, NULL) != 0){
      printf("Mutex initialization failed ! Aborting");
      exit(1);
  }

  pthread_t people_thread[tot_ppl];
  for(i=0;i<tot_ppl;++i){

    people_data[i].currently = -1;
    probability = (double)players/(double)(players+referees);
    if(probability >= 0.5){
      players_made += 1;
      people_data[i].type = 1;//player created
      people_data[i].entry_num = players_made;
      players-=1;
    }
    else{
      ref_made+=1;
      people_data[i].type = 2;// referee created
      people_data[i].entry_num = ref_made;
      referees-=1;
    }
    pthread_create(&people_thread[i],NULL,IAmNewHere, &people_data[i]);
    cur_gen = i+1;
    sleep(wait);
  }
  for(i=0;i<tot_ppl;++i)
      pthread_join(people_thread[i],NULL);
  return 0;
}
