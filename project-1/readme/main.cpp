#include<iostream>
#include"include/cardditect.h"
#include<fstream>
using namespace std;
using namespace cv;

/////////////////模板///////////////
bool Get_Template(Mat temp, vector<Card>&Card_Temp)
{
	//图像预处理
	Mat gray;
	cvtColor(temp, gray, COLOR_BGR2GRAY);

	Mat thresh;
	threshold(gray, thresh, 0, 255, THRESH_BINARY_INV|THRESH_OTSU);

	//轮廓检测
	vector <vector<Point>> contours;
	findContours(thresh, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours.size(); i++)
	{
		Rect rect = boundingRect(contours[i]);

		double ratio = double(rect.width) / double(rect.height);
		//筛选出字符轮廓
		if (ratio > 0.5 && ratio < 1)
		{
			/*rectangle(temp, rect, Scalar(0, 255, 0));*/
			Mat roi = temp(rect);  //将字符扣出，放入Card_Temp容器备用
			Card_Temp.push_back({ roi ,rect });
		}
	}

	if (Card_Temp.empty())
    {
        return false;
    }

	//进行字符排序，使其按（0、1、2...7、8、9）顺序排序
	for (int i = 0; i < Card_Temp.size()-1; i++)
	{
		for (int j = 0; j < Card_Temp.size() - 1 - i; j++)
		{
			if (Card_Temp[j].rect.x > Card_Temp[j + 1].rect.x)
			{
				Card temp = Card_Temp[j];
				Card_Temp[j] = Card_Temp[j + 1];
				Card_Temp[j + 1] = temp;
			}
		}
	}

	//for (int i = 0; i < Card_Temp.size(); i++)
	//{
	//	imshow(to_string(i), Card_Temp[i].mat);
	//	waitKey(0);
	//}

	return true;
}

///////////////////////找卡号//////////////////////////

bool Cut_Block(Mat src, vector<Card>&Block_ROI)
{
	//形态学操作、以便找到银行卡号区域轮廓
	Mat gray;
	cvtColor(src, gray, COLOR_BGR2GRAY);

	Mat gaussian;
	GaussianBlur(gray, gaussian, Size(3, 3), 0);

	Mat thresh;
	threshold(gaussian, thresh, 0, 255, THRESH_BINARY | THRESH_OTSU);
	//imshow("thresh",thresh);
	//waitKey(5000);
	Mat close;
	Mat kernel2 = getStructuringElement(MORPH_RECT, Size(15, 5));
	
	morphologyEx(thresh, close, MORPH_CLOSE, kernel2);
	//imshow("close",close);
	//waitKey(5000);
	vector<vector<Point>>contours;
	findContours(close, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < contours.size(); i++)
	{
		//通过面积、长宽比筛选出银行卡号区域
		double area = contourArea(contours[i]);

		if (area > 800 && area < 1400)
		{
			Rect rect = boundingRect(contours[i]);
			float ratio = double(rect.width) / double(rect.height);

			if (ratio > 2.8 && ratio < 4)
			{
				//rectangle(src, rect, Scalar(0, 255, 0), 2);
				Mat ROI = src(rect);
				Block_ROI.push_back({ ROI ,rect });
			}
		}
	}
	
	if (Block_ROI.size()!=4)return false;

	for (int i = 0; i < Block_ROI.size()-1; i++)
	{
		for (int j = 0; j < Block_ROI.size() - 1 - i; j++)
		{
			if (Block_ROI[j].rect.x > Block_ROI[j + 1].rect.x)
			{
				Card temp = Block_ROI[j];
				Block_ROI[j] = Block_ROI[j + 1];
				Block_ROI[j + 1] = temp;
			}
		}
	}

	//for (int i = 0; i < Block_ROI.size(); i++)
	//{
	//	imshow(to_string(i), Block_ROI[i].mat);
	//	waitKey(0);
	//}
	return true;
}

int main()
{

	Mat src = imread("Card/credit_card_05.png");   //源图像 银行卡
	Mat temp = imread("Model/ocr_a_reference.png"); //模板图像

	vector<Card>Card_Temp;
	if (!Get_Template(temp, Card_Temp))
	{
		cout << "modcut faild" << endl;
		system("pause");
		return -1;
	}
	vector<Card> Block_ROI;
	Cut_Block(src, Block_ROI);

	for (int i = 0; i < Block_ROI.size(); i++)
	{
		rectangle(src, Rect(Block_ROI[i].rect.tl(), Block_ROI[i].rect.br()), Scalar(0, 255, 0), 1);
	}

	imshow("src",src);
	waitKey(0);

    system("pause");
	return 0;
////////////////功能完整/////////////////

}