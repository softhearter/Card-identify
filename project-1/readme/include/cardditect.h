#pragma once
#include<opencv2/opencv.hpp>
#include<iostream>

struct Card
{
	cv::Mat mat;
	cv::Rect rect;
};
//////////////////键列模板/////////////////////
bool Get_Template(cv::Mat temp, std::vector<Card>&Card_Temp);
//////////////////识别区域/////////////////////
bool Cut_Block(cv::Mat src, std::vector<Card>&Block_ROI);