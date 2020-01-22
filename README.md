# optimystic

A C++ library for LP and MIP modeling which abstracts away CPLEX and Gurobi.

## Getting Started

You will need either a copy of CPLEX or Gurobi, and the corresponding license file. 
Please see [CPLEX](https://www.ibm.com/analytics/cplex-optimizer) or [Gurobi](https://www.gurobi.com/) for instructions.
If you are an academic, both companies provide free academic licenses.

### Prerequisites

You will need cmake and c++ compiler which supports c++14.

To install them on macOS, I recommend installing [homebrew](https://brew.sh/).
```
xcode-select --install
brew install cmake
```
For Ubuntu/Debian
```
apt install build-essential
apt install cmake
```

#Example Model
The following is an example of the syntax required to build a CPLEX or Gurobi model.
This example was taken from the Gurobi Documentation, found [here](https://www.gurobi.com/documentation/9.0/examples/mip1_cpp_cpp.html)
```
int main() {
    Model model = Model();
    model.setLogFile("miplog1.log");

    // Create variables
    auto x = model.addVar(0.0, 1.0, 1.0, SOLVER_BINARY, "x");
    auto y = model.addVar(0.0, 1.0, 1.0, SOLVER_BINARY, "y");
    auto z = model.addVar(0.0, 1.0, 2.0, SOLVER_BINARY, "z");

    // Add constraint: x + 2 y + 3 z <= 4
    std::vector<double> coeffs1 = {1.0, 2.0, 3.0};
    std::vector<std::shared_ptr<Variable>> variables1 = {x, y, z};
    model.addConstr(variables1, coeffs1, SOLVER_LESS_EQUAL, 4.0, "c0");

    // Add constraint: x + y >= 1
    std::vector<double> coeffs2 = {1.0, 1.0};
    std::vector<std::shared_ptr<Variable>> variables2 = {x, y};
    model.addConstr(variables2, coeffs2, SOLVER_GREATER_EQUAL, 1.0, "c1");

    // Optimize model
    model.setSense(SOLVER_MAXIMIZE);
    model.optimize();

    std::cout << "X value: " << x->getValue() << std::endl;
    std::cout << "Y value: " << y->getValue() << std::endl;
    std::cout << "Z value: " << z->getValue() << std::endl;

    std::cout << "Obj: " << model.getObjectiveValue() << std::endl;
}
```

## Built With

* [cmake](https://cmake.org/)
* [google-test](https://github.com/google/googletest)
* [CPLEX](https://www.ibm.com/analytics/cplex-optimizer)
* [Gurobi](https://www.gurobi.com/)

## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.

## Authors

* **Connor Riley** - *Initial work* - [Website](https://ctriley.github.io/)

See also the list of [contributors](https://github.com/your/project/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Antoine Legrain
