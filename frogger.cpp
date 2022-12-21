#include <iostream>

#define SIZE 9
#define TRUE 1
#define FALSE 0
#define XSTART 8
#define YSTART 4
#define RIGHT 1
#define LEFT 0
#define STATIC 2

using namespace std;

enum tile_type {LILYPAD, BANK, WATER, TURTLE, LOG};

struct board_tile{
    enum tile_type type;
    int occupied;
};

struct bug{
    int move;
    int state;
    int done;
};

void clear_row(struct board_tile board[SIZE][SIZE], int x);
void move_frog(struct board_tile board[SIZE][SIZE], char command, int* x_frog, int* y_frog);
void delete_log(struct board_tile board[SIZE][SIZE],int x, int y);
void print_board(struct board_tile board[SIZE][SIZE], struct bug bug_board[SIZE][SIZE]);
void init_board(struct board_tile board[SIZE][SIZE],struct bug bug_board[SIZE][SIZE]);
void add_bug(struct board_tile board[SIZE][SIZE], struct bug bug_board[SIZE][SIZE], int x_bug, int y_bug);
void move_bug(struct board_tile board[SIZE][SIZE], struct bug bug_board[SIZE][SIZE]);
char type_to_char(enum tile_type);

int main(){
    cout<<"Welcome to Frogger Game 1.0.0"<<endl;

    struct board_tile game_board[SIZE][SIZE];
    struct bug bug_on_board[SIZE][SIZE];
    init_board(game_board, bug_on_board);
    int lives=3;
    int x_frog = XSTART;
    int y_frog = YSTART;
    int num_turtle;
    int turtle_x, turtle_y;

    cout<<"How Many Turtles? ";
    cin>>num_turtle;
    if(num_turtle>0) cout<<"Enter Pairs:\n";

    for(int i=0; i<num_turtle;i++){
        cin>>turtle_x>>turtle_y;
        if((turtle_x>0) && (turtle_x<8) && (turtle_y>=0) && (turtle_y<=8) && (!game_board[turtle_x][turtle_y].occupied)){
            game_board[turtle_x][turtle_y].type = TURTLE;
        }
    }

    print_board(game_board, bug_on_board);
    while(!cin.eof()){
        bool isvalid=true;
        char command;
        cout<<"Enter Command: ";
        cin>>command;


        //adding log
        if(command=='l'){
            int log_row,log_start_col,log_end_col;
            cin>>log_row>>log_start_col>>log_end_col;
            //adding log
            //check turtle
            for(int i=0;i<9;i++){
                if(game_board[log_row][i].type == TURTLE){
                    isvalid = false;
                } else isvalid = true;
            }

            //check log size
            for(int i=log_start_col;i<=log_end_col;i++){
                if((log_row>0) && (log_row<8) && (log_start_col>=0) && (log_end_col>=0) && (log_start_col<=8) && (log_end_col<=8)  && (log_start_col<log_end_col) && (game_board[log_row][i].type != TURTLE) ){
                    isvalid = true;
                } else {
                    isvalid = false;
                    break;
                }
            }
            //adding log
            if (isvalid && !cin.eof()){
                for(int i=log_start_col;i<=log_end_col;i++){
                    game_board[log_row][i].type = LOG;
                }
                print_board(game_board, bug_on_board);
            } if(!isvalid && !cin.eof()) {
                print_board(game_board, bug_on_board);
            }

        }

        //clear row
        else if (command == 'c'){
            int x;
            cin >> x;
            if((x>0) && (x<8)){
                clear_row(game_board,x);
                cout << endl;
                print_board(game_board, bug_on_board);
            }
        }

        //del log
        else if (command == 'r'){
            int x,y;
            cin >> x >> y;
            if((x>0) && (x<0)){
                delete_log(game_board,x,y);
                cout << endl;
                print_board(game_board, bug_on_board);
            }
        }

        //adding bug
        else if (command == 'b'){
            int x_bug, y_bug;
            cin >> x_bug >> y_bug;
            if((x_bug > 0) && (x_bug < 8)) {
                    add_bug(game_board, bug_on_board, x_bug, y_bug);
            }
            cout << endl;
            print_board(game_board, bug_on_board);
        }

        //frog movement
        else if (command == 'w' || command == 'a' || command == 's' || command == 'd'){
            move_bug(game_board,bug_on_board);
            move_frog(game_board,command,&x_frog,&y_frog);
            //win condition
            if(game_board[x_frog][y_frog].type == LILYPAD){
                print_board(game_board, bug_on_board);
                cout << "Wahoo!! You Won" << endl;
                break;
            }
            //check if the frog touches WATER or stepped on a bug
            else if(game_board[x_frog][y_frog].type == WATER|| (bug_on_board[x_frog][y_frog].state)){
                lives--;
                if(!lives){

                    print_board(game_board, bug_on_board);
                    cout << " !!GAME OVER!! " << endl;
                    break;
                }else{
                    game_board[x_frog][y_frog].occupied = FALSE;
                    cout << "LIVES LEFT: " << lives;
                    x_frog = XSTART;
                    y_frog = YSTART;
                    game_board[x_frog][y_frog].occupied = TRUE;
                }
            }
            cout << endl;
            print_board(game_board, bug_on_board);

        }

        //ctrl+c=end
        else if (!cin.eof()){
            cout << "command not exist" << endl;
        }

    }

    cout << "Thank you for playing Frogger Game" << endl;
    return 0;
}

void move_frog(struct board_tile board[SIZE][SIZE], char command, int* x_frog, int* y_frog){
    int temp_x = *x_frog;
    int temp_y = *y_frog;
    //forward movement
    if(command == 'w'){
        if(temp_x -1 >= 0){
            board[temp_x][temp_y].occupied = FALSE;
            board[--temp_x][temp_y].occupied = TRUE;
        }
    }
    //side movement
    else if(command == 'a'){
        if(temp_y -1 >= 0){
            board[temp_x][temp_y].occupied = FALSE;
            board[temp_x][--temp_y].occupied = TRUE;
        }
    }
    else if(command == 's'){
        if(temp_x +1 <= 8){
            board[temp_x][temp_y].occupied = FALSE;
            board[++temp_x][temp_y].occupied = TRUE;
        }
    }
    //back movement
    else if(command == 'd'){
        if(temp_y +1 <= 8){
            board[temp_x][temp_y].occupied = FALSE;
            board[temp_x][++temp_y].occupied = TRUE;
        }
    }

    *x_frog = temp_x;
    *y_frog = temp_y;
}

void clear_row(struct board_tile board[SIZE][SIZE], int x){
    for(int i=0;i<=8;i++){if(board[x][i].occupied)return;}
    //change row to water
    for(int i=0;i<=8;i++){board[x][i].type = WATER;}
}

void delete_log(struct board_tile board[SIZE][SIZE],int x, int y){
    for(int i=0; i<=8; i++)
    {
        if(board[x][i].occupied)
            return;
    }
    if(board[x][y].type != LOG)
    {
        return;
    }else{
        board[x][y].type = WATER;
    }
    int a=y+1, b=y-1;
    while(board[x][a].type == LOG)
    {
        board[x][a].type = WATER;
        a++;
    }
    while(board[x][b].type == LOG)
    {
        board[x][b].type = WATER;
        b--;
    }
}

void add_bug(struct board_tile board[SIZE][SIZE], struct bug bug_board[SIZE][SIZE], int x_bug, int y_bug){
    if((y_bug >= 0) && (y_bug <= 8))
    {
        //bug can only be used on LOG and TURTLE
        if((board[x_bug][y_bug].type == LOG) || (board[x_bug][y_bug].type == TURTLE))
        {
          bug_board[x_bug][y_bug].state = TRUE;
          bug_board[x_bug][y_bug].move = RIGHT;
        }
    }
}

void init_board(struct board_tile board[SIZE][SIZE],struct bug bug_board[SIZE][SIZE]){
    for(int row=0; row<SIZE; row++){
        for(int col=0; col<SIZE; col++){
            board[row][col].occupied = FALSE;
            bug_board[row][col].state = FALSE;
            bug_board[row][col].move = STATIC;
            if(row == 0){
                if(col%2==0) board[row][col].type = LILYPAD;
                else board[row][col].type = WATER;
            }
            else if(row==8){
                board[row][col].type = BANK;
                if(col==4) board[row][col].occupied = TRUE;
            } else{
                board[row][col].type= WATER;
            }
        }
    }
}

void move_bug(struct board_tile board[SIZE][SIZE], struct bug bug_board[SIZE][SIZE]){
    for(int i=0 ; i<9 ; i++){
        for(int j=0 ; j<9 ; j++){
            if(bug_board[i][j].state){
                if(bug_board[i][j].move==RIGHT){
                    bug_board[i][j].state=FALSE;
                    bug_board[i][j].move=STATIC;
                    j++;
                    bug_board[i][j].state=TRUE;
                //checking if the bug is on the log or turtle edge and make the bug to change its movement
                    if(board[i][j+1].type==WATER || bug_board[i][j+1].state==TRUE){
                        bug_board[i][j].move=LEFT;
                    }else{
                        bug_board[i][j].move=RIGHT;
                    }
                }
                else if(bug_board[i][j].move==LEFT || bug_board[i][j-1].state==TRUE){
                    bug_board[i][j].state=FALSE;
                    bug_board[i][j].move=STATIC;
                    j--;
                    bug_board[i][j].state=TRUE;
                    if(board[i][j-1].type==WATER){
                        bug_board[i][j].move=RIGHT;
                    }else{
                        bug_board[i][j].move=LEFT;
                    }
                }
            }
        }
    }

}

void print_board(struct board_tile board[SIZE][SIZE], struct bug bug_board[SIZE][SIZE]){
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            char type_char = '\0';
            if (board[row][col].occupied) {
                type_char = 'F';
            } else if(bug_board[row][col].state){
                type_char = 'B';
            } else {
                type_char = type_to_char(board[row][col].type);
            }
            printf("%c ", type_char);
            bug_board[row][col].done = FALSE;
        }
        printf("\n");
    }
}

char type_to_char(enum tile_type type){
    char type_char = ' ';
    if(type == LILYPAD){
        type_char = 'o';
    } else if(type == BANK){
        type_char = 'x';
    } else if(type == WATER){
        type_char = '~';
    } else if(type == TURTLE){
        type_char = 'T';
    } else if (type == LOG){
        type_char = 'L';
    }
    return type_char;
}
