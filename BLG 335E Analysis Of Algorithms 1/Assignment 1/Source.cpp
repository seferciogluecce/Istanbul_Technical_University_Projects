#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <math.h>
#include <time.h>
 
using namespace std;
 
class Warehouse{
    int Id;
    int x_axis;
    int y_axis;
public:
     
    float distance;
    Warehouse();
    int getX();
    int getY();
    void setId(int);
    void setX(int);
    void setY(int);
    void setDistance(float);
    void print(FILE *);
};
 
Warehouse::Warehouse(){
    Id = 0;
    x_axis = 0;
    y_axis = 0;
    distance = 0;
}
int Warehouse::getX(){
    return x_axis;
}
int Warehouse::getY(){
    return y_axis;
}
void Warehouse::setId(int id){
    Id = id;
}
void Warehouse::setX(int x){
    x_axis = x;
}
void Warehouse::setY(int y){
    y_axis = y;
}
void Warehouse::setDistance(float distance){
    this->distance = distance;
}
void Warehouse::print(FILE * output){
    fprintf(output, "%d\t%d\t%d\t%.2f\n", Id, x_axis, y_axis, distance);
}
 
void IS(Warehouse*, int, int, FILE *);
void MS(Warehouse*, int, int, int);
void Merge(Warehouse*, int, int, int, int);
void LS(Warehouse*, int, int, FILE*);
int Index(Warehouse*, int);
 
int main(int argc,char** argv) {
 
    FILE *input;
    FILE *output;
     
    char* sentence = new char[22];
     
    input = fopen("warehouselocations.txt", "r");
    output = fopen("output.txt", "w");
     
    int N = atoi(argv[1]);
    int K = atoi(argv[2]);
    string search_type = argv[3];
    int x = atoi(argv[4]);
    int y = atoi(argv[5]);
 
    int id, x_axis, y_axis;
    float distance;
    char* temp;
 
    Warehouse warehouse;
 
    Warehouse *array = new Warehouse[N];
 
    int i = 0;
    if (input == NULL) perror("Error opening file");
    while (i <= N-1){
 
        fgets(sentence, 22, input);
        temp=strtok(sentence,"\t");
        id = atoi(temp);
        temp = strtok(NULL, "\t");
        x_axis = atoi(temp);
        temp = strtok(NULL, "\t");
        y_axis = atoi(temp);
        distance = sqrt(pow((x_axis - x), 2) + pow((y_axis - y), 2));
         
        warehouse.setId(id);
        warehouse.setX(x_axis);
        warehouse.setY(y_axis);
        warehouse.setDistance(distance);
 
        array[i] = warehouse;
 
        i++;
    }
    clock_t time;
    if (search_type == "IS"){
        time = clock();
        IS(array, N, K, output);
        time = clock() - time;
         
        cout << "Time to Sort = " << ((float)time )/ CLOCKS_PER_SEC << endl;
    }
    else if (search_type == "MS"){
        time = clock();
        MS(array, 0, N - 1, N);
        for (int i = 0; i < K; i++){
            array[i].print(output);
        }
        time = clock() - time;
        cout << "Time to Sort = " << ((float)time) / CLOCKS_PER_SEC << endl;
    }
    else{
        time = clock();
        LS(array, N, K, output);
        time = clock() - time;
        cout << "Time to Sort = " << ((float)time) / CLOCKS_PER_SEC << endl;
    }
     
    fclose(input);
    delete[] array;
    return 0;
}
 
 
void IS(Warehouse* array, int N, int K, FILE *output){
    int j;
    Warehouse temp_object;
    for (int i = 1; i < N; i++){
        j = i;
        while (j>0 && array[j - 1].distance > array[j].distance){
            temp_object = array[j];
            array[j] = array[j - 1];
            array[j - 1] = temp_object;
            j--;
        }
    }
 
    for (int i = 0; i < K; i++){
        array[i].print(output);
    }
}
 
void MS(Warehouse* array, int lower_index, int upper_index, int N){
    int middle;
    if (lower_index < upper_index){
        middle = (lower_index + upper_index) / 2;
        MS(array, lower_index, middle, N);
        MS(array, middle + 1, upper_index, N);
        Merge(array, lower_index, middle, upper_index, N);
    }
     
}
 
void Merge(Warehouse* array, int lower_index, int middle, int upper_index, int N){
    Warehouse* temp_array = new Warehouse[N];
    int i, j, k;
    i = lower_index;
    k = lower_index;
    j = middle + 1;
 
    while (i <= middle && j <= upper_index){
        if (array[i].distance < array[j].distance){
            temp_array[k] = array[i];
            k++;
            i++;
        }
        else{
            temp_array[k] = array[j];
            k++;
            j++;
        }
    }
    while (i <= middle){
        temp_array[k] = array[i];
        k++;
        i++;
    }
    while (j <= upper_index){
        temp_array[k] = array[j];
        k++;
        j++;
    }
 
    for (i = lower_index; i < k; i++){
        array[i] = temp_array[i];
    }
    delete[]temp_array;
}
 
void LS(Warehouse* array, int N, int K, FILE* output){
     
    Warehouse* temp_array = new Warehouse[K];
    for (int i = 0; i < K; i++){
        temp_array[i] = array[i];
    }
    int max_index = Index(temp_array, K);
 
    for (int j = K; j < N; j++){
        if (array[j].distance < temp_array[max_index].distance){
            temp_array[max_index] = array[j];
            max_index = Index(temp_array, K);
        }
    }
 
    for (int i = 0; i < K; i++){
        temp_array[i].print(output);
         
    }
    delete[]temp_array;
}
 
int Index(Warehouse*array, int K){
    int index = 0;
    int max_distance = 0;
    for (int i = 0; i < K; i++){ // en büyük distancela hangi elementte olduðunu bulduk
        if (array[i].distance > max_distance){
            max_distance = array[i].distance;
            index = i;
        }
    }
    return index;
}
