//
//  Roll number : 1 <= r < 2^20 [Bits 0 - 19]
//  Subject : 1 <= s < 2^4      [Bits 20 - 23]
//	Score : 0 <= sc <= 250      [Bits 24 - 31]
//

#include <stdio.h>
#include <string.h>

void printBits(int x)
{
    int i;
    int mask = (1 << 31);
    for (i = 0; i < 32; i++)
    {
        printf("%d", ((x & mask) ? 1 : 0));
        x <<= 1;
    }
    printf("\n");
}

unsigned int encode(int roll, int subject, int score) {
    return score << 24 | subject << 20 | roll;
}

void decode(unsigned int encoded, int *pRoll, int *pSubject, int *pScore) {
    *pRoll = encoded & (unsigned int)~0>>12;
    *pSubject = (encoded>>20) & (unsigned int)~0>>28;
    *pScore = (encoded>>24) & (unsigned int)~0>>24;
}

int main()
{
    int roll, subject, score;
    unsigned int encoded;
    char cmd[10];

    while (1) {
        printf("Enter command: ");
        scanf("%s", cmd);
        if (!strcmp(cmd, "encode")) {
            printf("Roll: ");
            scanf("%d", &roll);
            printf("Subject: ");
            scanf("%d", &subject);
            printf("Score: ");
            scanf("%d", &score);
            encoded = encode(roll, subject, score);
            printf("Encoded value is: %u\n", encoded);
            printBits(encoded);
        } else if (!strcmp(cmd, "decode")) {
            printf("Encoded value: ");
            scanf("%u", &encoded);
            decode(encoded, &roll, &subject, &score);
            printf("Roll: %d\nSubject: %d\nScore: %d\n", roll, subject, score);
        } else if (!strcmp(cmd, "quit") || !strcmp(cmd, "exit")) {
            printf("Bye!");
            break;
        } else {
            printf("Unknown command. Try again.\n");
        }
    }

    return 0;
}
