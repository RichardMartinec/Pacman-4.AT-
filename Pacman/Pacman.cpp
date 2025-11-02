#include <iostream>  
#include <stdio.h>  
#include <windows.h>  
#include <string>  
#include <vector>  
using namespace std;
char tmp_map[21][32];
char map[21][32] = {
    "+#############################+",
    "|..............#..............|",
    "|O###..#####...#...#####..###O|",
    "|.............................|",
    "|.###..#...#########...#..###.|",
    "|......#.......#.......#......|",
    "|####..####....#....####..####|",
    "    #..#...............#..#    ",
    "|####..#..####   ####..#..####|",
    "          #         #          ",

    "|####..#..###########..#..####|",
    "    #..#...............#..#    ",
    "|####..#...#########...#..####|",
    "|..............#..............|",
    "|O.##..####....#....####..##.O|",
    "|#..#.....................#..#|",
    "|...#..#...#########...#..#...|",
    "|......#.......#.......#......|",
    "|..#########...#...#########..|",
    "|.............................|",
    "+#############################+" };

void ShowMap() //vypisanie obsahu pola map na obrazovku
{
    for (int i = 0; i < 21; i++) //cyklus prechadza po riadkoch
    {
        printf("%s\n", map[i]);//vypis jedneho celeho riadku na obrazovku
    }
}

void gotoxy(short x, short y) //nastavenie polohy na obrazovke
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); //zistenie handle obrazovky
    COORD position = { x, y }; //vlozenie polohy kurzora x, y na obrazovke do struktury position
    SetConsoleCursorPosition(hStdout, position); //nastavenie kurzora na obrazovke na polohu x,y
}

struct walk //definovanie struktury pre BFS prehladavanie
{
    short walk_count;
    short x;
    short y;
    short back;
};

struct target //definovanie struktury pre cestu od ducha k pac-manovi
{
    short x;
    short y;
};

vector<target> najdena_cesta; //zadefinovanie premennej najdena_cesta ako vector so strukturou target
vector<walk> BFSPrehladavanie; //zadefinovanie premennej BFSPrehladavanie ako vector so strukturou walk

void PrehladajPolicko(char tmp_map[21][32], vector<walk>& BFSPrehladavanie, int x, int y, int wc, int back) //funckia na pridanie volneho policka pri BFS prehladavani
{
    if (tmp_map[y][x] == ' ' || tmp_map[y][x] == '.' || tmp_map[y][x] == 'O')// ak je policko s polohou x,y v tmp_map volne nieje tam stena #
    {
        tmp_map[y][x] = '#';//oznac volne policko x,y ako navstivene v tmp_map
        walk tmp; //zadefinovanie pomocnej premennej tmp pre ulozenie polohy x,y a walk_count a back volneho policka
        tmp.x = x; //poloha x volneho policka
        tmp.y = y; //poloha y volneho policka
        tmp.walk_count = wc; //walk_count volneho policka
        tmp.back = back; //back volneho policka
        BFSPrehladavanie.push_back(tmp); //prekopirovanie polohy x,y, walk_count a back volneho policka z tmp do BFSPrehladavanie
    }
}

vector<target> NajdiCestu(int sx, int sy, int x, int y, bool flee = false) //vstupy sx, sy su poloha ducha, suradnice x,y su poloha pacmanna
{
    char local_tmp_map[21][32];
    memcpy(local_tmp_map, map, sizeof(map)); //kopirovanie v pamati pola map do pola tmp_map

    vector<target> najdena_cesta; //zadefinovanie premennej najdena_cesta ako vector so strukturou target
    vector<walk> BFSPrehladavanie; //zadefinovanie premennej BFSPrehladavanie ako vector so strukturou walk

    walk tmp; //zadefinovanie pomocnej struktury formatu BFSPrehladavanie
    tmp.x = sx; //zapamatanie polohy x ducha 
    tmp.y = sy; //zapamatanie polohy y ducha 
    tmp.walk_count = 0; //nastavenie pocitadla poctu stvorcekov ducha od pacmana 
    tmp.back = -1; //nastavenie pocitadla cesty od ducha k pacmannovi
    BFSPrehladavanie.push_back(tmp);//prekopirovanie struktury tmp do BFSPrehladavanie

    local_tmp_map[sy][sx] = '#'; //oznaËenie poËiatoËnej pozÌcie ako navötÌvenej

    int targetX = x;
    int targetY = y;

    if (flee) {
        // duch sa snaûÌ Ìsù opaËn˝m smerom od Pac-Mana
        targetX = x + (x - sx);
        targetY = y + (y - sy);
    }

    int i = 0;
    while (i < BFSPrehladavanie.size())
    {
        if (BFSPrehladavanie[i].x == targetX && BFSPrehladavanie[i].y == targetY)// je poloha ducha rovnaka ako poloha pacmanna?
        {
            target tmp2;//zadefinovanie pomocnej struktury formatu najdena_cesta
            while (BFSPrehladavanie[i].walk_count != 0)//rob pokial nie je cesta od ducha k pacmannovi prazdna
            {
                tmp2.x = BFSPrehladavanie[i].x; //do pomocnej premennej tmp2.x vloz i-tu polohu x cesty od ducha k pacmannovi
                tmp2.y = BFSPrehladavanie[i].y; //do pomocnej premennej tmp2.y vloz i-tu polohu y cesty od ducha k pacmannovi
                najdena_cesta.push_back(tmp2); //prekopiruj i-tu polohu x,y cesty od ducha k pacmannovi do najdena_cesta
                i = BFSPrehladavanie[i].back; //nastavenie dalsieho policka na ceste v smere od ducha k pacmannovi 
            }
            break;
        }

        PrehladajPolicko(local_tmp_map, BFSPrehladavanie, BFSPrehladavanie[i].x + 1, BFSPrehladavanie[i].y, BFSPrehladavanie[i].walk_count + 1, i);
        PrehladajPolicko(local_tmp_map, BFSPrehladavanie, BFSPrehladavanie[i].x - 1, BFSPrehladavanie[i].y, BFSPrehladavanie[i].walk_count + 1, i);
        PrehladajPolicko(local_tmp_map, BFSPrehladavanie, BFSPrehladavanie[i].x, BFSPrehladavanie[i].y + 1, BFSPrehladavanie[i].walk_count + 1, i);
        PrehladajPolicko(local_tmp_map, BFSPrehladavanie, BFSPrehladavanie[i].x, BFSPrehladavanie[i].y - 1, BFSPrehladavanie[i].walk_count + 1, i);

        i++;


    }

    return najdena_cesta;
}

class PacMan {
private:
    int xcor;
    int ycor;

public:
    PacMan(int x, int y) {
        xcor = x;
        ycor = y;
    }

    void setPosition(int x, int y) {
        xcor = x;
        ycor = y;
    }

    void spawnP() {
        gotoxy(xcor, ycor);
        cout << "<";
    }

    void deleteP() {
        map[ycor][xcor] = ' ';
        gotoxy(xcor, ycor);
        cout << " ";
    }
};

// ==== DUCH BLINKY ====
class Blinky {
private:
    int xbcor;
    int ybcor;

public:
    Blinky(int bx, int by) {
        xbcor = bx;
        ybcor = by;
    }

    void setPosition(int bx, int by) {
        xbcor = bx;
        ybcor = by;
    }

    void spawnB() {
        gotoxy(xbcor, ybcor);
        cout << "B";
    }

    // FIX: teraz prijÌma starÈ s˙radnice, aby mazal ducha zo starej pozÌcie
    void deleteB(int old_bx, int old_by) {

        if (old_bx < 0 || old_bx >= 32 || old_by < 0 || old_by >= 21) return; // ochrana proti mimo mapy

        if (map[old_by][old_bx] == '.') {
            gotoxy(old_bx, old_by);
            cout << ".";
        }
        else if (map[old_by][old_bx] == ' ') {
            gotoxy(old_bx, old_by);
            cout << " ";
        }
    }
};

class Inky {
private:
    int xicor;
    int yicor;

public:
    Inky(int ix, int iy) {
        xicor = ix;
        yicor = iy;
    }

    void setPosition(int ix, int iy) {
        xicor = ix;
        yicor = iy;
    }

    void spawnI() {
        gotoxy(xicor, yicor);
        cout << "I";
    }

    void deleteI(int old_ix, int old_iy) {

        if (old_ix < 0 || old_ix >= 32 || old_iy < 0 || old_iy >= 21) return; // ochrana proti mimo mapy

        if (map[old_iy][old_ix] == '.') {
            gotoxy(old_ix, old_iy);
            cout << ".";
        }
        else if (map[old_iy][old_ix] == ' ') {
            gotoxy(old_ix, old_iy);
            cout << " ";
        }
    }
};

class Pinky {
private:
    int xpcor;
    int ypcor;

public:
    Pinky(int px, int py) {
        xpcor = px;
        ypcor = py;
    }

    void setPosition(int px, int py) {
        xpcor = px;
        ypcor = py;
    }

    void spawnPI() {
        gotoxy(xpcor, ypcor);
        cout << "P";
    }

    void deletePI(int old_px, int old_py) {

        if (old_px < 0 || old_px >= 32 || old_py < 0 || old_py >= 21) return; // ochrana proti pohybu mimo mapy

        if (map[old_py][old_px] == '.') {
            gotoxy(old_px, old_py);
            cout << ".";
        }
        else if (map[old_py][old_px] == ' ') {
            gotoxy(old_px, old_py);
            cout << " ";
        }
    }
};

class Clyde {
private:
    int xclcor;
    int yclcor;

public:
    Clyde(int cx, int cy) {
        xclcor = cx;
        yclcor = cy;
    }

    void setPosition(int cx, int cy) {
        xclcor = cx;
        yclcor = cy;
    }

    void spawnCL() {
        gotoxy(xclcor, yclcor);
        cout << "C";
    }

    void deleteCL(int old_cx, int old_cy) {

        if (old_cx < 0 || old_cx >= 32 || old_cy < 0 || old_cy >= 21) return; // ochrana proti mimo mapy

        if (map[old_cy][old_cx] == '.') {
            gotoxy(old_cx, old_cy);
            cout << ".";
        }
        else if (map[old_cy][old_cx] == ' ') {
            gotoxy(old_cx, old_cy);
            cout << " ";
        }
    }
};

int main()
{
    bool beh = true; // zapneme premennu running
    int x = 15; // pociatocna poloha pac-mana x  
    int y = 15; // pociatocna poloha pac-mana y  
    int old_x; // pomocne premenne na zapam‰tanie starej polohy x
    int old_y; //pomocne premenne na zapam‰tanie starej polohy y

    int bx = 15; // pociatocna poloha ducha
    int by = 7; // pociatocna poloha ducha
    int old_bx = bx; // FIX: uloûenie star˝ch s˙radnÌc ducha
    int old_by = by;

    int ix = 16;
    int iy = 9;
    int old_ix = ix;
    int old_iy = iy;

    int px = 14;
    int py = 9;
    int old_px = px;
    int old_py = py;

    int cx = 15;
    int cy = 9;
    int old_cx = cx;
    int old_cy = cy;

    int skore = 0; // pocitadlo skore

    bool superMode = false; // Pac-Man je v super reûime
    int superModeTimer = 0; // poËÌtadlo r·mcov, ako dlho super reûim trv·
    const int superModeDuration = 50; // napr. 50 frame-ov = Ëas super reûimu


    printf("Instrukcie:\n1. Sipky sluzia na pohyb Pac-mana\n2. Ziskavaj body jedenim bodiek vytvorenych duchom\n3. Nenechaj sa chytit duchom\n\n");
    printf("Zadaj hocijake pismeno\nStlac  Enter\n\nStart: ");

    char start;
    int rychlost = 3; //najpomalsia r˝chlosù

    cin >> start;

    system("cls"); // vymazanie obrazovky
    ShowMap(); //funkcia na zobrazenie mapy na obrazovke

    PacMan p1(x, y);
    p1.spawnP();

    Blinky b1(bx, by);
    Inky I1(ix, iy);
    Pinky P1(px, py);
    Clyde C1(cx, cy);
    int frame = 0;

    vector<target> cestaBlinky;
    vector<target> cestaInky;
    vector<target> cestaPinky;
    vector<target> cestaClyde;

    while (beh)
    {
        p1.setPosition(x, y);
        p1.deleteP();

        old_x = x;
        old_y = y;

        if (GetAsyncKeyState(VK_UP)) {
            if (map[y - 1][x] == '.') 
            { 
                y--; 
                skore++; 
            }
            else if (map[y - 1][x] == 'O')
            {
                y--;
                superMode = true;
                superModeTimer = superModeDuration;
                map[y][x] = ' '; // odstr·nenie power-upu z mapy

            }
            else if (map[y - 1][x] == ' ') 
            {
                y--;
            }
        }
        if (GetAsyncKeyState(VK_DOWN)) {
            if (map[y + 1][x] == '.') 
            { 
                y++;
                skore++; 
            }
            else if (map[y + 1][x] == 'O')
            {
                y++;
                superMode = true;
                superModeTimer = superModeDuration;
                map[y][x] = ' '; // odstr·nenie power-upu z mapy
            }
            else if (map[y + 1][x] == ' ') 
            { 
                y++;
            }
        }
        if (GetAsyncKeyState(VK_LEFT)) {
            if (map[y][x - 1] == '.') 
            { 
                x--;
                skore++; 
            }
            else if (map[y][x - 1] == 'O')
            {
                x--;
                superMode = true;
                superModeTimer = superModeDuration;
                map[y][x] = ' '; // odstr·nenie power-upu z mapy
            }
            else if (map[y][x - 1] == ' ') 
            {
                x--;
            }
        }
        if (GetAsyncKeyState(VK_RIGHT)) {
            if (map[y][x + 1] == '.') 
            { 
                x++;
                skore++; 
            }
            else if (map[y][x + 1] == 'O')
            {
                x++;
                superMode = true;
                superModeTimer = superModeDuration;
                map[y][x] = ' '; // odstr·nenie power-upu z mapy
            }
            else if (map[y][x + 1] == ' ') 
            { 
                x++; 
            }
        }
        //if (map[y][x] == 'O') { // zjedol power-up
        //    superMode = true;
        //    superModeTimer = superModeDuration;
        //    map[y][x] = ' '; // odstr·nenie power-upu z mapy
        //}



        if (old_x != x || old_y != y) {
            cestaBlinky = NajdiCestu(bx, by, x, y);
            cestaInky = NajdiCestu(ix, iy, x, y);
            cestaPinky = NajdiCestu(px, py, x, y);
            cestaClyde = NajdiCestu(cx, cy, x, y);
        }

        p1.setPosition(x, y);
        p1.spawnP();

        if (frame % rychlost == 0 && (!cestaBlinky.empty() || !cestaInky.empty() || !cestaPinky.empty() || !cestaClyde.empty()))
        {
            old_bx = bx;
            old_by = by;

            old_ix = ix;
            old_iy = iy;

            old_px = px;
            old_py = py;

            old_cx = cx;
            old_cy = cy;

            if (!cestaBlinky.empty()) {
                bx = cestaBlinky.back().x;
                by = cestaBlinky.back().y;
                cestaBlinky.pop_back();
            }

            if (!cestaInky.empty()) {
                ix = cestaInky.back().x;
                iy = cestaInky.back().y;
                cestaInky.pop_back();
            }

            if (!cestaPinky.empty()) {
                px = cestaPinky.back().x;
                py = cestaPinky.back().y;
                cestaPinky.pop_back();
            }

            if (!cestaClyde.empty()) {
                cx = cestaClyde.back().x;
                cy = cestaClyde.back().y;
                cestaClyde.pop_back();
            }
        }

        // FIX: najprv vymaû ducha zo starej pozÌcie
        b1.deleteB(old_bx, old_by);
        I1.deleteI(old_ix, old_iy);
        P1.deletePI(old_px, old_py);
        C1.deleteCL(old_cx, old_cy);

        // potom vykresli vöetko na nov˝ch pozÌci·ch
        b1.setPosition(bx, by); 
        b1.spawnB();
        I1.setPosition(ix, iy); 
        I1.spawnI();
        P1.setPosition(px, py); 
        P1.spawnPI();
        C1.setPosition(cx, cy); 
        C1.spawnCL();

        if (superMode) {
            superModeTimer--;
            if (superModeTimer <= 0) {
                superMode = false; // koniec super reûimu
            }
        }


        if (superMode) {
            // Pac-Man zjedol ducha
            if ((bx == x && by == y)) { bx = 15; by = 7; } // Blinky sa re-spawn
            if ((ix == x && iy == y)) { ix = 16; iy = 9; } // Inky
            if ((px == x && py == y)) { px = 14; py = 9; } // Pinky
            if ((cx == x && cy == y)) { cx = 15; cy = 9; } // Clyde
        }else if ((bx == x && by == y) || (ix == x && iy == y) || (px == x && py == y) || (cx == x && cy == y)) {
            break;
        }
        else if (skore == 328) {
            break;
        }

        gotoxy(32, 1);//nastavenie polohy na vypisanie skore
        cout << skore;//vypis skore
        Sleep(100);//oneskorenie, aby program bezal pomalsie
        frame++; //zvysenie frame o 1
    }

    system("cls"); //vymaz obrazovku
    printf("Game Over. Your score is : %i", skore); // vypis konecne skore
    cin.get(); 
    cin.get();
    cin.get();
    cin.get();
    cin.get();
    cin.get();

    return 0;
}
