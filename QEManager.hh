#ifndef QE_MANAGER_HH
#define QE_MANAGER_HH

#include <map>
#include <string>

class QEManager {
public:
    QEManager(const std::string& filename);
    double GetQE(double wavelength_nm) const;

private:
    std::map<double, double> qeMap;
};

#endif
