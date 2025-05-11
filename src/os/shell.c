#include "stdint.h"

typedef volatile char vchar;

#define WIDTH 80
#define HEIGHT 25

vchar* vga = (vchar*)0xB8000;

void new_line();
void clear();
void print(char* str);
void scroll_up(uint32_t num);

struct{
    int x, y;
} cursor;

void new_line(){
    cursor.x = 0;
    cursor.y++;
}

void clear(){
    for (int i = 0; i < WIDTH*HEIGHT*2; i+=2){
        vga[i] = ' ';
        vga[i+1] = 0x07;
    }
}

void print(char* str){
    for (int i = 0; str[i] != '\0'; i++){
        if (str[i] == '\n'){
            new_line();
            continue;
        }
        if (str[i] == '\r'){
            cursor.x = 0;
            continue;
        }

        if (cursor.x >= WIDTH){
            scroll_up(1);
            new_line();
            continue;
        }

        vga[(cursor.x+cursor.y)*2] = str[i];
        vga[(cursor.x+cursor.y)*2+1] = 0x0F;
        
        cursor.x++;
    }
}

void scroll_up(uint32_t num){
    for (int i = 0; i < WIDTH*(HEIGHT-1)*2; i+=2){
        vga[i] = vga[i+WIDTH*num];
        vga[i+1] = vga[i+WIDTH*num+1];
    }
    cursor.y-=num;
}