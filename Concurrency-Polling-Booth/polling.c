#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NEW_VOTER 0
#define WAITING_VOTER 1
#define ASSIGNED_VOTER 2
#define ZERO 0

typedef struct Booth Booth;
typedef struct EVM EVM;
typedef struct Voter Voter;

typedef struct EVM
{
  int idx;
  pthread_t evm_thread_id;
  Booth * booth;
  int number_of_slots;
  int flag;
} EVM;

typedef struct Voter
{
  int idx;
  pthread_t voter_thread_id;
  Booth * booth;
  EVM * evm;
  int status;
} Voter;

typedef struct Booth
{
  int idx;
  pthread_t booth_thread_id;
  int number_of_evms;
  int max_slots_in_evm;
  int number_of_voters;
  int finished_voters;
  pthread_mutex_t mutex;
  pthread_cond_t conditionvar_1;
  pthread_cond_t conditionvar_2;
  EVM ** evms;
  Voter ** voters;
} Booth;

void * thread_for_evm(void * args)
{
  EVM * evm = (EVM*)args;
  Booth * booth = evm->booth;
  pthread_mutex_t * mutex_point = &(booth->mutex);
  pthread_cond_t * condvar1_point = &(booth->conditionvar_1);
  pthread_cond_t * condvar2_point = &(booth->conditionvar_2);
  int i=0;
  int j=0;
  int maxnum=0;
  while(1==1)
  {
    pthread_mutex_lock(mutex_point);
    if(booth->finished_voters == booth->number_of_voters){pthread_mutex_unlock(mutex_point);break;}
    pthread_mutex_unlock(mutex_point);
 
    maxnum = rand()%(booth->max_slots_in_evm) + 1;
    j = ZERO;
 
    pthread_mutex_lock(mutex_point);evm->number_of_slots = maxnum;evm->flag = 0;
    pthread_mutex_unlock(mutex_point);
	printf("In booth %d and evm %d, %d slots are free.\n", booth->idx, evm->idx, maxnum);
    
    while(j<maxnum)
    {
      int modul = booth->number_of_voters;i = rand()%modul;

      pthread_mutex_lock(mutex_point);
      int checkstate = booth->voters[i]->status;
      if(checkstate == 1)
      {
      	booth->voters[i]->status = 2;booth->voters[i]->evm = evm;(booth->finished_voters)++;j++;printf("In booth %d and evm %d, voter %d has been alloted.\n", booth->idx, evm->idx, i);
      }
  
  	  int finished = booth->finished_voters;
  	  int total = booth->number_of_voters;    
      if(finished==total){pthread_mutex_unlock(mutex_point);break;}
      pthread_mutex_unlock(mutex_point);
    }

    if(j==0){printf("In booth %d, evm %d is closing.\n", booth->idx, evm->idx);break;}
    /* evm executing voting phase. */
    printf("In booth %d and evm %d has started voting phase.\n", booth->idx, evm->idx);
    pthread_mutex_lock(mutex_point);
    	evm->number_of_slots = j;evm->flag = 1;pthread_cond_broadcast(condvar1_point);
    	int evmslots = 1;
    	while(evm->number_of_slots)
    		pthread_cond_wait(condvar2_point, mutex_point);
    pthread_mutex_unlock(mutex_point);
    printf("In booth %d, evm %d has finished voting phase.\n", booth->idx, evm->idx);
  }
  return NULL;
}


void * thread_for_voter(void * args)
{
  int term=1;

  Voter * voter = (Voter *)args;
  pthread_mutex_t * mutex_point = &(voter->booth->mutex);
  pthread_cond_t * condvar1_point = &(voter->booth->conditionvar_1);
  pthread_cond_t * condvar2_point = &(voter->booth->conditionvar_2);
  
  if(term==1){
  pthread_mutex_lock(mutex_point);
  	voter->status = 1;
  	while(voter->status == 1)
  		pthread_cond_wait(condvar1_point, mutex_point);
  pthread_mutex_unlock(mutex_point);
}
  EVM * evm = voter->evm;
  int flagie=1;

  if(flagie==1){
  pthread_mutex_lock(mutex_point);
  int zero=0;
  while(evm->flag == zero)pthread_cond_wait(condvar1_point, mutex_point);
  (evm->number_of_slots)--;
  
  int axe=3;
  if(axe==3){
  printf("In booth number %d, evm number is %d and the voter number %d has casted its vote.\n",evm->booth->idx, evm->idx, voter->idx);pthread_cond_broadcast(condvar2_point);pthread_mutex_unlock(mutex_point);
  }
  }
  return NULL;
}


void * thread_for_booth(void * args)
{
  Booth * booth = (Booth *)args;int i=0;
  for(i=0; i<booth->number_of_evms; i++){
	  	booth->evms[i] = (EVM*)malloc(sizeof(EVM));
	  	booth->evms[i]->idx = i;
	  	booth->evms[i]->booth = booth;
	  	booth->evms[i]->number_of_slots = 0;
		  	booth->evms[i]->flag = 0;}
  for(i=0; i<booth->number_of_voters; i++){
      booth->voters[i] = (Voter*)malloc(sizeof(Voter));
	  booth->voters[i]->idx = i;
	  booth->voters[i]->booth = booth;
	  booth->voters[i]->status = NEW_VOTER;
	  booth->voters[i]->evm = NULL;}
  /* evms and voters threads start */
  i=0;
  while(i<booth->number_of_evms)
  {
  	pthread_create(&(booth->evms[i]->evm_thread_id),NULL,thread_for_evm,booth->evms[i]);
  	i++;
  }
  i=0;
  while(i<booth->number_of_voters)
  {
  	pthread_create(&(booth->voters[i]->voter_thread_id),NULL,thread_for_voter,booth->voters[i]);
  	i++;
  }
  i=0;
  while(i<booth->number_of_evms)
  {
  	pthread_join(booth->evms[i]->evm_thread_id, 0);
  	i++;
  }
  i=0;
  while(i<booth->number_of_voters)
  {
  	pthread_join(booth->voters[i]->voter_thread_id, 0);
  	i++;
  }
  /* freeing memory */
  i=0;
  while(i<booth->number_of_evms)
  {
  	free(booth->evms[i]);
  	i++;
  }
  i=0;
  while(i<booth->number_of_voters)
  {
  	free(booth->voters[i]);
  	i++;
  }
  free(booth->evms);
  free(booth->voters);
  printf("%d booth is signing off.\n", booth->idx);  
  return NULL;
}



int main()
{
  int number_of_booths = 1;
  /* --- input --- */
  scanf("%d", &number_of_booths);
  int number_of_evms[105]={0};
  int max_slots_in_evm[105]={0};
  int number_of_voters[105]={0};
  int i=0;
  
  for(i=0; i<number_of_booths; i++)
  	scanf("%d %d %d",&number_of_voters[i], &number_of_evms[i], &max_slots_in_evm[i]);
  
  printf("ELECTION STARTS.\n");
  Booth ** booths = (Booth**)malloc((sizeof(Booth*))*(number_of_booths));for(i=0; i<number_of_booths; i++)booths[i] = (Booth*)malloc(sizeof(Booth));
  

  i=0;
  while(i<number_of_booths)
  {
  	booths[i]->idx = i;
  	booths[i]->number_of_evms = number_of_evms[i];
  	booths[i]->max_slots_in_evm = max_slots_in_evm[i];
  	booths[i]->number_of_voters = number_of_voters[i];
  	booths[i]->finished_voters = 0;
  	booths[i]->evms = (EVM**)malloc(sizeof(EVM*)*number_of_evms[i]);
  	booths[i]->voters = (Voter**)malloc(sizeof(Voter*)*number_of_voters[i]);
  	pthread_mutex_init(&(booths[i]->mutex), NULL);
  	pthread_cond_init(&(booths[i]->conditionvar_1), NULL);
  	pthread_cond_init(&(booths[i]->conditionvar_2), NULL);
  	i++;
  }
  i=0;
  while(i<number_of_booths)
  {
  	pthread_create(&(booths[i]->booth_thread_id), NULL, thread_for_booth, booths[i]);
  	i++;
  }
  i=0;
  while(i<number_of_booths)
  {
  	pthread_join(booths[i]->booth_thread_id, 0);
  	i++;
  }
  i=0;
  while(i<number_of_booths){free(booths[i]);i++;}
  free(booths);printf("ELECTION COMPLETES.\n");
  return 0;
}
