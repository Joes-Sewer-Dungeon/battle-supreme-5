$modelname      "characters\lazlo.mdl"
$cdmaterials    "models\characters\lazlo"

$sequence bindPose "lazlo_lowpoly.dmx"

//start eye/face data
$eyeposition 0 0 70

//head controllers
$attachment "eyes" "ValveBiped.Bip01_Head1" -0.00 -1.69 68.1833 absolute
$attachment "mouth" "ValveBiped.Bip01_Head1" 0.80 -5.80 -0.15 rotate 0 -80 -90

$model body "lazlo_lowpoly.dmx" {
    eyeball righteye "ValveBiped.Bip01_Head1" -1.31201 -1.69 68.1833 "lazlo_eye_r" 1.17258924 4.8 "lazlo_pupil_r" 0.63
	eyeball lefteye "ValveBiped.Bip01_Head1" 1.30361 -1.69 68.1833 "lazlo_eye_l" 1.17258924 -4.8 "lazlo_pupil_l" 0.63

	$include "../../humans_sdk/bodyrules_xsi.qci"
}
//end eye/face data	

$surfaceprop "flesh"
$bbox -13 -13 0  13 13 72
$mostlyopaque
//$include "../../humans_sdk/standardhierarchy.qci"
//$include "../../humans_sdk/standardikchains.qci"
//$alwayscollapse "blender_implicit"
//$proceduralbones "../../humans_sdk/male.vrd"
//$include "../../humans_sdk/male/commonbones.qci"

//$includemodel player/male_anims.mdl
