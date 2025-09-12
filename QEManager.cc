#include "QEManager.hh"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

QEManager::QEManager(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Tried to open QE file: " << filename << std::endl;
        throw std::runtime_error("Cannot open QE file: " + filename);
    }
    
    std::string line;
    bool header_skipped = false;

    while (std::getline(file, line)) {
        if (!header_skipped) {
            header_skipped = true;
            continue;
        }
        
        std::istringstream ss(line);
        std::string wavelengthStr, qeStr;

        if (std::getline(ss, wavelengthStr, ',') && std::getline(ss, qeStr)) {
            try {
                double wavelength = std::stod(wavelengthStr);
                double qe = std::stod(qeStr);
                qeMap[wavelength] = qe;
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid line in QE file: " << line << std::endl;
            }
        }
    }

//    std::string line;
//    while (std::getline(file, line)) {
//        if (line.empty() || line[0] == '#') continue;

//        std::istringstream ss(line);
//        double wavelength, qe;
//        char comma;
//        if (ss >> wavelength >> comma >> qe) {
//            qeMap[wavelength] = qe;
//        }
//    }

    file.close();
}

double QEManager::GetQE(double wavelength_nm) const {
    if (qeMap.empty()) return 0.0;

    auto it = qeMap.lower_bound(wavelength_nm);

    if (it == qeMap.begin()) return it->second;
    if (it == qeMap.end()) return std::prev(it)->second;

    auto upper = it;
    auto lower = std::prev(it);

    double x0 = lower->first;
    double x1 = upper->first;
    double y0 = lower->second;
    double y1 = upper->second;

    return y0 + (wavelength_nm - x0) * (y1 - y0) / (x1 - x0);  // Linear interpolation
}
