/***************************************************************/
/*             SCALAR VARIABLE MATRIX FORMER                   */
/***************************************************************/

#include "slvFunctions.h"

void ScalarVarMatrixFormer(double** &ScalaraWest,double** &ScalaraEast,
double** &ScalaraSouth,double** &ScalaraNorth, double** &ScalaraCenter,
double** &ScalaraSource,int NumI,int NumJ,double** ScalarXConvFlux,
double** ScalarYConvFlux,
double** YjicoorCorners,double** XjicoorCorners,
double** YJIcoorCenter,double** XJIcoorCenter,
double** YjIcoorFrontFaces,double** XJicoorSideFaces,
double** ScalarXDiffCond,double** ScalarYDiffCond,
double** ScalarVar,double ThermalConduct,int DiffSchm,
double** TimeScaRelaxPar,double** BoundaryLeft,double** BoundaryRight,double** BoundaryTop,
double** BoundaryBottom)
{
   double** BndryCntrSrc;
   BndryCntrSrc = MemoryAllocater2D(NumJ,NumI);
   double** BndryRHSSrc;
   BndryRHSSrc = MemoryAllocater2D(NumJ,NumI);

   for(int I=1;I<NumI-1;I++)
{
      //if (BoundaryBottom[i][0]==1)
         {
		   BndryCntrSrc[1][I]=
 		   -1 * ThermalConduct /(YJIcoorCenter[1][I]-YJIcoorCenter[0][I])
		   * (XjicoorCorners[0][I]-XjicoorCorners[0][I-1]);

         BndryRHSSrc[1][I] =
		   ThermalConduct /(YJIcoorCenter[1][I]-YjIcoorFrontFaces[0][I])
		   * (XjicoorCorners[0][I]-XjicoorCorners[0][I-1])* 5;

         /*
         BndryRHSSrc[1][I] =
		   ThermalConduct /(YJIcoorCenter[1][I]-YjIcoorFrontFaces[0][I])
		   * (XjicoorCorners[0][I]-XjicoorCorners[0][I-1])* BoundaryBottom[I][1];
         */
         }

    //  if (BoundaryTop[i][0]==1)
         {
		   BndryCntrSrc[NumJ-2][I] =
		   -1 * ThermalConduct /(YJIcoorCenter[NumJ-1][I]-YJIcoorCenter[NumJ-2][I])
         * (XjicoorCorners[NumJ-2][I]-XjicoorCorners[NumJ-2][I-1]);

         BndryRHSSrc[NumJ-2][I] =
		   ThermalConduct /(YjIcoorFrontFaces[NumJ-2][I]-YJIcoorCenter[NumJ-2][I])
		   * (XjicoorCorners[NumJ-2][I]-XjicoorCorners[NumJ-2][I-1])* 0;

         /*
		   BndryRHSSrc[NumJ-2][I] =
		   ThermalConduct /(YjIcoorFrontFaces[NumJ-2][I]-YJIcoorCenter[NumJ-2][I])
		   * (XjicoorCorners[NumJ-2][I]-XjicoorCorners[NumJ-2][I-1])* BoundaryTop[I][1];
         */
         }
		}

    for(int J=1;J<NumJ-1;J++)
      {
      //if (BoundaryLeft[j][0]==1)
         {
         BndryCntrSrc[J][1] =
         -1 * ThermalConduct /(XJIcoorCenter[J][1]-XJIcoorCenter[J][0])
         * (YjicoorCorners[J][0]-YjicoorCorners[J-1][0]);

         BndryRHSSrc[J][1]=
         ThermalConduct / (XJIcoorCenter[J][1]-XJicoorSideFaces[J][0])
         * (YjicoorCorners[J][0]-YjicoorCorners[J-1][0])* 0 ;//BoundaryLeft[J][1];
         }
      //if (BoundaryRight[j][0]==1)
         {
     //   BndryCntrSrc[J][NumI-2] =
     //    -1 * ThermalConduct /(XJIcoorCenter[J][NumI-1]-XJIcoorCenter[J][NumI-2])
     //   * (YjicoorCorners[J][NumI-2]-YjicoorCorners[J-1][NumI-2]);

     //    BndryRHSSrc[J][NumI-2] =
     //    ThermalConduct / (XJicoorSideFaces[J][NumI-2]-XJIcoorCenter[J][NumI-2])
     //    * (YjicoorCorners[J][NumI-2]-YjicoorCorners[J-1][NumI-2])* 0; //BoundaryRight[J][1];
         }
      } 

//if (DiffSchm == 1)
		{
 		for(int J=1;J<NumJ-1;J++)
 			{
  			for(int I=1;I<NumI-1;I++)
  				{
            ScalaraWest[J][I] = (ScalarXDiffCond[J][I-1] + 0.5*ScalarXConvFlux[J][I-1]) *
  				(YjicoorCorners[J][I-1]-YjicoorCorners[J-1][I-1]);

  				ScalaraEast[J][I] = (ScalarXDiffCond[J][I] - 0.5*ScalarXConvFlux[J][I]) *
  				(YjicoorCorners[J][I]-YjicoorCorners[J-1][I]);

  				ScalaraSouth[J][I] = (ScalarYDiffCond[J-1][I] + 0.5*ScalarYConvFlux[J-1][I]) *
  				(XjicoorCorners[J-1][I]-XjicoorCorners[J-1][I-1]);

  				ScalaraNorth[J][I] = (ScalarYDiffCond[J][I] - 0.5*ScalarYConvFlux[J][I]) *
  				(XjicoorCorners[J][I]-XjicoorCorners[J][I-1]);

  				ScalaraCenter[J][I] =
  				(ScalaraWest[J][I]+ScalaraEast[J][I]+ScalaraSouth[J][I]+ScalaraNorth[J][I]+
  				ScalarXConvFlux[J][I]*(YjicoorCorners[J][I]-YjicoorCorners[J-1][I])-
  				ScalarXConvFlux[J][I-1]*(YjicoorCorners[J][I-1]-YjicoorCorners[J-1][I-1])+
  				ScalarYConvFlux[J][I]*(XjicoorCorners[J][I]-XjicoorCorners[J][I-1])-
  				ScalarYConvFlux[J-1][I]*(XjicoorCorners[J-1][I]-XjicoorCorners[J-1][I-1])
  				-BndryCntrSrc[J][I] ) / TimeScaRelaxPar[J][I];

  				ScalaraSource[J][I] = BndryRHSSrc[J][I] +
  				(1-TimeScaRelaxPar[J][I])* ScalaraCenter[J][I] * ScalarVar[J][I] ;
  				}
 			}
      }

// OUTLET TEMPERATURE BOUNDARY CONDTION PART [2/2]//
for(int J=1;J<NumJ-1;J++)
   {
  // if (BoundaryRight[J][0]==3)
      {
      ScalaraCenter[J][NumI-2] = ScalaraCenter[J][NumI-2] - ScalaraEast[J][NumI-2];
      ScalaraEast[J][NumI-2]=0;
      }
   //if (BoundaryLeft[J][0]==3)
      {
     // UmomaCenter[J][1] = UmomaCenter[J][1] - UmomaWest[J][1];
     // UmomaWest[J][1]=0;
      }
   }

MemoryDeAllocater2D(BndryCntrSrc,NumJ,NumI);
MemoryDeAllocater2D(BndryRHSSrc,NumJ,NumI);

}

