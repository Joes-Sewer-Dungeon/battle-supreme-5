$modelname "gibs\Hgibs.mdl"
$cdmaterials models\gibs\Hgibs
$scale 1

$bodygroup parts
{
// leave skull at top!
studio "HGIB_Skull1" scale 2.50
studio "HGIB_HMeat1" scale 4
studio "HGIB_Guts1" scale 2.5
studio "HGIB_Lung1" scale 2.8
studio "HGIB_Legbone1" scale 2
studio "HGIB_B_Gib1" scale 4
studio "HGIB_B_Bone1" scale 3
studio "Flesh1" scale 4
studio "Flesh2" scale 4
studio "Flesh3" scale 4
studio "Flesh4" scale 4
}

$attachment "bloodpoint" "Dummy01" 0.00 0.00 0 rotate 0 0 0

$sequence idle1    "HGIB_Anim1"

$collisionmodel "HGIB_Skull1" {

	// Mass in kilograms
	$automass
	$concave
}
"$KeyValues"
{
"particles"
{
   "effect"
   {
      "name"              "blood_trail_red_01_goop"
      "attachment_type"   "follow_attachment"
      "attachment_point" "bloodpoint"
   }

 }
	"prop_data"
	{
		// Only here so that the world_interactions section will get parsed
	}

	"world_interactions"
	{
		"onworldimpact"		"bloodsplat"
	}
}

