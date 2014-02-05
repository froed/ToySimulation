#include <iostream>
#include <fstream>

#include "Simulation.h"
#include "Timer.h"

#define PROGRAM_EXIT_OK 0
#define PROGRAM_EXIT_ERROR 1

int ExitWithError(const std::string message)
{
	cout << message <<  endl;
	return PROGRAM_EXIT_ERROR;
}

int main(int argc, char *argv[])
{

	bool noArgs = argc == 1;
	bool allArgs = argc == 6;

	if (!noArgs && !allArgs) return ExitWithError("Please provide either none or all arguments:\nVec3 PixelPosition, Vec3 PixelDirection, double Energy");
	
	double energy, position_x, position_y, direction_x, direction_y;

	if (noArgs)
	{
		// Einlesen der Anfangsbedingungen �ber Console
		cout << "Choose Start Energy, Position and Direction of the Particle" << endl;
		cout << "Enter the Energy: ";
		cin >> energy;
		cout << "Enter the Position of the Particle (x y):";
		cin >> position_x >> position_y;
		cout << endl;
		
		cout << "Enter the Direction of the Particle (x y):";
		cin >> direction_x >> direction_y;
	}
	else
	{
		sscanf(argv[1],"%lf",&energy);
		sscanf(argv[2],"%lf",&position_x);
		sscanf(argv[3],"%lf",&position_y);
		sscanf(argv[4],"%lf",&direction_x);
		sscanf(argv[5],"%lf",&direction_y);
	}

	printf("Energy=%.5f Pos=[%.2f/%.2f] Dir=[%.2f/%.2f]\n", energy, position_x, position_y, direction_x, direction_y);

	Vec2 position = Vec2(position_x, position_y);
	Vec2 direction = Vec2(direction_x, direction_y);

    // Einlesen der Pixel aus Sensordaten.txt
    ifstream Sensordaten;
    Sensordaten.open ("Sensordaten.txt", ios_base::in );

	if (!Sensordaten.is_open()) return ExitWithError("Error loading 'Sensordaten.txt'!");

    // Anzahl der Pixel die als erstes ind er Tabelle gegeben werden
    unsigned int numPixels;
    Sensordaten >> numPixels;

    // erstellen des sensors
    Sensor sensor;
    // �berpr�ft ob Datei ge�ffnet werden kann
    // Anzahl der Pixel die als erstes ind er Tabelle gegeben werden

    double sensor_x, sensor_y;
    Sensordaten >> sensor_x;
    Sensordaten >> sensor_y;

    cout << "Sensor Properties are being read ... "<< endl;
    // Definition der Sensor Properties
    sensor = Sensor(Vec2(sensor_x, sensor_y), numPixels);

    for(unsigned int i = 1; i <= numPixels; i++){
        // Deklaration der einzelnen Elemente die in den Pixel Array geschrieben werden sollen
        double p_x, p_y, p_size_x,p_size_y;
        // Einlesen der Parameter
        Sensordaten >> p_x;
        Sensordaten >> p_y;
        Sensordaten >> p_size_x;
        Sensordaten >> p_size_y;
        // Erstellen des neuen Pixels
        Pixel *pixel = new Pixel(Vec2(p_x, p_y), Vec2(p_size_x,p_size_y));
        // Hinzuf�gen des Pixels und �berpr�fung n�chster Nachbarn
        sensor.addP_sensorpixelpointer(pixel);
    }
    cout << "Data file has been loaded."<< endl;

    Sensordaten.close();

    Simulation simulation = Simulation(sensor);
    //Simulation wird mit Funktion run durchgef�hrt

	Timer simulationTime;
	simulationTime.start();

	simulation.run(energy, position, direction);

	simulationTime.stop();

    // Ausgabe der Sensor Properties
    simulation.printSensorProperties();
	
    simulation.printEnergies();

	printf("Simulation took %i us\n",simulationTime.getExecutionTime());
}
