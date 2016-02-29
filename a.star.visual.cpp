// a star visual.cpp: A program using the TL-Engine
//Josh Stone JStone3

#include <TL-Engine.h>	// TL-Engine include file and namespace
#include <fstream>
#include<iostream>
#include <sstream>
using namespace std;
using namespace tle;

const int numberCoords = 23;//Number of coordinates in output file.
//Coords structure.
struct coordinates
{
	int x;
	int y;
};
//Function used to read in the file containing the routes for each map.
void readFiles(string inputFile, coordinates coords[numberCoords])
{
	string word;
	string word1;
	string word2;
	ifstream infile;
	infile.open(inputFile);
	if (!infile)
	{
		cout << "ERROR: ";
		cout << "Can't open input file\n";
	}
	while (!infile.eof())
	{
		getline(infile, word);//Each set of co-odrinates starts with the map name. This is used to prevent the program trying 
		//to add the map name to the co-ordinate array.
			for (int i = 0; i < 9; i++)
			{
				infile >> coords[i].x >> coords[i].y;
			}
			infile >> word1;
			for (int i = 9; i < numberCoords; i++)
			{
				infile >> coords[i].x >> coords[i].y;				
				if (i == 22)
					{
						return;
					}
			}	
	}
	infile.close();
}
//Function used to set up the grid that represents each map.
void resetGrid(IMesh* squareMesh, IModel* mapSquares[10][10], string water, string wood, string open, string wall, 
	string start, string end, int mapArray[10][10], coordinates mapStart, coordinates mapEnd)
{
	int x;
	int y;
	for (int i = 0; i < 10; i++)//rows 
	{
		for (int j = 0; j < 10; j++)//collums 
		{
			mapSquares[i][j]->SetSkin(open);
		}
	}
	//Each square has a different skin based on its cost which is retrieved from the appropriate map array.
		for (int i = 0; i < 10; i++)//rows 
		{
			for (int j = 0; j < 10; j++)//collums 
			{
				if (mapArray[9 - i][j] == 0)
				{
					x = i;
					y = j;
					mapSquares[i][j] = squareMesh->CreateModel(y * 10, 0, x * 10);
					mapSquares[i][j]->SetSkin(wall);
				}
				if (mapArray[9 - i][j] == 1)
				{
					x = i;
					y = j;
					mapSquares[i][j] = squareMesh->CreateModel(y * 10, 0, x * 10);
					mapSquares[i][j]->SetSkin(open);
				}

				if (mapArray[9 - i][j] == 2)
				{
					x = i;
					y = j;
					mapSquares[i][j] = squareMesh->CreateModel(y * 10, 0, x * 10);
					mapSquares[i][j]->SetSkin(wood);
				}
				if (mapArray[9 - i][j] == 3)
				{
					x = i;
					y = j;
					mapSquares[i][j] = squareMesh->CreateModel(y * 10, 0, x * 10);
					mapSquares[i][j]->SetSkin(water);
				}
			}
		}		
		mapSquares[mapStart.y][mapStart.x]->SetSkin(start);
		mapSquares[mapEnd.y][mapEnd.x]->SetSkin(end);
}
//Function used to reset camera. 
void cameraReset(ICamera* &camera)
{
	camera->SetPosition(50, 100, -65);
	camera->ResetOrientation();
	camera->RotateX(40);
}
void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( ".//aStarMedia" );

	/**** Set up your scene here ****/
	const int mMapNumber = 8;
	const int dMapnumer = 23;
	//Array used to hold all coordinates of every node for both maps.
	coordinates coords[numberCoords];
	//Variables to hold start and end for each map.
	coordinates mStart;
	coordinates mEnd;
	coordinates dStart;
	coordinates dEnd;
	//Variables holding names of different skins.
	string water = "blue.png";
	string wood = "green.png";
	string open = "grey.png";
	string wall = "black.png";
	string start = "purple.png";
	string end = "red.png";
	string path = "yellow.png";
	//Variables used to hold text that will be displayed on screen to user.
	stringstream waterText;
	waterText << "Water = blue";
	stringstream woodText;
	woodText << "Wood = green";
	stringstream openText;
	openText << "Clear = grey";
	stringstream wallText;
	wallText << "Wall = black";
	stringstream startText;
	startText << "Start = purple";
	stringstream endText;
	endText << "End = red";
	stringstream pathText;
	pathText << "Path = yellow";
	stringstream resetCamera;
	resetCamera << "Reset Camera = C key";
	stringstream mMapText;
	mMapText << "mMap = 1 key";
	stringstream dMapText;
	dMapText << "dMap = 2 key";
	stringstream routeComplete;
	routeComplete << "Route complete";
	stringstream mMapDisplay;
	mMapDisplay << "mMap";
	stringstream dMapDisplay;
	dMapDisplay << "dMap";
	stringstream displayRouteText;
	displayRouteText << "Display Route = Enter key";
	//Different fonts used for displaying HUD information.
	IFont* infoText = myEngine->LoadFont("Arial", 26);
	IFont* gameFont = myEngine->LoadFont("Arial", 36);
	//Create and load array of squares.
	IMesh* squareMesh = myEngine->LoadMesh("cube.x");
	IModel* mapSquares[10][10];
	//Create camera.
	ICamera* camera = myEngine->CreateCamera(kFPS, 50, 100, -65);
	camera->RotateX(40);
	//Arrays used to store costs of each square for each map.
	int mMapArray[10][10];
	int dMapArray[10][10];
	//File names.
	string inputFile = "Output.txt";
	string mMap = "mMap.txt";
	string dMap = "dMap.txt";
	ifstream infile;
	//Read in input file and store in coords array.
	readFiles(inputFile, coords);
	//Start and end points of each map are then assigned to respective variables.
	mStart.x = coords[0].x;
	mStart.y = coords[0].y;
	mEnd.x = coords[8].x;
	mEnd.y = coords[8].y;
	dStart.x = coords[9].x;
	dStart.y = coords[9].y;
	dEnd.x = coords[22].x;
	dEnd.y = coords[22].y; 
	//Files storing node costs are read in and store in respective map arrays.
	ifstream readFiles;
	readFiles.open(mMap);
	if (!readFiles)
	{
		cout << "ERROR: ";
		cout << "Can't open mMap file\n";
	}
	while (!readFiles.eof())
	{
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				readFiles >> mMapArray[i][j];
				///open the file and put in the and then the new coordinates that we generate will relate to a positon in the array
			}							//and this will allow us to get the cost of that square.		
		}
	}
	readFiles.close();
	//Files storing node costs are read in and store in respective map arrays.
	readFiles.open(dMap);
	if (!readFiles)
	{
		cout << "ERROR: ";
		cout << "Can't open dMap file\n";
	}
	while (!readFiles.eof())
	{
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				readFiles >> dMapArray[i][j];
				///open the file and put in the and then the new coordinates that we generate will relate to a positon in the array
			}							//and this will allow us to get the cost of that square.		
		}
	}
	readFiles.close();
	//Grid of squares is then created and skins are set according to each squares cost which is gathered from each map array.
	float x;
	float y;
	for (int i = 0; i < 10; i++)//rows 
	{
		for (int j = 0; j < 10; j++)//collums 
		{
			if (mMapArray[9-i][j] == 0)
			{
				x = i;
				y = j;
				mapSquares[i][j] = squareMesh->CreateModel(y*10, 0, x*10);
				mapSquares[i][j]->SetSkin(wall);
			}
			if (mMapArray[9-i][j] == 1)
			{
				x = i;
				y = j;
				mapSquares[i][j] = squareMesh->CreateModel(y * 10, 0, x * 10);
				mapSquares[i][j]->SetSkin(open);	
			}

			if (mMapArray[9-i][j] == 2)
			{
				x = i;
				y = j;
				mapSquares[i][j] = squareMesh->CreateModel(y * 10, 0, x * 10);
				mapSquares[i][j]->SetSkin(wood);
			}
			if (mMapArray[9-i][j] == 3)
			{
				x = i;
				y = j;
				mapSquares[i][j] = squareMesh->CreateModel(y * 10, 0, x * 10);
				mapSquares[i][j]->SetSkin(water);
			}
		}
	}
	mapSquares[mStart.y][mStart.x]->SetSkin(start);
	mapSquares[mEnd.y][mEnd.x]->SetSkin(end);
	
	int pathCounter = 0;//Counter used to record how far along the path the program has moved.
	int mapNumber = 1;//Variable used to record which map is being displayed.
	//Bool variables used to record when key presses occur.
	bool route1KeyPressed = false;
	bool route2KeyPressed = false;
	bool resetCameraKeyPressed = false;
	//Bools used to indicate when a route has been fully displayed.
	bool route1Finished = false;
	bool route2Finished = false;
	//Bool used to record whether the user wants the route to be displayed.
	bool displayRoute = false;
	float timer = 0;//Timer used to alow the program to automatically display the route.
	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		float frameTime = myEngine->Timer();
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/
		if (myEngine->KeyHit(Key_Escape))
		{
			myEngine->Stop();
		}
		route1KeyPressed = (myEngine->KeyHit(Key_1));
		route2KeyPressed = (myEngine->KeyHit(Key_2));
		resetCameraKeyPressed = (myEngine->KeyHit(Key_C));

		if (resetCameraKeyPressed)
		{
			cameraReset(camera);
		}

		if (route1KeyPressed)
		{
			//Reset all variables involved in displaying a route to allow program to funciton properly after swapping between both maps.
			mapNumber = 1;
			resetGrid(squareMesh, mapSquares, water, wood, open, wall, start, end, mMapArray, mStart, mEnd);
			route1Finished = false;
			route2Finished = false;
			pathCounter = 0;
			timer = 0;
			displayRoute = false;
		}
		if (route2KeyPressed)
		{
			//Reset all variables involved in displaying a route to allow program to funciton properly after swapping between both maps.
			mapNumber = 2;
			resetGrid(squareMesh, mapSquares, water, wood, open, wall, start, end, dMapArray, dStart, dEnd);
			route1Finished = false;
			route2Finished = false;
			pathCounter = 0;
			timer = 0;
			displayRoute = false;
		}
		//Outputs information about square colour to user in top right of game window.
		infoText->Draw(startText.str(), 1000, 10, kMagenta, kLeft, kVCentre);
		infoText->Draw(endText.str(), 1000, 30, kRed, kLeft, kVCentre);
		infoText->Draw(openText.str(), 1000, 50, kGrey, kLeft, kVCentre);
		infoText->Draw(wallText.str(), 1000, 70, kBlack, kLeft, kVCentre);
		infoText->Draw(woodText.str(), 1000, 90, kGreen, kLeft, kVCentre);
		infoText->Draw(waterText.str(), 1000, 110, kBlue, kLeft, kVCentre);
		infoText->Draw(pathText.str(), 1000, 130, kYellow, kLeft, kVCentre);
		//Outputs information about key controls to user in top left of game window.
		gameFont->Draw(resetCamera.str(), 0, 20, kWhite, kLeft, kVCentre);
		gameFont->Draw(displayRouteText.str(), 0, 50, kWhite, kLeft, kVCentre);
		gameFont->Draw(mMapText.str(), 0, 80, kWhite, kLeft, kVCentre);
		gameFont->Draw(dMapText.str(), 0, 110, kWhite, kLeft, kVCentre);
		//Different text outputs are displayed based on what map is currently being displayed.
		if (mapNumber == 1)
		{
			gameFont->Draw(mMapDisplay.str(), 500, 20, kWhite, kLeft, kVCentre);
		}
		if (mapNumber == 2)
		{
			gameFont->Draw(dMapDisplay.str(), 500, 20, kWhite, kLeft, kVCentre);
		}
		//Enter key is used to start the automatic display of the route.
		if (myEngine->KeyHit(Key_Return))
		{
			displayRoute = true;
		}
		//Loop where route is dispalyed.
		if (displayRoute)
		{
			timer += frameTime;
			//Timer used to automate the process.
			if (timer >= 0.5)
			{
				if (mapNumber == 1)
				{
					if (pathCounter < 9)
					{
						mapSquares[coords[pathCounter].y][coords[pathCounter].x]->SetSkin(path);
						pathCounter++;
						timer = 0;
					}
					if (pathCounter == 9)
					{
						route1Finished = true;
					}
				}
				if (mapNumber == 2)
				{
					if (pathCounter < 9)
					{
						pathCounter = 9;
					}
					if (pathCounter < dMapnumer)
					{
						mapSquares[coords[pathCounter].y][coords[pathCounter].x]->SetSkin(path);
						pathCounter++;
						timer = 0;
					}
					if (pathCounter == dMapnumer)
					{
						route2Finished = true;
					}
				}
			}
		}
		//Dispaly message indicating route has been displayed.
		if (route1Finished || route2Finished)
		{
			gameFont->Draw(routeComplete.str(), 500, 80, kRed, kLeft, kVCentre);
		}
		
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
