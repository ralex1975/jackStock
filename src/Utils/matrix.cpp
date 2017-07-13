#include "matrix.h"
#include <math.h>
#include <QString>
#include <QtCore>




/****************************************************************
 *
 * Function:    ()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
CMatrix::CMatrix()
{
}







/****************************************************************
 *
 * Function:    ()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CMatrix::solveMatrix(int n)
{
    return( Gaussj(&n));
}





/****************************************************************
 *
 * Function:    ()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
void CMatrix::swap(double *a1, double *b1)
{
    double hold;

    hold = *a1;
    *a1 = *b1;
    *b1 = hold;

}


/****************************************************************
 *
 * Function:    ()
 *
 * Description: Gauss jourdan matrix inversion and solution
 *              B(N ,N) cofficient matrix becomes inverse
 *              Y(N)    original constant vektor
 *              W(N,M)  constant vector(s) become soulution vector
 *              DETERM  is the determinant
 *              NV      is number of constant vectors
 *
 *
 ****************************************************************/
bool CMatrix::Gaussj(int *n)
{
    double w[10][10],
            determ,
            pivot,
//            hold,
//            sum,
            t,
            big;


    int	index[10][4],
            i,
            j,
            k,
            l,
            nv,
            irow = 0,   // ajn 170713
            icol= 0,    // ajn 170713
            l1;

    nv=1;

    for( i = 1; i <= *n; i++)
    {
        w[i][1] = mkv.y[i];	       /* copmvk.y constant vektor */
        index[i][3] = 0;
    }

    determ = 1.0;
    for(i = 1; i <= *n; i++)
    {
        // Search for largest element
        big = 0.0;
        for(j = 1; j <= *n; j++)
        {
            if(index[j][3] != 1)
            {
                for(k=1; k <=*n; k++)
                {
                    if(index[k][3] > 1)
                    {
                        // ERROR1: matrix singular
                        return(false);
                    }

                    if( index[k][3] < 1)
                    {
                        if( sqrt((mkv.b[j][k]) * (mkv.b[j][k])) > big)
                        {
                            irow = j;
                            icol = k;
                            big = sqrt((mkv.b[j][k]) * (mkv.b[j][k]));
                        }
                    }

                }
                // end k loop
                k = *n;
            }
        }
        // end j loop

        j = *n;

        index[icol][3] = index[icol][3] + 1;
        index[i][1] = irow;
        index[i][2] = icol;

        // interchange rows to put pivot on diagonal
        if(irow!= icol)
        {
            determ = -determ;

            for(l = 1; l <= *n; l++)
            {
                swap(&mkv.b[irow][l], &mkv.b[icol][l]);
            }

            l = *n;

            if(nv > 0)
            {
                for(l = 1; l <= nv; l++)
                {
                    swap(&w[irow][l], &w[icol][l]);
                }
            }
        }
        //  if(irow!= icol)
        l = *n;


        // divide pivot row mkv.bmkv.y1 pivot column
        pivot = mkv.b[icol][icol];
        determ = determ * pivot;
        mkv.b[icol][icol] = 1.0;

        for(l = 1; l <= *n; l++)
        {
            mkv.b[icol][l] = mkv.b[icol][l] / pivot;
        }

        l = *n;

        if (nv > 0)
        {
            for(l = 1; l <= nv; l++)
            {
                w[icol][l] = w[icol][l] /pivot;
            }
        }

        l = nv;
        for(l1 = 1; l1 <= *n; l1++)
        {
            if(l1 != icol)
            {
                t = mkv.b[l1][icol];
                mkv.b[l1][icol] = 0.0;

                for(l = 1; l <= *n; l++)
                {
                    mkv.b[l1][l] = mkv.b[l1][l] - mkv.b[icol][l] *t;
                }

                l = *n;
                if(nv > 0)
                {
                    for(l=1; l<=nv; l++)
                    {
                        w[l1][l] = w[l1][l] - w[icol][l] * t;
                    }
                }
                l = nv;
            }
            //  if(l1! = icol)
        }
        l1 = *n;
    }
    // i loop

    i = *n;
    // interchange columns
    for(i = 1; i<= *n; i++)
    {
        l = *n - i + 1;

        if (index[l][1] != index[l][2])
        {
            irow=index[l][1];
            icol=index[l][2];
            for(k=1; k<=*n; k++)
            {
                swap(&mkv.b[k][irow], &mkv.b[k][icol]);
            }
        }
        /* end if index */
        k = *n;
    }
    /* i loop */

    i = *n;
    for(k=1; k <= *n; k++)
    {
        if( index[k][3] != 1)
        {
            // ERROR2: matris singular
            return(false);
        }
    }

    k=*n;

    for(i=1; i<=*n; i++)
    {
        mkv.coef[i] = w[i][1];
    }

    return(true);
}
/* Gaussj() */





/****************************************************************
 *
 * Function:    ()
 *
 * Description: n is equal with xylista.index
 *
 *              Following matrix will be solved: BX=Y
 *
 *              Enter values for the matrix B
 *              Calculate the mean of the x values.
 *
 ****************************************************************/
bool CMatrix::Laddakv2(int *n, double *xmed)
{
    int i;

    // Enter values for the matrix B

    // Calculate the mean of the x values.
    *xmed = 0;
    for(i=0; i<*n; i++)
    {
        *xmed += xylista.listax[i];
    }
    *xmed = (*xmed / (*n - 1));

    /* row 1 col 1 sum 1*/
    mkv.b[1][1] = 0;
    for(i = 1; i < *n; i++)
    {
        mkv.b[1][1] = mkv.b[1][1] + 1;
    }

    /* row 1 col 2 zero */
    mkv.b[1][2] = 0;


    // row 1 col 3 sum(x-values - xmed) ^ 2
    mkv.b[1][3] = 0;
    for(i = 0; i < *n - 1; i++)
    {
        mkv.b[1][3] = mkv.b[1][3] + (pow((xylista.listax[i] - *xmed), 2));
    }

    // row 2 col 1 is equal to: zero
    mkv.b[2][1] = 0;


    // Row 2 col 2 sum(x-values - xmed)^2
    mkv.b[2][2] = mkv.b[1][3];


    // Row 2 col 3 is equal noll
    mkv.b[2][3] = 0;


    // Row 3 col 1     sum(x-values - xmed)^2
    mkv.b[3][1] = mkv.b[1][3];



    // Row 3 row 2 is equal to noll
    mkv.b[3][2] = 0;


    // Row 3 Col 3     sum(x-values - xmed)^4
    mkv.b[3][3] = 0;
    for(i=0; i<*n-1; i++)
    {
        mkv.b[3][3] = mkv.b[3][3] + (pow((xylista.listax[i] - *xmed), 4));
    }


    // Add valuest to the matrix  Y */
    // Row 1  sum(y)
    mkv.y[1] = 0;
    for(i = 0; i < *n - 1; i++)
    {
        mkv.y[1] += xylista.listay[i];
    }


    // Row 2  sum(y*(x-values - xmed);
    mkv.y[2] = 0;
    for(i=0; i<*n-1; i++)
    {
        mkv.y[2]+=(xylista.listay[i]*(xylista.listax[i]-*xmed));
    }



    /* rad  3  sum(y*(x-valyes - xmed)^2)   */
    mkv.y[3] = 0;
    for(i=0; i<*n-1; i++)
    {
        mkv.y[3]=mkv.y[3]+( xylista.listay[i] * pow( (xylista.listax[i]-*xmed),2) );
    }
    return(true);
}

/****************************************************************
 *
 * Function:    ()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
bool CMatrix::Minkv2(int n)
{
    double xmed;

    Laddakv2(&n, &xmed);
    /* L?sning av matrisen bx=y  svaret finns i coef[i] */
    solveMatrix(3);

    /* ber?kning av de ursprungliga koefficienterna i ekvationen a+bx+cx^2=0 */

    mkv.coef[1]=mkv.coef[1]-(mkv.coef[2]*xmed)+(mkv.coef[3]*pow(xmed,2));
    mkv.coef[2]=mkv.coef[2]-2*mkv.coef[3]*xmed;
    Write_data(3);

    return(true);
}


/****************************************************************
 *
 * Function:    ()
 *
 * Description:.
 *
 *
 *
 *
 ****************************************************************/
void CMatrix::Write_data(int n)

/* print out answer */
{
    int i;
    QString str;
    /*  Pause();  */
    qDebug() << "Y=";
    for(i=1; i<=n; i++)
    {
        /* heltal skrivs ut dock ej "noll" */
        if((mkv.coef[i] < -1.1e-4) && (i==1))
        {
            str.sprintf("%.4lf", mkv.coef[i]);
            qDebug() << str;
        }
        else if((mkv.coef[i]>1.1e-4) && (i==1))
        {
            str.sprintf("%.4lf", mkv.coef[i]);
            qDebug() << str;
        }

        if((mkv.coef[i]<-1.1e-4) && (i==2))
        {
            str.sprintf("%.4lf*X ",mkv.coef[i]);
            qDebug() << str;
        }
        else if((mkv.coef[i]>1.1e-4) && (i==2))
        {
            str.sprintf("+%.4lf*X ",mkv.coef[i]);
            qDebug() << str;
        }

        if((mkv.coef[i]<-1.1e-4) && (i>2))
        {
            str.sprintf("%.4lf*X^%d ",mkv.coef[i],i-1);
            qDebug() << str;
        }
        else if((mkv.coef[i]>1.1e-4) && (i>2))
        {
            str.sprintf("+%.4lf*X^%d ",mkv.coef[i],i-1);
            qDebug() << str;
        }
    }

} /* end write_data */





