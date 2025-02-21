#ifndef SOURCE2SKIN_DX9_HELPER_H
#define SOURCE2SKIN_DX9_HELPER_H

#include <string.h>


//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
class CBaseVSShader;
class IMaterialVar;
class IShaderDynamicAPI;
class IShaderShadow;


//-----------------------------------------------------------------------------
// Init params/ init/ draw methods
//-----------------------------------------------------------------------------
struct Source2Skin_DX9_Vars_t
{
	Source2Skin_DX9_Vars_t() { memset( this, 0xFF, sizeof(*this) ); }

	int m_nBaseTexture;
	int m_nBaseTextureFrame;
	int m_nBaseTextureTransform;
	int m_nEnvmap;
	int m_nEnvmapFrame;
	int m_nBumpmap;
	int m_nBumpFrame;
	int m_nDiffuseWarp;
	int m_nAmbientOcclusion;
	int m_nMetallic;
};

void InitParamsSource2Skin_DX9( CBaseVSShader *pShader, IMaterialVar** params, const char *pMaterialName, Source2Skin_DX9_Vars_t &info );
void InitSource2Skin_DX9( CBaseVSShader *pShader, IMaterialVar** params, Source2Skin_DX9_Vars_t &info );
void DrawSource2Skin_DX9( CBaseVSShader *pShader, IMaterialVar** params, IShaderDynamicAPI *pShaderAPI, IShaderShadow* pShaderShadow,
						  Source2Skin_DX9_Vars_t &info, VertexCompressionType_t vertexCompression,
						  CBasePerMaterialContextData **pContextDataPtr );


#endif // SOURCE2SKIN_DX9_HELPER_H