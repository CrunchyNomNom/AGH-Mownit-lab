#include <stdio.h>
#include <gsl/gsl_ieee_utils.h>

int main (void) {
    float x = 1.0 / 3.0;
    printf("Dividing 1/3 by 2...");
    while(x != 0.0) {
        printf("\nx = %.48f <=>", x);
        gsl_ieee_printf_float(&x);
        x /= 2.0;
    }

    printf("\nThe x reached 0.\n");
    return 0;
}
