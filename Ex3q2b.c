#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>

#define MAX_DEGREE 10
#define BUFFER_SIZE 10
//reader
/*
 * The second program is the reader that accesses the shared buffer
 * retrieves the polynomial results, and calculates the polynomial value for a specific input.
 * It opens the shared memory segment created by the first program, uses the semaphore for synchronization
 * and processes a total of 5 results from the buffer. It prints the result and the calculated value for each polynomial.
 */
typedef struct Polynomial {
    int degree;
    int coefficients[MAX_DEGREE + 1];
} Polynomial;

void printPolynomial(const Polynomial* poly) {
    int printed = 0;

    for (int i = poly->degree; i >= 0; i--) {
        int coeff = poly->coefficients[i];
        if (coeff != 0) {
            if (printed) {
                if (coeff > 0) {
                    printf(" + ");
                } else {
                    printf(" - ");
                    coeff = -coeff;
                }
            } else {
                if (coeff < 0) {
                    printf("-");
                    coeff = -coeff;
                }
            }

            if (i == 0) {
                printf("%d", coeff);
            } else if (i == 1) {
                printf("%dx", coeff);
            } else {
                printf("%dx^%d", coeff, i);
            }

            printed = 1;
        }
    }

    printf("\n");
}

double calculatePolynomial(const Polynomial* poly, double x) {
    double result = 0.0;
    double power = 1.0;

    for (int i = 0; i <= poly->degree; i++) {
        result += poly->coefficients[i] * power;
        power *= x;
    }

    return result;
}

int main() {
    // Open the shared memory for the buffer
    int shm_fd = shm_open("/shared_buffer", O_RDWR, 0666);
    Polynomial* buffer = (Polynomial*)mmap(NULL, BUFFER_SIZE * sizeof(Polynomial), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Open the semaphore for synchronization
    sem_t* sem = sem_open("/buffer_sem", 0);

    // Read and process the results from the shared buffer
    int processedCount = 0;
    int totalCount = 5;  // Example: Process a total of 5 results

    while (processedCount < totalCount) {
        sem_wait(sem);
        int i;
        for (i = 0; i < BUFFER_SIZE; i++) {
            if (buffer[i].degree != -1) {  // Find a valid result in the buffer
                Polynomial result = buffer[i];
                buffer[i].degree = -1;  // Mark the slot as empty
                sem_post(sem);

                // Process the result
                printf("Result: ");
                printPolynomial(&result);

                // Calculate the polynomial for a specific value
                double x = 2.5;  // Example input value
                double calculatedResult = calculatePolynomial(&result, x);
                printf("Calculated Result for x = %.2f: %.2f\n", x, calculatedResult);

                processedCount++;
                break;
            }
        }
        sem_post(sem);

        if (i == BUFFER_SIZE) {
            sleep(1);  // Sleep for a while if the buffer is empty
        }
    }

    // Cleanup
    sem_close(sem);
    munmap(buffer, BUFFER_SIZE * sizeof(Polynomial));
    close(shm_fd);

    return 0;
}
