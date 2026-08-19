#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "utils.h"

Image *read_PPM(char *filename) 
{
    
    FILE *fp=fopen("filename","rb");

    Image picture;
    
    fscanf(fp,"%d %d",&picture.cols,&picture.rows);
   

    picture.image=(Color**)malloc(picture.rows*sizeof(Color*));
    for(int i=0;i<picture.rows;i++){
       picture.image[i]=(Color*)malloc(picture.cols*sizeof(Color));
    }
   
    for(int i=0;i<picture.rows;i++){
        for(int j=0;j<picture.cols;j++){
            Color pixel;
            fscanf(fp,"%d %d %d",&pixel.R,&pixel.G,&pixel.B);
            picture.image[i][j]=pixel;// 3 ta value ekshathe kore proti ta pixel banacchi
        }
    }  
    fclose(fp);
    return &picture;

}



void write_PPM(Image *image, char *filename)
{
	
    FILE *fp=fopen("secret.ppm","wb");
    fprintf(fp,"p3");
    
     fprintf(fp,"%d %d",image->cols,image->rows);
     fprintf(fp,"%d",255);
   
    for(int i=0;i<image->rows;i++){
        for(int j=0;j<image->cols;j++){
            Color pixel;
            fprintf(fp,"%d %d %d",&pixel.R,&pixel.G,&pixel.B);
        }
    }


}

void free_image(Image *image)
{
	
   for(int i=0;i<image->rows;i++){
       free(image->cols);
   }
   free(image);
}


Color *evaluate_one_pixel(Image *image, int row, int col)
{       

    int i;
    if(image){

         for(i=0;i<image->rows*image->cols;i++){  
            Color.R   = 0x00;
            pixelData[M].blue   = 0xef;
            pixelData[M].green  = 0x00;
         }
    }
}



}

Image *get_secret_image(Image *image)
{
	
}

int main()
{   
    Image *image;
    image = read_PPM("toy.ppm");
    write_PPM(image,"secret.ppm");
    free_image(image);
    evaluate_one_pixel(image,image->rows,image->cols);
    get_secret_image(Image *image)



	return 0;
}


