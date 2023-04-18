#include "circle.h"
#include "triangle.h"
#include <vector>
#include <algorithm>
int main() {
    vector <Circle> c;
    vector <EquilateralTriangle> tr;
    cout << "Enter the number of figures: " << endl;
    int num; int cur; int n_c = 0, n_tr = 0;
    cin >> num;
    for (int i=0; i<num; i++){
        cout << "If you want to enter a circle, put 1\nIf you want to put a triangle, put 2";
        cin >> cur;
        if (cur == 1){
            Circle new_Circle;
            new_Circle.InitFromDialog();
            c.push_back(new_Circle);
            n_c++;
        }
        if (cur == 2){
            EquilateralTriangle new_tr;
            new_tr.InitFromDialog();
            tr.push_back(new_tr);
            n_tr++;
        }
    }
    ////////////////////// Отрисовываем
    for (int i=0; i<n_c; i++){
        c[i].draw();
    }
    for (int i=0; i<n_tr; i++){
        tr[i].draw();
    }
    //////////////////// Площадь
    int sum_area = 0;
    for (int i=0; i<n_c; i++){
        sum_area += c[i].area();
    }
    for (int i=0; i<n_tr; i++){
        sum_area += tr[i].area();
    }
    cout << "Sum of all area: " << sum_area << endl;

    ///////////////////// Периметр
    int sum_perimetr = 0;
    for (int i=0; i<n_c; i++){
        sum_perimetr += c[i].perimeter();
    }
    for (int i=0; i<n_tr; i++){
        sum_perimetr += tr[i].perimeter();
    }
    cout << "Sum of all Perimeters: " << sum_perimetr << endl;

    ////////////////////// Память
    int sum_of_date_cir = 0, sum_of_date_tr=0;
    for (int i=0; i<n_c; i++){
        sum_of_date_cir += c[i].size();
    }
    cout << "Sum of date for circles: " << sum_of_date_cir<< endl;
    for (int i=0; i<n_tr; i++){
        sum_of_date_tr += tr[i].size();
    }
    cout << "Sum of date for triangles: " << sum_of_date_tr<< endl;
    ///////////////////////  Центр масс всей системы - это сумма центров всех масс, умноженных на вектора
    int sum_of_x=0,sum_of_y=0, sum_of_mass=0;
    for (int i=0; i<n_c; i++){
        int curr_mass; CVector2D vec{};
        curr_mass = c[i].mass();
        vec = c[i].position();
        sum_of_mass += curr_mass;
        sum_of_x += vec.x * curr_mass;
        sum_of_y += vec.y * curr_mass;

    }
    for (int i=0; i<n_tr; i++){
        int curr_mass; CVector2D vec{};
        curr_mass = tr[i].mass();
        vec = tr[i].position();
        sum_of_mass += curr_mass;
        sum_of_x += vec.x * curr_mass;
        sum_of_y += vec.y * curr_mass;
    }
    double x_center,y_center;
    x_center = sum_of_x/sum_of_mass;
    y_center = sum_of_y/sum_of_mass;
    cout << "The center of mass of the system: x: "<<x_center<<" y: " << y_center << endl;

    return 0;
}
