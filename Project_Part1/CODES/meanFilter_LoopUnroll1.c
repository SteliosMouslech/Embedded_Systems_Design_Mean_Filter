#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>


#define N 330    //number of rows
#define M 440    //number of columns
#define RADIUS 2 //so kernel is 3x3
#define file_in "dog_440x330.yuv"
#define file_out "outDogUnroll.y"
#define UNROLL 4 //if unroll factor changes loops must also be changed


#pragma arm section zidata="ram"
int image_Y[N][M]; // Original image only Y(luminance)
int newImg[N][M];  //final image after filter, only Y(luminance)

#pragma arm section

//read image file
void read() {
  int i,j;
  int point;
  
  FILE *frame_c;
  if((frame_c=fopen(file_in,"rb"))==NULL) {
    printf("current frame doesn't exist\n");
    exit(-1);
  }
  for(i=0; i<N; i++) {
    point=M%UNROLL; //UNROLL factor is 4
    for(j=0; j<point; j++) {
      image_Y[i][j]=fgetc(frame_c);
    }
    for(j=point; j<M; j+=UNROLL) {
       image_Y[i][j]=fgetc(frame_c);
       image_Y[i][j+1]=fgetc(frame_c);
       image_Y[i][j+2]=fgetc(frame_c);
       image_Y[i][j+3]=fgetc(frame_c);
	}
  }
  fclose(frame_c);
}

//write Y image to file
void write() {
  int i,j;
  int point;
  FILE *frame_yuv;
  frame_yuv=fopen(file_out,"wb");
  for(i=0; i<N; i++) {
  	point=M%UNROLL; //UNROLL factor is 4
    for(j=0; j<point; j++) {
      fputc(newImg[i][j],frame_yuv);
    }
    for(j=point; j<M; j+=UNROLL) {
      fputc(newImg[i][j],frame_yuv);
      fputc(newImg[i][j+1],frame_yuv);
      fputc(newImg[i][j+2],frame_yuv);
      fputc(newImg[i][j+3],frame_yuv);        
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
      //to fully unroll we assume to know the windowRadius beforehand!
	  //here we use windowRadius = 2
      i=-windowRadius;
	  xx=min(max(x+i,0),imgWidth-1);
	  j=-windowRadius;
      yy=min(max(y+j,0),imgHeight-1);
      newImg[y][x]=newImg[y][x] + image_Y[yy][xx];
      j++;
	  yy=min(max(y+j,0),imgHeight-1);
      newImg[y][x]=newImg[y][x] + image_Y[yy][xx];
	  j++;
	  yy=min(max(y+j,0),imgHeight-1);
      newImg[y][x]=newImg[y][x] + image_Y[yy][xx];
	  j++;
	  yy=min(max(y+j,0),imgHeight-1);
      newImg[y][x]=newImg[y][x] + image_Y[yy][xx];
	  j++;
	  yy=min(max(y+j,0),imgHeight-1);
      newImg[y][x]=newImg[y][x] + image_Y[yy][xx];
		
	  i++;
	  xx=min(max(x+i,0),imgWidth-1);
	  j=-windowRadius;
      yy=min(max(y+j,0),imgHeight-1);
      newImg[y][x]=newImg[y][x] + image_Y[yy][xx];
      j++;
	  yy=min(max(y+j,0),imgHeight-1);
      newImg[y][x]=newImg[y][x] + image_Y[yy][xx];
	  j++;
	  yy=min(max(y+j,0),imgHeight-1);
      newImg[y][x]=newImg[y][x] + image_Y[yy][xx];
	  j++;
	  yy=min(max(y+j,0),imgHeight-1);
      newImg[y][x]=newImg[y][x] + image_Y[yy][xx];
	  j++;
	  yy=min(max(y+j,0),imgHeight-1);
      newImg[y][x]=newImg[y][x] + image_Y[yy][xx];
        
	  i++;
	  xx=min(max(x+i,0),imgWidth-1);
	  j=-windowRadius;
      yy=min(max(y+j,0),imgHeight-1);
      newImg[y][x]=newImg[y][x] + image_Y[yy][xx];
      j++;
	  yy=min(max(y+j,0),imgHeight-1);
      newImg[y][x]=newImg[y][x] + image_Y[yy][xx];
	  j++;
	  yy=min(max(y+j,0),imgHeight-1);
      newImg[y][x]=newImg[y][x] + image_Y[yy][xx];
	  j++;
	  yy=min(max(y+j,0),imgHeight-1);
      newImg[y][x]=newImg[y][x] + image_Y[yy][xx];
	  j++;
	  yy=min(max(y+j,0),imgHeight-1);
      newImg[y][x]=newImg[y][x] + image_Y[yy][xx];
      
	  i++;
	  xx=min(max(x+i,0),imgWidth-1);
	  j=-windowRadius;
      yy=min(max(y+j,0),imgHeight-1);
      newImg[y][x]=newImg[y][x] + image_Y[yy][xx];
      j++;
	  yy=min(max(y+j,0),imgHeight-1);
      newImg[y][x]=newImg[y][x] + image_Y[yy][xx];
	  j++;
	  yy=min(max(y+j,0),imgHeight-1);
      newImg[y][x]=newImg[y][x] + image_Y[yy][xx];
	  j++;
	  yy=min(max(y+j,0),imgHeight-1);
      newImg[y][x]=newImg[y][x] + image_Y[yy][xx];
	  j++;
	  yy=min(max(y+j,0),imgHeight-1);
      newImg[y][x]=newImg[y][x] + image_Y[yy][xx];
              
	  i++;
	  xx=min(max(x+i,0),imgWidth-1);
	  j=-windowRadius;
      yy=min(max(y+j,0),imgHeight-1);
      newImg[y][x]=newImg[y][x] + image_Y[yy][xx];
      j++;
	  yy=min(max(y+j,0),imgHeight-1);
      newImg[y][x]=newImg[y][x] + image_Y[yy][xx];
	  j++;
	  yy=min(max(y+j,0),imgHeight-1);
      newImg[y][x]=newImg[y][x] + image_Y[yy][xx];
	  j++;
	  yy=min(max(y+j,0),imgHeight-1);
      newImg[y][x]=newImg[y][x] + image_Y[yy][xx];
	  j++;
	  yy=min(max(y+j,0),imgHeight-1);
      newImg[y][x]=newImg[y][x] + image_Y[yy][xx];
				  
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



