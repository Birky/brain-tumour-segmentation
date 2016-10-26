#include "Patient.h"


bts::Patient::Patient()
{

}


bts::Patient::~Patient()
{
}

bts::ProcessedData* bts::Patient::getProcessedData(std::string title)
{
	for (int i = 0; i < processedData.size(); i++)
	{
		ProcessedData* pd = &(processedData.at(i));
		if (title.compare(pd->getTitle()) == 0)
		{
			return pd;
		}
	}
}

void bts::Patient::addProcessedData(ProcessedData &pd)
{
	processedData.push_back(pd);
}