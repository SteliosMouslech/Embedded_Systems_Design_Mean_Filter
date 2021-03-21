#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>


#define N 330    //number of rows
#define M 440    //number of columns
#define RADIUS 2 //so kernel is 3x3
#define file_in "dog_440x330.yuv"
#define file_out "outDogInitial.y"


#pragma arm section zidata="ram"
int image_Y[N][M]; // Original image only Y(luminance)
int newImg[N][M];  //final image after filter, only Y(luminance)

#pragma arm section

//read image file
void read() {
  int i,j;
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
}

//write Y image to file
void write() {
  int i,j;
  FILE *frame_yuv;
  frame_yuv=fopen(file_out,"wb");
  for(i=0;i<N;i++) {
    for(j=0;j<M;j++) {
      fputc(newImg[i][j],frame_yuv);
    }
  }
  fclose(frame_yuv);
}

//min and max functions to use in meanFilter
int min(int x, int y) {
  if(x<y) {
    return x;
  }
  else {
    return y;
  }
}

int max(int x, int y) {
  if(x>y) {
    return x;
  }
  else {
    return y;
  }
}

//only for positive values
int myround(double x)
{
    return (int)(x + 0.5);
}

//mean filter implementation
void meanFilter(int windowRadius) {
  int x,y,i,j,xx,yy;
  int imgWidth=M;
  int imgHeight=N;

  for (x=0; x<imgWidth; x++) {
    for (y=0; y<imgHeight; y++) {
      newImg[y][x]=0;
      for (i=-windowRadius; i<=windowRadius; i++) {
        for (j=-windowRadius; j<=windowRadius; j++) {
          xx=min(max(x+i,0),imgWidth-1);
          yy=min(max(y+j,0),imgHeight-1);
          newImg[y][x]=newImg[y][x] + image_Y[yy][xx];
        }
      }
      newImg[y][x]=myround((double)newImg[y][x] / pow((double)(2*windowRadius+1),2));
    }
  }
}


int main() {
  read();
  meanFilter(RADIUS);
  write();
  
  return 0;
}



