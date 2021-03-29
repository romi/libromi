#include "gmock/gmock.h"
#include "data_provider/IGPS.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockGps : public IGps
{
public:
        MOCK_METHOD2(CurrentLocation, void(double& latitude, double& longitude));
};
#pragma GCC diagnostic pop