
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <cstdlib>
#include <ctime>


int main(int argc, char* argv[])
{
    // Since Google Mock depends on Google Test, InitGoogleMock() is also
    // responsible for initializing Google Test. Therefore there's no need
    // for calling testing::InitGoogleTest() separately.
    //
    //testing::InitGoogleTest(&argc, argv);
    testing::InitGoogleMock(&argc, argv);

    std::srand((unsigned)std::time(0));

    return RUN_ALL_TESTS();
}
