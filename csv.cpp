#include <sstream>

#include "csv.hpp"

using namespace std;

CSVReader::CSVReader (const string &filename):
   csv_stream (filename)
{

}

bool CSVReader::next ()
{
	string line;
	if (getline (this->csv_stream, line)) {
		this->current_row = CSVRow (line);
	}
	return !this->csv_stream.eof ();
}

CSVReader::CSVRow::CSVRow (const string &line)
{
	stringstream stream (line);
	string cell;
	while (getline (stream, cell, ',')) {
		this->columns.push_back (cell);
	}
}
