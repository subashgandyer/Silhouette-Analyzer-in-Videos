
#include "cv.h"
#include "highgui.h"
#include <conio.h>
#include <stdio.h>
#include "BlobResult.h"
#include "BlobExtraction.h"
#include "cvcam.h"
#include <iostream.h>
//#include "mouse.h"




FILE *fp;//to write in the file
FILE *fs;//to read from the file
char label[100][100],buffer[100],a[100],temp1[100];
int points[100];
int labc=0,pc=0,jis=0,fcount=0;
int X1,X2,Y1,Y2;
int cymin=0,cymax=0,cxmin=0,cxmax=0;
int enymin[100],enymax[100],enxmin[100],enxmax[100];
int num_lines,num_labels;
IplImage* color;

	CvCapture* capture;

// Video Ripper

CvVideoWriter* MyAVIwriter;



IplImage* graylive=0;

 IplImage* img_vid = 0; 

CvVideoWriter *writer = 0;
int isColor = 1;
int fps     = 25;  // or 30
int frameW  = 320; // 744 for firewire cameras
int frameH  = 240; // 480 for firewire cameras





IplImage* erodeimg;
IplImage* blobimg;
IplImage* src;
IplImage* blobmodel;
IplImage* loadlive;
IplImage* dst;
IplImage* back;

int blobcount;
int intblobcount;
int *blobcnt;
int flag=0;//for mouse click events
int x[60000],y[60000];
int pixcount;
int blobhist [584];
int blue,green,red;
int histindex=0;
 int xmin,ymin,xmax,ymax;
 int num;
CvPoint seed;

int k1;//index for blob analysis
int indexnumber=1;
int pixelcount;
int *currentmodelindex; 


#define MINTHRESHOLD 100;

int histogram[1168] = {0,0,0,0};

float Corrsum;

int cvGetIntensity(IplImage* filename,int x, int y)
{
	return ( ((uchar*)(filename->imageData + filename->widthStep*x)) [y] );
	
}


int cvGetIntensity(IplImage* filename,int x, int y,int color)
{
	return ( ((uchar*)(filename->imageData + filename->widthStep*x)) [(y*3)+color] );
	
}


void cvSeperateBlob()
{
	int intblobcount = 0;
	int xmin,ymin,xmax,ymax;
	xmin = ymin = 320;
	xmax = ymax = 0;
	int count = 0;
	pixelcount = 0;
	int x[60000],y[60000];
	for(int i=0;i<238;i++)//238
		for(int j=0;j<320;j++)//320
		{
			if( (cvGetIntensity(src,i,j)== 255)) 
			{
				((uchar*)(src->imageData + src->widthStep*i))[j]=0;

				pixelcount++;

				x[count] = i;
				y[count] = j;


				
				if(xmin > x[count])
				{
					xmin = x[count];
					
				}
				if(xmax < x[count])
				{
					xmax = x[count];
				}
				if(ymin > y[count])
				{
					ymin = y[count];
				}
				if(ymax < y[count])
				{
					ymax = y[count];
				}
				

				count++;

			}

				((uchar*)(blobimg->imageData + blobimg->widthStep*i))[j*3+2] = 0;
				((uchar*)(blobimg->imageData + blobimg->widthStep*i))[j*3+1] = 0;
				((uchar*)(blobimg->imageData + blobimg->widthStep*i))[j*3+0] = 255;


		}


		//printf("\n\nxmin: %d\txmax: %d\tymin: %d\tymax: %d\n\n",xmin,xmax,ymin,ymax);



//printf("\n PIXELCOUNT = %d",pixelcount);


	if(pixelcount > 1000) 
	{
		
		

		printf("HI");
		cvRectangle( loadlive, cvPoint(ymax,xmin),cvPoint(ymin,xmax),CV_RGB(0,0,255), 1 );
		printf("\n RECTAANGLE VERTICES : X1=%d X2=%d Y1=%d Y2=%d",ymin,ymax,xmin,xmax);

		X1=ymin;
		X2=ymax;
		Y1=xmin;
		Y2=xmax;
	//	vjfind(ymin,ymax,xmin,xmax);
	//	printf("\nHI HI ");
		cvShowImage("BOX",loadlive);
	//	cvWaitKey(0);

//		cvComputeHistogram(x,y);
		blobcount++;
	}

}






void cvFindBlob()
{
dst = cvCreateImage( cvGetSize(src), 8, 1 );
//cvCopyImage(src,dst);
CvMemStorage* storage = cvCreateMemStorage(0);
CvSeq* contour = 0;
//cvThreshold( src, src, 1, 255, CV_THRESH_BINARY );
//cvNamedWindow( "Source", 1 );
//cvShowImage( "Source", src );
CvScalar color = CV_RGB( 255,255,255 );
//CvScalar colorlow = CV_RGB(0,255,0);
CvScalar colorhigh = CV_RGB(255,0,0);


//cvconnectedcomp comp;
//cvFloodFill( src, seed, color, colorlow, colorhigh, 0, 8, 0 );

cvFindContours( src, storage, &contour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
cvZero( src );

int c=0;

for( ; contour != 0; contour = contour->h_next )
{
//	printf("%d   ",*contour);

//printf("%d  ",color);
// replace CV_FILLED with 1 to see the outlines 


cvDrawContours( src, contour, color, colorhigh, 0,CV_FILLED, 8);

//printf("\n Contours  :  %d , %d ",seed.x,seed.y);
//cvWaitKey(0);
//printf(comp->area);
//}

cvNamedWindow( "Components", 1 );
cvShowImage( "Components", src );



cvSeperateBlob();

cvShowImage("Blob",blobimg);



//cvWaitKey(0);		************************************************************************************





}

}



void on_mouse( int event, int x, int y, int flags)
{
	
    switch( event )
    {
    case CV_EVENT_LBUTTONDOWN:
        {
            CvPoint seed1 = cvPoint(x,y);
		   if(flag==0)
		   {
			   ymin=seed1.x;
		       fprintf(fp,"%d",ymin);
			   printf("\nx=%d,y=%d",x,y);
			   printf("\n ymin=%d",ymin);
		   }
		   if(flag==1)
		   {
			   ymax=seed1.x;
			   fprintf(fp,"%d",ymax);
			   printf("\nx=%d,y=%d",x,y);
			   printf("\n ymax=%d",ymax);
		   }
		   if(flag==2)
		   {
		       xmin=seed1.y;
			   fprintf(fp,"%d",xmin);
			   printf("\nx=%d,y=%d",x,y);
			   printf("\n xmin=%d",xmin);
		   }
		   if(flag==3)
		   {
			   xmax=seed1.y;
			   fprintf(fp,"%d",xmax);
			   printf("\nx=%d,y=%d",x,y);
			   printf("\n xmax=%d",xmax);
		   }
		//	printf("\n x=%d\ty=%d",x,y);
         //  printf("\n seed.x=%d\tseed.y=%d\n",seed.x,seed.y);
		  // printf("\n ymin=%d ymax=%d xmin=%d xmax=%d",ymin,ymax,xmin,xmax);
			//rem++;
			flag++;

           //to get the distance from a point
             // CBlobGetDistanceFromPoint(x,y);
           //   CBlobGetDistanceFromPoint obj1(x,y);
			//  obj1.operator();
				  // CBlobGetMinX();

			 // printf(CBlobGetMinX());

			if(flag==4)
				flag=0;
		//	fprintf(fp,"%d %d",x,y);
			fprintf(fp,"\n");
			break;
		  }
	case CV_EVENT_RBUTTONDOWN:
		{
			printf("\nEnter the Tag:");
			scanf("%s",temp1);
            fprintf(fp,"%s\n",temp1);
			temp1[0]='\0';
			break;
		}
	case CV_EVENT_MBUTTONDOWN:
		{
            cvRectangle(back, cvPoint(ymax,xmin),cvPoint(ymin,xmax),CV_RGB(255,0,255), 1 );
			//cvRectangle(color, cvPoint(100,50),cvPoint(25,75),CV_RGB(0,0,255), 1 );
            printf("\nDRAWN ");
		    cvShowImage("ENVI",back);
			//fclose(fp);
			break;
		}
    	
			
        
    }
}

void envidesign()
{

   
   	fp=fopen("c:\\vejey\\cord.txt","w");

    cvNamedWindow( "ENVI",1);
   
	cvSetMouseCallback( "ENVI",on_mouse);

    cvShowImage("ENVI",back);
	//cvWaitKey(0);
}





void vjfind(int x1,int x2,int y1,int y2)
{

	 //   printf("\n \nThe number of lines : %d",num_lines);
	 //	 printf("\n \n The nymber of Labels : %d",num_labels);

for(int i=0;i<cxmax;i++)
{
if(x2>enymax[i] && y1<enxmin[i] && x1<enymin[i] && y2>enxmax[i])
{
	printf("\n\n\n \n\n ********************* Entering region   : %s",label[i]);
		    
 

}




}


//	cvWaitKey(0);

}






void main()
{ 


//	int ncams = cvcamGetCamerasCount();//returns the number of available cameras in the 
						   //system
 //   printf("\n Number of cams: %d",ncams);

    



/*	

// Tell OpenCV to capture data from a camera.
CvCapture* camera = cvCaptureFromCAM(0);

// Print the camera's properties.

cvNamedWindow("My Window", CV_WINDOW_AUTOSIZE);
cvNamedWindow("LIVE FEED", CV_WINDOW_AUTOSIZE);
// Loop forever until the user quits.
while(1)
{

int keyCode = cvWaitKey(10);
IplImage* livefeed=cvQueryFrame(camera);
cvShowImage("LIVE FEED",livefeed);
if ('c' == (char)keyCode)
{
// The user pressed 'c' (for 'capture').

// Get the latest image from the camera. This
// image should NOT be released manually.
IplImage* image = cvQueryFrame(camera);

// Make the image show up in the named window.
cvShowImage("My Window", image);

// Save the image to a file.
static int imageCount = 0;
char filename[32];
sprintf(filename, "image%d.jpg", imageCount);
cvSaveImage(filename, image);
++imageCount;
}
else if(keyCode >= 0)
{

break;
}
}

// Destroy the capture object and the window.
cvReleaseCapture(&camera);
cvDestroyWindow("My Window");



}


*/
    
	//code for Environment Design
	
	
	//cvWaitKey(0);
	CvSize imgSize;
//	CBlob* img_new;
	IplConvKernel* B = cvCreateStructuringElementEx(3,3,2,2,CV_SHAPE_RECT,NULL);
	
	int winback = cvNamedWindow("Back",CV_WINDOW_AUTOSIZE);

capture= cvCaptureFromAVI("C:\\Documents and Settings\\Vejey\\Desktop\\New Folder\\project\\movies\\arjun.avi");
//CvCapture* capture = cvCaptureFromAVI("C:/vejey/VJ/TwoLeaveShop2cor_vct.avi");
  //   CvCapture* capture = cvCaptureFromCAM(0);

	back=cvQueryFrame(capture);
	
   //BackGround Image !!!!
    
	
	cvShowImage("Back",back);
    
	imgSize.width = back->width;
	imgSize.height = back->height;
	cvSaveImage("c:\\vejey\\zzz.jpg",back);
    back=cvLoadImage("c:\\vejey\\zzz.jpg");

	cvShowImage("Back",back);

    printf("\n Enter the environment Design");
//	scanf("%d",choice);
	
	envidesign();                                     // Please Add these lines!!!!!!**********
    cvWaitKey(0);//waiting 30 secs for the Environment Design

	//code for Video Surveillance
/*  
    CvSize imgSize;
//	CBlob* img_new;
	IplConvKernel* B = cvCreateStructuringElementEx(3,3,2,2,CV_SHAPE_RECT,NULL);
	
	int winback = cvNamedWindow("Back",CV_WINDOW_AUTOSIZE);

	CvCapture* capture = cvCaptureFromAVI("Z:/Final Sem/arjun.avi");

    // CvCapture* capture = cvCaptureFromCAM(0);

	IplImage* back=cvQueryFrame(capture);
	
   //BackGround Image !!!!

	cvShowImage("Back",back);
    
	imgSize.width = back->width;
	imgSize.height = back->height;
	cvSaveImage("c:\\vejey\\zzz.jpg",back);

*/
    

  //  cvWaitKey(0);
    


//KALMAN FILTER


/*
    const float A[] = { 1, 1, 0, 1 };

    IplImage* img = cvCreateImage( cvSize(500,500), 8, 3 );
    CvKalman* kalman = cvCreateKalman( 2, 1, 0 );
    // state is (phi, delta_phi) - angle and angle increment 
    CvMat* state = cvCreateMat( 2, 1, CV_32FC1 );
    CvMat* process_noise = cvCreateMat( 2, 1, CV_32FC1 );
    // only phi (angle) is measured 
    CvMat* measurement = cvCreateMat( 1, 1, CV_32FC1 );
    CvRandState rng;
    int code = -1;

    cvRandInit( &rng, 0, 1, -1, CV_RAND_UNI );

    cvZero( measurement );
    cvNamedWindow( "Kalman", 1 );

*/














    IplImage *outputImage;
	
	IplImage* live = cvCreateImage(imgSize,8,3 );
	IplImage* live1 = cvCreateImage(imgSize,8,3 );
	IplImage* live2 = cvCreateImage(imgSize,8,3 );
	IplImage* live3 = cvCreateImage(imgSize,8,3 );
	IplImage* live4 = cvCreateImage(imgSize,8,3 );
	IplImage* live5 = cvCreateImage(imgSize,8,3 );
	IplImage* live6 = cvCreateImage(imgSize,8,3 );


	loadlive = cvCreateImage(imgSize,8,3 );
	IplImage* loadlive1 = cvCreateImage(imgSize,8,3 );
	IplImage* loadlive2 = cvCreateImage(imgSize,8,3 );
	IplImage* loadlive3 = cvCreateImage(imgSize,8,3 );
	IplImage* loadlive4 = cvCreateImage(imgSize,8,3 );
	IplImage* loadlive5 = cvCreateImage(imgSize,8,3 );
	IplImage* loadlive6 = cvCreateImage(imgSize,8,3 );


	IplImage* loadback=cvLoadImage("c:\\vejey\\zzz.jpg",1);


	IplImage* grayback = cvCreateImage(imgSize,8,1 ); 
 graylive = cvCreateImage(imgSize,8,1 );
	IplImage* graysubimg = cvCreateImage(imgSize,8,1 );
	IplImage* threshimg = cvCreateImage(imgSize,8,1 );
	IplImage* erodeimg = cvCreateImage(imgSize,8,1 );
	blobimg = cvCreateImage(imgSize,8,3 );
	blobmodel = cvCreateImage(imgSize,8,3 );
	src = cvCreateImage(imgSize,8,1 );

	cvCvtColor(loadback,grayback,CV_BGR2GRAY);
				

	cvNamedWindow("LIVE",1);
//	cvNamedWindow("mainWin",1);
	cvNamedWindow("Ripper",1);
	//cvNamedWindow("Blob",1);
	cvNamedWindow("BOX",1);
	cvNamedWindow("GRAY",1);
	cvNamedWindow("GRAY SUBIMAGE",1);
	cvNamedWindow("Erode",1);
	//cvWaitKey(0);
    cvNamedWindow("BLOB",1);
CBlob blobWithBiggestPerimeter;                                                                                       
CBlob blobWithLessArea;
CBlobResult blobs;


printf("\n\n *******************");
  fclose(fp);                   // Add these files TOoo           ***************


fs=fopen("c:\\vejey\\cord.txt","r");


printf("\n\n gggggggggggggggggggggggggggg");
/*
while ( !feof(fs) )
{
fscanf(fs,"%s[^\n]",a);
printf("\n%s",a);
//fflush(fs);
}
*/
while ( !feof(fs) )
	{       
	  
        fgets(a,100,fs);
        //printf("\n*********************** :%s",a);

		if(num%5==0)
		{
			
				for(int i=0;a[i]!='\0';i++)
				{
		         label[labc][jis]=a[i];
				 jis++;
			     
			   
				}

			labc++;
			jis=0;
			printf("\n label :%s",label[labc-1]);
		}
		else
		{

			points[pc]=atoi(a);
			printf("points[%d] : %d",pc,points[pc]);
			
			pc++;
		    	
		}
				num++;

	}


num_lines=num-1;
num_labels=labc-1;



	for(int i=0;i<labc;i++)
	{
		printf("\n label[%d] is : %s",i,label[i]);
	}
    for(i=0;i<pc;i++)
	{
		if(i%4==0)
		{
			enymin[cymin]=points[i];
			cymin++;
		}
		if(i%4==1)
		{
			enymax[cymax]=points[i];
			cymax++;
		}
		if(i%4==2)
		{
			enxmin[cxmin]=points[i];
			cxmin++;
		}
		if(i%4==3)
		{
			enxmax[cxmax]=points[i];
			cxmax++;
		}

	}

    
		for(i=0;i<cymin;i++)
		printf("\n YMIN[%d] = %d",i,enymin[i]);
		for(i=0;i<cymax;i++)
		printf("\n YMAX[%d] = %d",i,enymax[i]);
		for(i=0;i<cxmin;i++)
		printf("\n XMIN[%d] = %d",i,enxmin[i]);
		for(i=0;i<cxmax;i++)
		printf("\n XMAX[%d] = %d",i,enxmax[i]);

cvWaitKey(0);

	for(int l=1;l<1500;l++)
	{
		live=cvQueryFrame(capture);
		cvSaveImage("c:\\vejey\\zz.jpg",live);
		loadlive=cvLoadImage("c:\\vejey\\zz.jpg",3);
		cvShowImage("LIVE",loadlive);
		blobcount = 1;
		cvCvtColor(loadlive,graylive,CV_BGR2GRAY);
		
       cvShowImage("GRAY",graylive);
	
  
     // Blob Image

		cvAbsDiff(grayback,graylive,graysubimg);
		cvShowImage("GRAY SUBIMAGE",graysubimg);
		
		
		cvThreshold(graysubimg,threshimg,30,255,CV_THRESH_BINARY);
		

		cvErode(threshimg,src,B,1);
		
        cvShowImage("Erode",threshimg);  


	// Extract the blobs using a threshold of 100 in the image
	blobs = CBlobResult( threshimg, NULL, 100, true );

	// create a file with some of the extracted features
	//blobs.PrintBlobs( "z:\\blobs.txt" );

	// discard the blobs with less area than 5000 pixels
	// ( the criteria to filter can be any class derived from COperadorBlob )
	blobs.Filter( blobs, B_INCLUDE, CBlobGetArea(), B_GREATER, 1000 );
	// create a file with filtered results
	//blobs.PrintBlobs( "z:\\filteredBlobs.txt" );



	// object with the blob with most perimeter in the image
	// ( the criteria to select can be any class derived from COperadorBlob )
	
	// from the filtered blobs, get the blob with biggest perimeter
	blobs.GetNthBlob( CBlobGetPerimeter(), 0, blobWithBiggestPerimeter );
	// get the blob with less area
	blobs.GetNthBlob( CBlobGetArea(), blobs.GetNumBlobs() - 1, blobWithLessArea );

             

    //to get the distance from a point
             // CBlobGetDistanceFromPoint(x,y);





             // BlobAnalysis(threshimg,100,back,0,0,);
  /*      

        for( int i = 0; i < blobs.GetNumBlobs(); i++ ) 
		{

        CBlob currentBlob = blobs.GetBlob(i);
        
	
		
		CvPoint val;
        CvSeqReader reader;
        cvStartReadSeq(currentBlob.Edges(), &reader);
        for( int j=0; j< currentBlob.Edges()->total; j++)
		{
		
		 CV_READ_SEQ_ELEM(val,reader);
         printf("\nx:%d y:%d\n", val.x,val.y);
	     
         k1=blobs.GetNumBlobs();
	   	printf("\n############ Number of blobs: %d",k1);
		cvWaitKey(0);

		}
	
		}
		*/

/*
              CBlobGetDistanceFromPoint obj1(150,150);
			  double d1=blobWithLessArea.MinX();
			  double d2=blobWithLessArea.MinY();
			  double d4=blobWithLessArea.MaxX();
			  double d5=blobWithLessArea.MaxY();
			  double d3=obj1.operator()(blobWithLessArea);
			  printf("\n\nThe distance from (150,150) is : %d",d3);
			  printf("\n\n The MINX of BLOB is : %d",d1);
			  printf("\n\n The MINY of BLOB is : %d",d2);
			  printf("\n\n The MAXX of BLOB is : %d",d4);
			  printf("\n\n The MAXY of BLOB is : %d",d5);
			  cvWaitKey(0);
*/
  
	/*    
double CBlobGetDistanceFromPoint::operator()(const CBlob &blob) const
 {
        double xmitjana, ymitjana;
        CBlobGetXCenter getXCenter;
        CBlobGetYCenter getYCenter;
 
         xmitjana = m_x - getXCenter( blob );
         ymitjana = m_y - getYCenter( blob );
 
         return sqrt((xmitjana*xmitjana)+(ymitjana*ymitjana));
 }
 */
        	  //printf("\n\n\n The distance is:%d",d);


   
// build an output image equal to the input but with 3 channels (to draw the coloured blobs)

outputImage = cvCreateImage( cvSize( threshimg->width, threshimg->height ), IPL_DEPTH_8U, 3 );
cvMerge( threshimg, threshimg, threshimg, NULL, outputImage );


// plot the selected blobs in a output image
blobWithBiggestPerimeter.FillBlob( outputImage, CV_RGB( 255, 0, 0 ));
blobWithLessArea.FillBlob( outputImage, CV_RGB( 0, 255, 0 ));

cvShowImage("BLOB",outputImage);


		cvFindBlob();
//cvSeperateBlob();
//writer=cvCreateVideoWriter("out.avi",CV_FOURCC('P','I','M','1'),fps,cvSize(frameW,frameH),isColor);
 //MyAVIwriter = cvCreateAVIWriter( "MyCapturedFrames.avi",CV_FOURCC('P','I','M','1'),24,cvSize(320,240) );

        vjfind(X1,X2,Y1,Y2);
    
        
   //kalman FIltering

CvKalman* kalman = cvCreateKalman(2, 1);
cvKalmanUpdateByTime (kalman);
CvMat* measurement = cvCreateMat(2, 1, CV_32FC1 );
cvKalmanUpdateByMeasurement(kalman,measurement);

/*
		CvKalman* kalman = cvCreateKalman(2, 2, 0);

cvSetZero(kalman->transition_matrix); // A
cvmSet(kalman->transition_matrix,0,0,1);
cvmSet(kalman->transition_matrix,1,1,1);

cvSetZero(kalman->measurement_matrix); // H
cvmSet(kalman->measurement_matrix,0,0,1);
cvmSet(kalman->measurement_matrix,0,1,1);

cvSetZero(kalman->process_noise_cov); // Q
cvmSet(kalman->process_noise_cov,0,0, 1e-5);
cvmSet(kalman->process_noise_cov,1,1, 1e-5);

cvSetZero(kalman->measurement_noise_cov); // R
cvmSet(kalman->measurement_noise_cov,0,0,1);
cvmSet(kalman->measurement_noise_cov,1,1,1);

cvSetZero(kalman->error_cov_post); // P
cvmSet(kalman->error_cov_post,0,0,1);
cvmSet(kalman->error_cov_post,1,1,1);

CvKalman* cvCreateKalman( int dynam_params, int measure_params, int control_params=0 );

*/

// Hided 
/*		
     cvRandSetRange( &rng, 0, 0.1, 0 );
        rng.disttype = CV_RAND_NORMAL;

        cvRand( &rng, state );

        memcpy( kalman->transition_matrix->data.fl, A, sizeof(A));
        cvSetIdentity( kalman->measurement_matrix, cvRealScalar(1) );
        cvSetIdentity( kalman->process_noise_cov, cvRealScalar(1e-5) );
        cvSetIdentity( kalman->measurement_noise_cov, cvRealScalar(1e-1) );
        cvSetIdentity( kalman->error_cov_post, cvRealScalar(1));
        //choose random initial state 
        cvRand( &rng, kalman->state_post );

        rng.disttype = CV_RAND_NORMAL;

        
            #define calc_point(angle)                                      \
                cvPoint( cvRound(img->width/2 + img->width/3*cos(angle)),  \
                         cvRound(img->height/2 - img->width/3*sin(angle)))

            float state_angle = state->data.fl[0];
            CvPoint state_pt = calc_point(state_angle);
        
   //          predict point position 
            const CvMat* prediction = cvKalmanPredict( kalman, 0 );
            float predict_angle = prediction->data.fl[0];
            CvPoint predict_pt = calc_point(predict_angle);
            float measurement_angle;
            CvPoint measurement_pt;

            cvRandSetRange( &rng, 0, sqrt(kalman->measurement_noise_cov->data.fl[0]), 0 );
            cvRand( &rng, measurement );

    //         generate measurement 
            cvMatMulAdd( kalman->measurement_matrix, state, measurement, measurement );

            measurement_angle = measurement->data.fl[0];
            measurement_pt = calc_point(measurement_angle);

     //        plot points 
            #define draw_cross( center, color, d )                                 \
                cvLine( img, cvPoint( center.x - d, center.y - d ),                \
                             cvPoint( center.x + d, center.y + d ), color, 1, 0 ); \
                cvLine( img, cvPoint( center.x + d, center.y - d ),                \
                             cvPoint( center.x - d, center.y + d ), color, 1, 0 )

            cvZero( img );
            draw_cross( state_pt, CV_RGB(255,255,255), 3 );
            draw_cross( measurement_pt, CV_RGB(255,0,0), 3 );
            draw_cross( predict_pt, CV_RGB(0,255,0), 3 );
            cvLine( img, state_pt, predict_pt, CV_RGB(255,255,0), 3, 0 );

          //   adjust Kalman filter state 
            cvKalmanCorrect( kalman, measurement );

            cvRandSetRange( &rng, 0, sqrt(kalman->process_noise_cov->data.fl[0]), 0 );
            cvRand( &rng, process_noise );
            cvMatMulAdd( kalman->transition_matrix, state, process_noise, state );

            cvShowImage( "Kalman", img );
            code = cvWaitKey( 100 );
	*/

			 	cvWaitKey(1); 
				
				printf("\n ..... Frame %d ended",l);
				printf("\n INdex number of model is........ %d",indexnumber);
		       fcount=l;
	}
	

	
printf("\n....... Done\n");

  

}

