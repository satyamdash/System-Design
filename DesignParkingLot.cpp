#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <cmath>

using namespace std;

// Enum for vehicle types
enum class VehicleType { CAR, BIKE, TRUCK };

// Structure for a parking spot with coordinates
struct ParkingSpot {
    int id;
    int row, col;  // Coordinates
    VehicleType type;
    bool isOccupied;

    ParkingSpot(int spotId, int r, int c, VehicleType vType)
        : id(spotId), row(r), col(c), type(vType), isOccupied(false) {}

    // Calculate distance from entrance (0,0)
    double distanceFromEntrance() const {
        return sqrt(row * row + col * col);
    }

    // Custom comparator for priority queue (Min-Heap)
    bool operator>(const ParkingSpot& other) const {
        return distanceFromEntrance() > other.distanceFromEntrance();
    }
};

// Parking Lot Class
class ParkingLot {
private:
    priority_queue<ParkingSpot, vector<ParkingSpot>, greater<ParkingSpot>> availableSpots;
    unordered_map<int, ParkingSpot> allSpots;
    unordered_map<string, int> vehicleToSpot;

public:
    ParkingLot(vector<ParkingSpot> spots) {
        for (const auto& spot : spots) {
            availableSpots.push(spot);
            allSpots[spot.id] = spot;
        }
    }

    void suggestNearestSpot(VehicleType type) {
        priority_queue<ParkingSpot, vector<ParkingSpot>, greater<ParkingSpot>> tempQueue = availableSpots;
        while (!tempQueue.empty()) {
            ParkingSpot spot = tempQueue.top();
            tempQueue.pop();
            if (!spot.isOccupied && spot.type == type) {
                cout << "Nearest available spot for your vehicle: ID " << spot.id
                     << " at (" << spot.row << ", " << spot.col << ")\n";
                return;
            }
        }
        cout << "No available spots for your vehicle type.\n";
    }

    bool parkVehicle(string licensePlate, VehicleType type) {
        priority_queue<ParkingSpot, vector<ParkingSpot>, greater<ParkingSpot>> tempQueue;
        bool parked = false;

        while (!availableSpots.empty()) {
            ParkingSpot spot = availableSpots.top();
            availableSpots.pop();
            if (!spot.isOccupied && spot.type == type) {
                spot.isOccupied = true;
                vehicleToSpot[licensePlate] = spot.id;
                allSpots[spot.id] = spot;
                cout << "Vehicle " << licensePlate << " parked at spot ID " << spot.id << "\n";
                parked = true;
            }
            tempQueue.push(spot);
        }
        availableSpots = tempQueue;
        return parked;
    }

    void removeVehicle(string licensePlate) {
        if (vehicleToSpot.find(licensePlate) != vehicleToSpot.end()) {
            int spotId = vehicleToSpot[licensePlate];
            allSpots[spotId].isOccupied = false;
            availableSpots.push(allSpots[spotId]);
            vehicleToSpot.erase(licensePlate);
            cout << "Vehicle " << licensePlate << " removed from spot " << spotId << "\n";
        } else {
            cout << "Vehicle not found in parking lot.\n";
        }
    }
};

// Main Function
int main() {
    vector<ParkingSpot> spots = {
        ParkingSpot(1, 0, 1, VehicleType::CAR),
        ParkingSpot(2, 1, 1, VehicleType::CAR),
        ParkingSpot(3, 2, 1, VehicleType::BIKE),
        ParkingSpot(4, 1, 2, VehicleType::TRUCK)
    };

    ParkingLot lot(spots);

    lot.suggestNearestSpot(VehicleType::CAR);
    lot.parkVehicle("ABC123", VehicleType::CAR);
    lot.suggestNearestSpot(VehicleType::BIKE);
    lot.parkVehicle("BIKE567", VehicleType::BIKE);

    lot.removeVehicle("ABC123");
    lot.suggestNearestSpot(VehicleType::CAR);

    return 0;
}
