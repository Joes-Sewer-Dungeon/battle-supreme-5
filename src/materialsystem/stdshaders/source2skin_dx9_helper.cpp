#include "BaseVSShader.h"
#include "source2skin_dx9_helper.h"

#include "source2skin_ps20b.inc"
#include "source2skin_vs20.inc"
#include "source2skin_ps30.inc"
#include "source2skin_vs30.inc"

#include "commandbuilder.h"
#include "convar.h"
#include "cpp_shader_constant_register_map.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static ConVar mat_fullbright( "mat_fullbright", "0", FCVAR_CHEAT );


//-----------------------------------------------------------------------------
// Initialize shader parameters
//-----------------------------------------------------------------------------
void InitParamsSource2Skin_DX9( CBaseVSShader *pShader, IMaterialVar** params,
								const char *pMaterialName,
								Source2Skin_DX9_Vars_t &info )
{
	InitIntParam( info.m_nMetallic, params, 0 );

	SET_FLAGS2( MATERIAL_VAR2_SUPPORTS_HW_SKINNING );
	SET_FLAGS2( MATERIAL_VAR2_LIGHTING_VERTEX_LIT );

	bool bBump = ( info.m_nBumpmap != -1 ) && g_pConfig->UseBumpmapping() && params[info.m_nBumpmap]->IsDefined();
	bool bEnvMap = ( info.m_nEnvmap != -1 ) && params[info.m_nEnvmap]->IsDefined();
	if ( bBump || bEnvMap )
	{
		SET_FLAGS2( MATERIAL_VAR2_NEEDS_TANGENT_SPACES );
	}
}

//-----------------------------------------------------------------------------
// Initialize shader
//-----------------------------------------------------------------------------
void InitSource2Skin_DX9( CBaseVSShader *pShader, IMaterialVar** params, 
						  Source2Skin_DX9_Vars_t &info )
{
	if ( params[info.m_nBaseTexture]->IsDefined() )
	{
		pShader->LoadTexture( info.m_nBaseTexture, TEXTUREFLAGS_SRGB );
	}

	if ( g_pConfig->UseBumpmapping() )
	{
		if ( ( info.m_nBumpmap != -1 ) && params[info.m_nBumpmap]->IsDefined() )
		{
			pShader->LoadBumpMap( info.m_nBumpmap );
			SET_FLAGS2( MATERIAL_VAR2_DIFFUSE_BUMPMAPPED_MODEL );
		}
	}

	if ( params[info.m_nEnvmap]->IsDefined() )
	{
		pShader->LoadCubeMap( info.m_nEnvmap, g_pHardwareConfig->GetHDRType() == HDR_TYPE_NONE ? TEXTUREFLAGS_SRGB : 0 );
	}

	if ( ( info.m_nDiffuseWarp != -1 ) && params[info.m_nDiffuseWarp]->IsDefined() )
	{
		pShader->LoadTexture( info.m_nDiffuseWarp );
	}

	if ( ( info.m_nAmbientOcclusion != -1 ) && params[info.m_nAmbientOcclusion]->IsDefined() )
	{
		pShader->LoadTexture( info.m_nAmbientOcclusion );
	}
}


//-----------------------------------------------------------------------------
// Draws the shader
//-----------------------------------------------------------------------------
void DrawSource2Skin_DX9_Internal( CBaseVSShader *pShader, IMaterialVar** params, 
								   IShaderDynamicAPI *pShaderAPI, 
								   IShaderShadow* pShaderShadow,
								   Source2Skin_DX9_Vars_t &info,
								   VertexCompressionType_t vertexCompression,
								   CBasePerMaterialContextData **pContextDataPtr )
{
	bool bMetallic = (info.m_nMetallic != -1) && params[info.m_nMetallic]->GetIntValue() != 0;

	bool bHasBaseTexture = IsTextureSet( info.m_nBaseTexture, params );
	bool bHasBump = IsTextureSet( info.m_nBumpmap, params );
	bool bHasEnvmap = IsTextureSet( info.m_nEnvmap, params );
	bool bHasDiffuseWarp = IsTextureSet( info.m_nDiffuseWarp, params );
	bool bHasAO = IsTextureSet( info.m_nAmbientOcclusion, params );

	if ( pShader->IsSnapshotting() )
	{
		pShaderShadow->EnableTexture( SHADER_SAMPLER0, true );
		pShaderShadow->EnableSRGBRead( SHADER_SAMPLER0, true );

		pShaderShadow->EnableTexture( SHADER_SAMPLER1, true );

		pShaderShadow->EnableTexture( SHADER_SAMPLER2, true );
		pShaderShadow->EnableSRGBRead( SHADER_SAMPLER2, true );

		pShaderShadow->EnableTexture( SHADER_SAMPLER3, true );
		pShaderShadow->EnableSRGBRead( SHADER_SAMPLER3, false );

		if ( bHasEnvmap )
		{
			pShaderShadow->EnableTexture( SHADER_SAMPLER4, true );
			if( g_pHardwareConfig->GetHDRType() == HDR_TYPE_NONE )
			{
				pShaderShadow->EnableSRGBRead( SHADER_SAMPLER4, true );
			}
		}

		pShaderShadow->EnableSRGBWrite( true );

		// Vertex format
		unsigned int flags = VERTEX_POSITION | VERTEX_NORMAL | VERTEX_FORMAT_COMPRESSED;
		int userDataSize = 4;

		int pTexCoordDim[1] = { 2 };
		int nTexCoordCount = 1;

		pShaderShadow->VertexShaderVertexFormat( flags, nTexCoordCount, pTexCoordDim, userDataSize );

		if ( g_pHardwareConfig->SupportsShaderModel_3_0() )
		{
			DECLARE_STATIC_VERTEX_SHADER( source2skin_vs30 );
			SET_STATIC_VERTEX_SHADER( source2skin_vs30 );

			DECLARE_STATIC_PIXEL_SHADER( source2skin_ps30 );
			SET_STATIC_PIXEL_SHADER_COMBO( METALLIC, bMetallic );
			SET_STATIC_PIXEL_SHADER( source2skin_ps30 );
		}
		else
		{
			DECLARE_STATIC_VERTEX_SHADER( source2skin_vs20 );
			SET_STATIC_VERTEX_SHADER( source2skin_vs20 );

			DECLARE_STATIC_PIXEL_SHADER( source2skin_ps20b );
			SET_STATIC_PIXEL_SHADER_COMBO( METALLIC, bMetallic );
			SET_STATIC_PIXEL_SHADER( source2skin_ps20b );
		}

		pShader->DefaultFog();

		pShaderShadow->EnableAlphaWrites( true );
	}
	if ( pShaderAPI )
	{

		if ( bHasBaseTexture )
		{
			pShader->BindTexture( SHADER_SAMPLER0, info.m_nBaseTexture, info.m_nBaseTextureFrame );
		}
		else
		{
			if ( bHasEnvmap )
			{
				pShaderAPI->BindStandardTexture( SHADER_SAMPLER0, TEXTURE_BLACK );
			}
			else
			{
				pShaderAPI->BindStandardTexture( SHADER_SAMPLER0, TEXTURE_WHITE );
			}
		}

		if ( !g_pConfig->m_bFastNoBump )
		{
			if ( bHasBump )
			{
				pShader->BindTexture( SHADER_SAMPLER1, info.m_nBumpmap, info.m_nBumpFrame );
			}
			else
			{
				pShaderAPI->BindStandardTexture( SHADER_SAMPLER1, TEXTURE_NORMALMAP_FLAT );
			}
		}
		else
		{
			if ( bHasBump )
			{
				pShaderAPI->BindStandardTexture( SHADER_SAMPLER1, TEXTURE_NORMALMAP_FLAT );
			}
		}

		if ( bHasDiffuseWarp )
		{
			pShader->BindTexture( SHADER_SAMPLER2, info.m_nDiffuseWarp );
		}
		else
		{
			pShaderAPI->BindStandardTexture( SHADER_SAMPLER2, TEXTURE_GREY );
		}

		if ( bHasAO )
		{
			pShader->BindTexture( SHADER_SAMPLER3, info.m_nAmbientOcclusion );
		}
		else
		{
			pShaderAPI->BindStandardTexture( SHADER_SAMPLER3, TEXTURE_WHITE );
		}

		if ( bHasEnvmap )
		{
			pShader->BindTexture( SHADER_SAMPLER4, info.m_nEnvmap, info.m_nEnvmapFrame );
		}

		if ( info.m_nBaseTextureTransform != -1 )
		{
			pShader->SetVertexShaderTextureTransform( VERTEX_SHADER_SHADER_SPECIFIC_CONST_0, info.m_nBaseTextureTransform );
		}

		pShaderAPI->SetPixelShaderFogParams( PSREG_FOG_PARAMS );

		LightState_t lightState = { 0, false, false };
		pShaderAPI->GetDX9LightState(&lightState);

		MaterialFogMode_t fogType = pShaderAPI->GetSceneFogMode();
		int fogIndex = ( fogType == MATERIAL_FOG_LINEAR_BELOW_FOG_Z ) ? 1 : 0;
		int numBones = pShaderAPI->GetCurrentNumBones();

		bool bWriteDepthToAlpha = pShaderAPI->ShouldWriteDepthToDestAlpha();
		bool bWriteWaterFogToAlpha = ( fogType == MATERIAL_FOG_LINEAR_BELOW_FOG_Z );

		if ( g_pHardwareConfig->SupportsShaderModel_3_0() )
		{
			DECLARE_DYNAMIC_VERTEX_SHADER( source2skin_vs30 );
			SET_DYNAMIC_VERTEX_SHADER_COMBO( DOWATERFOG,  fogIndex );
			SET_DYNAMIC_VERTEX_SHADER_COMBO( SKINNING,  numBones > 0 );
			SET_DYNAMIC_VERTEX_SHADER_COMBO( COMPRESSED_VERTS, (int)vertexCompression );
			SET_DYNAMIC_VERTEX_SHADER( source2skin_vs30 );

			DECLARE_DYNAMIC_PIXEL_SHADER( source2skin_ps30 );
			SET_DYNAMIC_PIXEL_SHADER_COMBO( WRITEWATERFOGTODESTALPHA, bWriteWaterFogToAlpha );
			SET_DYNAMIC_PIXEL_SHADER_COMBO( PIXELFOGTYPE, pShaderAPI->GetPixelFogCombo() );
			SET_DYNAMIC_PIXEL_SHADER_COMBO( WRITE_DEPTH_TO_DESTALPHA, bWriteDepthToAlpha );
			SET_DYNAMIC_PIXEL_SHADER_COMBO( NUM_LIGHTS,  lightState.m_nNumLights );
			SET_DYNAMIC_PIXEL_SHADER( source2skin_ps30 );
		}
		else
		{
			DECLARE_DYNAMIC_VERTEX_SHADER( source2skin_vs20 );
			SET_DYNAMIC_VERTEX_SHADER_COMBO( COMPRESSED_VERTS, (int)vertexCompression );
			SET_DYNAMIC_VERTEX_SHADER_COMBO( DOWATERFOG,  fogIndex );
			SET_DYNAMIC_VERTEX_SHADER_COMBO( SKINNING,  numBones > 0 );
			SET_DYNAMIC_VERTEX_SHADER( source2skin_vs20 );

			DECLARE_DYNAMIC_PIXEL_SHADER( source2skin_ps20b );
			SET_DYNAMIC_PIXEL_SHADER_COMBO( WRITEWATERFOGTODESTALPHA, bWriteWaterFogToAlpha );
			SET_DYNAMIC_PIXEL_SHADER_COMBO( PIXELFOGTYPE, pShaderAPI->GetPixelFogCombo() );
			SET_DYNAMIC_PIXEL_SHADER_COMBO( WRITE_DEPTH_TO_DESTALPHA, bWriteDepthToAlpha );
			SET_DYNAMIC_PIXEL_SHADER_COMBO( NUM_LIGHTS,  lightState.m_nNumLights );
			SET_DYNAMIC_PIXEL_SHADER( source2skin_ps20b );
		}

		pShaderAPI->SetPixelShaderStateAmbientLightCube( PSREG_AMBIENT_CUBE, !lightState.m_bAmbientLight );
		pShaderAPI->CommitPixelShaderLighting( PSREG_LIGHT_INFO_ARRAY );

		float vEyePos[4];
		pShaderAPI->GetWorldSpaceCameraPosition( vEyePos );
		pShaderAPI->SetPixelShaderConstant( PSREG_EYEPOS_SPEC_EXPONENT, vEyePos, 1 );
	}

	pShader->Draw();
}


//-----------------------------------------------------------------------------
// Draws the shader
//-----------------------------------------------------------------------------
void DrawSource2Skin_DX9( CBaseVSShader *pShader, IMaterialVar** params, 
						  IShaderDynamicAPI *pShaderAPI,
						  IShaderShadow* pShaderShadow,
						  Source2Skin_DX9_Vars_t &info,
						  VertexCompressionType_t vertexCompression,
						  CBasePerMaterialContextData **pContextDataPtr )
{
	DrawSource2Skin_DX9_Internal( pShader, params, pShaderAPI, pShaderShadow, 
		info, vertexCompression, pContextDataPtr );
}