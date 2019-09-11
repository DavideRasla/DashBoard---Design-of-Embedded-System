#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>

#include "types.h"
#include "io.h"
#include "Event.h"
#include "config.h"



/* --------------------------------------------------------------------------------
 *                              FSM Conformance Test
 * --------------------------------------------------------------------------------
 */

void Throttle_test_state_POS()
{
    CU_ASSERT_EQUAL(Throttle_Read(2000), 130);
    CU_ASSERT_EQUAL(Throttle_Read(4000),130);
}
void Throttle_test_state_NEG()
{
    CU_ASSERT_EQUAL(Throttle_Read(0), 0);
    CU_ASSERT_EQUAL(Throttle_Read(1999), -150);
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

void Fuel_Set_state(){
     CU_ASSERT_EQUAL(UpdateFuel(-1), -1);
     CU_ASSERT_EQUAL(UpdateFuel(RED_FUEL-1), 1);
     CU_ASSERT_EQUAL(UpdateFuel(RED_FUEL), 2);
     CU_ASSERT_EQUAL(UpdateFuel(YELLOW_FUEL + 1), 3);

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

   
    // Test on the FSM

    CU_pSuite FUEL_conformance = CU_add_suite("UpdateFuel Conformance Test", NULL, NULL);

    CU_add_test(FUEL_conformance, "Fuel setting conformance", Fuel_Set_state);
   
     
 

    // Running all the tests
    CU_set_output_filename("res-unit/DashBoard_Sensors-Test");
    CU_automated_run_tests();
    CU_list_tests_to_file();

    CU_cleanup_registry();

    return EXIT_SUCCESS;
}