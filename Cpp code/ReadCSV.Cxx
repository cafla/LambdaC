#ifndef READCSV_CXX
#define READCSV_CXX


#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
//Functions to read a CSV
//Adapted from https://stackoverflow.com/questions/1120140/how-can-i-read-and-parse-csv-files-in-c#, answer from Martin York (seen on july 2021)

class CSVRow//Class to read Row per row a CSV file
{
    public:
        std::string_view operator[](std::size_t index) const
        {
            return std::string_view(&m_line[m_data[index] + 1], m_data[index + 1] -  (m_data[index] + 1));
        }
        std::size_t size() const
        {
            return m_data.size() -1;
        }
        void readNextRow(std::istream& str)
        {
            std::getline(str, m_line);

            m_data.clear();
            std::string::size_type pos = 0;
            while((pos = m_line.find(',', pos)) != std::string::npos)
            {
                m_data.emplace_back(pos);
                ++pos;
            }
            // This checks for a trailing comma with no data after it.
            pos   = m_line.size()-1;//Issue with end of line /!\ Needs last line to have a return to the line
            m_data.emplace_back(pos);
        }
    private:
        std::string         m_line;
        std::vector<int>    m_data;
};

std::istream& operator>>(std::istream& str, CSVRow& data)
{
    data.readNextRow(str);
    return str;
}   

class CSVIterator //Iterator on CSVRows
{   
    public:
        typedef std::input_iterator_tag     iterator_category;
        typedef CSVRow                      value_type;
        typedef std::size_t                 difference_type;
        typedef CSVRow*                     pointer;
        typedef CSVRow&                     reference;

        CSVIterator(std::istream& str)  :m_str(str.good()?&str:NULL) { ++(*this); }
        CSVIterator()                   :m_str(NULL) {}

        // Pre Increment
        CSVIterator& operator++()               {if (m_str) { if (!((*m_str) >> m_row)){m_str = NULL;}}return *this;}
        // Post increment
        CSVIterator operator++(int)             {CSVIterator    tmp(*this);++(*this);return tmp;}
        CSVRow const& operator*()   const       {return m_row;}
        CSVRow const* operator->()  const       {return &m_row;}

        bool operator==(CSVIterator const& rhs) {return ((this == &rhs) || ((this->m_str == NULL) && (rhs.m_str == NULL)));}
        bool operator!=(CSVIterator const& rhs) {return !((*this) == rhs);}
    private:
        std::istream*       m_str;
        CSVRow              m_row;
};




std::string asString(std::string_view v) {return {v.data(), v.size()};}

std::vector<std::vector<std::string>> ReadCSV(std::string fileName)
{
	
    std::ifstream       file(fileName);
	std::vector<std::vector<std::string>> table;
	std::vector<std::string> oneLine;
	
	
    for(CSVIterator loop(file); loop != CSVIterator(); ++loop)
    {
		oneLine = {};
		for (std::size_t indexLine(0); indexLine< loop->size(); ++indexLine)
		{
			oneLine.push_back(asString((*loop)[indexLine]));
		}
		table.push_back(oneLine);
    }
	
	/*To display content
	for (std::size_t indexLine(0); indexLine< table.size(); indexLine++)
	{
		for ( std::size_t indexCell(0); indexCell< table[indexLine].size(); indexCell++)
			std::cout <<  table[indexLine][indexCell] << "; " ;
		std::cout << "\n";
	}
	std::cout  << std::endl;
	std::cout  << std::endl;
	*/
	return table;
}

#endif 
