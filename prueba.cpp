#include <iostream>
#include <vector>
#include "CImg.h"
using namespace cimg_library;
using namespace std;


vector<float>  Vectorizar(CImg<float>   & img )
{
         vector<float> R;
         cimg_forXY(img,x,y) 
          { 
              R.push_back( (img(x,y,0) + img(x,y,1) +  img(x,y,2))/3);
          }
         return R;
}


int main()
{
   
   CImg<float>   A("cara.jpeg");
   CImg<float>   B =  A.haar(false,3);
   CImg<float>   c  = B.crop(0,0,27,27);

   vector vc = Vectorizar(c);

   A.display();
   B.display();
   c.display();

   
   
   return 1;
}


