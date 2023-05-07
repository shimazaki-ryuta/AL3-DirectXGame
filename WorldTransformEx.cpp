#include "WorldTransform.h"
#include"MatrxFunction.h"
#include"VectorFunction.h"
void WorldTransform::UpdateMatrix()
{
	matWorld_ = MakeAffineMatrix(scale_,rotation_,translation_);

	TransferMatrix();
}