#include "Algorithm/Density.h"
#include "Algorithm/Distance.h"

namespace algorithm
{

Density::Density()
	: densityFunc(new DensityDistanceFunction3By3) ,
	  densityPerHit()
{

}

Density::~Density()
{
	delete densityFunc ;
}

float Density::compute(HitVec& hitVec)
{
	densityPerHit.clear() ;

	for( HitVec::const_iterator it = hitVec.begin() ; it != hitVec.end() ; ++it )
	{
		for( HitVec::const_iterator jt = it ; jt != hitVec.end() ; ++jt )
		{
			float d = densityFunc->eval(*it , *jt) ;
			densityPerHit[*it] += d ;

			if ( it != jt )
				densityPerHit[*jt] += d ;
		}

	}

	float density = 0 ;
	for ( std::map<caloobject::CaloHit* , float>::const_iterator it = densityPerHit.begin() ; it != densityPerHit.end() ; ++it )
		density += it->second ;

	density /= densityPerHit.size() ;
	return density ;
}

float Density::compute2(HitVec& hitVec)
{
	densityPerHit2.clear() ;

	for( HitVec::const_iterator it = hitVec.begin() ; it != hitVec.end() ; ++it )
	{
		for( HitVec::const_iterator jt = it ; jt != hitVec.end() ; ++jt )
		{
			float d = densityFunc->eval2(*it , *jt) ;
			densityPerHit2[*it] += d * (*jt)->getEnergy() ;

			if ( it != jt )
				densityPerHit2[*jt] += d * (*it)->getEnergy() ;
		}

	}

	float density = 0 ;
	for ( std::map<caloobject::CaloHit* , float>::const_iterator it = densityPerHit2.begin() ; it != densityPerHit2.end() ; ++it )
		density += it->second ;

	density /= densityPerHit2.size() ;
	return density ;
}

DensityDistanceFunction::DensityDistanceFunction()
{}

DensityDistanceFunction::~DensityDistanceFunction()
{}

DensityDistanceFunction3By3::DensityDistanceFunction3By3()
	: DensityDistanceFunction()
{}

DensityDistanceFunction3By3::~DensityDistanceFunction3By3()
{}

float DensityDistanceFunction3By3::eval(caloobject::CaloHit* hitA , caloobject::CaloHit* hitB) const
{
	if ( std::abs(hitA->getCellID()[0] - hitB->getCellID()[0]) <= 1 && std::abs(hitA->getCellID()[1] - hitB->getCellID()[1]) <= 1 && hitA->getCellID()[2] == hitB->getCellID()[2]
		 && Distance<caloobject::CaloHit,caloobject::CaloHit>().getDistance(hitA,hitB)<20 )
		return 1.0f ;
	else
		return 0.0f ;
}

float DensityDistanceFunction3By3::eval2(caloobject::CaloHit* hitA , caloobject::CaloHit* hitB) const
{
	if ( std::abs(hitA->getCellID()[0] - hitB->getCellID()[0]) <= 2 && std::abs(hitA->getCellID()[1] - hitB->getCellID()[1]) <= 2 && hitA->getCellID()[2] == hitB->getCellID()[2]
		 && Distance<caloobject::CaloHit,caloobject::CaloHit>().getDistance(hitA,hitB)<100 )
		return 1.0f ;
	else
		return 0.0f ;
}



} //namespace algorithm


