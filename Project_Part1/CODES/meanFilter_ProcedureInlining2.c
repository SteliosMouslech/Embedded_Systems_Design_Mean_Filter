#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>


#define N 330    //number of rows
#define M 440    //number of columns
#define RADIUS 2 //so kernel is 3x3
#define file_in "dog_440x330.yuv"
#define file_out "outDogInline.y"


#pragma arm section zidata="ram"
int image_Y[N][M]; // Original image only Y(luminance)
int newImg[N][M];  //final image after filter, only Y(luminance)

#pragma arm section

int main() {
  int x,y,i,j,xx,yy;
  int imgWidth=M;
  int imgHeight=N;
  int max;
  int windowRadius=RADIUS;
  FILE *frame_yuv; //write file

  //read image file
  FILE *frame_c;
  if((frame_c=fopen(file_in,"rb"))==NULL) {
    printf("current frame doesn't exist\n");
    exit(-1);
  }
  for(i=0;i<N;i++) {
    for(j=0;j<M;j++) {
      image_Y[i][j]=fgetc(frame_c);
    }
  }
  fclose(frame_c);
  
  
  
//mean filter implementation
  for (x=0; x<imgWidth; x++) {
    for (y=0; y<imgHeight; y++) {
      newImg[y][x]=0;
      for (i=-windowRadius; i<=windowRadius; i++) {
        for (j=-windowRadius; j<=windowRadius; j++) {
        	
          // inline of xx=min(max(x+i,0),imgWidth-1);
          if(x+i>0) { //max
          	max=x+i;
		  }
		  else {
		  	max=0;
		  }
		  if(max<imgWidth-1) { //min
		  	xx=max;
		  }
		  else {
		  	xx=imgWidth-1;
		  }
		  
		  //inline of yy=min(max(y+j,0),imgHeight-1);
		  if(y+j>0) { //max
          	max=y+j;
		  }
		  else {
		  	max=0;
		  }
		  if(max<imgHeight-1) { //min
		  	yy=max;
		  }
		  else {
		  	yy=imgHeight-1;
		  }
          newImg[y][x]=newImg[y][x] + image_Y[yy][xx];
        }
      }
      newImg[y][x]=(int)(((double)newImg[y][x] / ((double)((windowRadius<<1)+1)*((windowRadius<<1)+1)))+0.5);//inline rounding
    }
  }
  
  //write Y image to file
  frame_yuv=fopen(file_out,"wb");
  for(i=0;i<N;i++) {
    for(j=0;j<M;j++) {
      fputc(newImg[i][j],frame_yuv);
    }
  }
  fclose(frame_yuv);
  
  return 0;
}



