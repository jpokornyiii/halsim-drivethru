#include <chrono>
#include <thread>

#include <hal/HALBase.h>

extern "C" int HALSIM_InitExtension(void);

int main() {
    HAL_Initialize(500, 0);
    HALSIM_InitExtension();
    std::this_thread::sleep_for(std::chrono::seconds(5));
}