#include "ContourLines.h"

//	Double vector array that holds the gridFloat data
std::vector< std::vector<float>> GFdata;

//	Struct of the metainformation of the data. like number of columns and rows.
gridFloatData GFheader;

//	Vetor of a Vector of a Vector of a Vector of points, which represents the contour lines in each cell in each level 
std::vector< std::vector< struct lineSeg>> contours;

//	Name of file to be read
std::string fileName;

//	ints indicating the range and increment (for positive values) for the contour lines
int low = 1000, high = 2600, step = 50;

//	 color assigned to the low and high contour line
std::array<float, 3> lowColor = { 0,0,0 };
std::array<float, 3> highColor = { 1,1,1 };

//	indicates whether a marker for the ghighest point should be added
bool marker = true;

int numSteps;

//  define the window position on screen and size
int window_x;
int window_y;
int window_width = 600;
int window_height = 600;
int max_init_window_width = 800;
int max_init_window_height = 600;

float aspectRatio = 1;

int camLeft = 0, camRight = 1, camBottom = 0, camTop = 1, camNear = 0, camFar = 1;

//  variable representing the window title
char *window_title = "Contour Graph";

//  Tells whether to display the window full screen or not
//  Press Alt + Esc to exit a full screen.
int full_screen = 0;

//	Returns the case of the 4 combinations
int getContourCase(bool TL, bool TR, bool BL, bool BR) {
	int result = 0;
	if (BL)
		result++;
	if (BR)
		result += 2;
	if (TR)
		result += 4;
	if (TL)
		result += 8;
	return result;
}


//	
void getContourLine(int loopUpCase, int x, int y, float iso, std::vector<lineSeg> *lines) {
	lineSeg line;
	//	bottom Left Corner - done
	if (loopUpCase == 0 || loopUpCase == 15) {
		return;
	}
	else if (loopUpCase == 1 || loopUpCase == 14) {
		
		line.a.x = (float)x;
		line.a.y = lerp(GFdata.at(x).at(y), GFdata.at(x).at(y + 1), iso, y, y + 1);

		line.b.x = lerp(GFdata.at(x).at(y + 1), GFdata.at(x + 1).at(y + 1), iso, x, x + 1);
		line.b.y = (float)y + 1;

		lines->push_back(line);
		//std::cout << line.a.x << " " << line.a.y << " " << line.b.x << " " << line.b.y << "\n";
	}
	//	Bottom right Corner - done
	else if (loopUpCase == 2 || loopUpCase == 13) { 
		line.a.x = lerp(GFdata.at(x).at(y + 1), GFdata.at(x + 1).at(y + 1), iso, x, x + 1);
		line.a.y = (float)y + 1;

		line.b.x = (float)x + 1;
		line.b.y = lerp(GFdata.at(x + 1).at(y), GFdata.at(x + 1).at(y + 1), iso, y, y + 1);
		lines->push_back(line);
		//std::cout << line.a.x << " " << line.a.y << " " << line.b.x << " " << line.b.y << "\n";
	}
	//	Top right Corner - done
	else if (loopUpCase == 4 || loopUpCase == 11) {
		line.a.x = lerp(GFdata.at(x).at(y), GFdata.at(x + 1).at(y), iso, x, x + 1);
		line.a.y = (float)y;

		line.b.x = (float)x + 1;
		line.b.y = lerp(GFdata.at(x + 1).at(y), GFdata.at(x + 1).at(y + 1), iso, y, y + 1);
		lines->push_back(line);
		//std::cout << line.a.x << " " << line.a.y << " " << line.b.x << " " << line.b.y << "\n";
	}
	//	Top left Corner
	else if (loopUpCase == 7 || loopUpCase == 8) {
		line.a.x = (float)x;
		line.a.y = lerp(GFdata.at(x).at(y), GFdata.at(x).at(y + 1), iso, y, y + 1);

		line.b.x = lerp(GFdata.at(x).at(y), GFdata.at(x + 1).at(y), iso, x, x + 1);
		line.b.y = (float)y;
		lines->push_back(line);
		//std::cout << line.a.x << " " << line.a.y << " " << line.b.x << " " << line.b.y << "\n";
	}
	//	horizontal straight - done
	else if (loopUpCase == 3 || loopUpCase == 12) {
		line.a.x = (float)x;
		line.a.y = lerp(GFdata.at(x).at(y), GFdata.at(x).at(y + 1), iso, y, y + 1);

		line.b.x = (float)x + 1;
		line.b.y = lerp(GFdata.at(x + 1).at(y), GFdata.at(x + 1).at(y + 1), iso, y, y + 1);
		lines->push_back(line);
		//std::cout << line.a.x << " " << line.a.y << " " << line.b.x << " " << line.b.y << "\n";
	}
	//	vertical straight
	else if (loopUpCase == 6 || loopUpCase == 9) {
		line.a.x = lerp(GFdata.at(x).at(y), GFdata.at(x + 1).at(y), iso, x, x + 1);
		line.a.y = (float)y;

		line.b.x = lerp(GFdata.at(x).at(y + 1), GFdata.at(x + 1).at(y + 1), iso, x, x + 1);
		line.b.y = (float)y + 1;

		lines->push_back(line);
		//std::cout << line.a.x << " " << line.a.y << " " << line.b.x << " " << line.b.y << "\n";
	}

	//	Special cases
	else if (loopUpCase == 5) {
		line.a.x = lerp(GFdata.at(x).at(y), GFdata.at(x + 1).at(y), iso, x, x + 1);
		line.a.y = (float)y;

		line.b.x = (float)x + 1;
		line.b.y = lerp(GFdata.at(x + 1).at(y), GFdata.at(x + 1).at(y + 1), iso, y, y + 1);

		lines->push_back(line);

		lineSeg lineB;
		lineB.a.x = (float)x;
		lineB.a.y = lerp(GFdata.at(x).at(y), GFdata.at(x).at(y + 1), iso, y, y + 1);

		lineB.b.x = lerp(GFdata.at(x).at(y + 1), GFdata.at(x + 1).at(y + 1), iso, x, x + 1);
		lineB.b.y = (float)y + 1;

		lines->push_back(lineB);
	}
	else if (loopUpCase == 10) {
		line.a.x = (float)x;
		line.a.y = lerp(GFdata.at(x).at(y), GFdata.at(x).at(y + 1), iso, y, y + 1);

		line.b.x = lerp(GFdata.at(x).at(y), GFdata.at(x + 1).at(y), iso, x, x + 1);
		line.b.y = (float)y;
		lines->push_back(line);

		lineSeg lineB;
		lineB.a.x = lerp(GFdata.at(x).at(y + 1), GFdata.at(x + 1).at(y + 1), iso, x, x + 1);
		lineB.a.y = (float)y + 1;

		lineB.b.x = (float)x + 1;
		lineB.b.y = lerp(GFdata.at(x + 1).at(y), GFdata.at(x + 1).at(y + 1), iso, y, y + 1);

		lines->push_back(lineB);
	}
}

//	V1 = GFdata(x,y), V2 = GFdata(x + 1, y + 1), iso = contourlineHeight, a1 = x1 or y1, a2 = x2 or y2
float lerp(float v1, float v2, float iso, int a1, int a2) {
	return (iso - v2) * (a1 - a2) / (v1 - v2) + a2;
}

//-------------------------------------------------------------------------
//  Draws a line from x1,y1 to x2,y2 with color specified by color
//-------------------------------------------------------------------------
void DrawLine(point a, point b, std::array<float, 3> color) {
	glBegin(GL_LINES);

	glColor3f(color[0], color[1], color[2]);
	glVertex2f(a.x, a.y);
	glVertex2f(b.x, b.y);

	glEnd();
}

//-------------------------------------------------------------------------
//  Set OpenGL program initial state.
//-------------------------------------------------------------------------
void init()
{
	//  Set the frame buffer clear color to black. 
	glClearColor(1.0, 1.0, 1.0, 1.0); //White is a nice background color. 
	glOrtho(camLeft, camRight, camBottom, camTop, camNear, camFar);
	glViewport(0, 0, window_width, window_height);
}

//-------------------------------------------------------------------------
//  This function is passed to glutDisplayFunc in order to display 
//	OpenGL contents on the window.
//-------------------------------------------------------------------------
void display(void)
{
	//  Clear the window or more specifically the frame buffer...
	//  This happens by replacing all the contents of the frame buffer by the clear color (white in our case)
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < numSteps; i++) {	
		std::array<float, 3> color = { 0,0,0 };
		color[0] = lowColor[0] + i*(highColor[0] - lowColor[0]) / numSteps;
		color[1] = lowColor[1] + i*(highColor[1] - lowColor[1]) / numSteps;
		color[2] = lowColor[2] + i*(highColor[2] - lowColor[2]) / numSteps;
		/*
		std::cout << "Color "<< color[0] << " " << color[1] << " " << color[2] << "\n";
		std::cout << "number of contour line Segments " << contours.at(i).size() << "\n";
		std::cout << "\n";
		*/
		for (lineSeg line : contours.at(i)) {
			//std::cout << line.a.x << " " << line.a.y << "\n";
			DrawLine(line.a, line.b, color);
		}
		//std::cout << "\n";
	}

	if (marker) {
		int high_x = GFheader.posHighestPoint[0];
		int high_y = GFheader.posHighestPoint[1];

		lineSeg a;
		lineSeg b;
		a.a.x = high_x - 2;
		a.a.y = high_y - 2;

		a.b.x = high_x + 2;
		a.b.y = high_y + 2;

		b.a.x = high_x - 2;
		b.a.y = high_y + 2;

		b.b.x = high_x + 2;
		b.b.y = high_y - 2;

		DrawLine(a.a, a.b, { 1,0,0 });
		DrawLine(b.a, b.b, { 1,0,0 });
	}

	glutSwapBuffers();
}


//-------------------------------------------------------------------------
//  This function is passed to the glutReshapeFunc and is called
//  whenever the window is resized.
//-------------------------------------------------------------------------
void reshape(int w, int h)
{
	//  Stay updated with the window width and height
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//  Keep the correct aspect ratio
	if (aspectRatio*h > w)
		glViewport(0, 0, w, (int)(w / aspectRatio));
	else
		glViewport(0,0, (int)(aspectRatio*h), h);

	//  Print current width and height on the screen
	printf("Window Width: %d, Window Height: %d.\n", window_width, window_height);
}

//-------------------------------------------------------------------------
//  Gets keyboard input, if use hits escape, the window closes;
//-------------------------------------------------------------------------
void keyboard(unsigned char key, int x, int y)
{
	//  Print what key the user is hitting
	//printf("User is hitting the '%c' key.\n", key);
	//printf("ASCII code is %d.\n", key);
	switch (key)
	{
		//  User hits ESC key
	case 27:
		exit(1);
		break;
	}

	glutPostRedisplay();
}

//-------------------------------------------------------------------------
//  This function sets the window x and y coordinates
//  such that the window becomes centered
//-------------------------------------------------------------------------
void centerOnScreen()
{
	window_x = (glutGet(GLUT_SCREEN_WIDTH) - window_width) / 2;
	window_y = (glutGet(GLUT_SCREEN_HEIGHT) - window_height) / 2;
}

//-------------------------------------------------------------------------
//  Checks if the file exists.
//-------------------------------------------------------------------------
bool fileExists(const std::string& filename) {
	std::ifstream ifile(filename.c_str());
	return (bool)ifile;
}

//-------------------------------------------------------------------------
//  Program Main method.
//-------------------------------------------------------------------------
int main(int argc, char **argv)
{

	//	Reads in the command line input. If there isn't 11 args, then the program uses the defaults. 
	//	Where the ned_86879038 is used as the input file.
	bool autoSetVars = false;
	for (int i = 0; i < argc; i++) {
		if (std::string(argv[i]) == "auto") {
			autoSetVars = true;
			std::cout << "Setting auto variables\n";
		}
	}

	if (argc == 10 || argc == 12) {
		std::string i_fileName;
		int i_low = 0, i_high = 10, i_step = 1;
		std::array<float, 3> i_lowColor = { 0,0,0 };
		std::array<float, 3> i_highColor = { 1,1,1 };
		bool i_marker = true;
		
		int curArgPlace = 1;

		std::cout << std::string(argv[curArgPlace]) + ".hdr" << "\n";
		std::cout << std::string(argv[curArgPlace]) + ".flt" << "\n";

		if (fileExists(std::string(argv[curArgPlace]) + ".hdr") && fileExists(std::string(argv[1]) + ".flt")) {
			fileName = argv[curArgPlace];
		}
		else {
			std::cout << "Error loading input files. Exiting program\n";
			return 1;
		} 
		curArgPlace++;
		if (argc == 12) {
			if (!(std::istringstream(argv[curArgPlace]) >> i_low)) {
				std::cout << "Invalid low. Setting default of " << low << "for low\n";
			}
			else {
				low = i_low;
				std::cout << "low is set to " << argv[curArgPlace] << "\n";
			}
			curArgPlace++;
			if (!(std::istringstream(argv[curArgPlace]) >> i_high)) {
				std::cout << "Invalid low. Setting default of " << high << "for low\n";
			}
			else {
				high = i_high;
				std::cout << "high is set to " << argv[curArgPlace] << "\n";
			}
			curArgPlace++;
			if (!(std::istringstream(argv[curArgPlace]) >> i_step)) {
				std::cout << "Invalid low. Setting default of " << step << "for low\n";
			}
			else {
				step = i_step;
				std::cout << "step is set to " << argv[curArgPlace] << "\n";
			}
		}	
		curArgPlace++;
		
		for (int i = 0; i < 3; i++) {
			if (!(std::istringstream(argv[curArgPlace]) >> i_lowColor[i])) {
				std::cout << "Invalid low. Setting default of " << i_lowColor[i] << "for low\n";
			}
			else {
				lowColor[i] = i_lowColor[i];
				std::cout << "low color is set to " << argv[curArgPlace] << "\n";
			}
			curArgPlace++;
		}
		for (int i = 0; i < 3; i++) {
			if (!(std::istringstream(argv[curArgPlace]) >> i_highColor[i])) {
				std::cout << "Invalid low. Setting default of " << i_highColor[i] << "for low\n";
			}
			else {
				highColor[i] = i_highColor[i];
				std::cout << "high color is set to " << argv[curArgPlace] << "\n";
			}
			curArgPlace++;
		}
		if (!(std::string(argv[curArgPlace]) == "true" || std::string(argv[curArgPlace]) == "false")) {
			std::cout << "Invalid marker. Setting to default value of " << marker << "\n";
		}
		else {
			(std::string(argv[curArgPlace]) == "true") ? marker = true : marker = false;
		}
		curArgPlace++;
	}


	else {
		fileName = "ned_86879038";
	}
	if (GetGridFloatFromFile(fileName, GFdata, GFheader) == 1) {
		std::cout << "Error reading input files\n";
		return 1;
	}

	if (autoSetVars) {
		high = GFheader.highestValue;
		low = GFheader.lowestValue;
		step = -10;
	}

	camRight = GFheader.nrows / 12;
	camTop = GFheader.ncols / 12;

	//	Sizes the vectors to the right capacity
	if (step == 0) 
		throw std::overflow_error("Divide by zero exception");
	numSteps = (high - low) / step;

	if (step < 0) {
		numSteps *= -1;
		int temp = low;
		low = high;
		high = temp;
	}
	
	std::cout << "Number of rows is " << GFheader.nrows << "\nNumber of columns is " << GFheader.ncols << "\n";
	std::cout << "Top right corner elevation " << GFdata[0][0] << '\n';
	std::cout << "Bottom left corner elevation " << GFdata[GFheader.nrows - 1][GFheader.ncols - 1] << '\n';
	std::cout << "Highest Value = " << GFheader.highestValue << "\n";
	std::cout << "Highest point is x: " << GFheader.posHighestPoint[0] << " y: " << GFheader.posHighestPoint[1] << "\n";
	std::cout << "Number of steps= " << numSteps<< "\n";


	contours.resize(numSteps);

	int isoVal = low;
	for (int i = 0; i < numSteps; i++) {
		for (int j = 0; j < GFheader.nrows - 1; j++) {
			for (int k = 0; k < GFheader.ncols - 1; k++) {
				getContourLine(getContourCase(
					GFdata.at(j).at(k) >= isoVal,
					GFdata.at(j + 1).at(k) >= isoVal, 
					GFdata.at(j).at(k + 1) >= isoVal, 
					GFdata.at(j + 1).at(k + 1) >= isoVal), j, k, 
					(float)isoVal,
					&contours.at(i));
				//std::cout << contours.at(i).at(j).a.x << " ";
			}
			//std::cout << '\n';
		}
		isoVal += step;
		//std::cout << '\n';
	}
	if(GFheader.nrows < max_init_window_height)
		window_width = GFheader.nrows;
	if (GFheader.ncols < max_init_window_width)
		window_height = GFheader.ncols;
	aspectRatio = GFheader.ncols / GFheader.nrows;
	

	glutInit(&argc, argv);
	centerOnScreen();
	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(window_x, window_y);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow(window_title);

	//  View in full screen if the fullscreen flag is on
	if (full_screen)
		glutFullScreen();

	//  Set OpenGL program initial state.
	init();

	// Set the callback functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	//glutMotionFunc(motion);
	glOrtho(camLeft, camRight, camBottom, camTop, camNear, camFar);
	glutMainLoop();
	return 0;
}

