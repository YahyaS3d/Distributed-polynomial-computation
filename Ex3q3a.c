#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>

#define MAX_DEGREE 10
#define BUFFER_SIZE 10
#define MAX_INPUT_LENGTH 128
//writer
typedef struct Polynomial {
    int degree;
    int coefficients[MAX_DEGREE + 1];
} Polynomial;

Polynomial parsePolynomial(const char* input) {
    Polynomial poly;
    char* token = strtok(input, "(,:");
    poly.degree = atoi(token);

    token = strtok(NULL, ",:)");
    int i = poly.degree;
    while (token != NULL && i >= 0) {
        poly.coefficients[i] = atoi(token);
        token = strtok(NULL, ",:)");
        i--;
    }

    return poly;
}

void addPolynomials(const Polynomial* poly1, const Polynomial* poly2, Polynomial* result) {
    int maxDegree = poly1->degree > poly2->degree ? poly1->degree : poly2->degree;
    result->degree = maxDegree;

    for (int i = 0; i <= maxDegree; i++) {
        int coeff1 = (i <= poly1->degree) ? poly1->coefficients[i] : 0;
        int coeff2 = (i <= poly2->degree) ? poly2->coefficients[i] : 0;
        result->coefficients[i] = coeff1 + coeff2;
    }
}

void subPolynomials(const Polynomial* poly1, const Polynomial* poly2, Polynomial* result) {
    int maxDegree = poly1->degree > poly2->degree ? poly1->degree : poly2->degree;
    result->degree = maxDegree;

    for (int i = 0; i <= maxDegree; i++) {
        int coeff1 = (i <= poly1->degree) ? poly1->coefficients[i] : 0;
        int coeff2 = (i <= poly2->degree) ? poly2->coefficients[i] : 0;
        result->coefficients[i] = coeff1 - coeff2;
    }
}

void* threadFunction(void* args) {
    Polynomial* poly1 = ((Polynomial**)args)[0];
    Polynomial* poly2 = ((Polynomial**)args)[1];
    Polynomial* result = ((Polynomial**)args)[2];
    int index = *((int*)(((Polynomial**)args)[3]));

    addPolynomials(poly1, poly2, &result[index]);

    return NULL;
}

int main() {
    // Create shared memory for the buffer
    int shm_fd = shm_open("/shared_buffer", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, BUFFER_SIZE * sizeof(Polynomial));
    Polynomial* buffer = (Polynomial*)mmap(NULL, BUFFER_SIZE * sizeof(Polynomial), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Create a semaphore for synchronization
    sem_t* sem = sem_open("/buffer_sem", O_CREAT, 0666, 1);

    // Read input and write to the shared buffer
    char input[MAX_INPUT_LENGTH];

    while (1) {
        printf("Enter operation (or 'END' to exit): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "END") == 0) {
            break;
        }

        // Parse the polynomial and operation
        Polynomial poly1, poly2;
        Polynomial result;

        char* operand1 = strtok(input, " ");
        char* operation = strtok(NULL, " ");
        char* operand2 = strtok(NULL, "");

        poly1 = parsePolynomial(operand1);
        poly2 = parsePolynomial(operand2);

        // Perform the operation
        if (strcmp(operation, "ADD") == 0) {
            addPolynomials(&poly1, &poly2, &result);
        } else if (strcmp(operation, "SUB") == 0) {
            subPolynomials(&poly1, &poly2, &result);
        } else {
            printf("Invalid operation.\n");
            continue;
        }

        // Write the result to the shared buffer
        sem_wait(sem);
        int i;
        for (i = 0; i < BUFFER_SIZE; i++) {
            if (buffer[i].degree == -1) {  // Find an empty slot in the buffer
                buffer[i] = result;
                break;
            }
        }
        sem_post(sem);

        if (i == BUFFER_SIZE) {
            printf("Buffer is full. Cannot write the result.\n");
        } else {
            printf("Result written to the buffer.\n");
        }
    }

    // Cleanup
    sem_close(sem);
    sem_unlink("/buffer_sem");
    munmap(buffer, BUFFER_SIZE * sizeof(Polynomial));
    close(shm_fd);
    shm_unlink("/shared_buffer");

    return 0;
}
