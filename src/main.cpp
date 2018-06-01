//CS780-37 Proj2.2 Median 3x3 filter CPP
//Shawn Yang

#include <iostream>
#include <fstream>
using namespace std;


class Median3X3Filter {
public:
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int newMin;
    int newMax;
    int pixel;
    int ** mirrorFramedAry;
    int ** tempAry;
    int *neighborAry;
    ifstream * infile;
    ofstream *outfile;
    
    Median3X3Filter (ifstream *in_infile, ofstream *in_outfile){
        //step0
        infile=in_infile;
        outfile = in_outfile;
        *infile>>numRows>>numCols>>minVal>>maxVal;
        mirrorFramedAry= new int* [numRows+2];
        for (int i=0; i<numRows+2; i++){
            mirrorFramedAry[i]=new int[numCols+2];
            for (int j=0; j<numCols+2; j++){
                mirrorFramedAry[i][j]=0;
            }
        }
        tempAry=new int* [numRows+2];
        for (int i=0; i<numRows+2;i++){
            tempAry[i]=new int[numCols+2];
            for (int j=0; j<numCols+2; j++){
                tempAry[i][j]=0;
            }
        }
        neighborAry=new int[9];
        for (int i=0; i<9; i++){
            neighborAry[i]=0;
        }
    }

    void run (){
        //step1
        for (int r=1; r<numRows+1; r++){
            for (int c=1; c<numCols+1; c++){
                *infile>>mirrorFramedAry[r][c];
            }
        }
        //step2 mirror to frame
        for (int c=1; c< numCols+1; c++){
            //top
            mirrorFramedAry[0][c]=mirrorFramedAry[1][c];
            //buttom
            mirrorFramedAry[numRows+1][c]=mirrorFramedAry[numRows][c];
        }
        for (int r=0; r<numRows+2;r++){
            //left
            mirrorFramedAry[r][0]=mirrorFramedAry[r][1];
            //right
            mirrorFramedAry[r][numCols+1]=mirrorFramedAry[r][numCols];
        }
        
        //step3 process mirrorFramedAry -> neighborAry ->tempAry
        
        for (int r=1; r<numRows+1; r++){
            for (int c=1; c<numCols+1;c++){
                //neighborAry <- loadNeighbors
                neighborAry[0]=mirrorFramedAry[r-1][c-1];
                neighborAry[1]=mirrorFramedAry[r-1][c];
                neighborAry[2]=mirrorFramedAry[r-1][c+1];
                neighborAry[3]=mirrorFramedAry[r][c-1];
                neighborAry[4]=mirrorFramedAry[r][c];
                neighborAry[5]=mirrorFramedAry[r][c+1];
                neighborAry[6]=mirrorFramedAry[r+1][c-1];
                neighborAry[7]=mirrorFramedAry[r+1][c];
                neighborAry[8]=mirrorFramedAry[r+1][c+1];
                //sort neighborAry with Selection Sort
                selectionSort(neighborAry);
                //tempAry <- neighborAry
                tempAry[r][c]=neighborAry[4];
                if (r==1 && c==1){
                    newMin=tempAry[1][1];
                    newMax=tempAry[1][1];
                }
                else{
                    if (tempAry[r][c]<newMin)
                        newMin=tempAry[r][c];
                    if (tempAry[r][c]>newMax)
                        newMax=tempAry[r][c];
                }
            }
        }//step4 repeat step3 until all pixels processed
        //step5 output header
        *outfile<<numRows<<" "<<numCols<<" "<<newMin<<" "<<newMax<<endl;
        //step6 output tempary
        for (int r=1; r<numRows+1; r++){
            for (int c=1; c<numCols+1; c++){
                *outfile<<tempAry[r][c]<<" ";
            }
            *outfile<<endl;
        }
    }
    void selectionSort(int in_ary[]){
        int minIndex;
        int temp;
        //stops after the 5th run
        for (int i=0; i<5; i++){
            minIndex=i;
            for (int j=i+1; j<9; j++){
                if (in_ary[minIndex]>in_ary[j])
                    minIndex=j;
            }
            temp=in_ary[i];
            in_ary[i]=in_ary[minIndex];
            in_ary[minIndex]=temp;
        }
    }
};

int main(int argc, const char * argv[]) {
    ifstream infile;
    ofstream outfile;
    
    if (argc<3){
        cout<<"Please supply the input file and output file"<<endl;
        exit(1);
    }
    
    try {
        infile.open(argv[1]);
        outfile.open(argv[2]);
    } catch (exception &e) {
        cout<<"error opening input file / output file"<<endl;
        exit(1);
    }
    cout<<"Open I/O OK"<<endl;
    Median3X3Filter filter=Median3X3Filter(&infile, &outfile);
    filter.run();
    
    infile.close();
    outfile.close();
    
    
    return 0;
}
