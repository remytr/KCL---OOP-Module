#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <string>
using namespace std;
// Here is the function prototype. As the code runs line by line and the function code is at the bottom of the file, it acts as a definition.
double mri(vector<double>& gradientVal, double R, double C, double k, double W, double tau, int noOfReps);

// Here are the variables for equation 2. I am defining two vectors; gradientVal for the values in the text file and vecTemp which is a vector of all the temperatures for the corresponding repetition and gradient value from the text file.
// The variable data is the variable used to read in the gradient value from the text file line by line.
double R = 2.2;
double k = 1.1;
double C = 340;
double W = 148;
double tau = 10*pow(10,-6);
vector<double> gradientVal;
vector<float> vecTemp;
double data;


int main(int argc, char *argv[]) {
// The first argument is argument count and stores the number of command line arguments. Second argument is a vector. It's an array of character pointers to argument list.
// The following if statement states that if there are 3 arguements selected then an error message will display. This is because argv[0] is the working directory where the file is located, with arg[1] being the txt file and arg[2] = 100 (it is the number of repetitions).
// PLEASE NOTE that using xcode, you must select the file by clicking on edit scheme to alternate between the two txt files, the 100 argument must always be selected.
    if (argc !=3){
        cerr << "Incorrect number of input arguments. " << endl;
        exit(0);
    }
    
    int noOfReps = stoi(argv[2]);
// this converts the string 100 to integer of elemt 2. stoi is a c++ function.
// str2 is the txt file.
    
    ifstream inFile;
    string str = argv[1];

    // Open file
    inFile.open(str);
// If the file does not open an error message is displayed and returns a value of 1.
       if (!inFile){
           cerr << "Error opening gradient file: "  << argv[1] << endl;
           return 1;
       }
    else {
        cout << "Reading from file: " << argv[1] << endl;
    
    // The code below takes in each value of the txt file str and assigns it to the variable data, it then pushes the value back into the vector gradientVal of type double.
        while(inFile >> data)
            gradientVal.push_back(data);
    
        // Close file.
        inFile.close();
    }
        cout << "read " <<gradientVal.size()<< " data points (corresponding to " << (gradientVal.size())/100.0 << " ms)" << endl;
    cout << "simulating " << noOfReps << " repetitions" << endl;
    
    // Makes a call to the function to get the value for either G60.txt and G80.txt which will find the maximum temperature after 100 repetitions.
    double maxTemp = mri(gradientVal, R, C, k, W, tau, noOfReps);
    
    //Display parameters
    cout << "using parameters: " << endl;
    cout << " coil resistance = 2.2 ohms " << endl;
    cout << " coil heat capacity = 340 J/K " << endl;
    cout << " cooling capacity = 148 W/K " << endl;
    cout << " max temperature increase = " << maxTemp << endl;
    // The line above displays the maximum temperature before padding.
    
    
    // Creates a vector of 100 zeros called padding. If the maximum temperature  is greater than 30 Kelvin, a warning message will display.
    float padding[100] = {};
    if (maxTemp>30){
        cout << "Warning temperature exceeds the safety limit " << endl;
    }
        int p=0;
   
    // While the maximum temperature is greater than 30, the while loop will insert zeros to the end of the file of gradientVal, thus allowing the machine to cool down. It does this by adding the array padding which consists of 100 zeros. Increments p = p + 1.
    while(maxTemp>30){
            gradientVal.insert(gradientVal.end(),padding,padding+100);
         maxTemp = mri(gradientVal, R, C, k, W, tau, noOfReps);
        p=p+1;
        
    }
    
        cout << "The new maximum temperature reached is now : " << maxTemp << endl;
        cout << "Number of iterations taken to get below 30K: " << p << ", corresponding to " << p << " ms padded" << endl;
    }
  
// Below is the function called mri which calculates the temperature of the mri machine. I have declared all the variables that the function will use. The vector vecTemp will store all the values of the temperature of the corresponding gradient value. The two for loops calculates the temperature for each gradient value and up to the number of repetitions which is 100.
               
double mri(vector<double>& gradientVal, double R, double C, double k, double W, double tau, int noOfReps) {
    vector<float> vecTemp;
    double Temp = 0;
    for (int i = 0; i < noOfReps; i++){
        for (int j = 0; j < gradientVal.size(); j++) {
            Temp = Temp + (tau/C)*((pow(k*gradientVal[j],2)*R) - (W*Temp));
             vecTemp.push_back(Temp);
             
         }
   }
    
    // The lines below returns the maximum temperature and the line belows finds the maximum value within the vector of temperatures; vecTemp.
    
    double maxTemp = *max_element(vecTemp.begin(), vecTemp.end());
    return maxTemp;
}
