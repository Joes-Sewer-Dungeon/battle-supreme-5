#include "BaseVSShader.h"
#include "source2skin_dx9_helper.h"


BEGIN_VS_SHADER( Source2Skin, "Help for Source2Skin" )
	BEGIN_SHADER_PARAMS
		SHADER_PARAM( ENVMAP, SHADER_PARAM_TYPE_TEXTURE, "shadertest/shadertest_env", "envmap" )
		SHADER_PARAM( ENVMAPFRAME, SHADER_PARAM_TYPE_INTEGER, "0", "envmap frame number" )
		SHADER_PARAM( BUMPMAP, SHADER_PARAM_TYPE_TEXTURE, "models/shadertest/shader1_normal", "bump map" )
		SHADER_PARAM( BUMPFRAME, SHADER_PARAM_TYPE_INTEGER, "0", "frame number for $bumpmap" )
		SHADER_PARAM( DIFFUSEWARP, SHADER_PARAM_TYPE_TEXTURE, "", "diffuse warp texture" )
		SHADER_PARAM( AMBIENTOCCLUSION, SHADER_PARAM_TYPE_TEXTURE, "", "AO/curvature texture" )
		SHADER_PARAM( METALLIC, SHADER_PARAM_TYPE_BOOL, "0", "Use metalness" )
	END_SHADER_PARAMS

	void SetupVars( Source2Skin_DX9_Vars_t &info )
	{
		info.m_nBaseTexture = BASETEXTURE;
		info.m_nBaseTextureFrame = FRAME;
		info.m_nBaseTextureTransform = BASETEXTURETRANSFORM;
		info.m_nEnvmap = ENVMAP;
		info.m_nEnvmapFrame = ENVMAPFRAME;
		info.m_nBumpmap = BUMPMAP;
		info.m_nBumpFrame = BUMPFRAME;

		info.m_nDiffuseWarp = DIFFUSEWARP;
		info.m_nAmbientOcclusion = AMBIENTOCCLUSION;
		info.m_nMetallic = METALLIC;
	}

	SHADER_INIT_PARAMS()
	{
		Source2Skin_DX9_Vars_t vars;
		SetupVars( vars );
		InitParamsSource2Skin_DX9( this, params, pMaterialName, vars );
	}

	SHADER_INIT
	{
		Source2Skin_DX9_Vars_t vars;
		SetupVars( vars );
		InitSource2Skin_DX9( this, params, vars );
	}

	SHADER_DRAW
	{
		Source2Skin_DX9_Vars_t vars;
		SetupVars( vars );
		DrawSource2Skin_DX9( this, params, pShaderAPI, pShaderShadow, vars, vertexCompression, pContextDataPtr );
	}
END_SHADER