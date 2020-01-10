/* Boids using ASCII graphics
	-original NCurses code from "Game Programming in C with the Ncurses Library"
	 https://www.viget.com/articles/game-programming-in-c-with-the-ncurses-library/
	 and from "NCURSES Programming HOWTO"
	 http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/
	-Boids algorithms from "Boids Pseudocode:
	 http://www.kfish.org/boids/pseudocode.html
*/ 


#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<omp.h>
#include<sys/timeb.h>
#ifndef NOGRAPHICS
#include<unistd.h>
#include<ncurses.h>
#endif

      //used for displaying time
struct timeb startB, stopB;

void rule1(int boidNum);
void rule2(int boidNum);
void rule3(int boidNum);


#define DELAY 50000

	// population size, number of boids
#define POPSIZE 50
	// maximum screen size, both height and width
#define SCREENSIZE 100

	// boid location (x,y,z) and velocity (vx,vy,vz) in boidArray[][]
#define BX 0
#define BY 1
#define BZ 2
#define VX 3
#define VY 4
#define VZ 5

#ifndef NOGRAPHICS
	// maximum screen dimensions
int max_y = 0, max_x = 0;
#endif

	// location and velocity of boids
float boidArray[POPSIZE][6];

	// change in velocity is stored for each boid (x,y,z)
	// this is applied after all changes are calculated for each boid
float boidUpdate[POPSIZE][3];

void initBoids() {
int i;
	// calculate initial random locations for each boid, scaled based on the screen size
   for(i=0; i<POPSIZE; i++) {
      boidArray[i][BX] = (float) (random() % SCREENSIZE); 
      boidArray[i][BY] = (float) (random() % SCREENSIZE); 
      boidArray[i][BZ] = (float) (random() % SCREENSIZE); 
      boidArray[i][VX] = 0.0; 
      boidArray[i][VY] = 0.0; 
      boidArray[i][VZ] = 0.0; 
   }
}

#ifndef NOGRAPHICS
int drawBoids() {
int c, i;
float multx, multy;

	// update screen maximum size
   getmaxyx(stdscr, max_y, max_x);

	// used to scale position of boids based on screen size
   multx = (float)max_x / SCREENSIZE;
   multy = (float)max_y / SCREENSIZE;

   clear();

	// display boids
   for (i=0; i<POPSIZE; i++) {
      mvprintw((int)(boidArray[i][BX]*multy), (int)(boidArray[i][BY]*multx), "o");
   }

   refresh();

   usleep(DELAY);

	// read keyboard and exit if 'q' pressed
   c = getch();
   if (c == 'q') return(1);
   else return(0);
}
#endif


	// move the flock between two points
	// you can optimize this funciton if you wish, or you can replace it and move it's
	//    functionality into another function
void moveFlock() {
      int i;
      static int count = 0;
      static int sign = 1;
      float px, py, pz;	// (x,y,z) point which the flock moves towards


	// pull flock towards two points as the program runs
	// every 200 iterations change point that flock is pulled towards
   if (count % 2000 == 0) {
      sign = sign * -1;
   } 
   if (sign == 1) {
	// move flock towards position (40,40,40)
      px = 40.0;
      py = 40.0;
      pz = 40.0;
   } else {
	// move flock towards position (60,60,60)
      px = 60.0;
      py = 60.0;
      pz = 60.0;
   }
	// add offset (px,py,pz) to each boid location (x,y,z) in order to pull it
	// towards the current target point
   for(i=0; i<POPSIZE; i++) {
      boidUpdate[i][BX] += (px - boidArray[i][BX])/200.0;
      boidUpdate[i][BY] += (py - boidArray[i][BY])/200.0;
      boidUpdate[i][BZ] += (pz - boidArray[i][BZ])/200.0;
   }
   count++;

}

void moveBoids() {
   int i;
    float v1[3];
    float v2[3];
    float v3[3];


            /*** Your code goes here ***/
            /*** add the code to implement the three boids rules ***/
            /*** do not change any other code except to add timing to the main() routine ***/
            /*** do not replace the data structures defined in this program ***/
            /*** omp code should be used here ***/
            /*** you can call other functions from this location ***/

     //loop each boid through every rule to update its position
#     pragma omp parallel for 
   for(i=0; i<POPSIZE; i++){
         boidUpdate[i][0] = 0;
         boidUpdate[i][1] = 0;
         boidUpdate[i][2] = 0;
      //    boidArray[i][3] = 0;
      //    boidArray[i][4] = 0;
      //    boidArray[i][5] = 0;
      rule1(i);
      v1[0] = boidUpdate[i][0];
      v1[1] = boidUpdate[i][1];
      v1[2] = boidUpdate[i][2];
      boidUpdate[i][0] = 0;
      boidUpdate[i][1] = 0;
      boidUpdate[i][2] = 0;

      rule2(i);

      v2[0] = boidUpdate[i][0];
      v2[1] = boidUpdate[i][1];
      v2[2] = boidUpdate[i][2];
      boidUpdate[i][0] = 0;
      boidUpdate[i][1] = 0;
      boidUpdate[i][2] = 0;

      rule3(i);
      

      
      boidUpdate[i][0] += v1[0] + v2[0];
      boidUpdate[i][1] += v1[1] + v2[1];
      boidUpdate[i][2] += v1[2] + v2[2];

   }
   
   moveFlock();//rule 4

	// move boids by calculating updated velocity and new position
	// you can optimize this code if you wish and you can add omp directives here
   for (i=0; i<POPSIZE; i++) {
	// update velocity for each boid using value stored in boidUpdate[][]
      boidArray[i][VX] += boidUpdate[i][BX];
      boidArray[i][VY] += boidUpdate[i][BY];
      boidArray[i][VZ] += boidUpdate[i][BZ];
	// update position for each boid using newly updated velocity
      boidArray[i][BX] += boidArray[i][VX];
      boidArray[i][BY] += boidArray[i][VY];
      boidArray[i][BZ] += boidArray[i][VZ];
   }

}


int main(int argc, char *argv[]) {
      int i, count;
      int threadCount = 8;

#ifndef NOGRAPHICS
	// initialize curses
   initscr();
   noecho();
   cbreak();
   timeout(0);
   curs_set(FALSE);
     // Global var `stdscr` is created by the call to `initscr()`
   getmaxyx(stdscr, max_y, max_x);
#endif

	// place boids in initial position
   initBoids();

	// draw and move boids using ncurses 
	// do not calculate timing in this loop, ncurses will reduce performance
#ifndef NOGRAPHICS
   while(1) {
      if (drawBoids() == 1) break;
      moveBoids();
   }
#endif

	// calculate movement of boids but do not use ncurses to draw
	// this is used for timing the parallel implementation of the program
#ifdef NOGRAPHICS
   count = 1000;
	// read number of iterations from the command line
   if (argc == 2){
      sscanf(argv[1], "%d", &count);
   }
   printf("Number of iterations %d\n", count);

	/*** Start timing here ***/
      /*** timer started ***/
   ftime(&startB);
	/*** omp code can be used here ***/
#     pragma omp parallel for 
      for(i=0; i<count; i++) {
            moveBoids();
      }
	/*** End timing here ***/
   ftime(&stopB);
      /*** timer has been ended ***/
      //double timeTaken = (double)(timeEndt - timeStartt) / CLOCKS_PER_SEC;
   double elapse = (double)(1000.0*(stopB.time-startB.time)+(stopB.millitm-startB.millitm));
	/*** print timing results ***/
   printf("Execution time(in seconds): %lf\n", elapse/1000);

#endif

#ifndef NOGRAPHICS
	// shut down curses
   endwin();
#endif

}


void rule1(int boidNum){
      
      float centerPt[3];
      int i = 0;

      centerPt[0] = 0.0;//x
      centerPt[1] = 0.0;//y
      centerPt[2] = 0.0;//z

          //calc center for a boid by finding center of all other boids that don't include itself
#     pragma omp parallel for 
      for(i=0; i<POPSIZE; i++){
            if(i != boidNum){
                  centerPt[0] = centerPt[0] + boidArray[i][BX];
                  centerPt[1] = centerPt[1] + boidArray[i][BY];
                  centerPt[2] = centerPt[2] + boidArray[i][BZ];

            }
      }
      centerPt[0] /= (POPSIZE-1);
      centerPt[1] /= (POPSIZE-1);
      centerPt[2] /= (POPSIZE-1);
      /* centerPt[0] = centerPt[0] - boidArray[boidNum][BX];// centerPt[1] = centerPt[1] - boidArray[boidNum][BY];// centerPt[2] = centerPt[2] - boidArray[boidNum][BZ];
      // centerPt[0] = centerPt[0] / 200.0; // centerPt[1] = centerPt[1] / 200.0; // centerPt[2] = centerPt[2] / 200.0;*/
      boidUpdate[boidNum][BX] += (centerPt[0] - boidArray[boidNum][BX])/200.0;
      boidUpdate[boidNum][BY] += (centerPt[1] - boidArray[boidNum][BY])/200.0;
      boidUpdate[boidNum][BZ] += (centerPt[2] - boidArray[boidNum][BZ])/200.0;
      return;
}

void rule2(int boidNum){

     //float* vector;
      float a = 0.0;
      float b = 0.0;
      float c = 0.0;
      float z = 0.0;
      // vector = malloc(sizeof(float) * 3);
      // vector[0] = 0.0;
      // vector[1] = 0.0;
      // vector[2] = 0.0;
      float rx = 0.0;
      float ry = 0.0;
      float rz = 0.0;

      int i;
#     pragma omp parallel for 
      for(i=0; i<POPSIZE; i++){
            if(i != boidNum){

                  //a*a + b*b = c*c
                  a = (boidArray[boidNum][BX] - boidArray[i][BX]); //result is a^2
                  b = (boidArray[boidNum][BY] - boidArray[i][BY]); //result is b^2
                  z = (boidArray[boidNum][BZ] - boidArray[i][BZ]); //result z^2
                  c = pow(a, 2) + pow(b, 2) + pow(z, 2); //result is c^2
                  c = sqrt(c); //result is the distance between boidNum and i 



                  if((abs(c) > 2 && abs(c) < 5) ){//|| (c <= -2 && c >= -5)){//check position relative use pythagorian theorem
                        rx = rx - (boidArray[i][BX] - boidArray[boidNum][BX]);
                        ry = ry - (boidArray[i][BY] - boidArray[boidNum][BY]);
                        rz = rz - (boidArray[i][BZ] - boidArray[boidNum][BZ]);
                        
                  }
            }
      }
      boidUpdate[boidNum][BX] += rx;
      boidUpdate[boidNum][BY] += ry;
      boidUpdate[boidNum][BZ] += rz;

      return;
}

void rule3(int boidNum){

      float pVelo[3];
      int i;
      pVelo[0] = 0.0;//x
      pVelo[1] = 0.0;//y
      pVelo[2] = 0.0;//z
      
#     pragma omp parallel for 
      for(i=0; i<POPSIZE; i++){
            if(i != boidNum){
                  pVelo[0] += boidArray[i][3];
                  pVelo[1] += boidArray[i][4];
                  pVelo[2] += boidArray[i][5];
            }
      }

      pVelo[0] = pVelo[0] / (POPSIZE-1);
      pVelo[1] = pVelo[1] / (POPSIZE-1);
      pVelo[2] = pVelo[2] / (POPSIZE-1);


      boidUpdate[boidNum][0] +=   (pVelo[0] - boidArray[boidNum][3]) / 8;
      boidUpdate[boidNum][1] +=   (pVelo[1] - boidArray[boidNum][4]) / 8;
      boidUpdate[boidNum][2] +=   (pVelo[2] - boidArray[boidNum][5]) / 8;


      return;
}