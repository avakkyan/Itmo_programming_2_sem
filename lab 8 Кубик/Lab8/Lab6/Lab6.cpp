#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <glut.h>

using namespace std;

class Small_Cube {
private:
    unsigned char _color[3];

public:
    // шесть граней куба - шесть цветов
    unsigned int color[6];
    // длина ребра
    double size;

    void rotate_x() {
        swap(color[5], color[3]);
        swap(color[3], color[4]);
        swap(color[4], color[2]);
    }

    void rotate_y() {
        swap(color[2], color[1]);
        swap(color[1], color[3]);
        swap(color[3], color[0]);
    }

    void rotate_z() {
        swap(color[0], color[4]);
        swap(color[4], color[1]);
        swap(color[1], color[5]);
    }

    void setColor(int i, int color) {
        this->color[i] = color;
    }

    unsigned char* at(int i) {
        _color[0] = color[i] >> 16;
        _color[1] = color[i] >> 8;
        _color[2] = color[i];
        return _color;
    }

    // отрисовка куба:
    void draw() {
        glPushMatrix();
        glBegin(GL_QUADS);

        // верх
        glColor3ubv(at(0));
        glNormal3f(0, 0, 1);
        glVertex3f(size, size, size);
        glVertex3f(0, size, size);
        glVertex3f(0, 0, size);
        glVertex3f(size, 0, size);

        // низ
        glColor3ubv(at(1));
        glNormal3f(0, 0, -1);
        glVertex3f(size, 0, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, size, 0);
        glVertex3f(size, size, 0);

        // спереди
        glColor3ubv(at(2));
        glNormal3f(0, -1, 0);
        glVertex3f(size, 0, size);
        glVertex3f(0, 0, size);
        glVertex3f(0, 0, 0);
        glVertex3f(size, 0, 0);

        // сзади
        glColor3ubv(at(3));
        glNormal3f(0, 1, 0);
        glVertex3f(size, size, 0);
        glVertex3f(0, size, 0);
        glVertex3f(0, size, size);
        glVertex3f(size, size, size);

        // слева
        glColor3ubv(at(4));
        glNormal3f(-1, 0, 0);
        glVertex3f(0, size, size);
        glVertex3f(0, size, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, size);

        // справа
        glColor3ubv(at(5));
        glNormal3f(1, 0, 0);
        glVertex3f(size, size, 0);
        glVertex3f(size, size, size);
        glVertex3f(size, 0, size);
        glVertex3f(size, 0, 0);

        glEnd();
        glPopMatrix();
    }

    // отрисовка куба со смещением (x, y, z)
    void draw(double x, double y, double z) {
        glPushMatrix();
        glTranslated(x, y, z);
        draw();
        glPopMatrix();
    }
};

class Cube {
private:
    char left[3][3], right[3][3], up[3][3], front[3][3], back[3][3], down[3][3];

    bool ok[3][3][3];
    Small_Cube tmp[3][3];
    Small_Cube a[3][3][3];
    //угол поворота каждой грани
    int rotate[6];
    // размер кубика
    double size;
    // цвета граней
    unsigned int color[6];
    // скорость поворота
    int speed_torn = 5;


public:
    Cube() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                left[i][j] = 'g';
                front[i][j] = 'w';
                right[i][j] = 'b';
                back[i][j] = 'y';
                up[i][j] = 'o';
                down[i][j] = 'r';
            }
        }
    }

    void show() {                        //консольный вывод
        for (int i = 0; i < 3; i++) {
            cout << "    ";
            for (int j = 0; j < 3; j++) {
                cout << up[i][j];
            }
            cout << endl;
        }

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                cout << left[i][j];
            }

            cout << " ";

            for (int j = 0; j < 3; j++) {
                cout << front[i][j];
            }

            cout << " ";

            for (int j = 0; j < 3; j++) {
                cout << right[i][j];
            }

            cout << " ";

            for (int j = 0; j < 3; j++) {
                cout << back[i][j];
            }

            cout << endl;
        }

        for (int i = 0; i < 3; i++) {
            cout << "    ";
            for (int j = 0; j < 3; j++) {
                cout << down[i][j];
            }
            cout << endl;
        }

        cout << endl;
    }

    void save(string str) {               //запись файла
        ofstream fout("cube.out");

        for (int i = 0; i < 3; i++) {
            fout << "    ";
            for (int j = 0; j < 3; j++) {
                fout << up[i][j];
            }
            fout << endl;
        }

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                fout << left[i][j];
            }

            fout << " ";

            for (int j = 0; j < 3; j++) {
                fout << front[i][j];
            }

            fout << " ";

            for (int j = 0; j < 3; j++) {
                fout << right[i][j];
            }

            fout << " ";

            for (int j = 0; j < 3; j++) {
                fout << back[i][j];
            }

            fout << endl;
        }

        for (int i = 0; i < 3; i++) {
            fout << "    ";
            for (int j = 0; j < 3; j++) {
                fout << down[i][j];
            }
            fout << endl;
        }

        fout << endl;

        fout.close();
    }

    void read() {                    //чтение из файла
        ifstream fin("cube.in");
        string s;
        for (int i = 0; i < 6; i++) {
            fin >> s;
            if (s == "front") {
                for (int j = 0; j < 3; j++) {
                    for (int k = 0; k < 3; k++) {
                        fin >> front[j][k];
                    }
                }
            }
            else if (s == "left") {
                for (int j = 0; j < 3; j++) {
                    for (int k = 0; k < 3; k++) {
                        fin >> left[j][k];
                    }
                }
            }
            else if (s == "right") {
                for (int j = 0; j < 3; j++) {
                    for (int k = 0; k < 3; k++) {
                        fin >> right[j][k];
                    }
                }
            }
            else if (s == "back") {
                for (int j = 0; j < 3; j++) {
                    for (int k = 0; k < 3; k++) {
                        fin >> back[j][k];
                    }
                }
            }
            else if (s == "up") {
                for (int j = 0; j < 3; j++) {
                    for (int k = 0; k < 3; k++) {
                        fin >> up[j][k];
                    }
                }
            }
            else if (s == "down") {
                for (int j = 0; j < 3; j++) {
                    for (int k = 0; k < 3; k++) {
                        fin >> down[j][k];
                    }
                }
            }
        }

        fin.close();
    }
    //повороты по парам - по часовой и против часовой

    //левая пара 
    void Left() {
        int k = left[0][0];
        int c;
        int l;

        left[0][0] = left[2][0];
        left[2][0] = left[2][2];
        left[2][2] = left[0][2];
        left[0][2] = k;

        k = left[0][1];

        left[0][1] = left[1][0];
        left[1][0] = left[2][1];
        left[2][1] = left[1][2];
        left[1][2] = k;

        k = back[0][2];
        c = back[1][2];
        l = back[2][2];

        back[0][2] = down[2][0];
        back[1][2] = down[1][0];
        back[2][2] = down[0][0];

        down[2][0] = front[2][0];
        down[1][0] = front[1][0];
        down[0][0] = front[0][0];

        front[2][0] = up[2][0];
        front[1][0] = up[1][0];
        front[0][0] = up[0][0];

        up[2][0] = k;
        up[1][0] = c;
        up[0][0] = l;

        Rotate(4, speed_torn, 1);
    }

    void LeftB() {
        int k = left[0][0];
        int c;
        int l;

        left[0][0] = left[0][2];
        left[0][2] = left[2][2];
        left[2][2] = left[2][0];
        left[2][0] = k;

        k = left[0][1];

        left[0][1] = left[1][2];
        left[1][2] = left[2][1];
        left[2][1] = left[1][0];
        left[1][0] = k;

        k = back[0][2];
        c = back[1][2];
        l = back[2][2];

        back[0][2] = up[2][0];
        back[1][2] = up[1][0];
        back[2][2] = up[0][0];

        up[2][0] = front[2][0];
        up[1][0] = front[1][0];
        up[0][0] = front[0][0];

        front[2][0] = down[2][0];
        front[1][0] = down[1][0];
        front[0][0] = down[0][0];

        down[2][0] = k;
        down[1][0] = c;
        down[0][0] = l;

        Rotate(4, speed_torn, -1);
    }

    //правая пара
    void Right() {
        int k = right[0][0];
        int c;
        int l;

        right[0][0] = right[2][0];
        right[2][0] = right[2][2];
        right[2][2] = right[0][2];
        right[0][2] = k;

        k = right[0][1];

        right[0][1] = right[1][0];
        right[1][0] = right[2][1];
        right[2][1] = right[1][2];
        right[1][2] = k;

        k = back[0][0];
        c = back[1][0];
        l = back[2][0];

        back[0][0] = up[2][2];
        back[1][0] = up[1][2];
        back[2][0] = up[0][2];

        up[2][2] = front[2][2];
        up[1][2] = front[1][2];
        up[0][2] = front[0][2];

        front[2][2] = down[2][2];
        front[1][2] = down[1][2];
        front[0][2] = down[0][2];

        down[2][2] = k;
        down[1][2] = c;
        down[0][2] = l;

        Rotate(5, speed_torn, -1);
    }

    void RightB() {
        int k = right[0][0];
        int c;
        int l;

        right[0][0] = right[0][2];
        right[0][2] = right[2][2];
        right[2][2] = right[2][0];
        right[2][0] = k;

        k = right[0][1];

        right[0][1] = right[1][2];
        right[1][2] = right[2][1];
        right[2][1] = right[1][0];
        right[1][0] = k;

        k = back[0][0];
        c = back[1][0];
        l = back[2][0];

        back[0][0] = down[2][2];
        back[1][0] = down[1][2];
        back[2][0] = down[0][2];

        down[2][2] = front[2][2];
        down[1][2] = front[1][2];
        down[0][2] = front[0][2];

        front[2][2] = up[2][2];
        front[1][2] = up[1][2];
        front[0][2] = up[0][2];

        up[2][2] = k;
        up[1][2] = c;
        up[0][2] = l;

        Rotate(5, speed_torn, 1);
    }

    //передняя пара
    void Front() {
        int k = front[0][0];
        int c;
        int l;

        front[0][0] = front[2][0];
        front[2][0] = front[2][2];
        front[2][2] = front[0][2];
        front[0][2] = k;

        k = front[0][1];

        front[0][1] = front[1][0];
        front[1][0] = front[2][1];
        front[2][1] = front[1][2];
        front[1][2] = k;

        k = up[2][0];
        c = up[2][1];
        l = up[2][2];

        up[2][0] = left[2][2];
        up[2][1] = left[1][2];
        up[2][2] = left[0][2];

        left[2][2] = down[0][2];
        left[1][2] = down[0][1];
        left[0][2] = down[0][0];

        down[0][2] = right[0][0];
        down[0][1] = right[1][0];
        down[0][0] = right[2][0];

        right[0][0] = k;
        right[1][0] = c;
        right[2][0] = l;

        Rotate(1, speed_torn, -1);
    }

    void FrontB() {
        int k = front[0][0];
        int c;
        int l;

        front[0][0] = front[0][2];
        front[0][2] = front[2][2];
        front[2][2] = front[2][0];
        front[2][0] = k;

        k = front[0][1];

        front[0][1] = front[1][2];
        front[1][2] = front[2][1];
        front[2][1] = front[1][0];
        front[1][0] = k;

        k = up[2][0];
        c = up[2][1];
        l = up[2][2];

        up[2][0] = right[0][0];
        up[2][1] = right[1][0];
        up[2][2] = right[2][0];

        right[0][0] = down[0][2];
        right[1][0] = down[0][1];
        right[2][0] = down[0][0];

        down[0][2] = left[2][2];
        down[0][1] = left[1][2];
        down[0][0] = left[0][2];

        left[0][2] = l;
        left[1][2] = c;
        left[2][2] = k;

        Rotate(1, speed_torn, 1);
    }

    //задняя пара
    void Back() {
        int k = back[0][0];
        int c;
        int l;

        back[0][0] = back[2][0];
        back[2][0] = back[2][2];
        back[2][2] = back[0][2];
        back[0][2] = k;

        k = back[0][1];

        back[0][1] = back[1][0];
        back[1][0] = back[2][1];
        back[2][1] = back[1][2];
        back[1][2] = k;

        k = up[0][0];
        c = up[0][1];
        l = up[0][2];

        up[0][0] = right[0][2];
        up[0][1] = right[1][2];
        up[0][2] = right[2][2];

        right[2][2] = down[2][0];
        right[1][2] = down[2][1];
        right[0][2] = down[2][2];

        down[2][2] = left[2][0];
        down[2][1] = left[1][0];
        down[2][0] = left[0][0];

        left[0][0] = l;
        left[1][0] = c;
        left[2][0] = k;

        Rotate(0, speed_torn, 1);
    }

    void BackB() {
        int k = back[0][0];
        int c;
        int l;

        back[0][0] = back[0][2];
        back[0][2] = back[2][2];
        back[2][2] = back[2][0];
        back[2][0] = k;

        k = back[0][1];

        back[0][1] = back[1][2];
        back[1][2] = back[2][1];
        back[2][1] = back[1][0];
        back[1][0] = k;

        k = up[0][0];
        c = up[0][1];
        l = up[0][2];

        up[0][0] = left[2][0];
        up[0][1] = left[1][0];
        up[0][2] = left[0][0];

        left[2][0] = down[2][2];
        left[1][0] = down[2][1];
        left[0][0] = down[2][0];

        down[2][2] = right[0][2];
        down[2][1] = right[1][2];
        down[2][0] = right[2][2];

        right[0][2] = k;
        right[1][2] = c;
        right[2][2] = l;

        Rotate(0, speed_torn, -1);
    }

    //верхняя пара
    void Up() {
        int k = up[0][0];
        int c;
        int l;

        up[0][0] = up[2][0];
        up[2][0] = up[2][2];
        up[2][2] = up[0][2];
        up[0][2] = k;

        k = up[0][1];

        up[0][1] = up[1][0];
        up[1][0] = up[2][1];
        up[2][1] = up[1][2];
        up[1][2] = k;

        k = left[0][0];
        c = left[0][1];
        l = left[0][2];

        left[0][0] = front[0][0];
        left[0][1] = front[0][1];
        left[0][2] = front[0][2];

        front[0][0] = right[0][0];
        front[0][1] = right[0][1];
        front[0][2] = right[0][2];

        right[0][0] = back[0][0];
        right[0][1] = back[0][1];
        right[0][2] = back[0][2];

        back[0][0] = k;
        back[0][1] = c;
        back[0][2] = l;

        Rotate(3, speed_torn, -1);
    }


    void UpB() {
        int k = up[0][0];
        int c;
        int l;

        up[0][0] = up[0][2];
        up[0][2] = up[2][2];
        up[2][2] = up[2][0];
        up[2][0] = k;

        k = up[0][1];

        up[0][1] = up[1][2];
        up[1][2] = up[2][1];
        up[2][1] = up[1][0];
        up[1][0] = k;

        k = left[0][0];
        c = left[0][1];
        l = left[0][2];

        left[0][0] = back[0][0];
        left[0][1] = back[0][1];
        left[0][2] = back[0][2];

        back[0][0] = right[0][0];
        back[0][1] = right[0][1];
        back[0][2] = right[0][2];

        right[0][0] = front[0][0];
        right[0][1] = front[0][1];
        right[0][2] = front[0][2];

        front[0][0] = k;
        front[0][1] = c;
        front[0][2] = l;

        Rotate(3, speed_torn, 1);
    }

    //нижняя пара
    void Down() {
        int k = down[0][0];
        int c;
        int l;

        down[0][0] = down[2][0];
        down[2][0] = down[2][2];
        down[2][2] = down[0][2];
        down[0][2] = k;

        k = down[0][1];

        down[0][1] = down[1][0];
        down[1][0] = down[2][1];
        down[2][1] = down[1][2];
        down[1][2] = k;

        k = left[2][0];
        c = left[2][1];
        l = left[2][2];

        left[2][0] = back[2][0];
        left[2][1] = back[2][1];
        left[2][2] = back[2][2];

        back[2][0] = right[2][0];
        back[2][1] = right[2][1];
        back[2][2] = right[2][2];

        right[2][0] = front[2][0];
        right[2][1] = front[2][1];
        right[2][2] = front[2][2];

        front[2][0] = k;
        front[2][1] = c;
        front[2][2] = l;

        Rotate(2, speed_torn, 1);
    }
    
    void DownB() {
        int k = down[0][0];
        int c;
        int l;

        down[0][0] = down[0][2];
        down[0][2] = down[2][2];
        down[2][2] = down[2][0];
        down[2][0] = k;

        k = down[0][1];

        down[0][1] = down[1][2];
        down[1][2] = down[2][1];
        down[2][1] = down[1][0];
        down[1][0] = k;

        k = left[2][0];
        c = left[2][1];
        l = left[2][2];

        left[2][0] = front[2][0];
        left[2][1] = front[2][1];
        left[2][2] = front[2][2];

        front[2][0] = right[2][0];
        front[2][1] = right[2][1];
        front[2][2] = right[2][2];

        right[2][0] = back[2][0];
        right[2][1] = back[2][1];
        right[2][2] = back[2][2];

        back[2][0] = k;
        back[2][1] = c;
        back[2][2] = l;

        Rotate(2, speed_torn, -1);
    }



    void rotation() {    //"ручные" повороты
        string k;
        int count;
        cin >> k >> count;

        if (k == "left") {
            for (int i = 0; i < count; i++) {
                Left();
                show();
            }
        }
        else 
            if (k == "right") {
            for (int i = 0; i < count; i++) {
                Right();
                show();
            }
        }
        else 
            if (k == "front") {
            for (int i = 0; i < count; i++) {
                Front();
                show();
            }
        }
        else 
            if (k == "back") {
            for (int i = 0; i < count; i++) {
                Back();
                show();
            }
        }
        else 
            if (k == "up") {
            for (int i = 0; i < count; i++) {
                Up();
                show();
            }
        }
        else 
            if (k == "down") {
            for (int i = 0; i < count; i++) {
                Down();
                show();
            }
        }
        else 
            if (k == "leftB") {
            for (int i = 0; i < count; i++) {
                LeftB();
                show();
            }
        }
        else 
            if (k == "rightB") {
            for (int i = 0; i < count; i++) {
                RightB();
                show();
            }
        }
        else 
            if (k == "frontB") {
            for (int i = 0; i < count; i++) {
                FrontB();
                show();
            }
        }
        else 
            if (k == "backB") {
            for (int i = 0; i < count; i++) {
                BackB();
                show();
            }
        }
        else 
            if (k == "upB") {
            for (int i = 0; i < count; i++) {
                UpB();
                show();
            }
        }
        else 
            if (k == "downB") {
            for (int i = 0; i < count; i++) {
                DownB();
                show();
            }
        }
    }

    void Random() {           //рандом
        srand(time(0));
        int k = rand() % 47 + 13; //число тиков
        int l;
        int rotation;

        for (int i = 0; i < k; i++) {
            l = rand() % 2;               //вращаем или нет
            rotation = rand() % 6;   //направление вращения

            if (rotation == 0) {
                for (int j = 0; j < l; j++) {
                    Left();
                }
            }
            else 
                if (rotation == 1) {
                for (int j = 0; j < l; j++) {
                    Right();
                }
            }
            else 
                if (rotation == 2) {
                for (int j = 0; j < l; j++) {
                    Front();
                }
            }
            else 
                if (rotation == 3) {
                for (int j = 0; j < l; j++) {
                    Back();
                }
            }
            else 
                if (rotation == 4) {
                for (int j = 0; j < l; j++) {
                    Up();
                }
            }
            else 
                if (rotation == 5) {
                for (int j = 0; j < l; j++) {
                    Down();
                }
            }
        }

        cout << k << " different turns for random cube" << endl;
        show();
    }

    bool check_red() {            //проверка на красный крест
        bool ch = true;

        if (up[0][1] != 'r') {
            ch = false;
        }
        else if (up[1][2] != 'r') {
            ch = false;
        }
        else if (up[2][1] != 'r') {
            ch = false;
        }
        else if (up[1][0] != 'r') {
            ch = false;
        }

        return ch;
    }

    void Front_special_comb() {
        Front();
        Up();
        FrontB();
        UpB();
    }

    void Back_special_comb() {
        Back();
        Up();
        BackB();
        UpB();
    }

    void Right_special_comb() {
        Right();
        Up();
        RightB();
        UpB();
    }

    void Left_special_comb() {
        Left();
        Up();
        LeftB();
        UpB();
    }

    void first() {
        //Первый этап сборки - сборка нижнего яруса
        while (!check_red()) {
            if (front[1][2] == 'r' && up[1][2] != 'r') {
                Right();
            }
            else 
                if (front[1][2] == 'r') {
                while (up[1][2] == 'r') {
                    Up();
                }
                Right();
            }

            if (front[1][0] == 'r' && up[1][0] != 'r') {
                LeftB();
            }
            else 
                if (front[1][0] == 'r') {
                while (up[1][0] == 'r') {
                    Up();
                }
                LeftB();
            }

            if (down[1][0] == 'r' && up[1][0] != 'r') {
                LeftB();
                LeftB();
            }
            else 
                if (down[1][0] == 'r') {
                while (up[1][0] == 'r') {
                    Up();
                }
                LeftB();
                LeftB();
            }

            if (down[1][2] == 'r' && up[1][2] != 'r') {
                Right();
                Right();
            }
            else 
                if (down[1][2] == 'r') {
                while (up[1][2] == 'r') {
                    Up();
                }
                Right();
                Right();
            }

            if (left[1][0] == 'r' && up[0][1] != 'r') {
                BackB();
            }
            else 
                if (left[1][0] == 'r') {
                while (up[0][1] == 'r') {
                    Up();
                }
                BackB();
            }

            if (left[1][2] == 'r' && up[2][1] != 'r') {
                Front();
            }
            else 
                if (left[1][2] == 'r') {
                while (up[2][1] == 'r') {
                    Up();
                }
                Front();
            }

            if (right[1][0] == 'r' && up[2][1] != 'r') {
                FrontB();
            }
            else 
                if (right[1][0] == 'r') {
                while (up[2][1] == 'r') {
                    Up();
                }
                FrontB();
            }

            if (right[1][2] == 'r' && up[0][1] != 'r') {
                Back();
            }
            else 
                if (right[1][2] == 'r') {
                while (up[0][1] == 'r') {
                    Up();
                }
                Back();
            }

            if (back[1][0] == 'r' && up[1][2] != 'r') {
                RightB();
            }
            else 
                if (back[1][0] == 'r') {
                while (up[1][2] == 'r') {
                    Up();
                }
                RightB();
            }

            if (back[1][2] == 'r' && up[1][0] != 'r') {
                Left();
            }
            else 
                if (back[1][2] == 'r') {
                while (up[1][0] == 'r') {
                    Up();
                }
                Left();
            }

            if (front[0][1] == 'r' && up[1][2] != 'r') {
                Front();
                Right();
            }
            else 
                if (front[0][1] == 'r') {
                while (up[1][2] == 'r') {
                    Up();
                }
                Front();
                Right();
            }

            if (front[2][1] == 'r' && up[1][2] != 'r') {
                FrontB();
                Right();
            }
            else 
                if (front[2][1] == 'r') {
                while (up[1][2] == 'r') {
                    Up();
                }
                FrontB();
                Right();
            }

            if (left[0][1] == 'r' && up[2][1] != 'r') {
                Left();
                Front();
            }
            else 
                if (left[0][1] == 'r') {
                while (up[2][1] == 'r') {
                    Up();
                }
                Left();
                Front();
            }

            if (left[2][1] == 'r' && up[0][1] != 'r') {
                Left();
                BackB();
            }
            else 
                if (left[2][1] == 'r') {
                while (up[0][1] == 'r') {
                    Up();
                }
                Left();
                BackB();
            }

            if (right[0][1] == 'r' && up[0][1] != 'r') {
                Right();
                Back();
            }
            else 
                if (right[0][1] == 'r') {
                while (up[0][1] == 'r') {
                    Up();
                }
                Right();
                Back();
            }
            if (right[2][1] == 'r' && up[1][2] != 'r') {
                Right();
            }
            else 
                if (right[2][1] == 'r') {
                while (up[1][2] == 'r') {
                    Up();
                }
                Right();
            }

            if (back[0][1] == 'r' && up[0][1] != 'r') {
                Back();
            }
            else 
                if (back[0][1] == 'r') {
                while (up[0][1] == 'r') {
                    Up();
                }
                Back();
            }

            if (back[2][1] == 'r' && up[0][1] != 'r') {
                Back();
            }
            else 
                if (back[2][1] == 'r') {
                while (up[0][1] == 'r') {
                    Up();
                }
                Back();
            }

            if (down[0][1] == 'r' && up[2][1] != 'r') {
                Front();
                Front();
            }
            else 
                if (down[0][1] == 'r') {
                while (up[2][1] == 'r') {
                    Up();
                }
                Front();
                Front();
            }

            if (down[2][1] == 'r' && up[0][1] != 'r') {
                Back();
                Back();
            }
            else
                if (down[2][1] == 'r') {
                while (up[0][1] == 'r') {
                    Up();
                }
                Back();
                Back();
            }
        }
        //Правильный крест

        if (!check1()) {
            while (!check1()) {
                Up();
            }
            Front();
            Front();
        }
        else if (check1()) {
            Front();
            Front();
        }

        if (!check2()) {
            while (!check2()) {
                Up();
            }
            Right();
            Right();
        }
        else if (check2()) {
            Right();
            Right();
        }

        if (!check3()) {
            while (!check3()) {
                Up();
            }
            Back();
            Back();
        }
        else if (check3()) {
            Back();
            Back();
        }

        if (!check4()) {
            while (!check4()) {
                Up();
            }
            Left();
            Left();
        }
        else if (check4()) {
            Left();
            Left();
        }
        cout << "The right cross" << endl;
        show();
        //Второй этап сборки (первый слой до конца)

        while (!check_edge()) {
            if ((front[0][2] == 'r' && right[0][0] == 'b' && up[2][2] == 'w') || (front[0][2] == 'r' && right[0][0] == 'w' && up[2][2] == 'b')
                || (front[0][2] == 'w' && right[0][0] == 'r' && up[2][2] == 'b') || (front[0][2] == 'w' && right[0][0] == 'b' && up[2][2] == 'r')
                || (front[0][2] == 'b' && right[0][0] == 'r' && up[2][2] == 'w') || (front[0][2] == 'b' && right[0][0] == 'w' && up[2][2] == 'r')) {
                while (front[2][2] != 'w' || right[2][0] != 'b' || down[0][2] != 'r') {
                    Right_special_comb();
                }
            }

            if ((front[0][2] == 'w' && right[0][0] == 'g' && up[2][2] == 'r') || (front[0][2] == 'w' && right[0][0] == 'r' && up[2][2] == 'g')
                || (front[0][2] == 'r' && right[0][0] == 'w' && up[2][2] == 'g') || (front[0][2] == 'r' && right[0][0] == 'g' && up[2][2] == 'w')
                || (front[0][2] == 'g' && right[0][0] == 'r' && up[2][2] == 'w') || (front[0][2] == 'g' && right[0][0] == 'w' && up[2][2] == 'r')) {
                Up();
                while (front[2][0] != 'w' || left[2][2] != 'g' || down[0][0] != 'r') {
                    Front_special_comb();
                }
            }

            if ((front[0][2] == 'b' && right[0][0] == 'y' && up[2][2] == 'r') || (front[0][2] == 'b' && right[0][0] == 'r' && up[2][2] == 'y')
                || (front[0][2] == 'r' && right[0][0] == 'b' && up[2][2] == 'y') || (front[0][2] == 'r' && right[0][0] == 'y' && up[2][2] == 'b')
                || (front[0][2] == 'y' && right[0][0] == 'b' && up[2][2] == 'r') || (front[0][2] == 'y' && right[0][0] == 'r' && up[2][2] == 'b')) {
                UpB();
                while (back[2][0] != 'y' || right[2][2] != 'b' || down[2][2] != 'r') {
                    Back_special_comb();
                }
            }

            if ((front[0][2] == 'r' && right[0][0] == 'g' && up[2][2] == 'y') || (front[0][2] == 'r' && right[0][0] == 'y' && up[2][2] == 'g')
                || (front[0][2] == 'y' && right[0][0] == 'r' && up[2][2] == 'g') || (front[0][2] == 'y' && right[0][0] == 'g' && up[2][2] == 'r')
                || (front[0][2] == 'g' && right[0][0] == 'y' && up[2][2] == 'r') || (front[0][2] == 'g' && right[0][0] == 'r' && up[2][2] == 'y')) {
                Up();
                Up();
                while (back[2][2] != 'y' || left[2][0] != 'g' || down[2][0] != 'r') {
                    Left_special_comb();
                }
            } //рассмотрен первый угол сверху

            if ((right[0][2] == 'r' && back[0][0] == 'b' && up[0][2] == 'w') || (right[0][2] == 'r' && back[0][0] == 'w' && up[0][2] == 'b')
                || (right[0][2] == 'w' && back[0][0] == 'r' && up[0][2] == 'b') || (right[0][2] == 'w' && back[0][0] == 'b' && up[0][2] == 'r')
                || (right[0][2] == 'b' && back[0][0] == 'r' && up[0][2] == 'w') || (right[0][2] == 'b' && back[0][0] == 'w' && up[0][2] == 'r')) {
                Up();
                while (front[2][2] != 'w' || right[2][0] != 'b' || down[0][2] != 'r') {
                    Right_special_comb();
                }
            }

            if ((right[0][2] == 'w' && back[0][0] == 'g' && up[0][2] == 'r') || (right[0][2] == 'w' && back[0][0] == 'r' && up[0][2] == 'g')
                || (right[0][2] == 'r' && back[0][0] == 'w' && up[0][2] == 'g') || (right[0][2] == 'r' && back[0][0] == 'g' && up[0][2] == 'w')
                || (right[0][2] == 'g' && back[0][0] == 'r' && up[0][2] == 'w') || (right[0][2] == 'g' && back[0][0] == 'w' && up[0][2] == 'r')) {
                Up();
                Up();
                while (front[2][0] != 'w' || left[2][2] != 'g' || down[0][0] != 'r') {
                    Front_special_comb();
                }
            }

            if ((right[0][2] == 'b' && back[0][0] == 'y' && up[0][2] == 'r') || (right[0][2] == 'b' && back[0][0] == 'r' && up[0][2] == 'y')
                || (right[0][2] == 'r' && back[0][0] == 'b' && up[0][2] == 'y') || (right[0][2] == 'r' && back[0][0] == 'y' && up[0][2] == 'b')
                || (right[0][2] == 'y' && back[0][0] == 'b' && up[0][2] == 'r') || (right[0][2] == 'y' && back[0][0] == 'r' && up[0][2] == 'b')) {
                while (back[2][0] != 'y' || right[2][2] != 'b' || down[2][2] != 'r') {
                    Back_special_comb();
                }
            }

            if ((right[0][2] == 'r' && back[0][0] == 'g' && up[0][2] == 'y') || (right[0][2] == 'r' && back[0][0] == 'y' && up[0][2] == 'g')
                || (right[0][2] == 'y' && back[0][0] == 'r' && up[0][2] == 'g') || (right[0][2] == 'y' && back[0][0] == 'g' && up[0][2] == 'r')
                || (right[0][2] == 'g' && back[0][0] == 'y' && up[0][2] == 'r') || (right[0][2] == 'g' && back[0][0] == 'r' && up[0][2] == 'y')) {
                UpB();
                while (back[2][2] != 'y' || left[2][0] != 'g' || down[2][0] != 'r') {
                    Left_special_comb();
                }
            } //рассмотрен второй угол сверху

            if ((back[0][2] == 'r' && left[0][0] == 'b' && up[0][0] == 'w') || (back[0][2] == 'r' && left[0][0] == 'w' && up[0][0] == 'b')
                || (back[0][2] == 'w' && left[0][0] == 'r' && up[0][0] == 'b') || (back[0][2] == 'w' && left[0][0] == 'b' && up[0][0] == 'r')
                || (back[0][2] == 'b' && left[0][0] == 'r' && up[0][0] == 'w') || (back[0][2] == 'b' && left[0][0] == 'w' && up[0][0] == 'r')) {
                UpB();
                UpB();
                while (front[2][2] != 'w' || right[2][0] != 'b' || down[0][2] != 'r') {
                    Right_special_comb();
                }
            }

            if ((back[0][2] == 'w' && left[0][0] == 'g' && up[0][0] == 'r') || (back[0][2] == 'w' && left[0][0] == 'r' && up[0][0] == 'g')
                || (back[0][2] == 'r' && left[0][0] == 'w' && up[0][0] == 'g') || (back[0][2] == 'r' && left[0][0] == 'g' && up[0][0] == 'w')
                || (back[0][2] == 'g' && left[0][0] == 'r' && up[0][0] == 'w') || (back[0][2] == 'g' && left[0][0] == 'w' && up[0][0] == 'r')) {
                UpB();
                while (front[2][0] != 'w' || left[2][2] != 'g' || down[0][0] != 'r') {
                    Front_special_comb();
                }
            }

            if ((back[0][2] == 'b' && left[0][0] == 'y' && up[0][0] == 'r') || (back[0][2] == 'b' && left[0][0] == 'r' && up[0][0] == 'y')
                || (back[0][2] == 'r' && left[0][0] == 'b' && up[0][0] == 'y') || (back[0][2] == 'r' && left[0][0] == 'y' && up[0][0] == 'b')
                || (back[0][2] == 'y' && left[0][0] == 'b' && up[0][0] == 'r') || (back[0][2] == 'y' && left[0][0] == 'r' && up[0][0] == 'b')) {
                Up();
                while (back[2][0] != 'y' || right[2][2] != 'b' || down[2][2] != 'r') {
                    Back_special_comb();
                }
            }

            if ((back[0][2] == 'r' && left[0][0] == 'g' && up[0][0] == 'y') || (back[0][2] == 'r' && left[0][0] == 'y' && up[0][0] == 'g')
                || (back[0][2] == 'y' && left[0][0] == 'r' && up[0][0] == 'g') || (back[0][2] == 'y' && left[0][0] == 'g' && up[0][0] == 'r')
                || (back[0][2] == 'g' && left[0][0] == 'y' && up[0][0] == 'r') || (back[0][2] == 'g' && left[0][0] == 'r' && up[0][0] == 'y')) {
                while (back[2][2] != 'y' || left[2][0] != 'g' || down[2][0] != 'r') {
                    Left_special_comb();
                }
            } //рассмотрен третий угол сверху

            if ((left[0][2] == 'r' && front[0][0] == 'b' && up[2][0] == 'w') || (left[0][2] == 'r' && front[0][0] == 'w' && up[2][0] == 'b')
                || (left[0][2] == 'w' && front[0][0] == 'r' && up[2][0] == 'b') || (left[0][2] == 'w' && front[0][0] == 'b' && up[2][0] == 'r')
                || (left[0][2] == 'b' && front[0][0] == 'r' && up[2][0] == 'w') || (left[0][2] == 'b' && front[0][0] == 'w' && up[2][0] == 'r')) {
                UpB();
                while (front[2][2] != 'w' || right[2][0] != 'b' || down[0][2] != 'r') {
                    Right_special_comb();
                }
            }

            if ((left[0][2] == 'w' && front[0][0] == 'g' && up[2][0] == 'r') || (left[0][2] == 'w' && front[0][0] == 'r' && up[2][0] == 'g')
                || (left[0][2] == 'r' && front[0][0] == 'w' && up[2][0] == 'g') || (left[0][2] == 'r' && front[0][0] == 'g' && up[2][0] == 'w')
                || (left[0][2] == 'g' && front[0][0] == 'r' && up[2][0] == 'w') || (left[0][2] == 'g' && front[0][0] == 'w' && up[2][0] == 'r')) {
                while (front[2][0] != 'w' || left[2][2] != 'g' || down[0][0] != 'r') {
                    Front_special_comb();
                }
            }

            if ((left[0][2] == 'b' && front[0][0] == 'y' && up[2][0] == 'r') || (left[0][2] == 'b' && front[0][0] == 'r' && up[2][0] == 'y')
                || (left[0][2] == 'r' && front[0][0] == 'b' && up[2][0] == 'y') || (left[0][2] == 'r' && front[0][0] == 'y' && up[2][0] == 'b')
                || (left[0][2] == 'y' && front[0][0] == 'b' && up[2][0] == 'r') || (left[0][2] == 'y' && front[0][0] == 'r' && up[2][0] == 'b')) {
                Up();
                Up();
                while (back[2][0] != 'y' || right[2][2] != 'b' || down[2][2] != 'r') {
                    Back_special_comb();
                }
            }

            if ((left[0][2] == 'r' && front[0][0] == 'g' && up[2][0] == 'y') || (left[0][2] == 'r' && front[0][0] == 'y' && up[2][0] == 'g')
                || (left[0][2] == 'y' && front[0][0] == 'r' && up[2][0] == 'g') || (left[0][2] == 'y' && front[0][0] == 'g' && up[2][0] == 'r')
                || (left[0][2] == 'g' && front[0][0] == 'y' && up[2][0] == 'r') || (left[0][2] == 'g' && front[0][0] == 'r' && up[2][0] == 'y')) {
                Up();
                while (back[2][2] != 'y' || left[2][0] != 'g' || down[2][0] != 'r') {
                    Left_special_comb();
                }
            } //рассмотрен четвертый угол сверху

            if (front[2][2] != 'w' || right[2][0] != 'b' || down[0][2] != 'r') {
                if (front[2][2] == 'r' || right[2][0] == 'r' || down[0][2] == 'r') {
                    Right_special_comb();
                }
            }

            if (front[2][0] != 'w' || left[2][2] != 'g' || down[0][0] != 'r') {
                if (front[2][0] == 'r' || left[2][2] == 'r' || down[0][0] == 'r') {
                    Front_special_comb();
                }
            }

            if (back[2][0] != 'y' || right[2][2] != 'b' || down[2][2] != 'r') {
                if (back[2][0] == 'r' || right[2][2] == 'r' || down[2][2] == 'r') {
                    Back_special_comb();
                }
            }

            if (back[2][2] != 'y' || left[2][0] != 'g' || down[2][0] != 'r') {
                if (back[2][2] == 'r' || left[2][0] == 'r' || down[2][0] == 'r') {
                    Left_special_comb();
                }
            }
            // первый слой собран
        }
    }

    void second() {
        int p, c;

        while (!check_middle()) {
            c = 0;
            p = -1;

            while (c > p) {
                p = c;

                if (front[0][1] == 'w' || right[0][1] == 'w' || back[0][1] == 'w' || left[0][1] == 'w') {
                    if (front[0][1] != 'w') {
                        while (front[0][1] != 'w') {
                            Up();
                        }
                    }

                    if (up[2][1] == 'b') {
                        c++;
                        Up();
                        Right_special_comb();
                        FrontB();
                        UpB();
                        Front();
                        Up();
                    }
                    else if (up[2][1] == 'g') {
                        c++;
                        UpB();
                        LeftB();
                        UpB();
                        Left();
                        Up();
                        Front_special_comb();
                    }
                }

                if (front[0][1] == 'b' || right[0][1] == 'b' || back[0][1] == 'b' || left[0][1] == 'b') {
                    if (right[0][1] != 'b') {
                        while (right[0][1] != 'b') {
                            Up();
                        }
                    }

                    if (up[1][2] == 'y') {
                        c++;
                        Up();
                        Back_special_comb();
                        RightB();
                        UpB();
                        Right();
                        Up();
                    }
                    else if (up[1][2] == 'w') {
                        c++;
                        UpB();
                        FrontB();
                        UpB();
                        Front();
                        Up();
                        Right_special_comb();
                    }
                }

                if (front[0][1] == 'y' || right[0][1] == 'y' || back[0][1] == 'y' || left[0][1] == 'y') {
                    if (back[0][1] != 'y') {
                        while (back[0][1] != 'y') {
                            Up();
                        }
                    }

                    if (up[0][1] == 'g') {
                        c++;
                        Up();
                        Left_special_comb();
                        BackB();
                        UpB();
                        Back();
                        Up();
                    }
                    else if (up[0][1] == 'b') {
                        c++;
                        UpB();
                        RightB();
                        UpB();
                        Right();
                        Up();
                        Back_special_comb();
                    }
                }

                if (front[0][1] == 'g' || right[0][1] == 'g' || back[0][1] == 'g' || left[0][1] == 'g') {
                    if (left[0][1] != 'g') {
                        while (left[0][1] != 'g') {
                            Up();
                        }
                    }

                    if (up[1][0] == 'w') {
                        c++;
                        Up();
                        Front_special_comb();
                        LeftB();
                        UpB();
                        Left();
                        Up();
                    }
                    else if (up[1][0] == 'y') {
                        c++;
                        UpB();
                        BackB();
                        UpB();
                        Back();
                        Up();
                        Left_special_comb();
                    }
                }
            }

            if (front[1][2] != 'w') {
                Right_special_comb();
                FrontB();
                UpB();
                Front();
                Up();
            }

            if (right[1][2] != 'b') {
                Back_special_comb();
                RightB();
                UpB();
                Right();
                Up();
            }

            if (back[1][2] != 'y') {
                Left_special_comb();
                BackB();
                UpB();
                Back();
                Up();
            }

            if (left[1][2] != 'g') {
                Front_special_comb();
                LeftB();
                UpB();
                Left();
                Up();
            }
        }
    }

    void third() {
        while (!orange_cross_check()) {
            if (up[0][1] == 'o' && up[1][2] == 'o' && up[2][1] != 'o' && up[1][0] != 'o') {
                Left();
                Front_special_comb();
                LeftB();
            }
            else if (up[0][1] == 'o' && up[1][2] != 'o' && up[2][1] == 'o' && up[1][0] != 'o') {
                Left();
                Front_special_comb();
                LeftB();
            }
            else if (up[0][1] != 'o' && up[1][2] == 'o' && up[2][1] == 'o' && up[1][0] != 'o') {
                Back();
                Left_special_comb();
                BackB();
            }
            else if (up[0][1] == 'o' && up[1][2] != 'o' && up[2][1] != 'o' && up[1][0] == 'o') {
                Front();
                Right_special_comb();
                FrontB();
            }
            else if (up[0][1] != 'o' && up[1][2] == 'o' && up[2][1] != 'o' && up[1][0] == 'o') {
                Front();
                Right_special_comb();
                FrontB();
            }
            else if (up[0][1] != 'o' && up[1][2] != 'o' && up[2][1] == 'o' && up[1][0] == 'o') {
                Right();
                Back_special_comb();
                RightB();
            }
            else if (up[0][1] != 'o' && up[1][2] != 'o' && up[2][1] != 'o' && up[1][0] != 'o') {
                Front();
                Right_special_comb();
                FrontB();
            }
        } // собран оранжевый крест наверху

        bool ch = false;
        if (check_orange_right_cross())
            ch = true;
        else {
            Up();
            if (check_orange_right_cross())
                ch = true;
            else {
                Up();
                if (check_orange_right_cross())
                    ch = true;
                else {
                    Up();
                    if (check_orange_right_cross())
                        ch = true;
                }
            } // проверка на правильный оранжевый крест
        }

        if (!ch) {
            while (!check_orange_right_cross()) {
                Up();
                if (right[0][1] == 'b' && back[0][1] == 'y') {
                    Right();
                    Up();
                    RightB();
                    Up();
                    Right();
                    Up();
                    Up();
                    RightB();
                    Up();
                }
                else if (right[0][1] == 'b' && front[0][1] == 'w') {
                    Front();
                    Up();
                    FrontB();
                    Up();
                    Front();
                    Up();
                    Up();
                    FrontB();
                    Up();
                }
                else if (front[0][1] == 'w' && left[0][1] == 'g') {
                    Left();
                    Up();
                    LeftB();
                    Up();
                    Left();
                    Up();
                    Up();
                    LeftB();
                    Up();
                }
                else if (left[0][1] == 'g' && back[0][1] == 'y') {
                    Back();
                    Up();
                    BackB();
                    Up();
                    Back();
                    Up();
                    Up();
                    BackB();
                    Up();
                }
                else if (right[0][1] == 'b' && left[0][1] == 'g') {
                    Back();
                    Up();
                    BackB();
                    Up();
                    Back();
                    Up();
                    Up();
                    BackB();
                }
                else if (front[0][1] == 'w' && back[0][1] == 'y') {
                    Right();
                    Up();
                    RightB();
                    Up();
                    Right();
                    Up();
                    Up();
                    RightB();
                }
            }
        }  // собран правильный оранжевый крест

        cout << "The right orange cross" << endl;
        show();

        while (!check_upper_edges()) {
            if ((up[2][2] == 'o' && front[0][2] == 'w' && right[0][0] == 'b') || (up[2][2] == 'o' && front[0][2] == 'b' && right[0][0] == 'w') ||
                (up[2][2] == 'w' && front[0][2] == 'o' && right[0][0] == 'b') || (up[2][2] == 'w' && front[0][2] == 'b' && right[0][0] == 'o') ||
                (up[2][2] == 'b' && front[0][2] == 'w' && right[0][0] == 'o') || (up[2][2] == 'b' && front[0][2] == 'o' && right[0][0] == 'w')) {
                Up();
                Right();
                UpB();
                LeftB();
                Up();
                RightB();
                UpB();
                Left();
            }
            else if ((front[0][0] == 'w' && left[0][2] == 'g' && up[2][0] == 'o') || (front[0][0] == 'w' && left[0][2] == 'o' && up[2][0] == 'g') ||
                (front[0][0] == 'g' && left[0][2] == 'o' && up[2][0] == 'w') || (front[0][0] == 'g' && left[0][2] == 'w' && up[2][0] == 'o') ||
                (front[0][0] == 'o' && left[0][2] == 'g' && up[2][0] == 'w') || (front[0][0] == 'o' && left[0][2] == 'w' && up[2][0] == 'g')) {
                Up();
                Front();
                UpB();
                BackB();
                Up();
                FrontB();
                UpB();
                Back();
            }
            else if ((up[0][2] == 'o' && right[0][2] == 'b' && back[0][0] == 'y') || (up[0][2] == 'o' && right[0][2] == 'y' && back[0][0] == 'b') ||
                (up[0][2] == 'b' && right[0][2] == 'o' && back[0][0] == 'y') || (up[0][2] == 'b' && right[0][2] == 'y' && back[0][0] == 'o') ||
                (up[0][2] == 'y' && right[0][2] == 'b' && back[0][0] == 'o') || (up[0][2] == 'y' && right[0][2] == 'o' && back[0][0] == 'b')) {
                Up();
                Back();
                UpB();
                FrontB();
                Up();
                BackB();
                UpB();
                Front();
            }
            else if ((up[0][0] == 'o' && back[0][2] == 'y' && left[0][0] == 'g') || (up[0][0] == 'o' && back[0][2] == 'g' && left[0][0] == 'y') ||
                (up[0][0] == 'g' && back[0][2] == 'y' && left[0][0] == 'o') || (up[0][0] == 'g' && back[0][2] == 'o' && left[0][0] == 'y') ||
                (up[0][0] == 'y' && back[0][2] == 'o' && left[0][0] == 'g') || (up[0][0] == 'y' && back[0][2] == 'g' && left[0][0] == 'o')) {
                Up();
                Left();
                UpB();
                RightB();
                Up();
                LeftB();
                UpB();
                Right();
            }
            else {
                Up();
                Right();
                UpB();
                LeftB();
                Up();
                RightB();
                UpB();
                Left();
            }
        }   //Правильно поставлены углы

        for (int i = 0; i < 5; i++) {
            if (up[0][0] != 'o')
                while (up[0][0] != 'o') {
                    Back();
                    Down();
                    BackB();
                    DownB();
                }
            Up();
        } // Собрана оранжевая грань

        while (!isBuild()) {
            Up();
        }// доворачиваем последнюю сторону и кубик собран

    }

    void solve() {
        first();
        cout << "First layer is done" << endl;
        show();
        second();
        cout << "Second layer is done" << endl;
        show();
        third();
        cout << "Third layer is done" << endl;
        show();

    }

    bool isBuild() { 
        bool ch = true;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (front[i][j] != 'w' || right[i][j] != 'b' || back[i][j] != 'y' || left[i][j] != 'g' || up[i][j] != 'o' || down[i][j] != 'r') {
                    ch = false;
                }
            }
        }

        return ch;
    }

    bool check_upper_edges() {
        return ((up[2][2] == 'o' && front[0][2] == 'w' && right[0][0] == 'b') ||
            (up[2][2] == 'o' && front[0][2] == 'b' && right[0][0] == 'w') ||
            (up[2][2] == 'w' && front[0][2] == 'o' && right[0][0] == 'b') ||
            (up[2][2] == 'w' && front[0][2] == 'b' && right[0][0] == 'o') ||
            (up[2][2] == 'b' && front[0][2] == 'w' && right[0][0] == 'o') ||
            (up[2][2] == 'b' && front[0][2] == 'o' && right[0][0] == 'w')) &&
            ((front[0][0] == 'w' && left[0][2] == 'g' && up[2][0] == 'o') ||
                (front[0][0] == 'w' && left[0][2] == 'o' && up[2][0] == 'g') ||
                (front[0][0] == 'g' && left[0][2] == 'o' && up[2][0] == 'w') ||
                (front[0][0] == 'g' && left[0][2] == 'w' && up[2][0] == 'o') ||
                (front[0][0] == 'o' && left[0][2] == 'g' && up[2][0] == 'w') ||
                (front[0][0] == 'o' && left[0][2] == 'w' && up[2][0] == 'g')) &&
            ((up[0][2] == 'o' && right[0][2] == 'b' && back[0][0] == 'y') ||
                (up[0][2] == 'o' && right[0][2] == 'y' && back[0][0] == 'b') ||
                (up[0][2] == 'b' && right[0][2] == 'o' && back[0][0] == 'y') ||
                (up[0][2] == 'b' && right[0][2] == 'y' && back[0][0] == 'o') ||
                (up[0][2] == 'y' && right[0][2] == 'b' && back[0][0] == 'o') ||
                (up[0][2] == 'y' && right[0][2] == 'o' && back[0][0] == 'b')) &&
            ((up[0][0] == 'o' && back[0][2] == 'y' && left[0][0] == 'g') ||
                (up[0][0] == 'o' && back[0][2] == 'g' && left[0][0] == 'y') ||
                (up[0][0] == 'g' && back[0][2] == 'y' && left[0][0] == 'o') ||
                (up[0][0] == 'g' && back[0][2] == 'o' && left[0][0] == 'y') ||
                (up[0][0] == 'y' && back[0][2] == 'o' && left[0][0] == 'g') ||
                (up[0][0] == 'y' && back[0][2] == 'g' && left[0][0] == 'o'));
    }

    bool orange_cross_check() {
        bool ch = true;

        if (up[0][1] != 'o' || up[1][2] != 'o' || up[2][1] != 'o' || up[1][0] != 'o') {
            ch = false;
        }

        return ch;
    }

    bool check_orange_right_cross() {
        bool ch = true;

        if (up[0][1] != 'o' || up[1][2] != 'o' || up[2][1] != 'o' || up[1][0] != 'o') {
            ch = false;
        }

        if (front[0][1] != 'w' || right[0][1] != 'b' || back[0][1] != 'y' || left[0][1] != 'g') {
            ch = false;
        }

        return ch;
    }

    bool check_middle() {
        bool ch = true;

        if (front[1][0] != 'w' || front[1][2] != 'w' || right[1][0] != 'b' || right[1][2] != 'b' || back[1][0] != 'y' || back[1][2] != 'y' || left[1][0] != 'g' || left[1][2] != 'g') {
            ch = false;
        }

        return ch;
    }

    bool check_edge() {
        bool ch = true;

        for (int i = 0; i < 3; i++) {
            if (front[2][i] != 'w' || right[2][i] != 'b' || back[2][i] != 'y' || left[2][i] != 'g') {
                ch = false;
            }
        }

        if (down[0][0] != 'r' || down[0][2] != 'r' || down[2][0] != 'r' || down[2][2] != 'r') {
            ch = false;
        }

        return ch;
    }

    bool check1() {
        bool ch = true;

        if (up[2][1] != 'r' || front[0][1] != 'w') {
            ch = false;
        }

        return ch;
    }

    bool check2() {
        bool ch = true;

        if (up[1][2] != 'r' || right[0][1] != 'b') {
            ch = false;
        }

        return ch;
    }

    bool check3() {
        bool ch = true;

        if (up[0][1] != 'r' || back[0][1] != 'y') {
            ch = false;
        }

        return ch;
    }

    bool check4() {
        bool ch = true;

        if (up[1][0] != 'r' || left[0][1] != 'g') {
            ch = false;
        }

        return ch;
    }

    // визуализация
    // номер грани для поворотов
    int current;

    void clear(double size, unsigned int* color) {
        this->size = size;
        current = -1;

        int i, j, k;
        for (i = 0; i < 6; i++) {
            this->color[i] = color[i];
        }

        // верх
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                a[i][j][2].setColor(0, color[0]);
            }
        }
        // низ
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                a[i][j][0].setColor(1, color[1]);
            }
        }
        // спереди
        for (k = 0; k < 3; k++) {
            for (j = 0; j < 3; j++) {
                a[j][0][k].setColor(2, color[2]);
            }
        }
        // сзади
        for (k = 0; k < 3; k++) {
            for (j = 0; j < 3; j++) {
                a[j][2][k].setColor(3, color[3]);
            }
        }
        // слева
        for (i = 0; i < 3; i++) {
            for (k = 0; k < 3; k++) {
                a[0][k][i].setColor(4, color[4]);
            }
        }
        // справа
        for (i = 0; i < 3; i++) {
            for (k = 0; k < 3; k++) {
                a[2][k][i].setColor(5, color[5]);
            }
        }
        // размеры мелких деталей
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                for (k = 0; k < 3; k++) {
                    a[i][j][k].size = (size / 3.0) * 0.95;
                }
            }
        }
        // возврат на начальное состояние
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                left[i][j] = 'g';
                front[i][j] = 'w';
                right[i][j] = 'b';
                back[i][j] = 'y';
                up[i][j] = 'o';
                down[i][j] = 'r';
            }
        }

    }

    void draw() {
        const double K = 0.75;
        // корпус - это просто куб черного цвета, размер которого равен K*size
        glPushMatrix();
        glColor3f(0, 0, 0);
        glTranslatef(((1.0 - K) / 2) * size + K * size / 2, ((1.0 - K) / 2) * size + K * size / 2, ((1.0 - K) / 2) * size + K * size / 2);
        glutSolidCube(size * K);
        glPopMatrix();

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                for (int k = 0; k < 3; ++k) {
                    ok[i][j][k] = true;
                }
            }
        }

        if (current != -1) {
            glPushMatrix();
            int i, j, k;

            if (current == 0 || current == 1) {
                k = (current & 1) * 2;
                for (i = 0; i < 3; i++) {
                    for (j = 0; j < 3; j++) {
                        ok[i][j][k] = false;
                    }
                }
                glTranslated(size / 2, size / 2, 0);   // сдвигаемся к центру
                glRotatef(rotate[current], 0, 0, 1);   // крутим
                glTranslated(-size / 2, -size / 2, 0); // сдвигаемся обратно
                // рисуем
                for (i = 0; i < 3; i++) {
                    for (j = 0; j < 3; j++) {
                        a[i][j][k].draw(size / 3 * i, size / 3 * j, size / 3 * k);
                    }
                }
            }
            else if (current == 2 || current == 3) {
                j = (current & 1) * 2;
                for (i = 0; i < 3; i++) {
                    for (k = 0; k < 3; k++) {
                        ok[i][j][k] = false;
                    }
                }

                glTranslated(size / 2, 0, size / 2);
                glRotatef(rotate[current], 0, 1, 0);
                glTranslated(-size / 2, 0, -size / 2);
                for (i = 0; i < 3; i++) {
                    for (k = 0; k < 3; k++) {
                        a[i][j][k].draw(size / 3 * i, size / 3 * j, size / 3 * k);
                    }
                }
            }
            else if (current == 4 || current == 5) {
                i = (current & 1) * 2;
                for (j = 0; j < 3; j++) {
                    for (k = 0; k < 3; k++) {
                        ok[i][j][k] = false;
                    }
                }

                glTranslated(0, size / 2, size / 2);
                glRotatef(rotate[current], 1, 0, 0);
                glTranslated(0, -size / 2, -size / 2);
                for (j = 0; j < 3; j++) {
                    for (k = 0; k < 3; k++) {
                        a[i][j][k].draw(size / 3 * i, size / 3 * j, size / 3 * k);
                    }
                }
            }
            glPopMatrix();
        }

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                for (int k = 0; k < 3; k++) {
                    if (ok[i][j][k]) {
                        a[i][j][k].draw(size / 3 * i, size / 3 * j, size / 3 * k);
                    }
                }
            }
        }
    }
    const int CUBE_SIZE = 13;
    int TIMER = 5;

    // проекции угла поворота на оси
    int xRot = 24;
    int yRot = 44;
    int zRot = 0;
    // отдаление
    double translateZ = -35.0;

    void display() {
        glPushMatrix();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glColor3f(1, 0, 0);
        glTranslatef(0, 0, translateZ);
        glRotatef(xRot, 1, 0, 0);
        glRotatef(yRot, 0, 1, 0);
        glTranslatef(CUBE_SIZE / -2.0, CUBE_SIZE / -2.0, CUBE_SIZE / -2.0);
        draw();
        glPopMatrix();
        glutSwapBuffers();
    }

    void rotate90(int id, int sign) {
        int i, j, k;
        // если sign = -1, значит крутим 3 раза
        if (sign == -1)
            sign = 3;

        while (sign--) {
            switch (id) {
            case 0:
                k = 0;
                // копируем повёрнутую на 90 градусов верхнюю/нижнюю грань
                // в массив tmp, затем грани присваиваем tmp
                for (i = 0; i < 3; i++)
                    for (j = 0; j < 3; j++)
                        tmp[j][2 - i] = a[i][j][k];

                for (i = 0; i < 3; i++) {
                    for (j = 0; j < 3; j++) {
                        tmp[i][j].rotate_x();
                        a[i][j][k] = tmp[i][j];
                    }
                }
                break;
            case 1:
                k = 2;
                for (i = 0; i < 3; i++)
                    for (j = 0; j < 3; j++)
                        tmp[j][2 - i] = a[i][j][k];

                for (i = 0; i < 3; i++) {
                    for (j = 0; j < 3; j++) {
                        tmp[i][j].rotate_x();
                        a[i][j][k] = tmp[i][j];
                    }
                }
                break;
            case 2:
                j = 0;
                for (i = 0; i < 3; i++)
                    for (k = 0; k < 3; k++)
                        tmp[k][2 - i] = a[i][j][k];

                for (i = 0; i < 3; i++) {
                    for (k = 0; k < 3; k++) {
                        tmp[i][k].rotate_z();
                        a[i][j][k] = tmp[i][k];
                    }
                }
                break;
            case 3:
                j = 2;
                for (i = 0; i < 3; i++)
                    for (k = 0; k < 3; k++)
                        tmp[k][2 - i] = a[i][j][k];

                for (i = 0; i < 3; i++) {
                    for (k = 0; k < 3; k++) {
                        tmp[i][k].rotate_z();
                        a[i][j][k] = tmp[i][k];
                    }
                }
                break;
            case 4:
                i = 0;
                for (j = 0; j < 3; j++)
                    for (k = 0; k < 3; k++)
                        tmp[k][2 - j] = a[i][j][k];

                for (j = 0; j < 3; j++) {
                    for (k = 0; k < 3; k++) {
                        tmp[j][k].rotate_y();
                        a[i][j][k] = tmp[j][k];
                    }
                }
                break;
            case 5:
                i = 2;
                for (j = 0; j < 3; j++)
                    for (k = 0; k < 3; k++)
                        tmp[k][2 - j] = a[i][j][k];

                for (j = 0; j < 3; j++) {
                    for (k = 0; k < 3; k++) {
                        tmp[j][k].rotate_y();
                        a[i][j][k] = tmp[j][k];
                    }
                }
                break;
            }
        }
    }

    void timer(int RorL, int) {
        if (current != -1) {
            display();
            Rotate(current, speed_torn, RorL);
        }
    }

    void Rotate(int idx, int angle, int RorL) {
        if (current == -1 || current == idx) {
            if (RorL == -1) {
                rotate[idx] -= angle;
            }
            else if (RorL == 1) {
                rotate[idx] += angle;
            }

            if (rotate[idx] % 90 != 0) {
                current = idx;
                timer(RorL, 0);
            }
            else {
                // угол стал кратным 90, поварачиваем на массиве
                if ((rotate[idx] < 0) ^ (current == 2 || current == 3)) {
                    rotate90(idx, 1);
                }
                else {
                    rotate90(idx, -1);
                }
                rotate[idx] = 0;
                current = -1;
            }
        }
    }
};

const int CUBE_SIZE = 13;
const int TIMER = 5;
// обозначаем цвета
unsigned int color[6] = { 0xFFFAF0, 0x4169E1, 0x7B001C, 0xCD5700, 0x00541F,  0xFFCF40};

// проекции угла поворота на оси
int xRot = 24;
int yRot = 44;
int zRot = 0;
// отдаление
double translateZ = -35.0;
// скорость поворота, должно быть 90 % speed_torn = 0
int speed_torn = 3;

Cube cube;

void display() {         //вывод на экран
    glPushMatrix();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1, 0, 0);
    glTranslatef(0, 0, translateZ);
    glRotatef(xRot, 1, 0, 0);
    glRotatef(yRot, 0, 1, 0);
    glTranslatef(CUBE_SIZE / -2.0, CUBE_SIZE / -2.0, CUBE_SIZE / -2.0);
    cube.draw();
    glPopMatrix();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat fAspect = (GLfloat)w / (GLfloat)h;
    gluPerspective(60, fAspect, 1, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void init() {              //инициализация экрана
    // фон
    glClearColor(0.69, 0.81, 0.98, 1.0);
    // освещение
    float ambientLight[] = { 0.9, 0.9, 0.9, 1.0 };
    glShadeModel(GL_SMOOTH);
    glMateriali(GL_FRONT, GL_SHININESS, 128);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    // инициализируем куб
    cube.clear(CUBE_SIZE, color);
}

void specialKeys(int key, int, int) {   //кнопки управления
    if (key == GLUT_KEY_DOWN) {
        xRot += 3;
        if (xRot >= 360)
            xRot -= 360;
        glutPostRedisplay();
    }

    if (key == GLUT_KEY_UP) {
        xRot -= 3;
        if (xRot < 0)
            xRot += 360;
        glutPostRedisplay();
    }

    if (key == GLUT_KEY_RIGHT) {
        yRot += 3;
        if (yRot >= 360)
            yRot -= 360;
        glutPostRedisplay();
    }

    if (key == GLUT_KEY_LEFT) {
        yRot -= 3;
        if (yRot < 0)
            yRot += 360;
        glutPostRedisplay();
    }
}

void keys(unsigned char key, int, int) {           //виды кнопок с действиями
    if (cube.current == -1 && key == '1') {
        cube.Left();
    }
    else if (cube.current == -1 && key == '2') {
        cube.Right();
    }
    else if (cube.current == -1 && key == '3') {
        cube.Front();
    }
    else if (cube.current == -1 && key == '4') {
        cube.Back();
    }
    else if (cube.current == -1 && key == '5') {
        cube.Up();
    }
    else if (cube.current == -1 && key == '6') {
        cube.Down();
    }
}

void timer(int) {
    glutTimerFunc(TIMER, timer, 0);

    if (cube.current != -1) {
        cube.Rotate(cube.current, speed_torn, -1);
    }

    display();
}

void processMenu(int option) {   //варианты команд
    switch (option) {
    case 1:
        cube.Random();
        break;
    case 2:
        cube.solve();
        break;
    }
}

void createGLUTMenus() {            //меню кнопок
    glutCreateMenu(processMenu);

    glutAddMenuEntry("Random", 1);
    glutAddMenuEntry("Solve", 2);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 700);
    glutInitWindowPosition(1, 1);
    glutCreateWindow("Cube");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keys);
    glutTimerFunc(TIMER, timer, 0);
    glutSpecialFunc(specialKeys);
    createGLUTMenus();
    glutMainLoop();

    return 0;
}




/*                                             
        0 1 2                                  
        3 4 5                                 
        6 7 8                          
 0 1 2  0 1 2  0 1 2  0 1 2           
 3 4 5  3 4 5  3 4 5  3 4 5           
 6 7 8  6 7 8  6 7 8  6 7 8           
        0 1 2                                 
        3 4 5                               
        6 7 8                              
*/
