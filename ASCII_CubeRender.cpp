#include <iostream>
#include <cmath>
#include <unistd.h>
#include <vector>

using namespace std;


//Camera 
const float screen_height = 25;
const float screen_width = 25;
const float K2 = 6;
//Cube motion
float A = 0; //Theta
float B = 0; //Phi
float t_increment = 0.07;
float p_increment = 0.02;
//Cube configuration
const float width = 1;
const float height = 1;
const float w = width/2; 
const float h = height/2;
const float p = 16;
const float d = height/p;
const float K1 = K2*screen_width*3/(8*width);

void render_surface (vector<vector<char>> &screen, vector<vector<float>> &buffer, float theta_add, float phi_add, char surface);

int main (void){

    vector<vector<char>> map_screen(screen_height, vector<char>(screen_width, ' '));
    vector<vector<float>> zbuffer(screen_height, vector<float>(screen_width, 0)); 

    while (1){
        
        cout << "\x1b[2J";
        cout << "\x1b[H";
        
        render_surface (map_screen, zbuffer, 0, 0, '#'); //Surface 1
        render_surface (map_screen, zbuffer, 1.57, 0, '@'); //Surface 2
        render_surface (map_screen, zbuffer, 4.71, 0, '~'); //Surface 3
        render_surface (map_screen, zbuffer, 3.14, 0, '*'); //Surface 4
        
        for (const auto row: map_screen){
            for(const auto block: row){
                putchar(block);
            }
            cout << "\n";
        }

        for (int i = 0; i < screen_height; i++){
            fill(map_screen[i].begin(), map_screen[i].end(), ' '); //Reset screen
        }

        for (int i = 0; i < screen_height; i++){
            fill(zbuffer[i].begin(), zbuffer[i].end(), 0); //Reset zbuffer
        }
        usleep(10000);
        A += t_increment;
        B += p_increment;
    }
    

    return 0;
}

void render_surface (vector<vector<char>> &screen, vector<vector<float>> &buffer, float theta_add, float phi_add, char surface){
    //Pre cal A
    float cosA = cos(A + theta_add);
    float sinA = sin(A + theta_add);
    //Pre cal B
    float cosB = cos(B + phi_add);
    float sinB = sin(B + phi_add);
   //Pre calculate some indepent variable in (x,y,z).
    float sx = w*(cosA+sinA);
    float sy = w*sinB*(sinA - cosA) - h*cosB;
    float sz = w*cosB*(sinA - cosA) + h*sinB;
    
    float ooz;
    float x, y, z;
    int xp, yp;

    for (int i = 0; i < p; i++){
        for(int j = 0; j < p; j++){
        x = sx - i*d*sinA;
        y = sy + j*d*cosB + i*d*cosA*sinB; 
        z = sz - j*d*sinB + i*d*cosA*cosB;
        ooz = 1/(z + K2);

        xp = (int) screen_width/2 + K1*ooz*x;
        yp = (int) screen_height/2 - K1*ooz*y ;

        if (buffer[yp][xp] < ooz){
            buffer[yp][xp] = ooz;
            screen[yp][xp] = surface;
        }

        }
    }
}

