/******************************************************************************
	created:	2008-12-15   1:58
	file path:	d:\EFF\EFFEngine\Src\EFF3D\EFF3DRenderQueueGroupSolid.cpp
	author:		ChenA
	
	purpose:	
******************************************************************************/

#include "stdafx.h"
#include "EFF3DRenderQueueGroupSolid.h"
#include "EFF3DIRenderable.h"
#include "EFF3DMaterial.h"
#include "EFF3DEffect.h"
#include "EFF3DAutoParamDataSource.h"

#define new EFFNEW

EFF3D_BEGIN

EFF3DRenderQueueGroupSolid::EFF3DRenderQueueGroupSolid()
{
}

EFF3DRenderQueueGroupSolid::~EFF3DRenderQueueGroupSolid()
{
}

effVOID EFF3DRenderQueueGroupSolid::AddRenderable(EFF3DIRenderable * pRenderable)
{
	std::map<EFF3DMaterial *,std::vector<EFF3DIRenderable *>>::iterator it = m_mapRenderable.find(pRenderable->GetMaterial());

	if ( it != m_mapRenderable.end() )
	{
		it->second.push_back(pRenderable);
	}

	std::vector<EFF3DIRenderable *> aryRenderable;
	aryRenderable.push_back(pRenderable);
	m_mapRenderable[pRenderable->GetMaterial()] = aryRenderable;

}

effVOID EFF3DRenderQueueGroupSolid::RemoveRenderable(EFF3DIRenderable * pRenderable)
{
	std::map<EFF3DMaterial *,std::vector<EFF3DIRenderable *>>::iterator it = m_mapRenderable.find(pRenderable->GetMaterial());

	if ( it != m_mapRenderable.end() )
	{
		//因为RemoveRenderable不是经常发生，所以这里可以用遍历数组的方法删除
		std::vector<EFF3DIRenderable *> & aryRenderable = it->second;
		std::vector<EFF3DIRenderable *>::iterator itRenderable = aryRenderable.begin();
		for ( ; itRenderable != aryRenderable.end(); itRenderable++ )
		{
			if ( *itRenderable == pRenderable )
			{
				aryRenderable.erase(itRenderable);
				break;
			}
		}
	}
	else
	{
		//打印警告信息
	}
}

effVOID EFF3DRenderQueueGroupSolid::Render(EFF3DDevice * pDevice,EFF3DAutoParamDataSource * pDataSource)
{
	EFF3DIEntity * pLastEntity = NULL;
	std::map<EFF3DMaterial *,std::vector<EFF3DIRenderable *>>::iterator it,itend;
	itend = m_mapRenderable.end();
	for ( it = m_mapRenderable.begin(); it != itend; it++ )
	{
		EFF3DMaterial * pMaterial = it->first;
		std::vector<EFF3DIRenderable *> & aryRenderable = it->second;

		if ( aryRenderable.size() != 0 )
		{


			if ( pMaterial != NULL )
			{
				//pMaterial->OnMaterialBeforeRender(pMaterial,pDevice);
			}

			EFF3DEffect * pEffect = pMaterial->GetEffect();

			if ( pEffect != NULL )
			{
				EFF3DIRenderable * pRenderable = aryRenderable[0];
				//pEffect->OnEffectBeforeRender(pEffect,pRenderable);

				pEffect->UpdateAutoParametersPerEffect(pDevice,pDataSource);

				effUINT uiPass;
				pEffect->Begin(&uiPass,0);

				for ( effUINT i = 0; i < uiPass; i++ )
				{
					pEffect->BeginPass(i);

					for ( effUINT j = 0; j < aryRenderable.size(); j++ )
					{
						EFF3DIRenderable * pRenderable = aryRenderable[j];
						//pMaterial->OnRenderableBeforeRender(pMaterial,pEffect,pRenderable);
						EFF3DIEntity * pParentEntity = pRenderable->GetParentEntity();
						if ( pParentEntity != NULL && pParentEntity != pLastEntity )
						{
							pDataSource->SetRenderable(pRenderable);					
							pEffect->UpdateAutoParametersPerEntity(pDevice,pDataSource);
							pEffect->CommitChanges();
							pLastEntity = pParentEntity;
						}
						pRenderable->Render(pDevice);
						//pMaterial->OnRenderableEndRender(pMaterial,pRenderable);
					}
					pEffect->EndPass();
				}
				pEffect->End();

			}
			else
			{
				for ( effUINT i = 0; i < aryRenderable.size(); i++ )
				{
					EFF3DIRenderable * pRenderable = aryRenderable[i];
					pRenderable->Render(pDevice);
				}
			}

			if ( pMaterial != NULL )
			{
				//pMaterial->OnMaterialEndRender(pMaterial,pDevice);
			}
		}
	}

}

EFF3D_END