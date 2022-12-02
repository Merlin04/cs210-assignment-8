#include <iostream>
#include <math.h>

/*
 * Write the code for a Car class. It should represent the car objects that
 * move from location to location using fuel from their gas tank. Cars
 * should have variables representing their position in a two-dimensional
 * space, their fuel efficiency in miles per gallon, the size of their fuel
 * tank, and how much fuel they currently have. Cars should also have a
 * method move_to that tries to move to new coordinates. If the car has
 * enough fuel to drive to the new coordinates in a straight line, it does
 * so and returns true. If not, the car does not move and returns false.
 */

class Car {
public:
    double x;
    double y;
    double mpg;
    double tank_size;
    double fuel;

    Car() : Car(0, 0, 0, 0, 0) {}
    Car(double mpg, double tank_size, double fuel) : Car(0, 0, mpg, tank_size, fuel) {}
    Car(double x, double y, double mpg, double tank_size, double fuel) : x(x), y(y), mpg(mpg), tank_size(tank_size), fuel(fuel) {}

    bool move_to(double new_x, double new_y) {
        double distance = sqrt(pow(new_x - this->x, 2) + pow(new_y - this->y, 2));
        if(distance > this->fuel * this->mpg) {
            return false;
        }
        this->x = new_x;
        this->y = new_y;
        this->fuel -= distance / this->mpg;
        return true;
    }

    friend std::ostream& operator<<(std::ostream &out, const Car &car) {
        out << "Car at (" << car.x << ", " << car.y << "), fuel: " << car.fuel << " (MPG: " << car.mpg << ", tank size: " << car.tank_size << ")";
        return out;
    }
};

/*
 * Write a function that given an array of Cars and a point in two-dim.
 * space, returns an array containing a copy of each of the cars that can
 * move to that space. The input array and the Cars in it should not be
 * modified. The Cars in the output array should have the move_to method
 * called on them prior to returning. Make sure you are also returning the
 * length of the output array.
 */

Car* cars_can_move_to(Car* cars, size_t length, double x, double y, size_t* out_length) {
    auto res = new Car[length];
    size_t res_len = 0;
    for(size_t i = 0; i < length; i++) {
        res[res_len] = cars[i];
        if(res[res_len].move_to(x, y)) {
            res_len++;
        }
    }
    *out_length = res_len;
    return res;
}

/*
 * Write the code for a GasStation class. It should contain a location and
 * a price in dollars per gallon for refueling cars.
 */

class GasStation {
public:
    double x;
    double y;
    double price;

    GasStation() : GasStation(0, 0, 0) {}
    GasStation(double x, double y, double price) : x(x), y(y), price(price) {}

    friend std::ostream& operator<<(std::ostream &out, const GasStation &gas_station) {
        out << "Gas station at (" << gas_station.x << ", " << gas_station.y << "), price: " << gas_station.price;
        return out;
    }
};

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
/*
 * Write a modified version of your function for moving an array of cars that also takes an
array of GasStations. Cars that cannot directly move to the target location should check to see if
there is a suffix of the array of GasStations that they could move through (from earliest to latest
in the list) that would then allow them to move to the target location. (A suffix of the array is a
contiguous subset of the array containing the last element in the array.) You do not have to check
paths that are not suffixes of the list. If there is, they should be added to the output list as if they
could move there directly. In addition to the list of Cars being output, you should output a list of
the costs for each car to fill up its tank on the route to the location. (This is zero for the cars that
do not need to go to a GasStation.)
 */

Car* cars_can_move_to(
        Car* cars, size_t length,
        double x, double y,
        GasStation* gas_stations, size_t gas_stations_length,
        double* out_costs, size_t* out_length) {
    auto res = new Car[length];
    size_t res_len = 0;

    for(size_t i = 0; i < length; i++) {
        res[res_len] = cars[i];
        auto try_go_to = [&](GasStation* gas_end, double price) {
            // :( https://stackoverflow.com/a/72605846
            auto try_go_to_impl = [&](GasStation* gas_end, double price, auto& try_go_to) {
                if(res[res_len].move_to(x, y)) return price;
                if(res[res_len].move_to(gas_end->x, gas_end->y)) {
                    auto cost = (res[res_len].tank_size - res[res_len].fuel) * gas_end->price;
                    res[res_len].fuel = res[res_len].tank_size;
                    return try_go_to(gas_end - 1, price + cost, try_go_to);
                }
                return (double)-1;
            };
            return try_go_to_impl(gas_end, price, try_go_to_impl);
        };
        auto this_res = try_go_to(gas_stations + (gas_stations_length - 1), 0);
        if(this_res != -1) {
            out_costs[res_len] = this_res;
            res_len++;
        }
    }

    *out_length = res_len;
    return res;
}
#pragma clang diagnostic pop

void print_cars(Car* c, size_t length) {
    for(size_t i = 0; i < length; i++) {
        std::cout << c[i] << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "Movement test";
    auto car = Car(10, 10, 20.5, 36, 30.2);
    std::cout << car << std::endl;
    std::cout << car.move_to(12, 20) << std::endl;
    std::cout << car << std::endl;
    std::cout << car.move_to(-5, -1000) << std::endl;
    std::cout << car << std::endl;

    std::cout << std::endl;

    std::cout << "cars_can_move_to test" << std::endl;
    auto cars = new Car[3];
    cars[0] = Car(10, 10, 20.5, 36, 36);
    cars[1] = Car(10, 10, 30, 15, 15);
    cars[2] = Car(1, 1, 1, 1, 1);
    print_cars(cars, 3);
    size_t out_length;
    auto res = cars_can_move_to(cars, 3, 100, 100, &out_length);
    print_cars(res, out_length);
    print_cars(cars, 3);

    std::cout << std::endl;

    std::cout << "cars_can_move_to gas station test" << std::endl;
    auto gas_stations = new GasStation[3];
    gas_stations[0] = GasStation(200, 60, 2.5);
    gas_stations[1] = GasStation(500, 15, 3);
    gas_stations[2] = GasStation(700, 80, 2.2);
    auto res_costs = new double[3];
    res = cars_can_move_to(cars, 3, 1000, 100, gas_stations, 3, res_costs, &out_length);
    print_cars(res, out_length);
    for(size_t i = 0; i < out_length; i++) {
        std::cout << res_costs[i] << std::endl;
    }
    print_cars(cars, 3);




    return 0;
}
