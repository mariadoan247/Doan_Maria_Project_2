#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <omp.h>

#define DEBUG 0

/* ----------- Project 2 - Problem 2B - Matrix Mult Max -----------

    This file will multiply two matricies and find the second 
    largest value in the output matrix.
    Complete the TODOs in order to complete this program.
    Remember to make it parallelized!
*/ // ------------------------------------------------------ //

int main(int argc, char* argv[])
{
    // Catch console errors
    if (argc != 10)
    {
        printf("USE LIKE THIS: parallel_mult_second_largest   file_A.csv n_row_A n_col_A   file_B.csv n_row_B n_col_B   num_threads   result_second_largest.csv   time.csv\n");
        return EXIT_FAILURE;
    }

    // Get the input files
    FILE* inputMatrixA = fopen(argv[1], "r");
    FILE* inputMatrixB = fopen(argv[4], "r");

    char* p1;
    char* p2;

    // Get matrix A's dims
    int n_rowA = strtol(argv[2], &p1, 10);
    int n_colA = strtol(argv[3], &p2, 10);

    // Get matrix B's dims
    int n_rowB = strtol(argv[5], &p1, 10);
    int n_colB = strtol(argv[6], &p2, 10);

    // Get num threads
    int thread_count = strtol(argv[7], NULL, 10);

    // Get output files
    FILE* outputFile = fopen(argv[8], "w");
    FILE* outputTime = fopen(argv[9], "w");


    // TODO: malloc the two input matrices and the output matrix
    // Please use long int as the variable type
    // Storing matrices in dynamically allocated 1D arrays in the row-major order
    long int *matA = (long int *)malloc(n_rowA * n_colA * sizeof(long int));
    long int *matB = (long int *)malloc(n_rowB * n_colB * sizeof(long int));

    // TODO: Parse the input csv files and fill in the input matrices
    long int temp = 0;  // Temp variable for parsed values
    int offsetA = 0;    // Offset for matrixA indexing
    int offsetB = 0;    // Offset for matrixB indexing

    // Parse matrix A
    for (int i = 0; i < n_rowA; ++i)            // Iterate matrix A rows
    {
        for (int j = 0; j < n_colA; ++j)        // Iterate matrix A columns
        {
            fscanf(inputMatrixA, "%ld,", &temp); // Store int value into temp
            offsetA = i * n_colA + j;            // Set Offset for row-major ordering
            matA[offsetA] = temp;                // Store parsed value in matrix A
        }
    }

    // Parse matrix B
    for (int i = 0; i < n_rowB; ++i)            // Iterate matrix B rows
    {
        for (int j = 0; j < n_colB; ++j)        // Iterate matrix B columns
        {
            fscanf(inputMatrixB, "%ld,", &temp);// Store int value into temp
            offsetB = i * n_colB + j;           // Set Offset for row-major ordering
            matB[offsetB] = temp;               // Store parsed value in matrix B
        }
    }

    // Initialize largest and second largest variable
    long int largest = 0;
    long int secondLargest = 0;
    // Initialize the largest local value arrays with size thread_count
    // and all elements initialized to zero
    long int max[thread_count];
    memset(max, 0, thread_count*sizeof(long int));

    // We are interested in timing finding the second largest only
    // Record the start time
    double start = omp_get_wtime();
    
    // TODO: Parallelize finding the second largest
    // Parallelize with thread_count
    // Parallelize with thread_count and two reduction clauses that will get
    // the max of the largest value and the max of the secondLargest value
    # pragma omp parallel for num_threads(thread_count)\
            reduction(max: largest) reduction(max: secondLargest)
    for (int rA = 0; rA < n_rowA; ++rA)                     // Iterate matrix A rows
    {
        int my_rank = omp_get_thread_num();                 // Get the thread rank
        for (int cB = 0; cB < n_colB; ++cB)                 // Iterate matrix B columns
        {
            long int dotProd = 0;                           // Initialize dot product private var
            for (int cA = 0; cA < n_colA; ++cA)             // Iterate matrix columns
            {
                // Multiply matA with column vector of matB and add into dotProd
                dotProd += matA[rA * n_colA + cA] * matB[cA * n_colB + cB];
            }
            // If the dotProd is greater than the current max of the current thread, then
            // the current second max will replace the max and the max will be 
            // replaced with the dot product
            if (dotProd > max[my_rank])
            {
                secondLargest = max[my_rank];
                max[my_rank] = dotProd;
                largest = dotProd;
            }
            // If the dot product is only greater than the current second max of the 
            // current thread, then it will replace it
            else if (dotProd > secondLargest)
            {
                secondLargest = dotProd;
            }
        }
    }

    // Go through the sec_max and max arrays to obtain the second largest value
    for (int i = 0; i < thread_count; ++i)
    {
        // If one of the max values is less than the global max, but greater than the
        // current global second max, then the secondLargest value will be replaced with it
        if (max[i] < largest && max[i] > secondLargest)
        {
            secondLargest = max[i];
        }
    }

    // Record the finish time        
    double end = omp_get_wtime();

    // Write the second largest value into the results file
    fprintf(outputFile, "%ld", secondLargest);

    // Time calculation (in seconds)
    double time_passed = end - start;

    // Save time to file
    fprintf(outputTime, "%f", time_passed);

    // Cleanup
    fclose(inputMatrixA);
    fclose(inputMatrixB);
    fclose(outputFile);
    fclose(outputTime);
    // Remember to free your buffers!
    free(matA);
    free(matB);

    return 0;
}

