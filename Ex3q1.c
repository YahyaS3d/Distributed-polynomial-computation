#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_DEGREE 10
#define MAX_INPUT_LENGTH 128
// Polynomial structure - (degree: coefficient) operation (degree: coefficient)
//the operation can be: add/ sub/ multi
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

void mulPolynomials(const Polynomial* poly1, const Polynomial* poly2, Polynomial* result) {
    int maxDegree = poly1->degree + poly2->degree;
    result->degree = maxDegree;

    memset(result->coefficients, 0, sizeof(result->coefficients));

    for (int i = 0; i <= poly1->degree; i++) {
        for (int j = 0; j <= poly2->degree; j++) {
            result->coefficients[i + j] += poly1->coefficients[i] * poly2->coefficients[j];
        }
    }
}

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

int main() {
    char input[MAX_INPUT_LENGTH];

    while (1) {
//        printf("Enter operation (or 'END' to exit): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "END") == 0) {
            break;
        }

        Polynomial poly1, poly2, result;

        char* operand1 = strtok(input, " ");
        char* operation = strtok(NULL, " ");
        char* operand2 = strtok(NULL, "");

        poly1 = parsePolynomial(operand1);
        poly2 = parsePolynomial(operand2);

        if (strcmp(operation, "ADD") == 0) {
            addPolynomials(&poly1, &poly2, &result);
        } else if (strcmp(operation, "SUB") == 0) {
            subPolynomials(&poly1, &poly2, &result);
        } else if (strcmp(operation, "MUL") == 0) {
            mulPolynomials(&poly1, &poly2, &result);
        } else {
            printf("Invalid operation.\n");
            continue;
        }

        printPolynomial(&result);
    }

    return 0;
}