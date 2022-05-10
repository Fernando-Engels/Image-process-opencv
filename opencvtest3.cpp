#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;
int main(void)
{
    Mat img = imread("image2.JPG", IMREAD_COLOR);//read image
    resize(img, img, Size(400, 512), INTER_LINEAR);//resize image
    Mat out;
    Mat histImage;//the histogram image
    img.copyTo(out);
    img.copyTo(histImage);

    Mat B, G, R, Bhist ,Ghist, Rhist;
    int histSize = 256;// histogram size
    float range[] = { 0, 256 }; //the upper boundary is exclusive
    const float* histRange[] = { range };
    bool uniform = true, accumulate = false;
    

    //extracting each channel of the 3 channel image
    extractChannel(img, B, 0);    // Blue
    extractChannel(img, G, 1);    // Green
    extractChannel(img, R, 2);    // Red
    //calculating the histogram of the image
    calcHist(&B, 1, 0, Mat(), Bhist, 1, &histSize, histRange, uniform, accumulate);
    calcHist(&G, 1, 0, Mat(), Ghist, 1, &histSize, histRange, uniform, accumulate);
    calcHist(&R, 1, 0, Mat(), Rhist, 1, &histSize, histRange, uniform, accumulate);
    int hist_w = 512; int hist_h = 400;//histogram width and height
    //normalizing the histogram
    normalize(Bhist, Bhist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
    normalize(Ghist, Ghist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
    normalize(Rhist, Rhist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
    int bin_w = cvRound((double)hist_w / histSize);
  
    for (int i = 1; i < histSize; i++)//displaying the histogram with lines instead of just having the  numbers of each beam
    {
        line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(Bhist.at<float>(i - 1))),
            Point(bin_w * (i), hist_h - cvRound(Bhist.at<float>(i))),
            Scalar(255, 0, 0), 2, 8, 0);
        line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(Ghist.at<float>(i - 1))),
            Point(bin_w * (i), hist_h - cvRound(Ghist.at<float>(i))),
            Scalar(0, 255, 0), 2, 8, 0);
        line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(Rhist.at<float>(i - 1))),
            Point(bin_w * (i), hist_h - cvRound(Rhist.at<float>(i))),
            Scalar(0, 0, 255), 2, 8, 0);
    }
   
    addWeighted(histImage, 0.5, out, 0.5, 0, out);//adding  transparecy to histogram in the image
    imshow("calcHist Demo", histImage);
    waitKey(0);

    
    
   
    return 0;
}
