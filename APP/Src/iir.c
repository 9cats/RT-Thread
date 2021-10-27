#include "main.h"
#include "arm_math.h"

#define numStages 2 /* IIR滤波的个数 */

arm_biquad_casd_df1_inst_f32 iir_S;

float32_t IIRCoeffs32LP[5*numStages] = { /* 巴特沃斯低通滤波器系数 */

		1  ,2  ,1  ,  1.968877497385757902037539679440669715405  ,-0.970396601757115173647605388396186754107,
		1  ,2  ,1  ,  1.928508485082634393847911269403994083405  ,-0.929996442395254585555619541992200538516
//		1  ,2  ,1  ,  1.989861099912941577727565345412585884333  ,-0.990031590747123435569676530576543882489,
//		1  ,2  ,1  ,  1.975933280157109894759059898206032812595  ,-0.976102577658948922234571909939404577017
};
float32_t IIRStateF32[4*numStages];
//float32_t ScaleValue = 0.000042622708545487689559341792921998149  * 0.000042324375459749760577509575254495644  ;
float32_t ScaleValue =0.00037977609283935492802061761885568103 *  0.000371989328155101811775040676621983948    ;
void arm_iir_init(void)
{
	arm_biquad_cascade_df1_init_f32(&iir_S, numStages, IIRCoeffs32LP, IIRStateF32);
}
