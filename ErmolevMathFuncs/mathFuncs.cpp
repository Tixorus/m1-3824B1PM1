#include "stdio.h"
#include "stdlib.h"
#include "math.h"

double NthTaylorSin(double x, double prev, int n)
{
    return n == 0 ? x : (-(prev * x * x) / ((2 * n)*(2 * n + 1)));
}

double NthTaylorCos(double x, double prev, int n)
{
    return n == 0 ? 1 : (-(prev * x * x) / ((2 * n) * (2 * n -1)));
}

double NthTaylorExp(double x, double prev, int n)
{
    return n == 0 ? 1 : prev*x/n;
}

double NthTaylorLog(double x, double prev, int n)
{
    return n == 0 ? x : -prev*x*n/(n+1);
}

double OneToN(int n, double T(double, double, int), double x )
{
    double ret = 0, val = 0;
    int i;
    for (i = 0; i < n; i++)
    {
        val = T(x, val, i);
        ret += val;
    }
    return ret;
}

double NToOne(int n, double T(double, double, int), double x)
{
    double ret = 0;
    int i;
    double* arr = (double*)malloc(n * sizeof(double));
    arr[0] = T(x, 0, 0);
    for (i = 1; i < n; i++)
    {
        arr[i] = T(x, arr[i - 1], i);
    }
    for (i = n - 1; i >= 0; i--)
    {
        ret += arr[i];
    }
    free(arr);
    return ret;
}

double PairSumFunc(double* arr, int l, int r)
{
    double ret = 0;
    int i;
    if (r - l <= 5)
    {
        for (i = r-1; i >= l; i--) ret += arr[i];
        return ret;
    }
    return PairSumFunc(arr, l, (l + r) / 2) + PairSumFunc(arr, (l + r) / 2, r);
}

double PairSum(int n, double T(double, double, int), double x)
{
    int i;
    double ret = 0;
    double* arr = (double*)malloc(n * sizeof(double));
    arr[0] = T(x, 0, 0);
    for (i = 1; i < n; i++)
    {
        arr[i] = T(x, arr[i - 1], i);
    }
    ret = PairSumFunc(arr, 0, n);
    free(arr);
    return ret;
}



int main()
{
    double X, sinv, cosv, expv, logv;
    int sum;
    int n;
    printf("Select the X value that you would like to approximate Sin, Cos, Exponent and Log with via the Taylor series: ");
    scanf_s("%lf", &X);
    printf("Values being compared to: Sin: %.16lf, Cos:%.16lf, Exp:%.16lf, log:%.16lf \n\n", sin(X), cos(X), exp(X), log(1 + X));
    printf("Select the summing method you would like to use for the approximation:\n\n1. 1 to N\n2. N to 1. \n3. Pairs\n");
    scanf_s("%d", &sum);
    printf("\n\n");
    if (sum == 1)
    {
        for (n = 1; n < 100000000; n *= 2)
        {
            sinv = fabs(OneToN(n, NthTaylorSin, X)-sin(X));
            cosv = fabs(OneToN(n, NthTaylorCos, X) - cos(X));
            expv = fabs(OneToN(n, NthTaylorExp, X) - exp(X));
            printf("N = %d: Sin diff = %.16lf, Cos diff = %.16lf, Exp diff = %.16lf", n, sinv, cosv, expv);
            if (X <= 1 && X >= -1)
            {
                logv = fabs(OneToN(n, NthTaylorLog, X) - log(X + 1));
                printf(", Log diff = %.16lf", logv);
            }
            printf("\n\n");
        }
    }
    else if (sum == 2)
    {
        for (n = 1; n < 100000000; n *= 2)
        {
            sinv = fabs(NToOne(n, NthTaylorSin, X) - sin(X));
            cosv = fabs(NToOne(n, NthTaylorCos, X) - cos(X));
            expv = fabs(NToOne(n, NthTaylorExp, X) - exp(X));
            printf("N = %d: Sin diff = %.16lf, Cos diff = %.16lf, Exp diff = %.16lf", n, sinv, cosv, expv);
            if (X <= 1 && X >= -1)
            {
                logv = fabs(NToOne(n, NthTaylorLog, X) - log(X + 1));
                printf(", Log diff = %.16lf", logv);
            }
            printf("\n\n");
        }
    }
    else if (sum == 3)
    {
        for (n = 1; n < 100000000; n *= 2)
        {
            sinv = fabs(PairSum(n, NthTaylorSin, X) - sin(X));
            cosv = fabs(PairSum(n, NthTaylorCos, X) - cos(X));
            expv = fabs(PairSum(n, NthTaylorExp, X) - exp(X));
            printf("N = %d: Sin diff = %.16lf, Cos diff = %.16lf, Exp diff = %.16lf", n, sinv, cosv, expv);
            if (X <= 1 && X >= -1)
            {
                logv = fabs(PairSum(n, NthTaylorLog, X) - log(X + 1));
                printf(", Log diff = %.16lf", logv);
            }
            printf("\n\n");
        }
    }
}