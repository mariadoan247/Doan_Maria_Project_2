#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <omp.h>

#define DEBUG 0

/* ----------- Project 2 - Problem 1 - Matrix Mult -----------

    This file will multiply two matricies.
    Complete the TODOs in order to complete this program.
    Remember to make it parallelized!
*/ // ------------------------------------------------------ //

int main(int argc, char* argv[])
{
    // Catch console errors
    if (argc != 10)
    {
        printf("USE LIKE THIS: parallel_mult_mat_mat   mat_1.csv n_row_1 n_col_1   mat_2.csv n_row_2 n_col_2   num_threads   results_matrix.csv   time.csv\n");
        return EXIT_FAILURE;
    }

    // Get the input files
    FILE* inputMatrix1 = fopen(argv[1], "r");
    FILE* inputMatrix2 = fopen(argv[4], "r");

    char* p1;
    char* p2;

    // Get matrix 1's dims
    int n_row1 = strtol(argv[2], &p1, 10);
    int n_col1 = strtol(argv[3], &p2, 10);

    // Get matrix 2's dims
    int n_row2 = strtol(argv[5], &p1, 10);
    int n_col2 = strtol(argv[6], &p2, 10);

    // Get num threads
    int thread_count = strtol(argv[7], NULL, 10);

    // Get output files
    FILE* outputFile = fopen(argv[8], "w");
    FILE* outputTime = fopen(argv[9], "w");


    // TODO: malloc the two input matrices and the output matrix
    // Please use long int as the variable type
    // Storing matrices in dynamically allocated 1D arrays in the row-major order
    long int *mat1 = (long int *)malloc(n_row1 * n_col1 * sizeof(long int));
    long int *mat2 = (long int *)malloc(n_row2 * n_col2 * sizeof(long int));
    // The output matrix will have the same number as rows as the first matrix
    // and the same number of columns as the second matrix
    long int *mat_out = (long int *)malloc(n_row1 * n_col2 * sizeof(long int));

    // TODO: Parse the input csv files and fill in the input matrices
    long int temp = 0;  // Temp variable for parsed values
    int offset1 = 0;    // Offset for matrix1 indexing
    int offset2 = 0;    // Offset for matrix2 indexing
    int offset_out = 0; // Offset for output matrix

    // Parse matrix 1
    for (int i = 0; i < n_row1; ++i)            // Iterate matrix 1 rows
    {
        for (int j = 0; j < n_col1; ++j)        // Iterate matrix 1 columns
        {
            fscanf(inputMatrix1, "%ld,", &temp); // Store int value into temp
            offset1 = i * n_col1 + j;            // Set Offset for row-major ordering
            mat1[offset1] = temp;                // Store parsed value in matrix 1
        }
    }

    // Parse matrix 2
    for (int i = 0; i < n_row2; ++i)            // Iterate matrix 2 rows
    {
        for (int j = 0; j < n_col2; ++j)        // Iterate matrix 2 columns
        {
            fscanf(inputMatrix2, "%ld,", &temp);// Store int value into temp
            offset2 = i * n_col2 + j;           // Set Offset for row-major ordering
            mat2[offset2] = temp;               // Store parsed value in matrix 2
        }
    }

    // We are interesting in timing the matrix-matrix multiplication only
    // Record the start time
    double start = omp_get_wtime();
    
    // TODO: Parallelize the matrix-matrix multiplication
    // Perform the matrix-matrix multiplication
    // Parallelize with thread_count and a private clause that will prevent race conditions on the offsets.
    # pragma omp parallel for num_threads(thread_count) private(offset1, offset2, offset_out)
    for (int r1 = 0; r1 < n_row1; ++r1)                     // Iterate matrix 1 rows
    {
        for (int c2 = 0; c2 < n_col2; ++c2)                 // Iterate matrix 2 columns
        {
            offset_out = r1 * n_col2 + c2;
            for (int c1 = 0; c1 < n_col1; ++c1)             // Iterate matrix columns
            {
                offset1 = r1 * n_col1 + c1;                 // Set offset 1
                offset2 = c1 * n_col2 + c2;                 // Set offset 2
                // Multiply mat1 with column vector of mat2 and store in output matrix
                mat_out[offset_out] += mat1[offset1] * mat2[offset2];
            }
        }
    }

    // Record the finish time        
    double end = omp_get_wtime();

    // TODO: save the output matrix to the output csv file
    // Write the output CSV file
    for (int i = 0; i < n_row1; ++i)                    // Iterate output matrix rows
    {
        for (int j = 0; j < n_col2-1; ++j)              // Iterate output matrix columns
        {
            offset_out = i * n_col2 + j;                // Set output offset
            // write into output file
            fprintf(outputFile, "%ld,", mat_out[offset_out]);
        }
        // Write last element of row
        offset_out++;
        if (i != n_row1-1)
        {
            fprintf(outputFile, "%ld\n", mat_out[offset_out]);
        }
        else fprintf(outputFile, "%ld", mat_out[offset_out]);                      
    }

    // Time calculation (in seconds)
    double time_passed = end - start;

    // Save time to file
    fprintf(outputTime, "%f", time_passed);

    // Cleanup
    fclose(inputMatrix1);
    fclose(inputMatrix2);
    fclose(outputFile);
    fclose(outputTime);
    // Remember to free your buffers!
    free(mat1);
    free(mat2);
    free(mat_out);

    return 0;
}

