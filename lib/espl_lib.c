#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALLOC_DIGIT_STRING(STRING)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 \
    ret = calloc(strlen(#STRING) + 1, sizeof(char));                                                                                                                                                                                                                                                                                                                                                                                                                                                               \
    if (!ret)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      \
        return NULL;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               \
    strcpy(ret, #STRING);

char* get_digit_word(unsigned char digit)
{
    char* ret = NULL;
    switch (digit) {
    case 0:
        ALLOC_DIGIT_STRING(zero)
        break;
    case 1:
        ALLOC_DIGIT_STRING(one)
        break;
    case 2:
        ALLOC_DIGIT_STRING(two)
        break;
    case 3:
        ALLOC_DIGIT_STRING(three)
        break;
    case 4:
        ALLOC_DIGIT_STRING(four)
        break;
    case 5:
        ALLOC_DIGIT_STRING(five)
        break;
    case 6:
        ALLOC_DIGIT_STRING(six)
        break;
    case 7:
        ALLOC_DIGIT_STRING(seven)
        break;
    case 8:
        ALLOC_DIGIT_STRING(eight)
        break;
    case 9:
        ALLOC_DIGIT_STRING(nine)
        break;
    default:
        break;
    }
    return ret;
}

char* num_to_words(unsigned int num)
{
    char* ret = NULL;
    char space[] = " ";
    char* tmp;

    if (!num)
        return ret;

    while (num) {
        tmp = get_digit_word(num % 10);
        if (!tmp)
            goto error;

        if (!ret)
            ret = calloc(1, sizeof(char) * (strlen(tmp) + 1));
        else
            ret = realloc(ret, sizeof(char) * (strlen(ret) + strlen(tmp) + 1));

        if (!ret)
            goto error;

        strcat(ret, tmp);
        free(tmp);

        num /= 10;
        if (num) {
            ret = realloc(ret, sizeof(char) * (strlen(ret) + 2));
            strcat(ret, space);
        }
    }
    return ret;
error:
    if (!ret)
        free(ret);
    if (!tmp)
        free(tmp);
    return NULL;
}
