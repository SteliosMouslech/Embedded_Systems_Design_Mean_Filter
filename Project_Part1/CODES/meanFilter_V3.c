#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>


#define N 330    //number of rows
#define M 440    //number of columns
#define RADIUS 2 //so kernel is 5x5
#define UNROLL 4 //if unroll factor changes loops must also be changed
#define file_in "dog_440x330.yuv"
#define file_out "outDogV3.y"


#pragma arm section zidata="ram"
int image_Y[N+2*RADIUS][M+2*RADIUS]; // Original image only Y(luminance)
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
  point=M%UNROLL; //UNROLL factor is 4
  for(i=RADIUS; i<N+RADIUS; i++) {
    for(j=RADIUS;j<point+RADIUS;j++) {
      image_Y[i][j]=fgetc(frame_c);
    }  
    for(j=point+RADIUS;j<M+RADIUS;j+=UNROLL) {
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
  point=M%UNROLL; //UNROLL factor is 4
  for(i=0; i<N; i++) {
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

//mean filter implementation
void meanFilter(int windowRadius) {
  int x,y,i,j;
  int imgWidth=M;
  int imgHeight=N;
  double pixels_inKernel;
  int sum; //local sum variable
  //computing num of pixels in Kernel
  pixels_inKernel = (double)((windowRadius<<1)+1)*((windowRadius<<1)+1);

  for (y=windowRadius; y<imgHeight+windowRadius; y++) {
    for (x=windowRadius; x<imgWidth+windowRadius; x++) {
      sum=0;

      //to fully unroll we assume to know the windowRadius beforehand!
	  //here we use windowRadius = 2
	  j=-windowRadius;
	  i=-windowRadius;
	  sum=sum+image_Y[y+j][x+i];
	  i++;
	  sum=sum+image_Y[y+j][x+i];
	  i++;
	  sum=sum+image_Y[y+j][x+i];
	  i++;
	  sum=sum+image_Y[y+j][x+i];
	  i++;
	  sum=sum+image_Y[y+j][x+i];
	  
	  j++;
	  i=-windowRadius;
	  sum=sum+image_Y[y+j][x+i];
	  i++;
	  sum=sum+image_Y[y+j][x+i];
	  i++;
	  sum=sum+image_Y[y+j][x+i];
	  i++;
	  sum=sum+image_Y[y+j][x+i];
	  i++;
	  sum=sum+image_Y[y+j][x+i];
	  
	  j++;
	  i=-windowRadius;
	  sum=sum+image_Y[y+j][x+i];
	  i++;
	  sum=sum+image_Y[y+j][x+i];
	  i++;
	  sum=sum+image_Y[y+j][x+i];
	  i++;
	  sum=sum+image_Y[y+j][x+i];
	  i++;
	  sum=sum+image_Y[y+j][x+i];
	  
	  j++;
	  i=-windowRadius;
	  sum=sum+image_Y[y+j][x+i];
	  i++;
	  sum=sum+image_Y[y+j][x+i];
	  i++;
	  sum=sum+image_Y[y+j][x+i];
	  i++;
	  sum=sum+image_Y[y+j][x+i];
	  i++;
	  sum=sum+image_Y[y+j][x+i];
	  
	  j++;
	  i=-windowRadius;
	  sum=sum+image_Y[y+j][x+i];
	  i++;
	  sum=sum+image_Y[y+j][x+i];
	  i++;
	  sum=sum+image_Y[y+j][x+i];
	  i++;
	  sum=sum+image_Y[y+j][x+i];
	  i++;
	  sum=sum+image_Y[y+j][x+i];
	  //inline myround
      newImg[y-windowRadius][x-windowRadius]=(int)(((double)sum / pixels_inKernel)+0.5);
    }
  }
}


int main() {
  int i,j,point;
  point=M%UNROLL;
  read();
  
  //Get the values for corner cases of augmented matrix
  for (i=0; i<RADIUS; i++) {
    for (j=0; j<RADIUS; j++){
      //upper left and right corner values
      image_Y[i][j] = image_Y[RADIUS][RADIUS];
      image_Y[i][RADIUS+M+j] = image_Y[RADIUS][RADIUS+M-1];

      //lower left and right corner values
      image_Y[RADIUS+N+i][j] = image_Y[RADIUS+N-1][RADIUS];
      image_Y[RADIUS+N+i][RADIUS+M+j] = image_Y[RADIUS+N-1][RADIUS+M-1];
    }
	//FUSION
	//get the values for upper and lower rows of augmented matrix
	for(j=RADIUS; j<point+RADIUS; j++) {
      image_Y[i][j]=image_Y[RADIUS][j];
      image_Y[RADIUS+N+i][j]=image_Y[RADIUS+N-1][j];	
	}
    for(j=point+RADIUS; j<RADIUS+M; j++){
      image_Y[i][j]=image_Y[RADIUS][j];
      image_Y[RADIUS+N+i][j]=image_Y[RADIUS+N-1][j];
	  
	  j++;
	  image_Y[i][j]=image_Y[RADIUS][j];
      image_Y[RADIUS+N+i][j]=image_Y[RADIUS+N-1][j];
	  
	  j++;
	  image_Y[i][j]=image_Y[RADIUS][j];
      image_Y[RADIUS+N+i][j]=image_Y[RADIUS+N-1][j];
	  
	  j++;
	  image_Y[i][j]=image_Y[RADIUS][j];
      image_Y[RADIUS+N+i][j]=image_Y[RADIUS+N-1][j];
    }
  }

  //get the values for outer left and outer right columns of augmented matrix
  for (i=RADIUS; i<RADIUS+N; i++) {
    for (j=0; j<RADIUS; j++){
      image_Y[i][j]=image_Y[i][RADIUS];
      image_Y[i][RADIUS+M+j]=image_Y[i][RADIUS+M-1];
    }
  }
    
  meanFilter(RADIUS);
  write();
  
  return 0;
}

