// This is tbl/vepl/tests/vepl_test_y_gradient.cxx
#include <iostream>
#include <string>
#include "test_driver.h"
//:
// \file
//  Test of the vepl_y_gradient function.
//
// \author Peter Vanroose, K.U.Leuven, ESAT/PSI
// \date   7 October 2002, from vepl1/tests

#include <vepl/vepl_y_gradient.h>
#include <vcl_compiler.h>
#include <vxl_config.h> // for vxl_byte

int vepl_test_y_gradient()
{
  std::cout << "Starting vepl_y_gradient tests\n"
           << "Creating test and output images ...";
  vil_image_resource_sptr byte_img = CreateTest8bitImage(32,32),  byte_ori = CreateTest8bitImage(32,32);
  vil_image_resource_sptr shrt_img = CreateTest16bitImage(32,32), shrt_ori = CreateTest16bitImage(32,32);
  vil_image_resource_sptr int__img = CreateTest32bitImage(32,32), int__ori = CreateTest32bitImage(32,32);
  vil_image_resource_sptr flot_img = CreateTestfloatImage(32,32), flot_ori = CreateTestfloatImage(32,32);
  vil_image_resource_sptr dble_img = CreateTestdoubleImage(32,32),dble_ori = CreateTestdoubleImage(32,32);
  vil_image_resource_sptr colr_img = CreateTest24bitImage(32,32), colr_ori = CreateTest24bitImage(32,32);
  vil_image_resource_sptr colp_img = CreateTest3planeImage(32,32),colp_ori = CreateTest3planeImage(32,32);
  std::cout << " done\n";

  std::string m = "vepl_y_gradient";
#define args
  ONE_TEST(vepl_y_gradient,byte_img,byte_ori,vxl_byte,90104,m+"_byte",args);
  ONE_TEST(vepl_y_gradient,shrt_img,shrt_ori,vxl_uint_16,23199224,m+"_short",args);
  ONE_TEST(vepl_y_gradient,int__img,int__ori,vxl_uint_32,8912376,m+"_int",args);
  ONE_TEST(vepl_y_gradient,flot_img,flot_ori,float,282,m+"_float",args);
  ONE_TEST(vepl_y_gradient,dble_img,dble_ori,double,282,m+"_double",args);
  ONE_TEST(vepl_y_gradient,colr_img,colr_ori,vil_rgb<vxl_byte>,108104,m+"_colour",args);
  ONE_TEST(vepl_y_gradient,colp_img,colp_ori,vxl_byte,108104,m+"_planar",args);

  return 0;
}

TESTMAIN(vepl_test_y_gradient);
