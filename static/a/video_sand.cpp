#include <iostream>
#include <fstream>
#include <string>
#include <cv.h>
#include <highgui.h>

using namespace std;
using namespace cv;

#define NUM_FRAME 1000

void Image_to_video(char* name, char* res){
	int i = 0;
	IplImage* img = 0;
	char image_name[100];
	char video[30];
	sprintf(video,"video/%s%s", res, ".avi");
	printf("------------- image to video ... ----------------\n");
	CvVideoWriter *writer = 0;
	int isColor = 1;
	int fps     = 30; // or 25
	int frameW = 800; // 744 for firewire cameras
	int frameH = 600; // 480 for firewire cameras
	writer=cvCreateVideoWriter(video,CV_FOURCC('X','V','I','D'),fps,cvSize(frameW,frameH),isColor);
	printf("\tvideo height : %d\n\tvideo width : %d\n\tfps : %d\n", frameH, frameW, fps);
	//cvNamedWindow( "mainWin", CV_WINDOW_AUTOSIZE );
	while(i<NUM_FRAME)
	{
		sprintf(image_name, "%s/%s%d%s", "temp", "0results", i+1, ".jpg");
		img = cvLoadImage(image_name);
		if(!img)
		{
			
			int kkk=0;
			while (1)
			{	
				sprintf(image_name, "%s/%s%d%s", "temp", "0results", i-2, ".jpg");
				cout<<image_name<<endl;	
				img = cvLoadImage(image_name);
				//cvShowImage("mainWin", img);
				char key = cvWaitKey(20);
				cvWriteFrame(writer, img);
				kkk++;
				if(kkk == 45)break;

			}
           printf("Could not load image file...\n");

			break;
		}
		//cvShowImage("mainWin", img);
		char key = cvWaitKey(20);
		cvWriteFrame(writer, img);
		i++;
	}
	cvReleaseVideoWriter(&writer);
	//cvDestroyWindow("mainWin");
	
}




int main(int argc, char*argv[]) {  


string targetImPath =argv[1] ;
int num_imgInShou[6]={81,37,37,41,62,41};
	int num_pixInregion[6]={67318,17567,14978,68073,32856,279202};

	const int num_region = 6;
	int cou_img=1;
	int **region[6]; 

	for (int i=0;i<num_region;i++)
	{

		region[i] = new int*[num_pixInregion[i]]; 
		char regionpath[200];
		sprintf(regionpath,"%s%d%s","./regions/area", i+1, ".txt");
		ifstream ifs_region(regionpath);	
		for(int j=0;j<num_pixInregion[i];j++)
		{
			region[i][j] = new int[2];
			ifs_region>>region[i][j][0]>>region[i][j][1];
		}
		ifs_region.close();

	}
	cout<<region[5][268165][1]<<endl;

	string  sourceImPath="./videosand.jpg";

	Mat sourceIm = imread(sourceImPath, CV_LOAD_IMAGE_COLOR);
	Mat targetIm1 = imread(targetImPath, CV_LOAD_IMAGE_COLOR);
	//imshow("inininin",sourceIm);
	//waitKey();

	int rowIm = targetIm1.rows;
	int colIm = targetIm1.cols;	
	Mat  targetIm;
	if (rowIm != 600 && colIm != 800)
	{
		Size dsize = Size(800,600);
		resize(targetIm1,targetIm,dsize);
		//imshow("inininin",targetIm);
		//waitKey();
	}else
		targetIm = targetIm1.clone();

	int num_result=0;
	for(int i_region=0;i_region<num_region;i_region++)
	{
		//num_imgInShou[i_region];
		//num_pixInregion[i_region];
		cout<<num_imgInShou[i_region]<<endl;
		for(int i_imgInShou=0;i_imgInShou<num_imgInShou[i_region];i_imgInShou++)
		{
			//string shouImpath;
			char image_name[200];
			sprintf(image_name, "%s%d%s", "./hands/image",cou_img++, ".jpg");
			cout<<image_name<<endl;
			Mat shouIm = imread(image_name, CV_LOAD_IMAGE_COLOR);
			if(shouIm.empty()) 
			{
				cout<<cou_img<<endl; break;
			}
			Mat  shouImcpy = shouIm.clone();
			//cout<<shouIm.channels()<<endl;3
			for(int i=0;i<sourceIm.rows;i++)
				for(int j=0;j<sourceIm.cols;j++)
				{
					int r=(int)shouIm.at<uchar>(i,j*sourceIm.channels()+0);
					int g=(int)shouIm.at<uchar>(i,j*sourceIm.channels()+1);
					int b=(int)shouIm.at<uchar>(i,j*sourceIm.channels()+2);
					if (r<30 && g<30 && b<30)
					{
						shouImcpy.at<uchar>(i,j*sourceIm.channels()+0) = sourceIm.at<uchar>(i,j*sourceIm.channels()+0);
						shouImcpy.at<uchar>(i,j*sourceIm.channels()+1) = sourceIm.at<uchar>(i,j*sourceIm.channels()+1);
						shouImcpy.at<uchar>(i,j*sourceIm.channels()+2) = sourceIm.at<uchar>(i,j*sourceIm.channels()+2);
						//cout<<"sadfasf"<<endl;
					}


				}
				//imshow("asdf",shouImcpy);
				//waitKey();
				cout<<num_pixInregion[i_region]<<endl;
				for(int i_pix=0;i_pix<num_pixInregion[i_region];i_pix++)
				{
					int pix_r = region[i_region][i_pix][0];
					int pix_c = region[i_region][i_pix][1];
					// sourceIm.at<uchar>(pix_x,pix_y*sourceIm.channels()+n) = targetIm.at<uchar>(pix_x,pix_y*sourceIm.channels()+n);
					int r=(int)shouIm.at<uchar>(pix_r,pix_c*sourceIm.channels()+0);
					int g=(int)shouIm.at<uchar>(pix_r,pix_c*sourceIm.channels()+1);
					int b=(int)shouIm.at<uchar>(pix_r,pix_c*sourceIm.channels()+2);
					//cout<<r<<g<<b<<endl;
					/*imshow("asdf",shouImcpy);
					waitKey();*/
					if (r<30 && g<30 && b<30)
					{

						//cout<<i_imgInShou<<' '<<num_imgInShou[i_region]<<endl;
						shouImcpy.at<uchar>(pix_r,pix_c*sourceIm.channels()+0) = (sourceIm.at<uchar>(pix_r,pix_c*sourceIm.channels()+0))*(1-(double)(i_imgInShou+1)/num_imgInShou[i_region])+(targetIm.at<uchar>(pix_r,pix_c*targetIm.channels()+0))*((double)(i_imgInShou+1)/num_imgInShou[i_region]);
						shouImcpy.at<uchar>(pix_r,pix_c*sourceIm.channels()+1) = (sourceIm.at<uchar>(pix_r,pix_c*sourceIm.channels()+1))*(1-(double)(i_imgInShou+1)/num_imgInShou[i_region])+(targetIm.at<uchar>(pix_r,pix_c*targetIm.channels()+1))*((double)(i_imgInShou+1)/num_imgInShou[i_region]);
						shouImcpy.at<uchar>(pix_r,pix_c*sourceIm.channels()+2) = (sourceIm.at<uchar>(pix_r,pix_c*sourceIm.channels()+2))*(1-(double)(i_imgInShou+1)/num_imgInShou[i_region])+(targetIm.at<uchar>(pix_r,pix_c*targetIm.channels()+2))*((double)(i_imgInShou+1)/num_imgInShou[i_region]);

						//shouImcpy.at<uchar>(pix_r,pix_c*sourceIm.channels()+0) = targetIm.at<uchar>(pix_r,pix_c*sourceIm.channels()+0);
						//shouImcpy.at<uchar>(pix_r,pix_c*sourceIm.channels()+1) = targetIm.at<uchar>(pix_r,pix_c*sourceIm.channels()+1);
						//shouImcpy.at<uchar>(pix_r,pix_c*sourceIm.channels()+2) = targetIm.at<uchar>(pix_r,pix_c*sourceIm.channels()+2);
					}
					if (i_imgInShou == num_imgInShou[i_region]-1)
					{
						sourceIm.at<uchar>(pix_r,pix_c*sourceIm.channels()+0)=targetIm.at<uchar>(pix_r,pix_c*targetIm.channels()+0);
						sourceIm.at<uchar>(pix_r,pix_c*sourceIm.channels()+1)=targetIm.at<uchar>(pix_r,pix_c*targetIm.channels()+1);
						sourceIm.at<uchar>(pix_r,pix_c*sourceIm.channels()+2)=targetIm.at<uchar>(pix_r,pix_c*targetIm.channels()+2);
					}
				}
				//for(int i_imgInShou=0;i_imgInShou<num_imgInShou[i_region];i_imgInShou++)

				char image_name1[200];
				sprintf(image_name1, "%s/%s%d%s", "temp", "0results", ++num_result, ".jpg");
				cout<<image_name1<<endl;
				imwrite(image_name1, shouImcpy);
				/*imshow("shouIm",shouIm);
				waitKey();*/

		}


	}

	char image_name1[200];
	sprintf(image_name1, "%s/%s%d%s", "temp", "0results", ++num_result, ".jpg");
	cout<<image_name1<<endl;
	imwrite(image_name1, targetIm);



	Image_to_video(argv[1], argv[2]);


	 return 0;
	//system("pause");

}
