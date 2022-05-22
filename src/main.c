#include <stdio.h>
#include <stdbool.h>
#include <espl_lib.h>

int main()
{
    int i = 0;
    char runagain;
    bool again = true;

    while(again)
    {
        printf("Enter a Number\n");
        scanf("%i", &i);
        printf("The number you entered was: %s\n", num_to_words(i));
        printf("Do you want to run the program again= [y/n]\n");
        scanf(" %c", &runagain);
        if(runagain == 'n')
        {
            again = false;
        }
    }
    return 0;
}
