//Project 8.1 K Curvature Edge Detector


#include <iostream>
#include <fstream>
using namespace std;

class image{
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int **img;
    image(int numRows, int numCols, int minVal, int maxVal){
        this->numRows=numRows;
        this->numCols=numCols;
        this->minVal=minVal;
        this->maxVal=maxVal;
        //dynamically allocate
        img= new int*[numRows];
        for (int i=0; i<numRows;i++){
            img[i]=new int [numCols];
            //initialize img to 0
            for (int j=0; j<numCols;j++)
                img[i][j]=0;
        }
    }
    void plotPt2Img(){
        //todo
    }
    void prettyPrint (){
        //todo
    }
};


int countPoints(ifstream *in_infile){
    int count=0;
    int temp;
    while (*in_infile>>temp){
        count++;
    }
    return count/2;
}
int main(int argc, const char * argv[]) {
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int label;
    int numPts;
    int k;
    ifstream infile;
    ofstream outfile1;
    ofstream outfile2;
    ofstream outfile3;
    if (argc<5){
        cout<<"provide enough arguments and try again."<<endl;
        exit(1);
    }
    try {
        infile.open(argv[1]);
        outfile1.open(argv[2]);
        outfile2.open(argv[3]);
        outfile3.open(argv[4]);
    } catch (exception &e) {
        cout<<"error opening files"<<endl;
        exit(1);
    }
    //step0 get input data information
    try {
        infile>>numRows>>numCols>>minVal>>maxVal>>label;
    } catch (exception &e) {
        cout<<"error reading header"<<endl;
        exit(1);
    }
    numPts=countPoints(&infile);
    cout<<"points count:"<<numPts<<endl;
    infile.close();
    //get k
    cout<<"enter a value for K:";
    cin>>k;
    cout<<"you entered:"<<k<<endl;
    
    
    
    
    return 0;
}
