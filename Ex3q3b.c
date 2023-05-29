#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <pthread.h>

#define MAX_DEGREE 10
#define BUFFER_SIZE 10
//reader
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

void* threadFunction(void* args) {
    Polynomial* buffer = ((Polynomial**)args)[0];
    sem_t* sem = ((sem_t**)args)[1];
    int index = *((int*)(((Polynomial**)args)[2]));

    Polynomial result;
    int processedCount = 0;
    int totalCount = 5;

    while (processedCount < totalCount) {
        sem_wait(sem);
        if (buffer[index].degree != -1) {  // Find a valid result in the buffer
            result = buffer[index];
            buffer[index].degree = -1;  // Mark the slot as empty
            sem_post(sem);

            // Process the result
            printf("Result: ");
            printPolynomial(&result);

            // Calculate the polynomial for a specific value
            double x = 2.5;  // Example input value
            double calculatedResult = calculatePolynomial(&result, x);
            printf("Calculated Result for x = %.2f: %.2f\n", x, calculatedResult);

            processedCount++;
        } else {
            sem_post(sem);
            sleep(1);  // Sleep for a while if the buffer is empty
        }
    }

    return NULL;
}

int main() {
    // Open the shared memory for the buffer
    int shm_fd = shm_open("/shared_buffer", O_RDWR, 0666);
    Polynomial* buffer = (Polynomial*)mmap(NULL, BUFFER_SIZE * sizeof(Polynomial), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Open the semaphore for synchronization
    sem_t* sem = sem_open("/buffer_sem", 0);

    // Read and process the results from the shared buffer
    pthread_t threads[BUFFER_SIZE];
    int indices[BUFFER_SIZE];
    void* threadArgs[3] = { buffer, sem, indices };

    for (int i = 0; i < BUFFER_SIZE; i++) {
        indices[i] = i;
        pthread_create(&threads[i], NULL, threadFunction, threadArgs);
    }

    // Wait for all threads to finish
    for (int i = 0; i < BUFFER_SIZE; i++) {
        pthread_join(threads[i], NULL);
    }

    // Cleanup
    sem_close(sem);
    munmap(buffer, BUFFER_SIZE * sizeof(Polynomial));
    close(shm_fd);

    return 0;
}
