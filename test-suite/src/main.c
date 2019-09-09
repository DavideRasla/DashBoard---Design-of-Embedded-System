#include <stdlib.h>
#include <stdio.h>

#include "CUnit.h"
#include "Automated.h"

#include "types.h"
#include "io.h"
#include "Event.h"
#include "Config.h"

/* --------------------------------------------------------------------------------
 *                        Triangolation Functional Testing
 * --------------------------------------------------------------------------------
 */

#define LX_MIN  0
#define LX_MAX  SENSOR_DIST_MAX
#define RX_MIN  0
#define RX_MAX  SENSOR_DIST_MAX

#define LX_NOMINAL ((LX_MIN + LX_MAX) / 2)
#define RX_NOMINAL ((RX_MIN + RX_MAX) / 2)

#define LX_EPSILON  ((LX_MAX - LX_MIN) / 20)
#define RX_EPSILON  ((RX_MAX - RX_MIN) / 20)

#define LX_BOUNDARY_MIN (LX_MIN + LX_EPSILON)
#define LX_BOUNDARY_MAX (LX_MAX - LX_EPSILON)

#define RX_BOUNDARY_MIN (RX_MIN + RX_EPSILON)
#define RX_BOUNDARY_MAX (RX_MAX - RX_EPSILON)

#define LX_ROBUSTNESS_MIN (LX_MIN - LX_EPSILON)
#define LX_ROBUSTNESS_MAX (LX_MAX + LX_EPSILON)

#define RX_ROBUSTNESS_MIN (RX_MIN - RX_EPSILON)
#define RX_ROBUSTNESS_MAX (RX_MAX + RX_EPSILON)

/* ---------------------------------- Original Version ------------------------------------ */



void Throttle_test_state_POS()
{
    CU_ASSERT_EQUAL(Throttle_Read(2000), 130*log(2000));
    CU_ASSERT_EQUAL(Throttle_Read(4000), 130*log(4000));
}
void Throttle_test_state_NEG()
{
    CU_ASSERT_EQUAL(Throttle_Read(2000), 130*log(2000));
    CU_ASSERT_EQUAL(Throttle_Read(4000), 130*log(4000));
}
void Clutch_test_state_POS()
{
    CU_ASSERT_EQUAL(Clutch_Read(2000), 1);
    CU_ASSERT_EQUAL(Clutch_Read(4000), 1);
}
void Clutch_test_state_NEG()
{
    CU_ASSERT_EQUAL(Clutch_Read(1999), 0);
    CU_ASSERT_EQUAL(Clutch_Read(0), 0);
}




/* --------------------------------------------------------------------------------
 *                                    Main
 * --------------------------------------------------------------------------------
 */

int main(int argc, char* argv[])
{
    if(CUE_SUCCESS != CU_initialize_registry())
    {
        printf("Could not initialize CUnit registry!\r\n");
        
        return CU_get_error();
    }

    CU_set_error_action(CUEA_FAIL);    

    // Test on the FSM

    CU_pSuite conformance = CU_add_suite("I/O Conformance Test", NULL, NULL);

    CU_add_test(conformance, "Throttle Conformance Test Positive Accelleration", Throttle_test_state_POS);
    CU_add_test(conformance, "Throttle Conformance Test Negative Accelleration", Throttle_test_state_NEG);
    CU_add_test(conformance, "Throttle Conformance Test Clutch ON", Clutch_test_state_POS);
    CU_add_test(conformance, "Clutch Conformance Test Clutch OFF", Clutch_test_state_NEG);

    
 
  

    // Running all the tests
    CU_set_output_filename("res-unit/DashBoard_Sensors-Test");
    CU_automated_run_tests();
    CU_list_tests_to_file();

    CU_cleanup_registry();

    return EXIT_SUCCESS;
}