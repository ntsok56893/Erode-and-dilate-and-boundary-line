#pragma once

/**
* Created by 05 on 2018/8/20.
*/

#include <opencv2/core/core_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cmath>
#include <iostream>

using namespace std;
using namespace cv;

//修正圖，將邊緣映射，解決原圖邊緣無法做處理的問題
void imageA(Mat orImage, Mat result, int heigth, int width, int newHeigth, int newWidth) {
	//先填上四個邊角
	result.at<uchar>(0, 0) = orImage.at<uchar>(0, 0);
	result.at<uchar>(0, newWidth - 1) = orImage.at<uchar>(0, width - 1);
	result.at<uchar>(newHeigth - 1, 0) = orImage.at<uchar>(heigth - 1, 0);
	result.at<uchar>(newHeigth - 1, newWidth - 1) = orImage.at<uchar>(heigth - 1, width - 1);
	//將邊緣映射
	for (int i = 0; i < heigth; i++) {
		for (int j = 0; j < width; j++) {

			if (i == 0) {
				result.at<uchar>(i, j + 1) = orImage.at<uchar>(i, j);
			}
			if (i == heigth - 1) {
				result.at<uchar>(i + 2, j + 1) = orImage.at<uchar>(i, j);
			}
			if (j == 0) {
				result.at<uchar>(i + 1, j) = orImage.at<uchar>(i, j);
			}
			if (j == width - 1) {
				result.at<uchar>(i + 1, j + 2) = orImage.at<uchar>(i, j);
			}
			result.at<uchar>(i + 1, j + 1) = orImage.at<uchar>(i, j);
		}
	}
}

Mat input = imread("HW5.tif", 0); //讀圖
Mat img(input.rows + 2, input.cols + 2, CV_8U, Scalar(0)); //將圖修正，用以處理
Mat ori;  //原圖



namespace IP_HW5 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  btn_HW5;
	protected:

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->btn_HW5 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// btn_HW5
			// 
			this->btn_HW5->Location = System::Drawing::Point(93, 104);
			this->btn_HW5->Name = L"btn_HW5";
			this->btn_HW5->Size = System::Drawing::Size(75, 23);
			this->btn_HW5->TabIndex = 0;
			this->btn_HW5->Text = L"IP_HW5";
			this->btn_HW5->UseVisualStyleBackColor = true;
			this->btn_HW5->Click += gcnew System::EventHandler(this, &MyForm::btn_HW5_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 261);
			this->Controls->Add(this->btn_HW5);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void btn_HW5_Click(System::Object^  sender, System::EventArgs^  e) {
		
		
		Mat temp;  //暫存分界線的圖
		
		//侵蝕直到左邊較小的圈圈消失
		for(int c = 0; c < 30; c++){
			//修正圖
			imageA(input, img, input.rows, input.cols, input.rows + 2, input.cols + 2);
			//侵蝕
			for (int i = 1; i < img.rows - 1; i++) {
				for (int j = 1; j< img.cols - 1; j++) {
				
					if (                                  img.at<uchar>(i - 1, j) == 255 || 
						img.at<uchar>(i, j - 1) == 255 || img.at<uchar>(i, j) == 255 || img.at<uchar>(i, j + 1) == 255 ||
						                                  img.at<uchar>(i + 1, j) == 255 ) {
						input.at<uchar>(i -1, j-1) = 255;
					}
				}
			}
		}
		//膨脹直到右邊填滿
		for (int c = 0; c < 110; c++) {
			//修正圖
			imageA(input, img, input.rows, input.cols, input.rows + 2, input.cols + 2);
			//膨脹
			for (int i = 1; i < img.rows - 1; i++) {
				for (int j = 1; j < img.cols - 1; j++) {

					if (                                img.at<uchar>(i - 1, j) == 0 || 
						img.at<uchar>(i, j - 1) == 0 || img.at<uchar>(i, j) == 0 || img.at<uchar>(i, j + 1) == 0 ||
						                                img.at<uchar>(i + 1, j) == 0 ) {

						input.at<uchar>(i - 1, j - 1) = 0;
					}
				}
			}
		}
		//將膨脹的部分侵蝕到接近原來大小
		for (int c = 0; c < 70; c++) {
			//修正圖
			imageA(input, img, input.rows, input.cols, input.rows + 2, input.cols + 2);
			//分界線寬度為10像素
			if(c == 60) temp = input.clone();  //複製input圖到temp
			//侵蝕
			for (int i = 1; i < img.rows - 1; i++) {
				for (int j = 1; j< img.cols - 1; j++) {


					if (                                  img.at<uchar>(i - 1, j) == 255 || 
						img.at<uchar>(i, j - 1) == 255 || img.at<uchar>(i, j) == 255 || img.at<uchar>(i, j + 1) == 255 ||
						                                  img.at<uchar>(i + 1, j) == 255 ) {
						input.at<uchar>(i - 1, j - 1) = 255;
					}
				}
			}
		}
		temp = input - temp;  //侵蝕圖減去膨脹圖，得到10像素寬分界線
		ori = temp + ori;     //將分界線圖和原圖相加得到結果圖
		imshow("output", ori);
		imwrite("output.jpg", ori);
	}
	private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {

		ori = input.clone();  //複製input圖到ori
		threshold(input, input, 100, 255, THRESH_BINARY);  //將灰階圖做二值化
	}
	};
}
