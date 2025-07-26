#include <gtest/gtest.h>
#include <Onyx.h>

TEST(Onyx_Debugging, GetOne) {
    int one = Onyx::GetOne(); 
    EXPECT_EQ(one, 1); 
}

/**
 * @brief Onyx Initialization / Termination wrapper. 
 */
class Framework {
public:
    Framework() {
        Onyx::Initialize();
    }
    ~Framework() {
        Onyx::Shutdown();
    }
}; 


int main(int argc, char** argv) {
    Framework framework;    //RAII Onyx Wrapper, guaranteed to call Onyx::Shutdown() on termination.

    //Initialize and run our tests. 
    testing::InitGoogleTest(&argc, argv); 

    return RUN_ALL_TESTS(); 
}
