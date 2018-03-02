#ifndef CSV_HPP
#define CSV_HPP

#include <string>
#include <fstream>
#include <vector>

class CSVReader
{
public:
	class CSVRow
	{
		std::vector<std::string> columns;
		CSVRow () {}
		CSVRow (const std::string &line);
	public:
		const std::string &column (unsigned int index) const
		{
			return this->columns [index];
		}
		friend CSVReader;
	};
	CSVReader (const std::string &filename);
	bool next ();
	const CSVRow &row () const
	{
		return this->current_row;
	}
private:
	std::ifstream csv_stream;
	CSVRow current_row;
};

#endif // CSV_HPP
