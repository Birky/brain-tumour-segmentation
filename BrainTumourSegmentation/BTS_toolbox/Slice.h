#pragma once
#include <string>
#include <opencv2/core/mat.hpp>


namespace bts
{
	class Slice
	{
	public:
		Slice();
		~Slice();
	private:
		std::string filePath;
		cv::Mat data;
		int number;

	public:
		std::string getFilePath() const
		{
			return filePath;
		}

		void setFilePath(std::string file_path)
		{
			filePath = std::move(file_path);
		}

		cv::Mat getData() const
		{
			return data;
		}

		void setData(cv::Mat data_org)
		{
			data = std::move(data_org);
		}

		int getNumber() const
		{
			return number;
		}

		void setNumber(int number)
		{
			this->number = number;
		}
	};

	class EvaluatedSlice : public Slice
	{
		public: 
			int FP, FN, TP, TN; // TODO prerob na private s getters a setters
			float jaccard, dice;
	};
}

