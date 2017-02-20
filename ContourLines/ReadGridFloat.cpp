#include "ReadGridFloat.h"

//	Reverses the bytes of a float.
float ReverseFloat(const float inFloat)
{
	float retVal;
	char *floatToConvert = (char*)& inFloat;
	char *returnFloat = (char*)& retVal;

	// swap the bytes into a temporary buffer
	returnFloat[0] = floatToConvert[3];
	returnFloat[1] = floatToConvert[2];
	returnFloat[2] = floatToConvert[1];
	returnFloat[3] = floatToConvert[0];

	return retVal;
}

int GetGridFloatFromFile(std::string fileName, std::vector<std::vector<float>> &data, gridFloatData &GFdata) {
	//	Reads in the file responsable for determining how the gridFloat flt file is formatted.
	std::cout << "Reading in files\n";
	if (fileExists(fileName + ".hdr")) {
		std::ifstream file;
		std::string line;

		file.open(fileName + ".hdr");

		//	Parses through the hdr file and gets the relevant data out of it.
		while (std::getline(file, line)) {
			std::stringstream stream(line);
			std::string lineDefinition;
			stream >> lineDefinition;

			if (lineDefinition == "ncols") {
				std::cout << "ncols = ";
				stream >> GFdata.ncols;
				std::cout << GFdata.ncols << "\n";
			}
			else if (lineDefinition == "nrows") {
				std::cout << "nrows = ";
				stream >> GFdata.nrows;
				std::cout << GFdata.nrows << "\n";
			}
			else if (lineDefinition == "xllcorner") {
				std::cout << "xllcorner = ";
				stream >> GFdata.xllcorner;
				std::cout << GFdata.xllcorner << "\n";
			}
			else if (lineDefinition == "yllcorner") {
				std::cout << "yllcorner = ";
				stream >> GFdata.yllcorner;
				std::cout << GFdata.yllcorner << "\n";
			}
			else if (lineDefinition == "cellsize") {
				std::cout << "cellsize = ";
				stream >> GFdata.cellsize;
				std::cout << GFdata.cellsize << "\n";
			}
			else if (lineDefinition == "NODATA_value") {
				std::cout << "nodataValue =";
				stream >> GFdata.NODATA;
				std::cout << GFdata.NODATA << "\n";
			}
			else if (lineDefinition == "byteorder") {
				std::cout << "byteorder =";
				std::string order;
				stream >> order;
				std::cout << order << '\n';
				
				if (order == "LSBFIRST")
					GFdata.order = LSBFIRST;
				else if (order == "MSBFIRST")
					GFdata.order = MSBFIRST;
				else if (order == "VMS_FFLOAT")
					GFdata.order = VMS_FFLOAT;
				else {
					std::cout << "error reading byteorder\n";
					return 1;
				}
			}	
		}

	}
	else {
		std::cerr << "Unable to read .hdr file\n";
	}
	/*
	//	Reads in the prj file, which has info I don't care about right now. So it awaits implimentation
	std::cout << "\n";
	if (fileExists(fileName + ".prj")) {
		std::ifstream file;
		std::string line;

		file.open(fileName + ".prj");

		while (std::getline(file, line))
		{
			//cout << line << '\n';
		}

	}
	else {
		std::cerr << "Unable to read .prj file\n";
	}
	*/

	//	Reads the actual gridFloat data in, as determined by the gridFLoatData struct.
	std::cout << "\n";
	if (fileExists(fileName + ".flt")) {
		std::ifstream file;

		file.open(fileName + ".flt", std::ios::binary | std::ios::in);

		data.resize(GFdata.nrows, std::vector<float>(GFdata.ncols, (float)GFdata.NODATA));

		GFdata.highestValue = (float)GFdata.NODATA;
		GFdata.lowestValue = 99999999.0;
		for (int i = 0; i < GFdata.nrows; i++) {
			for (int j = 0; j < GFdata.ncols; j++) {
				float numReadIn;

				if (GFdata.order == LSBFIRST) {
					file.read((char *)&numReadIn, sizeof(numReadIn));
				}
				data[i][j] = numReadIn;
				if (numReadIn > GFdata.highestValue) {
					GFdata.highestValue = numReadIn;
					GFdata.posHighestPoint[0] = i;
					GFdata.posHighestPoint[1] = j;
				}
				if (numReadIn < GFdata.lowestValue)
					GFdata.lowestValue = numReadIn;
			}
			//std::cout << data[i][0] << " i :" << i << "   j :" << 0 << " \n";
		}

	}
	else {
		std::cerr << "Unable to read .flt file\n";
	}
	return 0;
}	