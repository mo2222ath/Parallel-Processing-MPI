#include <stdio.h>
#include <stdlib.h> 
#include "mpi.h"


main(int argc, char **argv)
{
    int numberOfTasks;
    int RankOfTask;
    int numberWorkers;
    int partRows;
    int offset;
    int i,j,k;
    int r1,c1,r2,c2,temp;
    int a[r1][c1],b[r2][c2],c[r1][c2];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &RankOfTask);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfTasks);

    numberWorkers = numberOfTasks-1;

    MPI_Status status;
    
    
    if (RankOfTask == 0)
    {
        printf("Welcome to Matrix multiplication program! \n");

        printf("Please enter dimensions of the first matrix: \n");
        scanf("%d", &r1);
        scanf("%d", &c1);
        

        printf("Please enter its elements: \n");
        for (i = 0; i <  r1; i++) {
          for (j = 0; j < c1; j++) {
            scanf("%d", &temp);
            a[i][j] = temp;
          }
        }

        printf("Please enter just the second dimension of the second matrix: \n");
        scanf("%d", &c2);
        r2 = c1;
        

        printf("Please enter its elements: \n");
        for (i = 0; i <  r2; i++) {
          for (j = 0; j < c2; j++) {
            scanf("%d", &temp);
            b[i][j] = temp;
            printf(" its elements: %d \n" , b[i][j]);
          }
        }

        partRows = r1/numberWorkers;
        offset = 0;

        for (i=1; i<=numberWorkers; i++)
        {
            MPI_Send(&offset, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&partRows, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&a[offset][0], partRows*c1, MPI_INT,i,1, MPI_COMM_WORLD);
            MPI_Send(&b, c1*c2, MPI_INT, i, 1, MPI_COMM_WORLD);
            offset = offset + partRows;
        }

         printf("Rank # %d \n" , RankOfTask);

        for (i=1; i<=numberWorkers; i++)
        {
              
            MPI_Recv(&offset, 1, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&partRows, 1, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&c[offset][0], partRows*c2, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
        }

        printf("the result matrix:\n");
        for (i=0; i<r1; i++)
        {
            for (j=0; j<c2; j++)
                printf("%d   ", c[i][j]);
            printf ("\n");
        }
    }

 
    if (RankOfTask > 0)
    {
        
        MPI_Recv(&offset, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&partRows, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&a, partRows*c1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&b, c1*c2, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

        for (k=0; k<r1; k++)
            for (i=0; i<partRows; i++)
            {
                for (j=0; j<r1; j++)
                    c[i][k] += + a[i][j] * b[j][k];
            }
       

        MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&partRows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&c, partRows*c2, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}
