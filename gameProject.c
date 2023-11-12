#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

FILE*fileOutput;

struct Soldier {
    int team;
    int health;
    int strength;
    bool isAlive;
    struct Soldier *next;
};

typedef struct Stack {
    struct Soldier *top;
} Stack;

Stack *casualty = NULL;

struct Soldier soldiers[100];
int totalSoldiers = 0;
int teamOrder = 1;

void initializeStack();
void push(Stack *stack, struct Soldier *soldier);
struct Soldier *pop(Stack *stack);
struct Soldier *top(Stack *stack);

void addSoldiers(int team, int health, int strength);
void callReinforcement(int team);
void criticalShot();
void fight();
int CalcDamage(struct Soldier aggressor, struct Soldier recipient);

void takenInput(char input[]);


int main() {
    
    char input[100];
    FILE *fileInput = fopen("input.txt", "r");
    fileOutput = fopen("output.txt", "w");

    if (fileInput == NULL || fileOutput == NULL) {
        perror("File could not open!");
        return 1;
    }

    initializeStack();

    while (fgets(input, sizeof(input), fileInput)) {
        takenInput(input);
    }

    fclose(fileInput);

    while (casualty->top != NULL) {
        struct Soldier *temp = pop(casualty);
        free(temp);
    }
     int aliveTeam1 = 0, aliveTeam2 = 0;

    for (int i = 0; i < totalSoldiers; i++) {
        if (soldiers[i].isAlive) {
            if (soldiers[i].team == 1) {
                aliveTeam1++;
            } else if (soldiers[i].team == 2) {
                aliveTeam2++;
            }
        }
    }

   if (aliveTeam1 > aliveTeam2) {
        printf("Team 1 wins!\n");
        fprintf(fileOutput, "Team 1 wins!\n");
    } else if (aliveTeam2 > aliveTeam1) {
        printf("Team 2 wins!\n");
        fprintf(fileOutput, "Team 2 wins!\n");
    } else {
        printf("It's a draw!\n");
        fprintf(fileOutput, "It's a draw!\n");
    }

    fclose(fileOutput);

    while (casualty->top != NULL) {
        struct Soldier *temp = pop(casualty);
        free(temp);
    }

    return 0;
}

void initializeStack() {
    casualty = malloc(sizeof(Stack));
    casualty->top = NULL;
}

void push(Stack *stack, struct Soldier *soldier) {
    soldier->next = stack->top;
    stack->top = soldier;
}

struct Soldier *pop(Stack *stack) {
    if (stack->top == NULL) {
        return NULL;
    }

    struct Soldier *popped = stack->top;
    stack->top = stack->top->next;
    popped->next = NULL;
    return popped;
}

struct Soldier *top(Stack *stack) {
    return stack->top;
}

void addSoldiers(int team, int health, int strength) {
    printf("Soldiers is adding: Side %d - H:%d, S:%d\n", team, health, strength);
    fprintf(fileOutput, "Soldiers is adding: Side %d - H:%d, S:%d\n", team, health, strength);
    soldiers[totalSoldiers].team = team;
    soldiers[totalSoldiers].health = health;
    soldiers[totalSoldiers].strength = strength;
    soldiers[totalSoldiers].isAlive = true;
    totalSoldiers++;
}

void callReinforcement(int team) {
    int health = rand() % 101;
    int strength = rand() % 1000;
    printf("Called reinforcements to side %d\n", team);
    printf("S-H:%d S:%d\n", health, strength);
    fprintf(fileOutput, "Called reinforcements to side %d\n", team);
    fprintf(fileOutput, "S-H:%d S:%d\n", health, strength);


    addSoldiers(team,health,strength);
}

void fight() {
    int i = 0;
    for (i = 0; i < totalSoldiers; i++) {
        if (soldiers[i].team == teamOrder && soldiers[i].isAlive) {
            int recipient = rand() % totalSoldiers;
            while (soldiers[i].team == soldiers[recipient].team || !soldiers[recipient].isAlive) {
                recipient = rand() % totalSoldiers;
            }

            int damage = CalcDamage(soldiers[i], soldiers[recipient]);
            soldiers[recipient].health -= damage;

            if (soldiers[recipient].health <= 0) {
                soldiers[recipient].isAlive = false;
                printf("On team %d has a casualty\n", soldiers[recipient].team, recipient);
                fprintf(fileOutput, "On team %d has a casualty\n", soldiers[recipient].team, recipient);


                struct Soldier *newNode = malloc(sizeof(struct Soldier));
                newNode->team = soldiers[recipient].team;
                newNode->next = NULL;

                push(casualty, newNode);
            }
        }
    }
    teamOrder = (teamOrder == 1) ? 2 : 1;
}

void criticalShot() {
    if (totalSoldiers == 0) {
        printf("No soldiers on the game!\n");
        fprintf(fileOutput, "No soldiers on the game!\n");
        return;
    }

    int recipient = -1;
    for (int i = 0; i < totalSoldiers; i++) {
        if (soldiers[i].team != teamOrder && soldiers[i].isAlive) {
            recipient = i;
            break;
        }
    }
    if (recipient != -1) {
        soldiers[recipient].isAlive = false;
        printf("Critical shot:\n");
        printf("On team %d has a casualty\n", soldiers[recipient].team);
        fprintf(fileOutput, "Critical shot:\n");
        fprintf(fileOutput, "On team %d has a casualty\n", soldiers[recipient].team);

         struct Soldier *newNode = malloc(sizeof(struct Soldier));
        newNode->team = soldiers[recipient].team;
        newNode->next = NULL;

        push(casualty, newNode);
    } else {
        fprintf(fileOutput, "Invalid recipient");
        printf("Invalid recipient");
    }

    teamOrder = (teamOrder == 1) ? 2 : 1;
}


int CalcDamage(struct Soldier aggressor, struct Soldier recipient) {
    int damage = (aggressor.strength - recipient.strength) * 0.05 + 50;
    return damage;
}

void takenInput(char input[]) {
    char letter;
    int team, health, strength;

    sscanf(input, "%c", &letter);

    if (letter == 'A') {
        sscanf(input, "%c %d %d,%d", &letter, &team, &health, &strength);
        addSoldiers(team, health, strength);
    } else if (letter == 'R') {
        sscanf(input, "%c %d", &letter, &team);
        callReinforcement(team);
    } else if (letter == 'F') {
        fight();
    } else if (letter == 'C') {
        criticalShot();
    } else {
        printf("Invalid entry!");
    }

    
}
