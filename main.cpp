#include<iostream>
using namespace std;

const char gStageData[] = "########\n"
                          "# .. p #\n"
                          "# oo   #\n"
                          "#      #\n"
                          "########";
const int gStageWidth = 8;
const int gStageHeight = 5;

enum Object {
    OBJ_SPACE,
    OBJ_WALL,
    OBJ_GOAL,
    OBJ_BLOCK,
    OBJ_BLOCK_ON_GOAL,
    OBJ_MAN,
    OBJ_MAN_ON_GOAL,

    OBJ_UNKNOWN,
};

void initialize(Object * state, int w, int h, const char* stageData);
void draw(const Object * state, int w, int h);
void update(Object * state, char input, int w, int h);
bool checkClear( const Object * state, int w, int h);


int main() {
    //state array get.
    Object * state = new Object[ gStageWidth * gStageHeight ];
    initialize(state, gStageWidth,gStageHeight,gStageData);
    //main loop
    while (true) {
        //draw stage
        draw(state, gStageWidth, gStageHeight);
        //check clear
        if (checkClear(state, gStageWidth, gStageHeight)) {
            break;
        }
        cout << "w:up, a:left, s:down, d:right. command?" <<endl;
        char input;
        cin >> input;
        //update state
        update(state, input, gStageWidth, gStageHeight);

    }

    //Clear message.
    cout << "Congratulation! you win." << endl;
    //manage mem.
    delete[] state;
    state = 0;

    return 0;
}

void initialize(Object * state, int width, int height, const char* stageData) {
    const char* d = stageData; //read only pointer
    int x = 0;
    int y = 0;
    while (*d != '\0') {
        Object t;
        switch ( *d ) {
            case '#' : t = OBJ_WALL; break;
            case ' ' : t = OBJ_SPACE; break;
            case 'o' : t = OBJ_BLOCK; break;
            case 'O' : t = OBJ_BLOCK_ON_GOAL; break;
            case '.' : t = OBJ_GOAL; break;
            case 'p' : t = OBJ_MAN; break;
            case 'P' : t = OBJ_MAN_ON_GOAL; break;
            case '\n': //next row
                x=0;
                y++;
                t = OBJ_UNKNOWN;
                break;
            default: t = OBJ_UNKNOWN; break; //unknown data
        }
        d++;
        if ( t != OBJ_UNKNOWN) {        //ignore unknown data
            state [y * width + x] = t;  //write
            x++;
        }
    }
}

void draw(const Object * state, int width, int height){
    //Object enum
    const char font[] = {' ', '#', '.', 'o', 'O', 'p', 'P'};

    //read state and draw stage. (loop)
    for (int y = 0; y < height; y++){
        for (int x = 0; x <width; x++){
            Object o = state[y * width + x];
            cout << font [o];
        }
        cout << endl;
    }
}

void update(Object * s, char input, int w, int h){
    int dx = 0;
    int dy = 0;
    switch (input) {
        case 'w': dy = -1; break; //up
        case 'a': dx = -1; break; //left
        case 's': dy = 1; break; //down
        case 'd': dx = 1; break; //right
    }

    int i = -1; //init int
    for (i = 0; i < w * h; i++){
        if (s[i] == OBJ_MAN || s[i] == OBJ_MAN_ON_GOAL){
            break;
        }
    }
    int x = i % w;  //x is modular of i
    int y = i / w;  //y is quotient of i

    int target_x = x + dx;
    int target_y = y + dy;
    //check coordinate.
    if ( target_x < 0 || target_y < 0 || target_x >= w || target_y >= h) {
        return;
    }
    int p = y * w + x;  //current character position
    int target_p = target_y * w + target_x; //target character position
    if ( s[target_p] == OBJ_SPACE || s[target_p] == OBJ_GOAL){
        s[target_p] = (s[target_p] == OBJ_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
        s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
    }
    else if (s[target_p] == OBJ_BLOCK || s[target_p == OBJ_BLOCK_ON_GOAL]){
        int target_x2 = target_x + dx;
        int target_y2 = target_y + dy;
        if (target_x2 < 0 || target_y2 < 0 || target_x2 >= w || target_y2 >= h){
            return ;
        }
        int target_p2 = (target_y + dy) * w + (target_x + dx);
        if (s[target_p2] == OBJ_SPACE || s[target_p2] == OBJ_GOAL){
            s[target_p2] = (s[target_p2] == OBJ_GOAL) ? OBJ_BLOCK_ON_GOAL : OBJ_GOAL;
            s[target_p] = (s[target_p] == OBJ_BLOCK_ON_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
            s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
        }
    }
}

bool checkClear(const Object * s, int width, int height) {
    for ( int i = 0; i < width * height; i++){
        if(s[i]==OBJ_BLOCK){
            return false;
        }
    }
    return true;
}