#include <stdio.h>
#include <string.h>



typedef int Integer;



struct point {
    Integer x;
    Integer y;
};

typedef struct point PT;

void printPoint(PT p) {  // void printPoint(struct point p) will still work
    printf("(%d, %d)\n", p.x,  p.y); 
}



PT makePoint(Integer x, Integer y) {
    PT tmp;
    tmp.x = x;
    tmp.y = y;
    return tmp;
}

PT addPoint(PT p1, PT p2) {
    PT tmp;
    tmp.x = p1.x + p2.x;
    tmp.y = p1.y + p2.y;
    return tmp;
}

int main()
{
    PT p1, p2;
    p1 = makePoint(5, 10);
    p2 = p1;
    printPoint(p1);
    printPoint(p2);
    PT p3 = addPoint(p1, p2);
    printPoint(p3);
    return 0;
}