/*Zachary Cushon
This code is my own work, I did not recieve help from any outside sources that are not referenced*/
#include <thread>   
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <random>
using namespace std;

// mutex lockg
pthread_mutex_t mlock;

// bool variable accesible by all threads
bool complete = false;

int* arr;
int **matrix;
int dim;

// Helper function
bool evaluation(int location){
    if (location == 1)
        return true;
    else
        return false;
}

// This code was used so frequently I thought it was ridiculous not to use a method.
void adjust(int location,int &count1, int &count0){
    if (location == 1)
            count1++;
        else
            count0++;
}

// Helper function referenced from stack overflow to print the matrix in a clean manner 
//https://stackoverflow.com/questions/69962700/print-2d-array-as-a-method-in-c
void printMatrix(int** matrix,int dim) {
    cout<<"\n Printing Matrix : \n";
    for(int i=0 ; i<=dim-1 ; i++) {
        for(int j=0 ; j<=dim-1 ; j++)
            cout<< *(*(matrix+i)+j)<<" ";
        cout<<endl;
    }
    cout<<endl;
}

void *supervisor(void* pArg){
    
    int flag = dim * dim;
    int oc1,oc0;

    while (!complete){
        oc1 = 0;
        oc0 = 0;
        pthread_mutex_lock(&mlock);
        for (int i = 0; i < dim; i++)
            for (int j = 0; j < dim; j++)
                if (matrix[i][j] == 1)
                    oc1++;
                else   
                    oc0++;
            if (oc0 >= flag || oc1 >= flag)
                complete = true;
            else 
                complete = false; //just to make sure
        pthread_mutex_unlock(&mlock);       
    }
    pthread_exit(NULL);
    return (void*) 0;
}

void *checkSpot(void* pArg){
    int posI;
    int posJ;
    while (!complete){        
        //srand(time(NULL));
        posI = rand() % (dim) + 0; // Random column from 0 to dimension
        posJ = rand() % (dim) + 0; // Random Row from 0 to dimension

        cout << "EVALUATING POSITION " << posI << ","<< posJ<<endl; // Telling professor Vajda what is going on
        cout << "Initial Matrix:" << endl;
        printMatrix(matrix,dim);

        int count1 = 0;
        int count0 = 0; // Start counting occurences of 1's and 0's
        
        
        bool edge = false;
        bool corner = false; // denotes if the starting index is an edge piece or corner piece.
        bool upper;
        bool left;

       

        
        
        // This code is to help classify our selection
        
        if ((posI != 0 && posI != dim-1) && (posJ != 0 && posJ != dim -1)){
            edge = false;
        }
        else if ((posI == 0 || posI == dim-1) && (posJ == 0 || posJ == dim-1)){
            corner = true;
            edge = false;
        } else {
            edge = true;
        }
        pthread_mutex_lock(&mlock);
        //positions to be modified to perserved the initial position in memory    
        int currentI = posI;
        int currentJ = posJ;

        // Inner piece algorithm
        if (!edge && !corner){
            
        
            
            // Count occurences of 1s and 0s
            for (int i = -1; i <1; i++)
                for (int j = -1; j < 1; j++){
                    if (!((i == 0) && (j == 0))){
                        adjust(matrix[posI+i][posJ+j], count1, count0);
                    }
                    // Compare the counted up variables
                    if (count1 > count0){
                        matrix[posI][posJ] = 1;
                    }else{
                        matrix[posI][posJ] = 0;
                    }
                
                }
        // Corner Algorithm   
        } else if (corner || (corner && edge)) {
            
            // Collect Information-------------
            
            // Upper or lower extreme
            if (posI == 0)
                upper = true;
            else
                upper = false;
            
            // Left or right extreme
            if (posJ == 0) // left bound
                left = true;
            else// right bound
                left = false;

            // Start alg----------------------        

            // Vertical traversal depending on information gathered, prevents out of bounds

            if (upper)
                currentI += 1;
            else
                currentI -= 1;

            // Adjust count now that we have moved up or down one
            adjust(matrix[currentI][currentJ], count1, count0);
            
            // Horizontal Traversal, again avoiding index out of bounds/seg faults

            if (left)
                currentJ += 1;
            else
                currentJ -=1;

            // Adjust count now that we have moved left or right one
            adjust(matrix[currentI][currentJ], count1, count0);

            // Account for up or down again
            if (upper)
                currentI -= 1;
            else
                currentI += 1;

            // Final eval
            adjust(matrix[currentI][currentJ], count1, count0);
            
            // Cast votes
            if (count1 > count0)
                matrix[posI][posJ]= 1;
            else
                matrix[posI][posJ]= 0;

        } else if (edge && !corner){
            if (posJ == 0 || posJ == dim){
            //Specific edge
            
            // Decide if this is on the left or right ledge
            if (posJ == 0)
                left = true;
            else
                left = false;

            

            currentI++;

            adjust(matrix[currentI][currentJ], count1, count0);

            currentI-=2;

            adjust(matrix[currentI][currentJ], count1, count0);
            
            currentI += 2;

            if (left)
                currentJ++;
            else    
                currentJ--;

            adjust(matrix[currentI][currentJ], count1, count0);

            for (int i = 0; i < 2; i++){
                currentI--;
                adjust(matrix[currentI][currentJ], count1, count0); 
            }    

                // Cast votes
            if (count1 > count0)
                matrix[posI][posJ]= 1;
            else
                matrix[posI][posJ]= 0;   

        }else{

                if (posI == 0) // upper bound
                    upper = true;
                else// lower bound
                    upper = false;

            currentJ--;

                adjust(matrix[currentI][currentJ], count1, count0);

            if (upper)
                currentI++;
                else
                currentI--;

            adjust(matrix[currentI][currentJ], count1, count0);

                for (int i = 0; i < 1; i++){
                    currentJ++;
                    adjust(matrix[currentI][currentJ], count1, count0);
                }
                if (count1 > count0)
                matrix[posI][posJ]= 1;
            else
                matrix[posI][posJ]= 0;
        }
        }
        cout << "Changed matrix:"<<endl;
        printMatrix(matrix,dim);
        // Mutex unlock now that we are out of critical section 
        pthread_mutex_unlock(&mlock);
        
        
        
    }
    pthread_exit(NULL);
    return (void*)0;
}


/* Generates a matrix of Dimension x Dimension
   Which is then populated with random (1/0) and returned*/

int** genMatrix(){
    
    srand(time(0));
    int** matrix = 0;
    matrix = new int*[dim];

    for (int i = 0; i < dim; i++){
        matrix[i] = new int[dim];
        for (int j = 0; j < dim; j++){
            matrix[i][j] = (rand() % 2 + 1)-1;
        }
    }
    return matrix;
}

void problem1(int m,int dim){

    pthread_t threadArray[m];
    pthread_t T[1];
    // initalize each thread in the array
    pthread_create(&T[0],NULL,supervisor,NULL); 
    for (int i = 0; i < m; i++){
     pthread_create(&threadArray[i],NULL, checkSpot,NULL);
    }
    pthread_join(T[0],NULL);
    for (int i = 0; i < m; i++){
     pthread_join(threadArray[i],NULL);
    }
    cout << "Done!"<<endl;
    
}

int* genArr(){
    srand(time(NULL));
    int* arr = new int[dim];
    for (int i = 0; i < dim; i++)
        arr[i] = rand() % 50;
    return arr;
}

void * checksort(void* pArg){
    int prev;
    int sort=0;
while (!complete){
     sort = 0;
     for (int i = 1; i < dim;i++){
        prev = arr[i-1];
        if (prev <= arr[i])
            sort++;
        else
            sort = 0;
        i++;
        cout <<sort<<endl;
    }
    if (sort > dim/2 -1)
        complete = true;
    else
        false;
}
pthread_exit(NULL);
return (void*) 0;    
}

void printArr(){
    for (int i = 0; i < dim; i++)
        cout << arr[i] << ", ";
        cout <<endl;
}

void * thread2(void* pArg){

    while (!complete){
        bool right = false;
        int index,atIndex,toMove,swap; 
        //generate random index
        index = rand() % (dim) + 0;
        atIndex = arr[index];
        pthread_mutex_lock(&mlock);
            if (index != 0)
                toMove = arr[index -1]; // check left
            else  {
                toMove = arr[index +1]; // check right
                right = true;
            }
        if ((toMove > atIndex) && !(right)){ // if neighbor to the left is greater than our index
            arr[index] = toMove; //assign index to greater value
            arr[index-1] = atIndex; // assign left value to lesser value
        }else if ((toMove < atIndex) && (right)){ // if neighbor to the right is less than index
            arr[index] = toMove; // place larger number in index
            arr[index+1] = atIndex; // place greater value in upper position
        }
        pthread_mutex_unlock(&mlock);
        printArr();
    }
    pthread_exit(NULL);
    return (void*) 0;
}


void problem2(int m){
    // create random arr
    arr = genArr();
    pthread_t threadArray[m];
    pthread_t T[1];
    // initalize each thread in the array
    pthread_create(&T[0],NULL,checksort,NULL);
    for (int i = 0; i < m; i++){
     pthread_create(&threadArray[i],NULL, thread2,NULL);
    }
    pthread_join(T[0],NULL);
    for (int j = 0; j< m; j++)
        pthread_join(threadArray[j],NULL);
    
    cout << endl <<" Thread sorted output:"<<endl;
     for (int i = 0; i < dim; i ++){
         cout << arr[i]<<" ";
     }


}

// This method will serve as the menu

int main(int argc, char** argv) {
    
    dim = atoi(argv[1]);
    int m;
    

    // Initalize the user_input variable
    string user_input;
    bool ex = false;

    // Input validation loop
    while (true){
        // Display Menu
        cout << "1.) Problem 1"<< endl;
        cout << "2.) Problem 2"<< endl;
        cout << "3.) Exit"<< endl;

        // Take user input
        cin >> user_input;
       

        if (user_input == "1"){    

            cout << "How many threads would you like to initalize?" << endl;
            cin >> m;
            matrix = genMatrix();
            // Make sure that there are a reasonable number of threads generated.
            while (m > 100 || m < 0 || m == NULL){
                cout << "Please try inputting a value between 0 - 100 for the second argument"<< endl;
                cin >> m;
            }

                problem1(m,dim);

        }
        else if (user_input =="2"){
            cout << "How many threads would you like to initalize?" << endl;
            cin >> m;
            // Make sure that there are a reasonable number of threads generated.
            while (m > 100 || m < 0 || m == NULL){
                cout << "Please try inputting a value between 0 - 100 for the second argument"<< endl;
                cin >> m;
        }
            problem2(m);
            cout<< endl;

        }
        else if (user_input == "exit" || user_input == "Exit" || user_input == "3"){
            exit(0);
        }
        else {
            cout << "Try selecting a valid menu option." << endl;
        }
    }
        return 0;
}