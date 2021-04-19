#include <stdio.h>
#include <unistd.h>
#include <stdexcept>
#include <r.h>
#include <MemBuffer.h>
#include "PiCamera.h"

using namespace romi;
using namespace std;

void store_jpeg(rpp::MemBuffer& jpeg, const char *name_prefix, int index)
{
        char filename[512];
        snprintf(filename, sizeof(filename), "%s_%04d.jpg", name_prefix, index);
        r_info("File %s, Length %d", filename, (int) jpeg.size());
        
        int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if (fd >= 0) {
                write(fd, jpeg.data().data(), jpeg.size());
                fsync(fd);
                close(fd);
        } else {
                r_err("Failed to write to %s", filename);
        }
}

void grab_series(PiCamera& camera, const char *name_prefix, int number)
{
        for (int i = 0; i < number; i++) {
                rpp::MemBuffer& jpeg = camera.grab_jpeg();
                store_jpeg(jpeg, name_prefix, i);
        }
}

int main()
{
        try {
                PiCameraSettings settings;
                init_for_camera_v2(settings, kV2HalfWidth, kV2HalfHeight);
                
                PiCamera camera(settings);

                clock_sleep(4.0);
                
                grab_series(camera, "test", 10);
                
        } catch (runtime_error& re) {
                r_err("Caught exception: %s", re.what());
        }
}
