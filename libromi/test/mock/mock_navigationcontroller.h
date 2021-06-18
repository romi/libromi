#include "gmock/gmock.h"
#include "rover/INavigationController.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

class MockNavigationController : public romi::INavigationController
{
public:
        MOCK_METHOD(double, estimate_correction, (double cross_track_error,
                                                  double orientation_error), (override));
};
#pragma GCC diagnostic pop
