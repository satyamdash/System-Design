#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

// Enum for vehicle types
enum class VehicleType { CAR, BIKE, TRUCK };

// Vehicle Class
class Vehicle {
public:
    string licensePlate;
    VehicleType type;

    Vehicle(string plate, VehicleType vType){
        this->licensePlate = plate;
        this->type = vType;
    }
};

// Parking Spot Class
class ParkingSpot {
public:
    int id;
    VehicleType type;
    bool isOccupied;

    ParkingSpot(int spotId, VehicleType vType) : id(spotId), type(vType), isOccupied(false) {}
};

// Parking Lot Class
class ParkingLot {
private:
    vector<ParkingSpot> spots;
    unordered_map<string, int> vehicleToSpot;

public:
    ParkingLot(int carSpots, int bikeSpots, int truckSpots) {
        int id = 1;
        for (int i = 0; i < carSpots; i++) spots.push_back(ParkingSpot(id++, VehicleType::CAR));
        for (int i = 0; i < bikeSpots; i++) spots.push_back(ParkingSpot(id++, VehicleType::BIKE));
        for (int i = 0; i < truckSpots; i++) spots.push_back(ParkingSpot(id++, VehicleType::TRUCK));
    }

    bool parkVehicle(Vehicle vehicle) {
        for (ParkingSpot &spot : spots) {
            if (!spot.isOccupied && spot.type == vehicle.type) {
                spot.isOccupied = true;
                vehicleToSpot[vehicle.licensePlate] = spot.id;
                cout << "Vehicle " << vehicle.licensePlate << " parked at spot " << spot.id << endl;
                return true;
            }
        }
        cout << "No available spot for " << vehicle.licensePlate << endl;
        return false;
    }

    void removeVehicle(string licensePlate) {
        if (vehicleToSpot.find(licensePlate) != vehicleToSpot.end()) {
            int spotId = vehicleToSpot[licensePlate];
            for (ParkingSpot &spot : spots) {
                if (spot.id == spotId) {
                    spot.isOccupied = false;
                    vehicleToSpot.erase(licensePlate);
                    cout << "Vehicle " << licensePlate << " removed from spot " << spotId << endl;
                    return;
                }
            }
        }
        cout << "Vehicle not found.\n";
    }
};

// Main Function
int main() {
    ParkingLot lot(2, 2, 1); // 2 Car spots, 2 Bike spots, 1 Truck spot

    Vehicle car1("ABC123", VehicleType::CAR);
    Vehicle bike1("BIKE567", VehicleType::BIKE);

    lot.parkVehicle(car1);
    lot.parkVehicle(bike1);

    lot.removeVehicle("ABC123");

    return 0;
}
