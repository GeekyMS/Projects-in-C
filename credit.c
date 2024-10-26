#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //defining variables and asking for input
    long n = get_long("Please Enter your credit card number: ");
    long x = n;
    int count = 0;

    //calculating number of digits
    while (x > 0)
    {
        x = x / 10;
        count++;
    }

    long z = 1;
    int k = 0;

    //loop for finding divisor to calculate first two digis
    do
    {
        z *= 10;
        k++;
    }
    while (k < count - 2);

    //defining more variables, giving the value of n to y and h to use for determining sum
    int first_two_digits = n / z;
    int first_digit = first_two_digits / 10;
    long y = n / 10;
    long h = n;
    int sum1 = 0;
    int sum2 = 0;

    //for loop for checksum
    for (int i = 0; i < count; i++)
    {
        int f = y % 10;
        y /= 100;
        int g = 2 * (f);
        sum1 = sum1 + (g % 10) + (g / 10);
    }

    for (int i = 0; i < count; i++)
    {
        int e = h % 10;
        h /= 100;
        sum2 += e;
    }

    int sum = sum1 + sum2;

    //applying conditions for validity and giving appropriate output
    if (sum % 10 == 0)
    {
        if (count == 15 && (first_two_digits == 34 || first_two_digits == 37))      //condition for AMEX
        {
            printf("AMEX\n");
        }
        else if (count == 16 && (first_two_digits == 51 || first_two_digits == 52 || first_two_digits == 53 || first_two_digits == 54
                                 || first_two_digits == 55))        //condition for MASTERCARD
        {
            printf("MASTERCARD\n");
        }
        else if ((count == 13 || count == 16) && first_digit == 4)      //condition for VISA
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }

}