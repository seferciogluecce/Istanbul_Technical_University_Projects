//Ece Naz Sefercioðlu
//150130140

#define _GNU_SOURCE
// for shared memory and semaphores
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
// handling signals
#include <signal.h>
//use fork
#include <unistd.h>
// other necessary headers
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define KEYSEM  2568   //ids for semaphores to use
#define KEYSEM2 8741
#define KEYSEM3 5632
#define KEYSEM4 2588
#define KEYSEM5 5820
#define KEYSEM6 5698

#define KEYSHM  1478


void sem_wait(int semid, int val) {          // Semaphore decrement operation to set it wait for a signal
	struct sembuf semaphore;
	semaphore.sem_num = 0;
	semaphore.sem_op = (-1 * val);
	semaphore.sem_flg = 1;
	semop(semid, &semaphore, 1);
}

void sem_signal(int semid, int val) {        // Semaphore increment operation to let it run
	struct sembuf semaphore;
	semaphore.sem_num = 0;
	semaphore.sem_op = val;
	semaphore.sem_flg = 1;
	semop(semid, &semaphore, 1);
}

void mysignal(int signum) { // Signal handling function
}; 

void mysigset(int num) {
	struct sigaction mysigaction;
	mysigaction.sa_handler = (void *)mysignal;
	//using signal-catching function identified by sa_handler
	mysigaction.sa_flags = 0;
	// sigaction system call is used to cahnge the action taken by a process on receipt of a specific signal (specified by num)
	sigaction(num, &mysigaction, NULL);
}



int *globalcp = NULL; //pointer to our shared memory

int  full, empty; //semaphores for syncing register and interview threads of hr department
int trainingSeq;//semaphores for syncing interviewer and trainer threads
int peps = 0; //people counter for interviewers
int noOfLine; //number of applicants - 1
int regDur;  // registration duration of recepcionist
int regs = 0; //number of applicants registered
int* ni;  //array to hold the duration of interview for each applicant
int* nt; //array to hold the duration of training for each applicant
int * intSucces; //array to hold the outcome of interview results for each applicant
int * trainSucces;  //array to hold the outcome of training results for each applicant
int nr;  //registration duration
int trainPeps=0;
int pepst=0; //people counter for trainers
pthread_mutex_t mutex[3]; //mutexes for data exclusion for interviewers

pthread_mutex_t mutext[4]; //mutexes for data exclusion for trainers





void* registerP() {   //register applicants and give them to interviewers

	while (1) {

		sem_wait(empty, 1); //while empty semaphore is not empty

globalcp[regs] =0;     //set status bit to 0 as applicant is registered      
		printf("Applicant %d applied to receptionist(status of applicant %d: %d)\n", regs + 1, regs+1 ,globalcp[regs] ); //register next applicant
		sleep(regDur);                 // wait for nr seconds
		printf("Applicant %d's registration is done(status of applicant %d: %d)\n", regs + 1, regs+1 ,globalcp[regs]);
		                     
		regs++;															//number of people registered increased


		sem_signal(full, 1);	//signal full semaphore to let interviewers run

		if (regs == noOfLine-1) { //if registered aplicants number equals to total number of applicants  
			sleep(1);
		
		printf("Registeration process is done.\n");

			semctl(empty, 0, IPC_RMID, 0);  //terminate empty semaphore
			break;   //break out of the while loop
		}

		sleep(1);
	}

pthread_exit(NULL);  //terminate the thread


}

void* interview(void *i) {
	while (1) {
		int m = (intptr_t)i; //number of interviewer

		sem_wait(full, 1);  //wait for a signal from recepcionist to start interview

		pthread_mutex_lock(&mutex[m]);  //lock the data with mutex of the interviewer
		peps++; //interviewed people is increased
		long id = (long)i + 1;   //order of interviewer 
		printf("Interviewer %ld started interview with Applicant %d (status of applicant %d: %d)\n", id, peps, peps, globalcp[peps-1]  );
		int  current = peps;
		globalcp[peps-1] =intSucces[current-1]; 
		sleep(ni[current - 1]); //sleep for the interview time of applicant
		printf("Interviewer %ld finished interview with Applicant %d (status of applicant %d: %d)\n", id, current, current,globalcp[current-1]);
		
		sem_signal(trainingSeq,1);
		pthread_mutex_unlock(&mutex[m]); //let the data be available
		
		if (current == noOfLine) //if the current applicant equals total number of applicants
		{
						sleep(1);
			printf("Interview process is done.\n");
			semctl(full, 0, IPC_RMID, 0);  //terminate full semaphore
			break;    //break out of the while loop
		}

		sleep(1);

	}
	pthread_exit(NULL);

}


void*train(void*i) {  //training given applicant
	printf("%d trained someone\n", i);
	
	
	
	while (1) {
		int  current;
		int m = (intptr_t)i; //number of trainer

		sem_wait(trainingSeq, 1);  //wait for a signal from interviewer to start training

		pthread_mutex_lock(&mutext[m]);  //lock the data with mutex of the interviewer
		pepst++; //trained people is increased
		long id = (long)i + 1;   //order of trainer
		
		if(globalcp[peps-1]==1){
		printf("Trainer %ld started training with Applicant %d (status of applicant %d: %d)\n", id, peps, peps, globalcp[pepst-1]  );
		  current = pepst;
		globalcp[pepst-1] =trainSucces[current-1]; 
		sleep(ni[current - 1]); //sleep for the interview time of applicant
		printf("Trainer %ld finished training with Applicant %d (status of applicant %d: %d)\n", id, current, current,globalcp[current-1]);
	}
	else{
		globalcp[pepst-1] =trainSucces[current-1]; 		
		}
		pthread_mutex_unlock(&mutext[m]); //let the data be available
		
		if (current == noOfLine) //if the current applicant equals total number of applicants
		{
						sleep(1);
			printf("Training process is done.\n");
						sem_signal(trainingSeq,noOfLine+1); //send signal to main process to end execution
			break;    //break out of the while loop
		}

		sleep(1);

	}
	pthread_exit(NULL);
	

}


int main(int argc, char * argv[]) {

	//********************************   //file read filling variables operation
	char * fname = argv[1];  //read from file
	FILE *myFile = fopen(fname, "r");
	noOfLine = 0;
	char cont;
	if (myFile == NULL)
		printf("error");
	while (!feof(myFile)) {  //count number of lines in the file
		cont = fgetc(myFile);
		if (cont == '\n') {
			noOfLine++;
		}

	}
	noOfLine--;  //number of lines of file also the number of applicants - 1 

	ni = (int*)malloc((noOfLine - 1) * sizeof(int));  //interview time for each applicant
	nt = (int*)malloc((noOfLine - 1) * sizeof(int));  //training time for each applicant
	intSucces = (int*)malloc((noOfLine - 1) * sizeof(int));  //succes result of interview for each applicant
	trainSucces = (int*)malloc((noOfLine - 1) * sizeof(int)); //succes result of interview for each applicant

	rewind(myFile);

	int  read;
	int a;
	fscanf(myFile, "%d", &nr);  //read the registration duration from first line

	for (a = 1; a < noOfLine; a++) {   //read interview duration, training duration, and succes results for interview and trainins 

		fscanf(myFile, "%d %d %d %d", &ni[a - 1], &nt[a - 1], &intSucces[a - 1], &trainSucces[a - 1]);
	}

	//*********************************
	regDur = nr;  //registration duration on global variable
 printf("Registeration time is: %d seconds\n",nr); //infor the registration time

	int f; 
	int shmid=0; //shared memory id
	int myOrder = 0;  //order of running child process
	int  interviewSeq = 0;  //semaphore ids
	int localInt; // locally defined integer
	mysigset(12); //signal handler with num=12
	int child[2]; //child process ids

int i;
	for (i = 0; i < 2 + noOfLine - 1; i++) {               // Fork two + number of applicants child processes
		f = fork();                         // child processes are hr department  and trial department
		child[i] = f;

		if (f == 0) {
			break;
		}
		else if (i < 0) {
			printf("Fork Error\n");
			exit(1);
		}
	}


	if (f == 0) {     //   Children Processes



		if (i == 0) {               // Child process - HR department 

			pause();        // Wait for the  signal from parent to start register - interview applicants

			interviewSeq = semget(KEYSEM3, 1, 0); //  sync between registrar and interviewer
												  //semctl(interviewSeq, 0, SETVAL, 0);
															//semaphores to sync between recepcionist and interviewer threads
			full = semget(KEYSEM5, 1, 0700 | IPC_CREAT);  //interview with the signal
			semctl(full, 0, SETVAL, 0); 

			empty = semget(KEYSEM6, 1, 0700 | IPC_CREAT);  //register till zero value
			semctl(empty, 0, SETVAL, 0);
														//****		
			

			shmid = shmget(KEYSHM, (noOfLine - 1) * sizeof(int), 0);  
			globalcp = (int *)shmat(shmid, 0, 0);  // attaching shared memory to thread  the status array of applicants
			
			int c;
				for(c=0;c<3;c++)
					pthread_mutex_init(&mutex[c],NULL);
  

			sem_signal(empty, noOfLine-1);  //signal empty semaphore to number of applicants




			pthread_attr_t attr; // thread attribute initilaziton to join them
			pthread_attr_init(&attr);
			pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);



			pthread_t * interviewer = malloc(3 * sizeof(pthread_t)); // interview threads
			pthread_t * receptionist = malloc(sizeof(pthread_t));    //receptionist thread 
		
		
			int rc;  //error handling integer
			rc = pthread_create(&receptionist, &attr, registerP, NULL);  //creta threads
			if (rc) {
				printf("\n Error: Thread not created");
				exit(EXIT_FAILURE);
			}
			int a;
			for (a = 0; a < 3; a++) {

				rc = pthread_create(&interviewer[a], &attr, interview, ((void*)(intptr_t)a));

				if (rc) {
					printf("\n Error: Thread not created");
					exit(EXIT_FAILURE);
				}
			}			

			int b;

			for (b = 0; b < 3; b++) {                     // join threads
				rc = pthread_join(interviewer[b], NULL);        
				if (rc) {
					printf("\n Error: Thread not joined");
					exit(EXIT_FAILURE);
				}
			}
			rc = pthread_join(receptionist, NULL);        
														  
			if (rc) {
				printf("\n Error: Thread not joined");
				exit(EXIT_FAILURE);
			}



			free(receptionist); // Free receptionist and interviewer threads 
			receptionist = NULL;

			free(interviewer);
			interviewer = NULL;
	
			pthread_attr_destroy(&attr); // not needed, cleans
			
			for(c=0;c<3;c++)
				pthread_mutex_destroy(&mutex[c]);  //destroy used mutexes
  
			

			shmdt(globalcp);
			semctl(full, 0, IPC_RMID, 0); // Delete semaphores used by thread functions
			semctl(empty, 0, IPC_RMID, 0);


			sem_signal(interviewSeq, 1);       // Signal to trial department
			exit(0);

		}
		else if(i=1) {        // Child Process - Trial Department 
			
			pause();  //wait signal from interviewers to start training
			
			trainingSeq = semget(KEYSEM4, 1, 0);   //
			
			shmid = shmget(KEYSHM, (noOfLine - 1) * sizeof(int), 0);  //attaching shared memory to child process
			globalcp = (int *)shmat(shmid, 0, 0);
			
			int c;
			for (c = 0; c<4; c++)  //data exclusion for trainers
				pthread_mutex_init(&mutext[c], NULL);

			printf("The tariners started training.\n");

			pthread_attr_t attr; // attribute creation to join threads
			pthread_attr_init(&attr);
			pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

			pthread_t * trainer = malloc(4 * sizeof(pthread_t)); 
			
			int rc; //error handling integer
			int a;
			for (a = 0; a < 3; a++) { 					//create 4 trainer thread from trial department process

				rc = pthread_create(&trainer[a], &attr, train, ((void*)(intptr_t)a));

				if (rc) {
					printf("\n Error: Thread not created");
					exit(EXIT_FAILURE);
				}
			}
			int b = 0;
			for (; b < 3; b++) {              			//join created threads       
				rc = pthread_join(trainer[b], NULL);        
															
				if (rc) {
					printf("\n Error: Thread not joined");
					exit(EXIT_FAILURE);
				}
			}
		
		
			pthread_attr_destroy(&attr);   //  attr not needed anymore


			for (c = 0; c<3; c++)
				pthread_mutex_destroy(&mutext[c]);  //destroy used mutexes


			
			printf("Training process is done.\n");
			
			shmdt(globalcp);
			sem_signal(trainingSeq, 1);  //fin exe
			exit(0);

		}

	}
	else {   // Parent process  --  creates resources


		interviewSeq = semget(KEYSEM3, 1, 0700 | IPC_CREAT); // semaphore for sync between hr and trial department
		semctl(interviewSeq, 0, SETVAL, 0);

		trainingSeq = semget(KEYSEM4, 1, 0700 | IPC_CREAT); // semaphore for sync between trial department and main process
		semctl(trainingSeq, 0, SETVAL, 0);

		shmid = shmget(KEYSHM, (noOfLine - 1) * sizeof(int), 0700 | IPC_CREAT); //creating a shared memory area
		globalcp = (int *)shmat(shmid, 0, 0);    //attaching shared memory address
	
		//no value initiaizing needed as it will start with register thread
	
		
		sleep(2);
	
	
		kill(child[0], 12);    //sends signal to hr department to start register - interview process
			kill(child[1], 12); //signal to trial department

		sem_wait(trainingSeq, noOfLine+1);  //waits for trainers to end training

		
		//removing created semaphores and ahred memory
		semctl(interviewSeq, 0, IPC_RMID, 0);
		semctl(trainingSeq, 0, IPC_RMID, 0);
		shmdt(globalcp);  
		shmctl(shmid, IPC_RMID, 0);
		exit(0);
	}


	return 0;
}
