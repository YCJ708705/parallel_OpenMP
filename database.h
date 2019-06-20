#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
const int NTx=16;
const int NRx=16;
const int NRd=4096;
const int DataTypeNum=3;

const static double     MetersPerPoint  =   (double)6;                
const static double     ThPerPoint   =   (double)180/(NTx);           
const static double     PhPerPoint   =   (double)180/(NRx);

using namespace std;

/////Class create

class database{
    double ****coordinate;     //[][][][0]=Real; [][][][1]=Img; [][][][2]=abs()
    int num_x_domain;
    int num_y_domain;
    int num_z_domain;
    int Ndatatype;
public:
    database(int x,int y, int z, int datanum);
    ~database();
    void writedebug();
    void setDataSize();
    double *FindMaxPeak();
    void setData(double value,int x, int y, int z, int idata);
    double getData(int x, int y, int z, int idata);
    double  ****gen4d_double_dynamic_array(int dim1,int dim2, int dim3, int dim4);
    void    free4d_double_dynamic_array(double ****array4d,int dim1,int dim2, int dim3, int dim4);
    
};



////functin create
database::database(int x,int y, int z,int datanum){
    num_x_domain=x;
    num_y_domain=y;
    num_z_domain=z;
    Ndatatype=datanum;
    coordinate = gen4d_double_dynamic_array(x,y,z,Ndatatype);
}

database::~database(){
    free4d_double_dynamic_array(coordinate,num_x_domain,num_y_domain,num_z_domain,Ndatatype);
}

void database::setData(double value,int x,int y,int z,int idata){
    coordinate[x][y][z][idata]=value;
}

double database::getData(int x, int y, int z, int idata){
    return coordinate[x][y][z][idata];
}



double ****database::gen4d_double_dynamic_array(int dim1,int dim2, int dim3, int dim4){
    double ****m4darray;
    //m4darray=(double****)malloc(sizeof(double***)*dim1);
    m4darray=new double***[dim1];
    for (int d1 = 0; d1 < dim1; d1++) {
        //m4darray[d1]=(double***)malloc(sizeof(double**)*dim2);
        m4darray[d1]=new double**[dim2];
        for (int d2 = 0; d2 < dim2; d2++) {
            //m4darray[d1][d2]=(double**)malloc(sizeof(double*)*dim3);
            m4darray[d1][d2]=new double*[dim3];
            for (int d3 = 0; d3 < dim3; d3++) {
                //m4darray[d1][d2][d3]=(double*)malloc(sizeof(double)*dim4);
                m4darray[d1][d2][d3]=new double[dim4];

                for(int d4=0;d4<dim4;d4++){
                    m4darray[d1][d2][d3][d4]=0;
                }
            }
        }
    }
    return m4darray;
}
void database::free4d_double_dynamic_array(double ****array4d,int dim1,int dim2, int dim3, int dim4){

    for (int d1 = 0; d1 < dim1; ++d1) {
        for (int d2 = 0; d2 < dim2; ++d2) {
            for (int d3 = 0; d3 < dim3; ++d3) {
                free(array4d[d1][d2][d3]);
            }
            free(array4d[d1][d2]);
        }
        free(array4d[d1]);
    }
    free(array4d);
}
void database::writedebug(){
    
    FILE *fo = fopen("result.txt","w");
    for (int iTx = 0; iTx < NTx; iTx++){
        for (int iRx = 0; iRx < NRx; iRx++){
            for (int iRd = 0; iRd < NRd; iRd++){
                fprintf(fo,"(%d,%d,%d) => %f %f %f\n",iTx,iRx,iRd,coordinate[iTx][iRx][iRd][0],coordinate[iTx][iRx][iRd][1],coordinate[iTx][iRx][iRd][2]);
            }
        }
    }
    fclose(fo);

}

double *database::FindMaxPeak(){
    double *Peak = new double[4];
    double Max=0;
    for (int iTx = 0; iTx < NTx; iTx++){
        for (int iRx = 0; iRx < NRx; iRx++){
            for (int iRd = 0; iRd < NRd; iRd++){
                if(coordinate[iTx][iRx][iRd][2]>Max){
                    Max=coordinate[iTx][iRx][iRd][2];
                    Peak[0]=iTx;
                    Peak[1]=iRx;
                    Peak[2]=iRd;
                    Peak[3]=coordinate[iTx][iRx][iRd][2];
                }
            }
        }
    }
    return Peak;
}