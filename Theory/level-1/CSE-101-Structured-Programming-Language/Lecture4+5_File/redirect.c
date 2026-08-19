#include <stdio.h>

int main () {
   FILE *read;
   FILE *write;

   printf("This text is redirected to stdout ( AKA, prints in the usual console!\n");

   write = freopen("out.txt", "w", stdout);//programme nijee likhbe
   read =  freopen("in.txt", "r", stdin);

   printf("This text is redirected to file.txt\n");
   int i,limit;
   scanf("%d",&limit);
   for (i=1;i<=limit;i++)
   {
       printf("%d\n",i);
   }

   //fclose(read);
   //fclose(write);
   return(0);
}


// scanf =m, n
// for (i=0;i<m;i++)
// {
//     for (j=0;j<n;j++)
//     {
//         scanf a[i,j]
//     }
// }

// 3 4 

// 1 2 3 4
// 5 6 7 8
// 7 8 9