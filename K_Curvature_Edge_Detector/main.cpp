//Project 8.1 K Curvature Edge Detector

#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;
class Image;
class BoundaryPt;
class K_Curvature;

class BoundaryPt {
public:
    int x;
    int y;
    double curvature;
    int localMax;//0 false, 1 true
    int corner; //1 not corner, 8 is corner
    BoundaryPt(){
        this->x=-1;
        this->y=-1;
        curvature=-1;
        localMax=-1;
        corner=-1;
    }
    BoundaryPt(int x, int y){
        this->x=x;
        this->y=y;
        curvature=-1;
        localMax=-1;
        corner=-1;
    }
};
class Image{
public:
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int **img;
    Image(int numRows, int numCols, int minVal, int maxVal){
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
    void plotPt2Img(BoundaryPt *ptAry, int numPoints){
        for (int i=0; i<numPoints; i++){
            img[ptAry[i].y][ptAry[i].x]=ptAry[i].corner;
        }
    }
    void prettyPrint (ofstream *outfile){
        for (int r=0; r<numRows;r++){
            for (int c=0; c<numCols;c++){
                if (img[r][c]!=0)
                    *outfile<<img[r][c]<<" ";
                else
                    *outfile<<"  ";
            }
            *outfile<<endl;
        }
    }
};

class K_Curvature{
public:
    ifstream *infile;
    ofstream *outfile1;
    ofstream *outfile2;
    ofstream *outfile3;
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int label;
    int k;
    int countPts;
    int numPts;
    BoundaryPt *ptAry;
    int q;
    int p;
    int r;
    int beginIndex;
    int index;
    int x;
    int y;
    Image *image;
    K_Curvature(ifstream *infile, ofstream *outfile1, ofstream *outfile2,
                ofstream *outfile3, int numPts){
        this->infile=infile;
        this->outfile1=outfile1;
        this->outfile2=outfile2;
        this->outfile3=outfile3;
        this->numPts=numPts;
        //get k
        cout<<"enter a value for K:";
        cin>>k;
        cout<<"you entered:"<<k<<endl;
        index=0;
        ptAry=new BoundaryPt[numPts];
        //step1,2,3
        *infile>>numRows>>numCols>>minVal>>maxVal>>label;
        while ((*infile>>y) && (*infile>>x)){
            storePt(x, y, index);
            *outfile3<<"index:"<<index<<", y:"<<ptAry[index].y<<", x:"<<ptAry[index].x
            <<", Curvature:"<<ptAry[index].curvature<<endl;
            index++;
        }
    }
    void run(){
        double curvatureTemp;
        //step4
        q=0;
        p=k;
        r=2*k;
        while (true){
            //step5
            index=p;
            curvatureTemp=computeCurvature(q, p, r);
            ptAry[index].curvature=curvatureTemp;
            *outfile3<<"after step5: curvature calculation:"<<endl;
            *outfile3<<"q:"<<q<<", p:"<<p<<", r:"<<r<<endl;
            *outfile3<<"index:"<<index<<", y:"<<ptAry[index].y<<", x:"<<ptAry[index].x
            <<", Curvature:"<<ptAry[index].curvature<<"LocalMax:"<<ptAry[index].localMax
            <<"Corner:"<<ptAry[index].corner<<endl;
            //step7 break out of while loop
            if(p==k-1)
                break;
            //step6
            p=(p+1)%numPts;
            q=(q+1)%numPts;
            r=(r+1)%numPts;
        }
        //step8
        printPtAry(outfile3);
        //step9
        computeLocalMaxima(ptAry);
        //step10
        setCorner(ptAry);
        //step11
        print_X_Y_Corner(outfile1);
        //step12
        image= new Image(numRows, numCols, minVal, maxVal);
        //step13
        image->plotPt2Img(ptAry, numPts);
        //step14
        image->prettyPrint(outfile2);
    }
    void printPtAry(ofstream *outfile){
        *outfile<<"ptAry:"<<endl;
        for (int i=0; i< numPts;i++){
            *outfile<<"#"<<i<<", y:"<<ptAry[i].y<<", x:"<<ptAry[i].x
            <<", Curvature:"<<ptAry[i].curvature<<"LocalMax:"<<ptAry[i].localMax
            <<"Corner:"<<ptAry[i].corner<<endl;
        }
    }
    void print_X_Y_Corner(ofstream *outfile){
        *outfile<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<endl;
        *outfile<<label<<endl;
        *outfile<<numPts<<endl;
        for (int i=0; i<numPts;i++){
            *outfile<<ptAry[i].y<<" "<<ptAry[i].x<<" "<<ptAry[i].corner<<endl;
        }
    }
    void storePt(int x, int y, int index){
        ptAry[index] = BoundaryPt(x, y);
    }
    double computeCurvature(int q, int p, int r){
        double denominator_qp=((double)ptAry[q].x-(double)ptAry[p].x);
        double denominator_pr=((double)ptAry[p].x-(double)ptAry[r].x);
        double numerator_qp=((double)ptAry[q].y-(double)ptAry[p].y);
        double numerator_pr=((double)ptAry[p].y-(double)ptAry[r].y);
        double slope_qp;
        double slope_pr;
        if (denominator_qp==0)
            slope_qp=100000.0; //a fixed aproximation
            //slope_qp=(double)numerator_qp/0.0001; //alternative solution
        else
            slope_qp=(double)numerator_qp/denominator_qp;
        if (denominator_pr==0)
            slope_pr=100000.0;
            //slope_pr=(double)numerator_pr/0.0001; //alternative solution
        else
            slope_pr=(double)numerator_pr/denominator_pr;
        return (slope_qp-slope_pr);
    }
    void computeLocalMaxima(BoundaryPt *ptAry){
        int isMax;
        for (int i=0; i<numPts;i++){
            isMax=1; //1=true, 0=false
            for (int j=0; j<k;j++){
                if ((ptAry[(i+k)%numPts].curvature<ptAry[(i+j)%numPts].curvature)||
                    (ptAry[(i+k)%numPts].curvature<ptAry[(i+2*k-j)%numPts].curvature))
                    isMax=0;
            }
            ptAry[(i+k)%numPts].localMax=isMax;
        }
    }
    void setCorner (BoundaryPt *ptAry){
        int cornerVal;
        int isCorner;
        for (int i=0; i<numPts;i++){
            isCorner=8;
            if (ptAry[(i+k)%numPts].localMax==1){
                for (int j=0; j<k-1;j++){
                    if (ptAry[(i+j)%numPts].localMax==1 ||
                        ptAry[(i+2*k-j)%numPts].localMax==1){
                        isCorner=1;
                    }
                }
            }
            else{
                isCorner=1;
                
            }
            ptAry[(i+k)%numPts].corner=isCorner;
            cornerVal=ptAry[(i+k)%numPts].corner;
        }
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
    K_Curvature *k_curv;
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
    infile.open(argv[1]);
    k_curv=new K_Curvature(&infile, &outfile1, &outfile2, &outfile3, numPts);
    k_curv->run();
    
    infile.close();
    outfile1.close();
    outfile2.close();
    outfile3.close();
    return 0;
}
