/*
	Program 2: Wumpus, version 2 (bats, arrows, and all arrays are dynamic)

	Course: CS 211, Fall 2022. Tues 10am lab
  System: Windows using Replit
  Author: Maddy Pooley (mpool3)

  Late Submission Essay:
  The first difference I noticed between my code and the solution code is how we assigned random variables to our   different game elements (user, Wumpus, pits, bats, and arrow). In mine, I first assigned random values to each element. Then I had while loops for each element that assigned a new value each time and looped until the random value did not match another value already assigned. In the solution code, they dynamically allocated an array of 7 random values and assigned them to the elements from there. While I think my approach is simpler and easier to understand personally, this other approach seems more efficient. Another difference I’m seeing is the use of bool in the solution code. I didn’t use bool anywhere in my code, but I can definitely see the benefits of it. This was used when assigning random values to the previously-discussed random value array to determine if a value was already in the array, returning true if so. It was also used when determining whether or not a room was adjacent to another. This implementation in particular is very useful as room adjacency is crucial to many aspects of the program, for example when moving rooms or when shootings arrows. I think this would have been a good addition to my code to simplify it, as I checked for adjacency by comparing different elements to each index of whatever array line was appropriate, like this for example: if (*wumpusPos == roomArray[*userPos][0] || *wumpusPos == roomArray[*userPos][1] || *wumpusPos == roomArray[*userPos][2]) to check if the Wumpus is next to the player. This code, while easy to understand, can bog down the program when used as much as is necessary in this project. I think the boolean approach is very smart. I also noticed, in the arrow shooting function, there’s a lot of dynamic allocation and bools in the solution code. For example, the roomsForArrowTravel array and the wumpusIsAlive and playerIsAlive bools. This, in my opinion, show the possible downsides to using a lot of arrays or other containers. For my arrow shooting function, I take the user-inputted rooms and check for adjacency to determine if the shot is valid. Then, I simply compare element locations to determine who dies, etc. For example: if(*arrowPos == *wumpusPos) to check if the arrow shot the Wumpus. While the solution code does make sense, I think my approach is more user friendly and simpler to implement. An overarching difference I see between the two programs is the use of functions in each. I have functions for each menu option along with an extra helper function to take care of ricocheting arrows. The solution code, on the other hand, implements many different functions and calls them within each other. The solution code’s approach definitely reduces the program’s size and increases efficiency. After I finished my program, I noticed places in which I could have used more helper functions to increase the readability of my code, but in general I think my code is straightforward and easy to understand. Overall, I see a lot of pros and cons of both programs.
*/
#include <stdio.h>
#include <stdlib.h>		// for srand

//--------------------------------------------------------------------------------
//print cave map
void DisplayCave()
{
    printf( "\n       ______18______             \n"
    		"      /      |       \\           \n"
    		"     /      _9__      \\          \n"
    		"    /      /    \\      \\        \n"
    		"   /      /      \\      \\       \n"
    		"  17     8        10     19       \n"
    		"  | \\   / \\      /  \\   / |    \n"
    		"  |  \\ /   \\    /    \\ /  |    \n"
    		"  |   7     1---2     11  |       \n"
    		"  |   |    /     \\    |   |      \n"
    		"  |   6----5     3---12   |       \n"
    		"  |   |     \\   /     |   |      \n"
    		"  |   \\       4      /    |      \n"
    		"  |    \\      |     /     |      \n"
    		"  \\     15---14---13     /       \n"
    		"   \\   /            \\   /       \n"
    		"    \\ /              \\ /        \n"
    		"    16---------------20           \n"
    		"\n");
}

//--------------------------------------------------------------------------------
//print instructions
void DisplayInstructions()
{
    printf( "Hunt the Wumpus:                                             \n"
    		"The Wumpus lives in a completely dark cave of 20 rooms. Each \n"
    		"room has 3 tunnels leading to other rooms.                   \n"
    		"                                                             \n"
    		"Hazards:                                                     \n"
        	"1. Two rooms have bottomless pits in them.  If you go there you fall and die.   \n"
        	"2. Two other rooms have super-bats.  If you go there, the bats grab you and     \n"
        	"   fly you to some random room, which could be troublesome.  Then those bats go \n"
        	"   to a new room different from where they came from and from the other bats.   \n"
    		"3. The Wumpus is not bothered by the pits or bats, as he has sucker feet and    \n"
        	"   is too heavy for bats to lift.  Usually he is asleep.  Two things wake       \n"
        	"    him up: Anytime you shoot an arrow, or you entering his room.  The Wumpus   \n"
        	"    will move into the lowest-numbered adjacent room anytime you shoot an arrow.\n"
        	"    When you move into the Wumpus' room, then he wakes and moves if he is in an \n"
        	"    odd-numbered room, but stays still otherwise.  After that, if he is in your \n"
        	"    room, he snaps your neck and you die!                                       \n"
        	"                                                                                \n"
        	"Moves:                                                                          \n"
        	"On each move you can do the following, where input can be upper or lower-case:  \n"
        	"1. Move into an adjacent room.  To move enter 'M' followed by a space and       \n"
        	"   then a room number.                                                          \n"
        	"2. Shoot your guided arrow through a list of up to three adjacent rooms, which  \n"
        	"   you specify.  Your arrow ends up in the final room.                          \n"
        	"   To shoot enter 'S' followed by the number of rooms (1..3), and then the      \n"
        	"   list of the desired number (up to 3) of adjacent room numbers, separated     \n"
        	"   by spaces. If an arrow can't go a direction because there is no connecting   \n"
        	"   tunnel, it ricochets and moves to the lowest-numbered adjacent room and      \n"
        	"   continues doing this until it has traveled the designated number of rooms.   \n"
        	"   If the arrow hits the Wumpus, you win! If the arrow hits you, you lose. You  \n"
        	"   automatically pick up the arrow if you go through a room with the arrow in   \n"
        	"   it.                                                                          \n"
        	"3. Enter 'R' to reset the person and hazard locations, useful for testing.      \n"
        	"4. Enter 'C' to cheat and display current board positions.                      \n"
        	"5. Enter 'D' to display this set of instructions.                               \n"
        	"6. Enter 'P' to print the maze room layout.                                     \n"
        	"7. Enter 'X' to exit the game.                                                  \n"
        	"                                                                                \n"
        	"Good luck!                                                                      \n"
        	" \n\n");
  }

//function to move rooms
void MoveRooms(int **roomArray, int *userPos, int *moveNum){
  int userMove;
  int i;

  scanf("%d", &userMove); //input room user wants to move to
  
  for (i = 0; i < 3; i++){ //finds array row relating to userPos and checks adjacent rooms for userMove
    if (userMove == roomArray[*userPos][i]){
      *userPos = userMove; //update userPos and moveNum
      (*moveNum)++;
    }
  }
  if (*userPos != userMove){ //if userPos was not updated in for loop, userMove was not adjacent
    printf("Invalid move.  Please retry. \n");
  }
}

//function to use in ShootArrow() function to account for arrow ricochet
void ArrowRicochet(int **roomArray, int *roomNum, int *prevRoom, int *wumpusPos, int *userPos, int *arrowPos){
  printf("Room %d is not adjacent.  Arrow ricochets...\n", *roomNum);
  *arrowPos = roomArray[*prevRoom][0];

  if (*arrowPos == *wumpusPos){
    printf( "Your arrow ricochet killed the Wumpus, you lucky dog!\n"
      "Accidental victory, but still you win!\n");
    printf("\nExiting Program ...\n");
    exit(0);
  }
  else if (*arrowPos == *userPos){
    printf( "You just shot yourself, and are dying.\n"
      "It didn't take long, you're dead.\n");
    printf("\nExiting Program ...\n");
    exit(0);
  }
  else{
    *wumpusPos = roomArray[*wumpusPos][0];
  }
}

//function to shoot arrow takes user inputted rooms, checks if valid shot, checks if kills Wumpus/user
void ShootArrow(int **roomArray, int numRooms, int room1, int room2, int room3, int *arrowPos, int *wumpusPos, int *userPos, int *moveNum){
  //if player has arrow
  if (*arrowPos == -1){
    int i;
    printf("Enter the number of rooms (1..3) into which you want to shoot, followed by the rooms themselves: ");
  
    scanf("%d", &numRooms);

    //if user tries to shoot through too many rooms
    if ((numRooms) > 3){
      printf("Sorry, the max number of rooms is 3.  Setting that value to 3.");
      numRooms = 3;
    }
  
    if (numRooms == 1){
      scanf("%d", &room1);

      //if room1 is adjacent to user room
      if (room1 == roomArray[*userPos][0] || room1 == roomArray[*userPos][1] || room1 == roomArray[*userPos][2]){
        //kills Wumpus if user shot arrow into Wumpus room
        if (room1 == *wumpusPos){
          printf( "Wumpus has just been pierced by your deadly arrow! \n"
              "Congratulations on your victory, you awesome hunter you.\n");
          printf("\nExiting Program ...\n");
          exit(0);
        }
        //Wumpus moves to lowest adjacent room if not shot
        else if (room1 != *wumpusPos){
          *wumpusPos = roomArray[*wumpusPos][0];
          *arrowPos = room1;
        }
        //kills user if user shot into their current room
        if (room1 == *userPos){
          printf( "You just shot yourself.  \n"
              "Maybe Darwin was right.  You're dead.\n");
          printf("\nExiting Program ...\n");
          exit(0);
        }
      }
      //if room isn't adjacent, arrow ricochets, calls ArrowRicochet function
      else if (room1 != roomArray[*userPos][0] && room1 != roomArray[*userPos][1] && room1 != roomArray[*userPos][2]){
        ArrowRicochet(roomArray, &room1, userPos, wumpusPos, userPos, arrowPos);
      }
    }
    else if (numRooms == 2){
      scanf("%d %d", &room1, &room2);

      //if rooms are adjacent to each other
      if ((room1 == roomArray[*userPos][0] || room1 == roomArray[*userPos][1] || room1 == roomArray[*userPos][2]) &&
          (room2 == roomArray[room1][0] || room2 == roomArray[room1][1] || room2 == roomArray[room1][2])){
        //kills Wumpus if user shot arrow into Wumpus room
        if (room1 == *wumpusPos || room2 == *wumpusPos){
          printf( "Wumpus has just been pierced by your deadly arrow! \n"
              "Congratulations on your victory, you awesome hunter you.\n");
          printf("\nExiting Program ...\n");
          exit(0);
        }
        //Wumpus moves to lowest adjacent room if not shot
        else if (room1 != *wumpusPos && room2 != *wumpusPos){
          *wumpusPos = roomArray[*wumpusPos][0];
          *arrowPos = room2;
        }
        //kills user if user shot into their current room
        if (room1 == *userPos || room2 == *userPos){
          printf( "You just shot yourself.  \n"
              "Maybe Darwin was right.  You're dead.\n");
          printf("\nExiting Program ...\n");
          exit(0);
        }
      }
      //if room isn't adjacent, arrow ricochets, calls ArrowRicochet function
      else if (room1 != roomArray[*userPos][0] && room1 != roomArray[*userPos][1] && room1 != roomArray[*userPos][2]){
        ArrowRicochet(roomArray, &room1, userPos, wumpusPos, userPos, arrowPos);
      }
      else if (room2 != roomArray[room1][0] && room2 != roomArray[room1][1] && room2 != roomArray[room1][2]){
        ArrowRicochet(roomArray, &room2, &room1, wumpusPos, userPos, arrowPos);
      }
    }
    else if (numRooms == 3){
      scanf("%d %d %d", &room1, &room2, &room3);

      //if rooms are adjacent to each other
      if ((room1 == roomArray[*userPos][0] || room1 == roomArray[*userPos][1] || room1 == roomArray[*userPos][2]) &&
          (room2 == roomArray[room1][0] || room2 == roomArray[room1][1] || room2 == roomArray[room1][2]) &&
          (room3 == roomArray[room2][0] || room3 == roomArray[room2][1] || room3 == roomArray[room2][2])){
        //kills Wumpus if user shot arrow into Wumpus room
        if (room1 == *wumpusPos || room2 == *wumpusPos || room3 == *wumpusPos){
          printf( "Wumpus has just been pierced by your deadly arrow! \n"
              "Congratulations on your victory, you awesome hunter you.\n");
          printf("\nExiting Program ...\n");
          exit(0);
        }
        //kills Wumpus if user shot arrow into Wumpus room
        else if (room1 != *wumpusPos && room2 != *wumpusPos && room3 != *wumpusPos){
          *wumpusPos = roomArray[*wumpusPos][0];
          *arrowPos = room3;
        }
        //kills user if user shot into their current room
        if (room1 == *userPos || room2 == *userPos || room3 == *userPos){
          printf( "You just shot yourself.  \n"
              "Maybe Darwin was right.  You're dead.\n");
          printf("\nExiting Program ...\n");
          exit(0);
        }
      }
      //if room isn't adjacent, arrow ricochets, calls ArrowRicochet function
      else if (room1 != roomArray[*userPos][0] && room1 != roomArray[*userPos][1] && room1 != roomArray[*userPos][2]){
        ArrowRicochet(roomArray, &room1, userPos, wumpusPos, userPos, arrowPos);
      }
      else if (room2 != roomArray[room1][0] && room2 != roomArray[room1][1] && room2 != roomArray[room1][2]){
        ArrowRicochet(roomArray, &room2, &room1, wumpusPos, userPos, arrowPos);
      }
      else if (room3 != roomArray[room2][0] && room3 != roomArray[room2][1] && room3 != roomArray[room2][2]){
        ArrowRicochet(roomArray, &room3, &room2, wumpusPos, userPos, arrowPos);
      }
    }
  }
  //doesn't let player shoot arrow if they don't have it
  else{
    printf("Sorry, you can't shoot an arrow you don't have.  Go find it.\n");
  }
  (*moveNum)++;
}

//function for user to set room numbers
void ResetLocation(int *userPos, int *wumpusPos, int *pit1Pos, int *pit2Pos, int *bat1Pos, int *bat2Pos, int *arrowPos){
  printf("Enter the room locations (1..20) for player, wumpus, pit1, pit2, bats1, bats2, and arrow: \n\n");
  scanf("%d %d %d %d %d %d %d", userPos, wumpusPos, pit1Pos, pit2Pos, bat1Pos, bat2Pos, arrowPos);
}

//function to display current room assignments
void Cheat(int userPos, int wumpusPos, int pit1Pos, int pit2Pos, int bat1Pos, int bat2Pos, int arrowPos){
  printf( "Cheating! Game elements are in the following rooms: \n"
    		"Player Wumpus Pit1 Pit2 Bats1 Bats2 Arrow  \n"
    		"%4d %5d %6d %5d %5d %5d %5d \n\n", 
			userPos,
			wumpusPos,
			pit1Pos,
			pit2Pos,
			bat1Pos,
			bat2Pos,
			arrowPos
		);
}

//function to check if rooms adjacent to userPos have the Wumpus or a pit
void CheckObstacles(int **roomArray, int *userPos, int *wumpusPos, int *pit1Pos, int *pit2Pos, int *bat1Pos, int *bat2Pos, int *arrowPos){
  int i;
  int m;
  int n;

  if (*wumpusPos == *userPos && *wumpusPos % 2 != 0){ //if user enters odd Wumpus room
    *wumpusPos = roomArray[*wumpusPos][0];
    printf( "You hear a slithering sound, as the Wumpus slips away. \nWhew, that was close! \n");
  }
  if (*wumpusPos == *userPos && *wumpusPos % 2 == 0){ //if user enters even Wumpus room
    printf(	"You briefly feel a slimy tentacled arm as your neck is snapped. \nIt is over.\n");
    printf("\nExiting Program ...\n");
    exit(0);
  }
  if (*pit1Pos == *userPos || *pit2Pos == *userPos){ //if user enters a pit room
    printf("Aaaaaaaaahhhhhh....   \n");
	  printf("    You fall into a pit and die. \n");
    printf("\nExiting Program ...\n");
    exit(0);
  }
  if (*bat1Pos == *userPos){
    printf("Woah... you're flying! \n");
    *userPos = rand() % 20 + 1;
    printf("You've just been transported by bats to room %d.\n", *userPos);

    //randomize new bat room until it's not the same as before, the other bats, or the user
    int origBat1Pos = *bat1Pos;
    *bat1Pos = rand() % 20 + 1;
    while (*bat1Pos == origBat1Pos || *bat1Pos == *userPos || *bat1Pos == *bat2Pos){
      *bat1Pos = rand() % 20 + 1;
    }
  }
  if (*bat2Pos == *userPos){
    printf("Woah... you're flying! \n");
    *userPos = rand() % 20 + 1;
    printf("You've just been transported by bats to room %d.\n", *userPos);

    //randomize new bat room until it's not the same as before, the other bats, or the user
    int origBat2Pos = *bat2Pos;
    *bat2Pos = rand() % 20 + 1;
    while (*bat2Pos == origBat2Pos || *bat2Pos == *userPos || *bat2Pos == *bat1Pos){
      *bat2Pos = rand() % 20 + 1;
    }
  }
  if (*arrowPos == *userPos){
    *arrowPos = -1;
    printf("Congratulations, you found the arrow and can once again shoot.\n");
  }

  printf("You are in room %d. ", *userPos);
  
  for (i = 0; i < 3; i++){ //finds array row relating to userPos and checks adjacent rooms for Wumpus
    if (*wumpusPos == roomArray[*userPos][i]){
      printf("You smell a stench. ");
    }
  }
  for (m = 0; m < 3; m++){ //finds array row relating to userPos and checks adjacent rooms for pits
    if (*pit1Pos == roomArray[*userPos][m] || *pit2Pos == roomArray[*userPos][m]){
      printf("You feel a draft. ");
    }
  }
  for (n = 0; n < 3; n++){ //finds array row relating to userPos and checks adjacent room for bats
    if (*bat1Pos == roomArray[*userPos][n] || *bat2Pos == roomArray[*userPos][n]){
      printf("You hear rustling of bat wings. ");
    }
  }
  printf("\n");
}

//--------------------------------------------------------------------------------

int main(void){
  int wumpusPos;
  int userPos;
  int pit1Pos;
  int pit2Pos;
  int bat1Pos;
  int bat2Pos;
  int arrowPos;
  int userMove;
  char menuChoice;
  int count = 0;
  int moveNum = 1;

  int numRooms;
  int room1;
  int room2;
  int room3;

  //randomize pit, Wumpus, and user rooms
  srand(1);
  userPos = rand() % 20 + 1;
  wumpusPos = rand() % 20 + 1;
  pit1Pos = rand() % 20 + 1;
  pit2Pos = rand() % 20 + 1;
  bat1Pos = rand() % 20 + 1;
  bat2Pos = rand() % 20 + 1;
  arrowPos = rand() % 20 + 1;

  //if user, Wumpus, pits, bats, or arrow are assigned the same room, randomize again
  while (wumpusPos == userPos){
    wumpusPos = rand() % 20 + 1;
  }
  while (pit1Pos == userPos || pit1Pos == wumpusPos){
    pit1Pos = rand() % 20 + 1;
  }
  while (pit2Pos == userPos || pit2Pos == wumpusPos || pit2Pos == pit1Pos){
    pit2Pos = rand() % 20 + 1;
  }
  while (bat1Pos == userPos || bat1Pos == wumpusPos || bat1Pos == pit1Pos || bat1Pos == pit2Pos){
    bat1Pos = rand() % 20 + 1;
  }
  while (bat2Pos == userPos || bat2Pos == wumpusPos || bat2Pos == pit1Pos || bat2Pos == pit2Pos || bat2Pos == bat1Pos){
    bat2Pos = rand() % 20 + 1;
  }
  while (arrowPos == userPos || arrowPos == wumpusPos || arrowPos == pit1Pos || arrowPos == pit2Pos || arrowPos == bat1Pos || arrowPos == bat2Pos){
    arrowPos = rand() % 20 + 1;
  }

  //dynamically allocate memory for the cave array
  int **roomArray = (int**)malloc(21 * sizeof(int*));

  int i;
  for (i = 0; i < 21; i++){
    roomArray[i] = (int*)malloc(3 * sizeof(int));
  }

  //set adjacent rooms, leaving index 0 empty so the index can correspond with the room number
  roomArray[0][0] = 0;
  roomArray[0][1] = 0;
  roomArray[0][2] = 0;
  
  roomArray[1][0] = 2;
  roomArray[1][1] = 5;
  roomArray[1][2] = 8;
  
  roomArray[2][0] = 1;
  roomArray[2][1] = 3;
  roomArray[2][2] = 10;
  
  roomArray[3][0] = 2;
  roomArray[3][1] = 4;
  roomArray[3][2] = 12;
  
  roomArray[4][0] = 3;
  roomArray[4][1] = 5;
  roomArray[4][2] = 14;
  
  roomArray[5][0] = 1;
  roomArray[5][1] = 4;
  roomArray[5][2] = 6;
  
  roomArray[6][0] = 5;
  roomArray[6][1] = 7;
  roomArray[6][2] = 15;
  
  roomArray[7][0] = 6;
  roomArray[7][1] = 8;
  roomArray[7][2] = 17;
  
  roomArray[8][0] = 1;
  roomArray[8][1] = 7;
  roomArray[8][2] = 9;
  
  roomArray[9][0] = 8;
  roomArray[9][1] = 10;
  roomArray[9][2] = 18;
  
  roomArray[10][0] = 2;
  roomArray[10][1] = 9;
  roomArray[10][2] = 11;
  
  roomArray[11][0] = 10;
  roomArray[11][1] = 12;
  roomArray[11][2] = 19;

  roomArray[12][0] = 3;
  roomArray[12][1] = 11;
  roomArray[12][2] = 13;

  roomArray[13][0] = 12;
  roomArray[13][1] = 14;
  roomArray[13][2] = 20;

  roomArray[14][0] = 4;
  roomArray[14][1] = 13;
  roomArray[14][2] = 15;

  roomArray[15][0] = 6;
  roomArray[15][1] = 14;
  roomArray[15][2] = 16;

  roomArray[16][0] = 15;
  roomArray[16][1] = 17;
  roomArray[16][2] = 20;

  roomArray[17][0] = 7;
  roomArray[17][1] = 16;
  roomArray[17][2] = 18;

  roomArray[18][0] = 9;
  roomArray[18][1] = 17;
  roomArray[18][2] = 19;

  roomArray[19][0] = 11;
  roomArray[19][1] = 18;
  roomArray[19][2] = 20;

  roomArray[20][0] = 13;
  roomArray[20][1] = 16;
  roomArray[20][2] = 19;

  //check adjacent rooms for hazards
  CheckObstacles(roomArray, &userPos, &wumpusPos, &pit1Pos, &pit2Pos, &bat1Pos, &bat2Pos, &arrowPos);

  //continue taking user inputs until progam is exited
  do {
    printf("\n%d. Enter your move (or 'D' for directions): ", moveNum);
    scanf(" %c", &menuChoice);

    //call correct function according to user menu option choice
    if (menuChoice == 'M' || menuChoice == 'm'){
      MoveRooms(roomArray, &userPos, &moveNum);
    }
    else if (menuChoice == 'S' || menuChoice == 's'){
      ShootArrow(roomArray, numRooms, room1, room2, room3, &arrowPos, &wumpusPos, &userPos, &moveNum);
    }
    else if (menuChoice == 'R' || menuChoice == 'r'){
      ResetLocation(&userPos, &wumpusPos, &pit1Pos, &pit2Pos, &bat1Pos, &bat2Pos, &arrowPos);
    }
    else if (menuChoice == 'C' || menuChoice == 'c'){
      Cheat(userPos, wumpusPos, pit1Pos, pit2Pos, bat1Pos, bat2Pos, arrowPos);
    }
    else if (menuChoice == 'D' || menuChoice == 'd'){
      DisplayCave();
      DisplayInstructions();
    }
    else if (menuChoice == 'P' || menuChoice == 'p'){
      DisplayCave();
    }
    else if (menuChoice == 'X' || menuChoice == 'x'){
      printf("\nExiting Program ...\n");
      exit(0);
    }
    else {
      printf("Invalid move.  Please retry. \n");
    }

    //check adjacent rooms for hazards after every turn
    CheckObstacles(roomArray, &userPos, &wumpusPos, &pit1Pos, &pit2Pos, &bat1Pos, &bat2Pos, &arrowPos);
  } while (count == 0);
  
  return 0;
}