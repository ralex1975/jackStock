#ifndef MATRIX_H
#define MATRIX_H

class CMatrix
{
private:


     void swap(double *a1, double *b1);
     bool Gaussj(int *n);
     void Write_data(int n);


     struct minstakvadrat
     {
         double coef[10];
         double b[10][10];
         double y[10];
     };

     struct CMatrix::minstakvadrat mkv;


public:


     struct tabell
     {
        double	listax[500];
        double	listay[500];
        int     index;
        int     maxantal;
        int     sistutskrivna;
        float   koll;
        int     allautskrivna;    /* kan tas bort med lite vilja */
        int     framat;
     };


     struct tabell xylista;


    CMatrix();
    bool solveMatrix(int n);
    bool Minkv2(int n);
    bool Laddakv2(int *n, double *xmed);
};

#endif // MATRIX_H
